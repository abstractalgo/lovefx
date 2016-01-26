#include "..\\startupfx.h"
#if (STARTUPFX_ID==6)
#include "..\\lovefx.hpp"
#include "..\\thirdparty\\tiny_obj_loader.hpp"
#include "..\\aamath.hpp"
#include "..\\thirdparty\\\glm\\glm.hpp"
#include "..\\thirdparty\glm\gtc\matrix_transform.hpp"

using namespace lovefx;
using namespace aamath;

//TwBar*      myBar;
LFXprogram  prog;
LFXuloc     texloc;
LFXuloc     modelMatLoc, viewMatLoc, projMatLoc;
LFXtex2d    texobj;
std::vector<LFXmesh> meshes;
std::vector<tinyobj::material_t> materials;

transform t;
mat4 projMat, viewMat;
float time = 0;
fps_camera camera;

void mouseMoveHandler(int, int, unsigned int x, unsigned int y)
{
    static unsigned int last_x, last_y;
    float dx = (float)(x - last_x);
    float dy = (float)(y - last_y);
    float dt = 0.01f;
    camera.update(dx, dy, dt);

    /*printf("\r              ");
    printf("\r%d %d", x - last_x, y - last_y);*/
    last_x = x;
    last_y = y;
    //RECT rect;
    //GetWindowRect(g_hWnd, &rect);
    //ClipCursor(&rect);

    //SetCursorPos((rect.left+rect.right)/2, (rect.top+rect.bottom)/2);
}

void keyDownHandler(unsigned int key)
{
    if (key == INPUT_KEY_LEFT)
        camera.horizontalAngle += 0.035f;
    if (key == INPUT_KEY_RIGHT)
        camera.horizontalAngle -= 0.035f;

    if (key == INPUT_KEY_UP)
        camera.verticalAngle += 0.035f;
    if (key == INPUT_KEY_DOWN)
        camera.verticalAngle -= 0.035f;

    if (key == INPUT_KEY_W)
        camera.position += camera.dir * 0.008f;
    if (key == INPUT_KEY_S)
        camera.position -= camera.dir * 0.008f;
    if (key == INPUT_KEY_D)
        camera.position += camera.right * 0.008f;
    if (key == INPUT_KEY_A)
        camera.position -= camera.right * 0.008f;

    camera.updateMatrix();
}

void InitApp()
{
    PerfMarker("Init", 0xFFFF0000);
    //myBar = TwNewBar("Options");
    INPUT_f_keyDown(keyDownHandler);
    //INPUT_f_mouseMove(mouseMoveHandler);

    glGenTextures(1, &texobj);
    glBindTexture(GL_TEXTURE_2D, texobj);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    file::loadTGA("effects\\res\\sponza_details_diff.tga", GL_TEXTURE_2D, texobj);
    //file::loadBMP("effects\\res\\hehe.bmp", GL_TEXTURE_2D, texobj);

    program::createFromFiles(prog, "effects\\res\\shaders\\sponza.vs", 0, 0, 0, "effects\\res\\shaders\\sponza.fs", 0);
    program::log(prog);
    program::location(prog, "modelMat", modelMatLoc);
    program::location(prog, "viewMat", viewMatLoc);
    program::location(prog, "projMat", projMatLoc);

    perfMarkerStartEx("OBJ parsing", 0xFF88FF88);
    std::vector<tinyobj::shape_t> shapes;
    std::string inputfile = "effects\\res\\crytek_sponza\\sponza.obj";
    std::string err;
    bool ret = tinyobj::LoadObj(shapes, materials, err, inputfile.c_str(), "effects\\res\\crytek_sponza\\");
    if (!err.empty()) { printf("%s\n",err); }
    if (!ret) { exit(1); }
    perfMarkerEnd();

    perfMarkerStartEx("Buidling OGL data", 0xFF8888FF);
    for (size_t i = 0; i < shapes.size(); i++)
    {
        assert(shapes[i].mesh.indices.size() % 3 == 0);
        assert(shapes[i].mesh.positions.size() == shapes[i].mesh.normals.size());
        assert(shapes[i].mesh.positions.size() / 3 * 2 == shapes[i].mesh.texcoords.size());

        LFXmesh msh;
        mesh::create(msh);
        mesh::addVertexBuffer(msh, "iPos", prog, shapes[i].mesh.positions.size() * sizeof(float), 3, GL_FLOAT, shapes[i].mesh.positions.data());
        mesh::addVertexBuffer(msh, "iNor", prog, shapes[i].mesh.normals.size() * sizeof(float), 3, GL_FLOAT, shapes[i].mesh.normals.data());
        mesh::addVertexBuffer(msh, "iUV", prog, shapes[i].mesh.texcoords.size() * sizeof(float), 2, GL_FLOAT, shapes[i].mesh.texcoords.data());
        mesh::addIndexBuffer(msh, shapes[i].mesh.indices.size() * sizeof(unsigned int), shapes[i].mesh.indices.data());
        msh.count = shapes[i].mesh.indices.size();
        meshes.push_back(msh);
    }
    perfMarkerEnd();

    glClearColor(0.0f, 0.2f, 0.4f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    program::use(prog);
    float aspect; utils::getAspectRatio(aspect);
    projMat.perspectiveCamera(45.0f, aspect, 0.0001f, 10.0f);

}
void RenderApp()
{
    perfMarkerStart("Frame");
    // model
    t.updateMat();
    glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, t.mat.e);
    // view
    time += 0.003;
    float x = cos(time) * 2.0f;
    float z = sin(time) * 2.0f;

    viewMat.lookAt(vec3f(x, 2.0f, z), vec3f(0.0f,0.0f,0.0f));
    //viewMat.lookAtFrom(vec3f(0.0f, 0.2f, 0.0f), vec3f(x, 0.0f, z));
    glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, camera.mat.e);
    // projection
    glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, projMat.e);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (size_t i = 0; i < meshes.size(); i++)
    {
        mesh::draw(meshes[i]);
    }
    perfMarkerEnd();

    LFX_ERRCHK();
}

void CleanupApp()
{
    for (size_t i = 0; i < meshes.size(); i++)
        mesh::destroy(meshes[i]);
}

void mainApp()
{
    

    
}

#endif