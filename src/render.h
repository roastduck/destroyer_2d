#ifndef RENDER_H_
#define RENDER_H_

#include <string>
#include <unordered_map>
#include "loader.h"
#include "matter.h"

class Window;

class Render
{
public:
    Render(const Render &) = delete;
    Render(Render &&) = delete;

    static Render &getInstance();

    /// @param worldScale : returned from World::getScale()
    void drawRigid(const b2Body *b, float worldScale) noexcept;

    /// @param worldScale : returned from World::getScale()
    void drawParticleSystem(const b2ParticleSystem *s, float worldScale) noexcept;

    void drawLine(float x1, float y1, float x2, float y2) noexcept;

    static void setWindow(const Window *_window) { mWindow = _window, updateWindowSize(); }

private:
    Render();

    float smoothstep(float x) { return x * x * (3 - 2 * x); }

    // Because of glDisable, these gen** functions cannot be used after glEnable-ed

    GLuint genShader(GLenum type, const std::string &source);

    GLuint genProgram(GLuint vertexShader, GLuint fragmentShader);

    /// first render to a framebuffer with color blending
    void particleRender1(const b2Vec2 *centers, float32 radius, const b2ParticleColor *colors, int32 count) noexcept;
    void genParticleTexture1() noexcept;

    /// then render to screen with alpha threshold
    void particleRender2(float depth) noexcept;
    void genParticleTexture2() noexcept;

    /// the intermediate buffer between two redering procedures
    void genParticleFrameBuffer() noexcept;

    /// controls the two rendering procedures
    void drawParticles(const b2Vec2 *centers, float32 radius, const b2ParticleColor *colors, int32 count, float depth) noexcept;

    void genCircleTexture() noexcept;

    GLuint getTextureFromPixels(const unsigned char pixels[][4], int width, int height);

    /// update windowWidth and windowHeight from mWindow
    /// @return bool. true when changed.
    static bool updateWindowSize();

    GLuint particleTexture1, particleProgram1, particleTexture2, particleProgram2, particleFrameBuffer,
           circleTexture, circleProgram;

    const std::string particleFragmentShader1Source =
        "#version 330\n"
        "layout(location = 0) out vec4 color;\n" // corresponding to DrawBuffers[0]
        "uniform sampler2D texture;\n"
        "void main()\n"
        "{\n"
        "   color = gl_Color * texture2D(texture, gl_TexCoord[0].st);\n"
        "}\n";

    const std::string particleVertexShader2Source =
        "#version 330\n"
        "void main()\n"
        "{\n"
        "   gl_Position = gl_Vertex;\n"
        "   gl_TexCoord[0] = gl_MultiTexCoord0;\n"
        "}\n";

    const std::string particleFragmentShader2Source =
        "#version 330\n"
        "uniform sampler2D texture;\n"
        "out vec4 colorOut;\n"
        "void main()\n"
        "{\n"
        "   vec4 temp = texture2D(texture, gl_TexCoord[0].st);\n"
        "   if (temp.a < 0.1) temp = vec4(0, 0, 0, 0); else temp.a = 0.5;\n"
        "   colorOut = temp;\n"
        "}\n";

    const std::string circleFragmentShaderSource =
        "#version 330\n"
        "uniform sampler2D texture;\n"
        "out vec4 colorOut;\n"
        "void main()\n"
        "{\n"
        "   colorOut = gl_Color * texture2D(texture, gl_TexCoord[0].st);\n"
        "   if (colorOut.a > 0) gl_FragDepth = gl_FragCoord.z; else gl_FragDepth = 100;\n"
        "}\n";

    std::unordered_map<int, GLuint> cachedTexture;

    static const Window *mWindow;
    static int windowWidth, windowHeight;
};

inline Render &Render::getInstance()
{
    static Render *p = 0;
    if (!p) p = new Render();
    return *p;
}

#endif // RENDER_H_
