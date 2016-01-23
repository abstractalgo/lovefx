#include "../startupfx.h"
#if (STARTUPFX_ID==1)
#include "../lovefx.hpp"

#ifdef USE_ANTTWBAR_BACKEND
TwBar *myBar;
#endif

void h_MouseUp(int btn)
{
    if (INPUT_MOUSE_LEFT == btn)
        printf("Left mouse button click.\n");
}

void InitApp()
{
    INPUT_f_mouseUp(h_MouseUp);
#ifdef USE_ANTTWBAR_BACKEND
    myBar = TwNewBar("Simple sample bar");
#endif
    glClearColor(0.0f, 0.2f, 0.4f, 1.0f);
}

void RenderApp()
{
    perfMarkerStart("Frame");
    glClear(GL_COLOR_BUFFER_BIT);
    perfMarkerEnd("Frame");
}

void CleanupApp()
{

}

#endif