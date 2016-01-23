#include "../startupfx.h"
#if (STARTUPFX_ID==2)
#include "../lovefx.hpp"

// https://software.intel.com/en-us/blogs/2014/07/15/an-investigation-of-fast-real-time-gpu-based-image-blur-algorithms
// http://dev.theomader.com/gaussian-kernel-calculator/

LFX_quad quad;
int width, height;
LFX_rt sceneRT, horBlurRT;
LFX_blur_pass blurPass;

GLfloat tri_coord[] = { 0, 0, 0.5, 0, 0, 0.5, -0.1, -0.1, -0.7, -0.1, -0.1, -0.5 };

using namespace lovefx;

void InitApp()
{
    // setup
    glColor3f(1, 1, 1);
    utils::getResolution(width, height);

    // create rendertargets
    rt::create(sceneRT, width, height);
    rt::create(horBlurRT, width, height);

    // init fs quad
    utils::initFSQuad(quad);

    // create pass
    pp::create(blurPass, quad);
    blurPass.uTex = sceneRT.col0;
    blurPass.uRes[0] = width;
    blurPass.uRes[1] = height;
}

void RenderApp()
{
    // draw scene to the color fbo
    {
        glClearColor(0.2, 0.2, 0.2, 1);
        lovefx::fbo::bind_clear(sceneRT.fbo, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // draw triangles
        lovefx::utils::drawTris2f(tri_coord, 2);
    }

    // draw blurs
    {
        // common setup for both blur passes
        glClearColor(0.0f, 0.0f, 0.f, 1.0f);

        // draw horizontal blur
        lovefx::fbo::bind_clear(horBlurRT.fbo, GL_COLOR_BUFFER_BIT);
        blurPass.uTex = sceneRT.col0;
        blurPass.uXY[0] = 1;
        blurPass.uXY[1] = 0;
        pp::draw(blurPass);
        // draw vertical blur (+ horizontal)
        lovefx::fbo::bind_clear(0, GL_COLOR_BUFFER_BIT);
        blurPass.uTex = horBlurRT.col0;
        blurPass.uXY[0] = 0;
        blurPass.uXY[1] = 1;
        pp::draw(blurPass);
    }
}

void CleanupApp()
{
    /*fbo::destroy(col_fbo, col_rb, col_tex);
    fbo::destroy(fbp_fbo, fbp_rb, fbp_tex);
    program::destroy(prog);
    utils::destroyFSQuad(quad);*/
}

#endif