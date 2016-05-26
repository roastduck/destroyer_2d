/**
 * We use OpenGL legacy mode mostly for its simplicity, except for using
 * GLSL to draw particles. We render pariticles into a framebuffer with
 * alpha blending, and then render from the buffer to screen, to create a
 * continuous surface.
 */

#include <cmath>
#include <cctype>
#include <cstring>
#include <cassert>
#include <algorithm>
#include "window.h"
#include "render.h"

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

void Render::drawRigid(const b2Body *b, float worldScale) noexcept
{
    const Rigid *m = (const Rigid*)b->GetUserData();
    for (const b2Fixture *f = b->GetFixtureList(); f; f = f->GetNext())
    {
        const b2Shape *shape = f->GetShape();
        FixtureRenderer *renderer = NULL;
        switch (shape->GetType())
        {
            case b2Shape::e_polygon:
            case b2Shape::e_circle:
                renderer = new PolygonRenderer(b, f, worldScale);
                break;
            default:
                assert(false);
        }
        // DO NOT render any other thing before deleting renderer
        // the 3 steps must happen EXACTLY in this order !
        renderer->drawEdge();
        renderer->drawMain();
        renderer->drawAlert();
        assert(renderer != NULL);
        delete renderer;
        renderer = NULL;

        if (m->getIsUserCreated())
        {
            int key = m->getKeyBinded();
            if (key)
            {
                if (! cachedLetter.count(key))
                    cachedLetter[key] = getTextureFromLetter(key);
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, cachedLetter[key]);

                b2Vec2 pos(b->GetPosition() + b2Vec2(0.5f, 0.5f));
                float d = m->getDepth() - 0.5f;
                glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 1.0f), glColor4f(1.0f, 1.0f, 1.0f, 1.0f), glVertex3f(pos.x - 0.3f, pos.y - 0.3f, d);
                glTexCoord2f(1.0f, 1.0f), glColor4f(1.0f, 1.0f, 1.0f, 1.0f), glVertex3f(pos.x + 0.3f, pos.y - 0.3f, d);
                glTexCoord2f(1.0f, 0.0f), glColor4f(1.0f, 1.0f, 1.0f, 1.0f), glVertex3f(pos.x + 0.3f, pos.y + 0.3f, d);
                glTexCoord2f(0.0f, 0.0f), glColor4f(1.0f, 1.0f, 1.0f, 1.0f), glVertex3f(pos.x - 0.3f, pos.y + 0.3f, d);
                glEnd();

                glDisable(GL_TEXTURE_2D);
            }
        }
    }
}

void Render::drawParticleSystem(const b2ParticleSystem *s, float worldScale) noexcept
{
    assert(s->GetParticleGroupCount() > 0);
    float depth = ((Matter*)(s->GetParticleGroupList()->GetUserData()))->getDepth();
    float displayedScale = ((ParticleSystem*)(s->GetParticleGroupList()->GetUserData()))->getDisplayedRadiusScale();
    drawParticles(s->GetPositionBuffer(), PARTICLE_RADIUS * displayedScale / worldScale, s->GetColorBuffer(), s->GetParticleCount(), depth);
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

void Render::drawPopup(const std::string &s, float l, float r, float d, float u) noexcept
{
    assert(l < r && d < u);
    const float X[4] = { l, r, r, l }, Y[4] = { d, d, u, u };

    if (! cachedText.count(s))
        cachedText[s] = getTextureFromText(s);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, cachedText[s]);
    glBegin(GL_QUADS);
    for (int i = 0; i < 4; i++)
    {
        glTexCoord2f(X[i]==l ? 0.0f : 1.0f, Y[i]==d ? 1.0f : 0.0f);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f), glVertex3f(X[i], Y[i], -2.0f);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glLineWidth(POPUP_BOARDER);
    glBegin(GL_LINES);
    for (int i = 1; i <= 8; i++)
    {
        glColor4f(POPUP_BOARDER_R, POPUP_BOARDER_G, POPUP_BOARDER_B, POPUP_BOARDER_A);
        glVertex3f(X[i/2%4], Y[i/2%4], -2.0f);
    }
    glEnd();
    glLineWidth(1.0f);
}

Render::FixtureRenderer::FixtureRenderer(const b2Body *_b, const b2Fixture *_f, float _scale) noexcept
    : render(Render::getInstance()), b(_b), m((Rigid*)(b->GetUserData())), f(_f),
      shape(f->GetShape()), worldScale(_scale)
{}

Render::PolygonRenderer::PolygonRenderer(const b2Body *_b, const b2Fixture *_f, float _scale) noexcept
    : FixtureRenderer(_b, _f, _scale)
{
    if (shape->GetType() == b2Shape::e_polygon)
    {
        localCenter = ((b2PolygonShape*)shape)->m_centroid;
        vert = std::vector<b2Vec2>(((b2PolygonShape*)shape)->GetVertexCount());
        localVert = std::vector<b2Vec2>(((b2PolygonShape*)shape)->GetVertexCount());
        for (size_t i = 0; i < vert.size(); i++)
        {
            localVert[i] = ((b2PolygonShape*)shape)->GetVertex(i);
            vert[i] = b->GetWorldPoint(localVert[i]);
        }
    } else if (shape->GetType() == b2Shape::e_circle)
    {
        localCenter = ((b2CircleShape*)shape)->m_p;
        vert = std::vector<b2Vec2>(36);
        localVert = std::vector<b2Vec2>(36);
        for (size_t i = 0; i < 36; i++)
        {
            float dgr = (float)i / 36 * 2 * PI;
            localVert[i] = localCenter + b2Vec2(shape->m_radius * cos(dgr), shape->m_radius * sin(dgr));
            vert[i] = b->GetWorldPoint(localVert[i]);
        }
    } else
        assert(false);
}

void Render::PolygonRenderer::drawEdge() noexcept
{
    glBegin(GL_LINES);
    for (size_t _i = 1; _i <= vert.size() * 2; _i++)
    {
        size_t i = _i / 2 % vert.size();
        assert(i >= 0 && i < vert.size());
        glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
        glVertex3f(vert[i].x, vert[i].y, m->getDepth());
    }
    glEnd();
}

void Render::PolygonRenderer::drawMain() noexcept
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE);
    switch (m->getRenderMethod())
    {
    case Matter::RENDER_COLOR:
        glBegin(GL_POLYGON);
        for (size_t i = 0; i < vert.size(); i++)
        {
            glColor4f(m->getColorR(), m->getColorG(), m->getColorB(), m->getColorA());
            glVertex3f(vert[i].x, vert[i].y, m->getDepth());
        }
        glEnd();
        break;

    case Matter::RENDER_TEXTURE:
    case Matter::RENDER_COLOR_WITH_TEXTURE:
        {
            auto name = m->getImage();
            if (! render.cachedImage.count(name))
                render.cachedImage[name] = render.getTextureFromPixels(IMAGES[name], IMAGES_W[name], IMAGES_H[name]);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, render.cachedImage[name]);

            float scaleX = 0, scaleY = 0;
            for (size_t i = 0; i < localVert.size(); i++)
            {
                scaleX = std::max(scaleX, fabsf(localVert[i].x - localCenter.x));
                scaleY = std::max(scaleY, fabsf(localVert[i].y - localCenter.y));
            }
            glBegin(GL_POLYGON);
            for (size_t i = 0; i < vert.size(); i++)
            {
                glTexCoord2f((localVert[i].x - localCenter.x) / scaleX * 0.5f + 0.5f, (localVert[i].y - localCenter.y) / -scaleY * 0.5f + 0.5f);
                if (m->getRenderMethod() == Matter::RENDER_COLOR_WITH_TEXTURE)
                    glColor4f(m->getColorR(), m->getColorG(), m->getColorB(), m->getColorA());
                else
                    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
                glVertex3f(vert[i].x, vert[i].y, m->getDepth());
            }
            glEnd();

            glDisable(GL_TEXTURE_2D);
        }

        break;

    default:
        assert(false);
    }
    glDisable(GL_BLEND);
}

void Render::PolygonRenderer::drawAlert() noexcept
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    if (m->getAlert())
    {
        glBegin(GL_POLYGON);
        for (size_t i = 0; i < vert.size(); i++)
        {
            glColor4f(m->getAlertColorR(), m->getAlertColorG(), m->getAlertColorB(), m->getAlertColorA());
            glVertex3f(vert[i].x - ALERT_LINE_WIDTH, vert[i].y - ALERT_LINE_WIDTH, m->getDepth() + 1.0f);
        }
        glEnd();
    }
    glDisable(GL_BLEND);
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
                (type == GL_VERTEX_SHADER ? "vertex" : "fragment") +
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
    assert(count > 0);
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

    glPointSize(radius);

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

GLuint Render::getTextureFromPixels(const unsigned char pixels[][4], int width, int height) noexcept
{
    GLuint ret;
    glGenTextures(1, &ret);
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, ret);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    glDisable(GL_TEXTURE_2D);
    return ret;
}

GLuint Render::getTextureFromText(const std::string &s) noexcept
{
    int lines(1), columns(0), maxLength(0);
    for (char c : s)
    {
        assert(isupper(c) || c==',' || c=='.' || c==' ' || c=='\n');
        if (c == '\n')
            lines++, columns = 0;
        else
            columns++;
        maxLength = std::max(maxLength, columns);
    }

    assert(maxLength > 0);
    int h = lines * FONT_H + 2 * POPUP_PADDING, w = maxLength * FONT_W + 2 * POPUP_PADDING;
    unsigned char *raw = new unsigned char [h * w * 4];
    auto pixels = (unsigned char (*) [4]) raw;
    std::fill((unsigned*)pixels, ((unsigned*)pixels) + h * w, *((unsigned*)FONT_BG_COLOR));

    int i(POPUP_PADDING), j(POPUP_PADDING);
    for (char c : s)
        if (c == '\n')
            i += FONT_H, j = POPUP_PADDING;
        else
        {
            int id = c==',' ? 26 : c=='.' ? 27 : c==' ' ? 28 : c-'A';
            for (int p = 0; p < FONT_H; p++)
                for (int q = 0; q < FONT_W; q++)
                {
                    assert((i+p) * w + (j+q) >= 0 && (i+p) * w + (j+q) < h*w);
                    assert(p * FONT_W + q >= 0 && p * FONT_W + q < FONT_H * FONT_W);
                    memcpy(pixels[(i+p) * w + (j+q)], FONT[id][p * FONT_W + q], 4);
                }
            j += FONT_W;
        }
    int ret = getTextureFromPixels(pixels, w, h);
    delete [] raw;
    return ret;
}

GLuint Render::getTextureFromLetter(char c) noexcept
{
    assert(isupper(c));
    return getTextureFromPixels(FONT[c - 'A'], FONT_W, FONT_H);
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

