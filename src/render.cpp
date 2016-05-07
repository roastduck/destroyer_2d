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

void Render::drawRigid(const b2Body *b) noexcept
{
    const Matter *m = (const Matter*)b->GetUserData();
    for (const b2Fixture *f = b->GetFixtureList(); f; f = f->GetNext())
    {
        const b2Shape *shape = f->GetShape();
        switch (shape->GetType())
        {
            case b2Shape::e_polygon:
                glBegin(GL_POLYGON);
                for (int i = 0; i < ((b2PolygonShape*)shape)->GetVertexCount(); i++)
                {
                    b2Vec2 pos(b->GetWorldPoint(((b2PolygonShape*)shape)->GetVertex(i)));
                    glColor4f(m->getColorR(), m->getColorG(), m->getColorB(), m->getColorA());
                    glVertex3f(pos.x, pos.y, 0.0f);
                }
                glEnd();
                break;

            default:
                assert(false);
        }
    }
}

void Render::drawParticleSystem(const b2ParticleSystem *s) noexcept
{
    drawParticles(s->GetPositionBuffer(), PARTICLE_RADIUS, s->GetColorBuffer(), s->GetParticleCount());
}

void Render::drawParticles(const b2Vec2 *centers, float32 radius, const b2ParticleColor *colors, int32 count) noexcept
{
    assert(centers != NULL);
    assert(colors != NULL);

	static unsigned int particle_texture = 0;

	if (!particle_texture || !glIsTexture(particle_texture))
	{
		// generate a "gaussian blob" texture procedurally
		glGenTextures(1, &particle_texture);
		b2Assert(particle_texture);
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
		glBindTexture(GL_TEXTURE_2D, particle_texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE, TSIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex);
		glDisable(GL_TEXTURE_2D);

		glEnable(GL_POINT_SMOOTH);
	}

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, particle_texture);

    glEnable(GL_POINT_SPRITE);
    glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);

	glPointSize(radius * PARTICLE_SIZE_SCALE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, &centers[0].x);
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(4, GL_UNSIGNED_BYTE, 0, &colors[0].r);

	glDrawArrays(GL_POINTS, 0, count);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

