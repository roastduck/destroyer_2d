#ifndef RENDER_H_
#define RENDER_H_

#include <GLFW/glfw3.h>
#include "matter.h"

class Render
{
public:
    static void drawRigid(const b2Body *b) noexcept;

    static void drawParticleSystem(const b2ParticleSystem *s) noexcept;

private:
    static float smoothstep(float x) { return x * x * (3 - 2 * x); }

    static void drawParticles(const b2Vec2 *centers, float32 radius, const b2ParticleColor *colors, int32 count) noexcept;
};

#endif // RENDER_H_
