#include "../startupfx.h"
#if (STARTUPFX_ID==5)
#include "../lovefx.hpp"
#include "gl-matrix.h"

mat4_t transform;
GLuint prog;
GLint tr_loc;
double time = 0;
GLuint fsquad;

const double FRAME_TIME_60FPS = 1000.0 / 60.0;    // 60FPS
double frame_time = FRAME_TIME_60FPS;
double frame_diff;

void InitApp()
{
    glClearColor(0.0f, 0.2f, 0.4f, 1.0f);
    transform = mat4_create(0);
    mat4_identity(transform);

    lovefx::program::createFromFiles(prog, "res\\shaders\\tr.vs", 0, 0, 0, "res\\shaders\\tr.fs", 0);
    lovefx::program::use(prog);
    lovefx::program::location(prog, "mMat", tr_loc);

    lovefx::utils::initFSQuad(fsquad);
    lovefx::timer::init();
}

void RenderApp()
{
    time += 0.00001;
    glClear(GL_COLOR_BUFFER_BIT);
    mat4_rotateZ(transform, 0.001, 0);
    glUniformMatrix4dv(tr_loc, 1, GL_FALSE, transform);
    lovefx::utils::drawFSQuad(fsquad);
    lovefx::timer::update();
    printf("FPS: %4.2lf  \r", lovefx::timer::FPS);

    double fr = lovefx::timer::DIFF / 1000.0;
    frame_diff = frame_time - fr;
    if (frame_diff > 0)
    {
        frame_time = FRAME_TIME_60FPS;
        Sleep(frame_diff);
    }
    else
    {
        frame_time = FRAME_TIME_60FPS + frame_diff;
    }
    /*
    time=17 fr=2 diff>0 sleep diff time=17
    time=17 fr=20 diff<0 time=17+diff
    time=14 fr=20 diff<0 time=17+diff
    time=11 fr=10 diff>0 sleep diff time=17
    */
}

void CleanupApp()
{
    lovefx::utils::destroyFSQuad(fsquad);
    delete[] transform;
}

#endif