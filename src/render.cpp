/**
 * We use OpenGL legacy mode mostly for its simplicity, except for using
 * GLSL to draw particles. We render pariticles into a framebuffer with
 * alpha blending, and then render from the buffer to screen, to create a
 * continuous surface.
 */

#include <cassert>
#include "window.h"
#include "render.h"

// We need these 4 from glext.h, and define them here rather than relying on
// the header, which is not universally available.
#ifndef GL_POINT_SPRITE
#define GL_POINT_SPRITE                   0x8861
#endif
#ifndef GL_COORD_REPLACE
#define GL_COORD_REPLACE                  0x8862
#endif
#ifndef GL_GENERATE_MIPMAP
#define GL_GENERATE_MIPMAP                0x8191
#endif
#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE                  0x812F
#endif

Render::Render()
    : particleTexture1(0), particleProgram1(0), particleTexture2(0), particleProgram2(0),
      particleFrameBuffer(0)
{
    particleProgram1 = genProgram
    (
        0,
        genShader(GL_FRAGMENT_SHADER, particleFragmentShader1Source)
    );
    particleProgram2 = genProgram
    (
        genShader(GL_VERTEX_SHADER, particleVertexShader2Source),
        genShader(GL_FRAGMENT_SHADER, particleFragmentShader2Source)
    );
    genParticleTexture1();
    genParticleTexture2();
    genParticleFrameBuffer();
}

void Render::drawRigid(const b2Body *b) noexcept
{
    glEnable(GL_BLEND);

    const Rigid *m = (const Rigid*)b->GetUserData();
    for (const b2Fixture *f = b->GetFixtureList(); f; f = f->GetNext())
    {
        const b2Shape *shape = f->GetShape();
        switch (shape->GetType())
        {
            case b2Shape::e_polygon:
               
                // the 3 steps must happen EXACTLY in this order !

                glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA);

                glBegin(GL_POLYGON);
                for (int i = 0; i < ((b2PolygonShape*)shape)->GetVertexCount(); i++)
                {
                    b2Vec2 pos(b->GetWorldPoint(((b2PolygonShape*)shape)->GetVertex(i)));
                    glColor4f(m->getColorR(), m->getColorG(), m->getColorB(), m->getColorA());
                    glVertex3f(pos.x, pos.y, m->getDepth());
                }
                glEnd();

                glBegin(GL_LINES);
                for (int _i = 1; _i <= ((b2PolygonShape*)shape)->GetVertexCount() * 2; _i++)
                {
                    int i = _i / 2 % ((b2PolygonShape*)shape)->GetVertexCount();
                    b2Vec2 pos(b->GetWorldPoint(((b2PolygonShape*)shape)->GetVertex(i)));
                    glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
                    glVertex3f(pos.x, pos.y, m->getDepth());
                }
                glEnd();
                
                if (m->getAlert())
                {
                    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                    glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);

                    glBegin(GL_POLYGON);
                    for (int i = 0; i < ((b2PolygonShape*)shape)->GetVertexCount(); i++)
                    {
                        b2Vec2 pos(b->GetWorldPoint(((b2PolygonShape*)shape)->GetVertex(i)));
                        glColor4f(m->getAlertColorR(), m->getAlertColorG(), m->getAlertColorB(), m->getAlertColorA());
                        glVertex3f(pos.x - ALERT_LINE_WIDTH, pos.y - ALERT_LINE_WIDTH, m->getDepth() + 1.0f);
                    }
                    glEnd();
                }

                break;

            default:
                assert(false);
        }
    }

    glDisable(GL_BLEND);
}

void Render::drawParticleSystem(const b2ParticleSystem *s) noexcept
{
    assert(s->GetParticleGroupCount() > 0);
    float depth = ((Matter*)(s->GetParticleGroupList()->GetUserData()))->getDepth();
    drawParticles(s->GetPositionBuffer(), PARTICLE_RADIUS, s->GetColorBuffer(), s->GetParticleCount(), depth);
}

void Render::drawLine(float x1, float y1, float x2, float y2) noexcept
{
    glBegin(GL_LINES);
    glColor4f(TIP_LINE_COLOR_R, TIP_LINE_COLOR_G, TIP_LINE_COLOR_B, TIP_LINE_COLOR_A);
    glVertex3f(x1, y1, -1.0f);
    glColor4f(TIP_LINE_COLOR_R, TIP_LINE_COLOR_G, TIP_LINE_COLOR_B, TIP_LINE_COLOR_A);
    glVertex3f(x2, y2, -1.0f);
    glEnd();
}

GLuint Render::genShader(GLenum type, const std::string &source)
{
    GLuint id = glCreateShader(type);
    const char *str = source.c_str();
    glShaderSource(id, 1, &str, 0);
    glCompileShader(id);
    GLint status;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
        glGetShaderInfoLog(id, infoLogLength, 0, strInfoLog);
        std::string errMsg(
				std::string("Compile failure in: ") +
				(type == GL_VERTEX_SHADER ? "vertex" : type == GL_GEOMETRY_SHADER ? "geometry" : "fragment") +
				"shader: " +
				strInfoLog
			);
        delete[] strInfoLog;
        throw std::runtime_error(errMsg);
    }
    return id;
}

GLuint Render::genProgram(GLuint vertexShader, GLuint fragmentShader)
{
    assert(fragmentShader);

    GLuint id = glCreateProgram();
    if (vertexShader)
        glAttachShader(id, vertexShader);
    if (fragmentShader)
        glAttachShader(id, fragmentShader);
    glLinkProgram(id);
    GLint status;
    glGetProgramiv(id, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
        glGetShaderInfoLog(id, infoLogLength, 0, strInfoLog);
        std::string errMsg(std::string("Link failuer: ") + strInfoLog);
        delete[] strInfoLog;
        throw std::runtime_error(errMsg);
    }
    return id;
}

void Render::genParticleTexture1() noexcept
{
    // generate a "gaussian blob" texture procedurally
    glGenTextures(1, &particleTexture1);
    const int TSIZE = 64;
    unsigned char tex[TSIZE][TSIZE][4];
    for (int y = 0; y < TSIZE; y++)
    {
        for (int x = 0; x < TSIZE; x++)
        {
            float fx = (x + 0.5f) / TSIZE * 2 - 1;
            float fy = (y + 0.5f) / TSIZE * 2 - 1;
            float dist = sqrtf(fx * fx + fy * fy);
            unsigned char intensity = (unsigned char)(dist <= 1 ? smoothstep(1 - dist) * 255 : 0);
            tex[y][x][0] = tex[y][x][1] = tex[y][x][2] = 128;
            tex[y][x][3] = intensity;
        }
    }
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, particleTexture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE, TSIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex);

    assert(particleProgram1 && glIsProgram(particleProgram1));
    glUseProgram(particleProgram1);
    GLuint imageLocation = glGetUniformLocation(particleProgram1, "texture");
    glUniform1i(imageLocation, 0);
    glUseProgram(0);

    glDisable(GL_TEXTURE_2D);
}

void Render::genParticleTexture2() noexcept
{
    glEnable(GL_TEXTURE_2D);

    glGenTextures(1, &particleTexture2);
    glBindTexture(GL_TEXTURE_2D, particleTexture2);
    assert(windowWidth != 0 && windowHeight != 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, windowWidth, windowHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    assert(particleProgram2 && glIsProgram(particleProgram2));
    glUseProgram(particleProgram2);
    GLuint imageLocation = glGetUniformLocation(particleProgram2, "texture");
    glUniform1i(imageLocation, 0);
    glUseProgram(0);

    glDisable(GL_TEXTURE_2D);
}

void Render::genParticleFrameBuffer() noexcept
{
    glEnable(GL_TEXTURE_2D);

    glGenFramebuffers(1, &particleFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, particleFrameBuffer);
    
    assert(particleTexture2 && glIsTexture(particleTexture2));
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, particleTexture2, 0);
    GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_TEXTURE_2D);
}

void Render::particleRender1(const b2Vec2 *centers, float32 radius, const b2ParticleColor *colors, int32 count) noexcept
{
    assert(particleTexture1 && glIsTexture(particleTexture1));
    assert(particleProgram1 && glIsProgram(particleProgram1));

    glUseProgram(particleProgram1);

    glBindFramebuffer(GL_FRAMEBUFFER, particleFrameBuffer);
    assert(windowWidth != 0 && windowHeight != 0);
    glViewport(0, 0, windowWidth, windowHeight);
    glClearColor(BG_COLOR_R, BG_COLOR_G, BG_COLOR_B, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_POINT_SMOOTH);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, particleTexture1);

    glEnable(GL_POINT_SPRITE);
    glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);

	glPointSize(radius * PARTICLE_SIZE_SCALE);

	glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_ONE, GL_ZERO, GL_ONE, GL_ONE); // this is used for alpha test
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE); // this is used for display particles
    // REMEMBER TO MODIFY

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, &centers[0].x);
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(4, GL_UNSIGNED_BYTE, 0, &colors[0].r);

	glDrawArrays(GL_POINTS, 0, count);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glUseProgram(0);
}

void Render::particleRender2(float depth) noexcept
{
    assert(particleTexture2 && glIsTexture(particleTexture2));
    assert(particleProgram2 && glIsProgram(particleProgram2));

    glUseProgram(particleProgram2);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, particleTexture2);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_QUADS);
    glTexCoord2f(1, 1), glVertex3f(1.0f, 1.0f, depth);
    glTexCoord2f(0, 1), glVertex3f(-1.0f, 1.0f, depth);
    glTexCoord2f(0, 0), glVertex3f(-1.0f, -1.0f, depth);
    glTexCoord2f(1, 0), glVertex3f(1.0f, -1.0f, depth);
    glEnd();

    glDisable(GL_BLEND);

	glDisable(GL_TEXTURE_2D);
    glUseProgram(0);
}

void Render::drawParticles(const b2Vec2 *centers, float32 radius, const b2ParticleColor *colors, int32 count, float depth) noexcept
{
    assert(centers != NULL);
    assert(colors != NULL);

    if (updateWindowSize())
    {
        glDeleteTextures(1, &particleTexture2);
        particleTexture2 = 0;
        genParticleTexture2();

        glDeleteFramebuffers(1, &particleFrameBuffer);
        particleFrameBuffer = 0;
        genParticleFrameBuffer();
    }

    particleRender1(centers, radius, colors, count);
    particleRender2(depth);
}

bool Render::updateWindowSize()
{
    int _w, _h;
    mWindow->getWidthHeight(&_w, &_h);
    if (_w == windowWidth && _h == windowHeight) return false;
    windowWidth = _w, windowHeight = _h;
    return true;
}

const Window *Render::mWindow = 0;
int Render::windowWidth = 0, Render::windowHeight = 0;

