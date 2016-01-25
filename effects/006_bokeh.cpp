#include "..\\startupfx.h"
#if (STARTUPFX_ID==6)
#include "..\\lovefx.hpp"
#include "..\\thirdparty\\tiny_obj_loader.hpp"
#include "..\\aamath.hpp"

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

void keyDownHandler(unsigned int key)
{
    if (key == INPUT_KEY_LEFT)
        t.R.y += 0.01;
    if (key == INPUT_KEY_RIGHT)
        t.R.y -= 0.01;

    if (key == INPUT_KEY_UP)
        t.R.x += 0.01;
    if (key == INPUT_KEY_DOWN)
        t.R.x -= 0.01;
}

void InitApp()
{
    PerfMarker("Init", 0xFFFF0000);
    //myBar = TwNewBar("Options");
    INPUT_f_keyDown(keyDownHandler);

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
    program::use(prog);
    float aspect; utils::getAspectRatio(aspect);
    projMat.perspectiveCamera(45, aspect, 0.0001, 1000);
}

void RenderApp()
{
    perfMarkerStart("Frame");
    // model
    t.updateMat();
    glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, t.mat.e);
    // view
    time += 0.03;
    float x = cos(time) * 20.0f;
    float z = sin(time) * 20.0f;
    viewMat.lookAt(vec3f(x,20,z), vec3f(0, 0, 0), vec3f(0, 1, 0));
    glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, viewMat.e);
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