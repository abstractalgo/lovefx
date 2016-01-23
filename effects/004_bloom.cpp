#include "../startupfx.h"
#if (STARTUPFX_ID==4)
#include "../lovefx.hpp"

void InitApp()
{
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