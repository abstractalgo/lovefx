#include "../startupfx.h"
#if (STARTUPFX_ID==8)
#include "../lovefx.hpp"
#include "../thirdparty/glm/glm.hpp"
#include "../thirdparty/glm/gtc/matrix_transform.hpp"
#include "../thirdparty/glm/gtc/type_ptr.hpp"

// ----------------------------------------------------------------------------- VARIABLES
GLuint fsquad;
LFXrendertarget defRT, smallRT;
int width, height;
GLuint displayProgram;
GLint inTex_uloc;
// resize_mipmap
void resize_mipmap();
// resize_shader
struct
{
    GLuint program;
    GLint inTex_uloc, resolution_uloc;
} resshdr;
void resize_shader();
// resize_compute
void resize_compute();
// resize
uint8_t resizeMode = 1;
void resize();

// ----------------------------------------------------------------------------- EVENT HANDLERS

void h_keyUp(unsigned int k)
{
    switch (k)
    {
    case INPUT_KEY_ESC:
        PostQuitMessage(0);
        break;
    case INPUT_KEY_1:
        resizeMode = 1;
        break;
    case INPUT_KEY_2:
        resizeMode = 2;
        break;
    case INPUT_KEY_3:
        resizeMode = 3;
        break;
    case INPUT_KEY_4:
        resizeMode = 4;
        break;
    }
}

// ----------------------------------------------------------------------------- VARIOUS FUNCTIONS

void resize_mipmap()
{
    PerfMarker("mipmap", 0xFFFFC90E);
}
void resize_shader()
{
    PerfMarker("shader", 0xFF880015);
    glDisable(GL_DEPTH_TEST);
    lovefx::fbo::use(smallRT.fbo);
    lovefx::program::use(resshdr.program);
    glBindTexture(GL_TEXTURE_2D, defRT.col0);
    glUniform1i(resshdr.inTex_uloc, 0);
    lovefx::utils::drawFSQuad(fsquad);
    glEnable(GL_DEPTH_TEST);
}
void resize_compute()
{
    PerfMarker("compute", 0xFF3F48CC);
}

void resize()
{
    if (1 == resizeMode)
    {
        resize_mipmap();
    }

    else if (2 == resizeMode)
    {
        resize_shader();
    }
    
    else //if (3 == resizeMode)
    {
        resize_compute();
    }
}

// ----------------------------------------------------------------------------- MAIN APP

void InitApp()
{
    PerfMarker("INIT", 0xFF0000FF);
    // globals
    {
        PerfMarker("g_init", 0xFF8888FF);
        lovefx::utils::getResolution(width, height);
        // setup handlers
        INPUT_f_keyUp(h_keyUp);
        // full screen quad
        lovefx::utils::initFSQuad(fsquad);
        // default render target
        lovefx::rt::create(defRT, width, height);
        // small RT
        lovefx::rt::create(smallRT, width/2, height/2);
        // init shaders
        const char* vs = LFX_GLSL(330,
            layout(location = 0) in vec2 aPosition;
            out vec2 uv;

            void main()
            {
                gl_Position = vec4(aPosition, 0.5, 1.0);
                uv = (aPosition + vec2(1.0)) * 0.5;
            }
        );
        const char* fs = LFX_GLSL(330,
            uniform sampler2D inTex;
            in vec2 uv;
            out vec4 fragColor;

            void main()
            {
                vec4 col = texture(inTex, uv);
                fragColor = col;
            }
        );
        lovefx::program::createFromSource(displayProgram, vs, 0, 0, 0, fs, 0);
        lovefx::program::log(displayProgram);
        lovefx::program::location(displayProgram, "inTex", inTex_uloc);
        // set texture active
        glActiveTexture(GL_TEXTURE0);
    }
    // shader version init
    {
        PerfMarker("vS_init", 0xFFFF8888);
        // init shaders
        const char* vs = LFX_GLSL(330,
            layout(location = 0) in vec2 aPosition;
            out vec2 uv;

            void main()
            {
                gl_Position = vec4(aPosition, 0.5, 1.0);
                uv = (aPosition + vec2(1.0)) * 0.5;
            }
        );
        const char* fs = LFX_GLSL(330,
            uniform sampler2D inTex;
            uniform vec2 resolution;
            in vec2 uv;
            out vec4 fragColor;

            void main()
            {
                vec4 col = texture(inTex, uv);
                for (int i = 0; i < 20; i++)
                {
                    col = texture(inTex, uv);
                }
                fragColor = col;
            }
        );
        lovefx::program::createFromSource(resshdr.program, vs, 0, 0, 0, fs, 0);
        lovefx::program::log(resshdr.program);
        lovefx::program::location(resshdr.program, "inTex", resshdr.inTex_uloc);
        lovefx::program::location(resshdr.program, "resolution", resshdr.resolution_uloc);
    }
}

void RenderApp()
{
    PerfMarker("frame", 0xFFFF6666);
    // 1. render a scene
    {
        PerfMarker("scene", 0xFF494949);
        lovefx::fbo::use(defRT.fbo);
        glClearColor(1, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // TODO: render scene
    }

    // 2. do a resize
    resize();

    // 3. display
    {
        PerfMarker("display", 0xFF888888);
        lovefx::fbo::use(GL_NONE);
        glClearColor(0, 0, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        lovefx::program::use(displayProgram);
        glBindTexture(GL_TEXTURE_2D, smallRT.col0);
        glUniform1i(inTex_uloc, 0);
        lovefx::utils::drawFSQuad(fsquad);
    }

    LFX_ERRCHK();
}

void CleanupApp()
{
    lovefx::utils::destroyFSQuad(fsquad); // delete fullscreen quad
}

#endif