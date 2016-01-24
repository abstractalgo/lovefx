#include "..\\startupfx.h"
#include <iostream>
#if (STARTUPFX_ID==6)
#include "..\\lovefx.hpp"
#include "..\\thirdparty\\tiny_obj_loader.hpp"


//TwBar*      myBar;
LFXprogram  prog;
LFXuloc     texloc;
LFXtex2d    texobj;
size_t      shape_cnt;
LFXmesh*    meshes;

using namespace lovefx;

void InitApp()
{
    PerfMarker("Init", 0xFFFF0000);
    //myBar = TwNewBar("Simple sample bar");

    glGenTextures(1, &texobj);
    glBindTexture(GL_TEXTURE_2D, texobj);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    file::loadTGA("effects\\res\\sponza_details_diff.tga", GL_TEXTURE_2D, texobj);
    //file::loadBMP("effects\\res\\hehe.bmp", GL_TEXTURE_2D, texobj);

    program::createFromFiles(prog, "effects\\res\\shaders\\sponza.vs", 0, 0, 0, "effects\\res\\shaders\\sponza.fs", 0);
    program::log(prog);
    //program::location(prog, "tex", texloc);

    perfMarkerStartEx("OBJ parsing", 0xFF88FF88);
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string inputfile = "effects\\res\\crytek_sponza\\sponza.obj";
    std::string err;
    bool ret = tinyobj::LoadObj(shapes, materials, err, inputfile.c_str(), "effects\\res\\crytek_sponza\\");
    if (!err.empty()) { std::cerr << err << std::endl; }
    if (!ret) { exit(1); }
    std::cout << "# of shapes    : " << shapes.size() << std::endl;
    std::cout << "# of materials : " << materials.size() << std::endl;
    for (size_t i = 0; i < materials.size(); i++)
    {
        printf("material[%ld].name = %s\n", i, materials[i].name.c_str());
        printf("  material.map_Kd = %s\n", materials[i].diffuse_texname.c_str());
    }
    perfMarkerEnd();

    perfMarkerStartEx("Buidling OGL data", 0xFF8888FF);
    shape_cnt = shapes.size();
    meshes = new LFXmesh[shape_cnt];
    for (size_t i = 0; i < shape_cnt; i++)
    {
        LFXmesh& mesh = meshes[i];
        mesh::create(mesh);
        mesh::addVertexBuffer(mesh, "iPos", shapes[i].mesh.positions.size() * sizeof(float), prog, 3, GL_FLOAT, shapes[i].mesh.positions.data());
        mesh::addVertexBuffer(mesh, "iNor", shapes[i].mesh.normals.size() * sizeof(float), prog, 3, GL_FLOAT, shapes[i].mesh.normals.data());
        mesh::addVertexBuffer(mesh, "iUV", shapes[i].mesh.texcoords.size() * sizeof(float), prog, 2, GL_FLOAT, shapes[i].mesh.texcoords.data());
        mesh::addIndexBuffer(mesh, shapes[i].mesh.indices.size() * sizeof(unsigned int), shapes[i].mesh.indices.data());
        mesh.count = shapes[i].mesh.indices.size();
    }
    perfMarkerEnd();

    glClearColor(0.0f, 0.2f, 0.4f, 1.0f);
    program::use(prog);
}

void RenderApp()
{
    perfMarkerStart("Frame");
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (size_t i = 0; i < 20; i++)
    {
        mesh::draw(meshes[i]);
    }
    perfMarkerEnd();
}

void CleanupApp()
{

}

void mainApp()
{
}

#endif