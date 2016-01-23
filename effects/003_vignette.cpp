#include "../startupfx.h"
#if (STARTUPFX_ID==3)
#include "../lovefx.hpp"

LFXprogram prog_vignette, prog_filmgrain;
GLuint quad;
int width, height;
GLuint fbo_col, rb_col, tex_col;
GLuint fbo_fg, rb_fg, tex_fg;
GLint tex_loc_fg, tex_loc_vig;
GLint tSize_loc, colored_loc, coloramount_loc, grainsize_loc, lumamount_loc, grainamount_loc, timer_loc;

float colored       = 1;
float coloramount   = 0.25;
float grainsize     = 1.72;
float lumamount     = 0.64;
float grainamount   = 0.1;
float timer         = 0;

LFX_vignette_pass vignettePass;

GLfloat tri_coord[] = { 0, 0, 0.5, 0, 0, 0.5, -0.1, -0.1, -0.7, -0.1, -0.1, -0.5 };

#ifdef USE_ANTTWBAR_BACKEND
TwBar *bar;
#endif

using namespace lovefx;

void InitApp()
{
    PerfMarker("Init", 0);
    // setup
    lovefx::utils::getResolution(width, height);
    glClearColor(0.4, 0.4, 0.4, 1.0);

    // init fs quad
    lovefx::utils::initFSQuad(quad);

    // create fbos
    fbo::create(width, height, fbo_col, rb_col, tex_col);
    fbo::create(width, height, fbo_fg, rb_fg, tex_fg);

    // create programs
    program::createFromFiles(prog_filmgrain, "effects\\res\\shaders\\fs.vs", 0, 0, 0, "effects\\res\\shaders\\filmgrain.fs", 0);
    program::location(prog_filmgrain, "tex", tex_loc_fg);
    program::location(prog_filmgrain, "tSize", tSize_loc);
    program::location(prog_filmgrain, "colored", colored_loc);
    program::location(prog_filmgrain, "coloramount", coloramount_loc);
    program::location(prog_filmgrain, "grainsize", grainsize_loc);
    program::location(prog_filmgrain, "lumamount", lumamount_loc);
    program::location(prog_filmgrain, "grainamount", grainamount_loc);
    program::location(prog_filmgrain, "timer", timer_loc);

    program::createFromFiles(prog_vignette, "effects\\res\\shaders\\fs.vs", 0, 0, 0, "effects\\res\\shaders\\vignette.fs", 0);
    program::log(prog_vignette);
    lovefx::program::location(prog_vignette, "tex", tex_loc_vig);
    
    pp::create(vignettePass, quad);

#ifdef USE_ANTTWBAR_BACKEND
    // gui
    bar = TwNewBar("Filmgrain");
    TwAddVarRW(bar, "Colored", TW_TYPE_FLOAT, &colored, " min=0 max=1 step=1.0 ");
    TwAddVarRW(bar, "Color amount", TW_TYPE_FLOAT, &coloramount, " min=0 max=1 step=0.01 ");
    TwAddVarRW(bar, "Grain size", TW_TYPE_FLOAT, &grainsize, " min=1 max=4 step=0.01 ");
    TwAddVarRW(bar, "Lum amount", TW_TYPE_FLOAT, &lumamount, " min=0 max=1 step=0.01 ");
    TwAddVarRW(bar, "Grain amount", TW_TYPE_FLOAT, &grainamount, " min=0 max=1 step=0.01 ");
#endif
}

void RenderApp()
{
    PerfMarker("Frame", 0);

    // draw scene
    {
        PerfMarker("Scene", 0);
        fbo::bind_clear(fbo_col);
        glColor3f(0.8, 0.8, 0.8);
        lovefx::utils::drawTris2f(tri_coord, 1);
        glColor3f(0.3, 0.2, 0.4);
        lovefx::utils::drawTris2f(tri_coord + 6, 1);
    }
    // draw fullscreen quad with filmgrain shader
    {
        PerfMarker("Filmgrain", 0);
        fbo::bind_clear(fbo_fg);
        program::use(prog_filmgrain);
        glBindTexture(GL_TEXTURE_2D, tex_col);
        glUniform1i(tex_loc_fg, 0);
        glUniform2f(tSize_loc, (float)width, (float)height);
        glUniform1f(colored_loc, colored);
        glUniform1f(coloramount_loc, coloramount);
        glUniform1f(grainsize_loc, grainsize);
        glUniform1f(lumamount_loc, lumamount);
        glUniform1f(grainamount_loc, grainamount);
        glUniform1f(timer_loc, ::timer += 0.01);
        lovefx::utils::drawFSQuad(quad);
    }
    // draw fullscreen quad with vignette shader
    {
        PerfMarker("Vignette", 0);
        fbo::use(0);
        pp::draw(vignettePass);
    }
}

void CleanupApp()
{
    lovefx::utils::destroyFSQuad(quad);
    lovefx::program::destroy(prog_filmgrain);
    lovefx::program::destroy(prog_vignette);
    fbo::destroy(fbo_col, rb_col, tex_col);
    fbo::destroy(fbo_fg, rb_fg, tex_fg);
}

#endif