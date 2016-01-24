#include "..\\startupfx.h"
#include <iostream>
#if (STARTUPFX_ID==6)
#include "..\\lovefx.hpp"
#include "thirdparty\\tiny_obj_loader.hpp"
#include "thirdparty\\nv_dds.hpp"


//TwBar *myBar;
GLuint quad;
GLuint prog;
GLint texloc;
GLuint texobj;

GLuint* vaos;
std::vector<tinyobj::shape_t> shapes;
std::vector<tinyobj::material_t> materials;

using namespace lovefx;

struct v3
{
    float x, y, z;
};
struct vertex
{
    size_t id;
    float x, y, z;
    float u, v;
    v3 n;
};

void InitApp()
{
    //myBar = TwNewBar("Simple sample bar");
    glClearColor(0.0f, 0.2f, 0.4f, 1.0f);

    glGenTextures(1, &texobj);
    glBindTexture(GL_TEXTURE_2D, texobj);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    file::loadTGA("effects\\res\\sponza_details_diff.tga", GL_TEXTURE_2D, texobj);
    //file::loadBMP("effects\\res\\hehe.bmp", GL_TEXTURE_2D, texobj);

    //CDDSImage image;
    //image.load("effects\\res\\textures\\sponza_curtain_green_diff.dds");
    //image.upload_texture2D();

    //lovefx::utils::initFSQuad(quad);
    program::createFromFiles(prog, "effects\\res\\shaders\\sponza.vs", 0, 0, 0, "effects\\res\\shaders\\sponza.fs", 0);
    program::log(prog);
    //program::location(prog, "tex", texloc);

    std::string inputfile = "effects\\res\\crytek_sponza\\sponza.obj";

    std::string err;
    bool ret = tinyobj::LoadObj(shapes, materials, err, inputfile.c_str(), "effects\\res\\crytek_sponza\\");

    if (!err.empty()) { std::cerr << err << std::endl; }
    if (!ret) { exit(1); }

    std::cout << "# of shapes    : " << shapes.size() << std::endl;
    std::cout << "# of materials : " << materials.size() << std::endl;

    vaos = new GLuint[shapes.size()];
    glGenVertexArrays(shapes.size(), vaos);
    for (size_t i = 0; i < 20; i++)
    {
        glBindVertexArray(vaos[i]);

        GLuint vbo_pos;
        GLuint vbo_nor;
        GLuint vbo_uv;
        GLuint vbo_idx;

        glGenBuffers(1, &vbo_pos);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_pos);
        glBufferData(GL_ARRAY_BUFFER, shapes[i].mesh.positions.size() * sizeof(float), shapes[i].mesh.positions.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);
        glBindAttribLocation(prog, 0, "iPos");

        glGenBuffers(1, &vbo_nor);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_nor);
        glBufferData(GL_ARRAY_BUFFER, shapes[i].mesh.normals.size() * sizeof(float), shapes[i].mesh.normals.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(1);
        glBindAttribLocation(prog, 1, "iNor");

        glGenBuffers(1, &vbo_uv);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_uv);
        glBufferData(GL_ARRAY_BUFFER, shapes[i].mesh.texcoords.size() * sizeof(float), shapes[i].mesh.texcoords.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(2);
        glBindAttribLocation(prog, 2, "iUV");

        glGenBuffers(1, &vbo_idx);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_idx);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, shapes[i].mesh.indices.size() * sizeof(unsigned int), shapes[i].mesh.indices.data(), GL_STATIC_DRAW);
    }

    glUseProgram(prog);
}

void RenderApp()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (size_t i = 0; i < 20; i++)
    {
        glBindVertexArray(vaos[i]);

        glDrawElements(
            GL_TRIANGLES,                       // mode
            shapes[i].mesh.indices.size(),      // count
            GL_UNSIGNED_INT,                    // type
            (void*)0                            // element array buffer offset
            );
    }
}

void CleanupApp()
{

}

void mainApp()
{
}

#endif