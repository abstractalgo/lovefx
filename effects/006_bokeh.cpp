#include "..\\startupfx.h"
#if (STARTUPFX_ID==6)
#include "..\\lovefx.hpp"
#include "..\\thirdparty\\tiny_obj_loader.hpp"
#include "..\\aamath.hpp"

using namespace lovefx;
using namespace aamath;

TwBar*      myBar;
LFXprogram  prog;
LFXuloc     texloc;
LFXuloc     mvploc;
LFXtex2d    texobj;
std::vector<LFXmesh> meshes;
std::vector<tinyobj::material_t> materials;

mat4f mvpMat;

void InitApp()
{
    PerfMarker("Init", 0xFFFF0000);
    //myBar = TwNewBar("Options");

    mvpMat.lookAt(vec3f(0, 0, 0), vec3f(1, -1, -1), vec3f(1,1,1));

    glGenTextures(1, &texobj);
    glBindTexture(GL_TEXTURE_2D, texobj);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    file::loadTGA("effects\\res\\sponza_details_diff.tga", GL_TEXTURE_2D, texobj);
    //file::loadBMP("effects\\res\\hehe.bmp", GL_TEXTURE_2D, texobj);

    program::createFromFiles(prog, "effects\\res\\shaders\\sponza.vs", 0, 0, 0, "effects\\res\\shaders\\sponza.fs", 0);
    program::log(prog);
    program::location(prog, "mvp", mvploc);
    //program::location(prog, "tex", texloc);

    perfMarkerStartEx("OBJ parsing", 0xFF88FF88);
    std::vector<tinyobj::shape_t> shapes;
    std::string inputfile = "effects\\res\\crytek_sponza\\sponza.obj";
    std::string err;
    bool ret = tinyobj::LoadObj(shapes, materials, err, inputfile.c_str(), "effects\\res\\crytek_sponza\\");
    if (!err.empty()) { printf("%s\n",err); }
    if (!ret) { exit(1); }
    for (size_t i = 0; i < materials.size(); i++)
    {
        printf("material[%ld].name = %s\n", i, materials[i].name.c_str());
        printf("  material.map_Kd = %s\n", materials[i].diffuse_texname.c_str());
    }
    perfMarkerEnd();

    perfMarkerStartEx("Buidling OGL data", 0xFF8888FF);
    for (size_t i = 0; i < shapes.size(); i++)
    {
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
    program::use(prog);
}

void RenderApp()
{
    perfMarkerStart("Frame");
    glUniformMatrix4fv(mvploc, 1, GL_FALSE, mvpMat.e);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (size_t i = 0; i < 20; i++)
    {
        mesh::draw(meshes[i]);
    }
    perfMarkerEnd();
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