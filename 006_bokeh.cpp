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
    
}

void RenderApp()
{
    
}

void CleanupApp()
{

}

void mainApp()
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
    //program::use(prog);
    //program::location(prog, "tex", texloc);

    std::string inputfile = "effects\\res\\crytek_sponza\\sponza.obj";

    std::string err;
    bool ret = tinyobj::LoadObj(shapes, materials, err, inputfile.c_str(), "effects\\res\\crytek_sponza\\");

    if (!err.empty()) { std::cerr << err << std::endl; }
    if (!ret) { exit(1); }

    std::cout << "# of shapes    : " << shapes.size() << std::endl;
    std::cout << "# of materials : " << materials.size() << std::endl;

    /*for (size_t i = 0; i < 20; i++) {
    printf("shape[%ld].name = %s\n", i, shapes[i].name.c_str());
    printf("Size of shape[%ld].indices: %ld\n", i, shapes[i].mesh.indices.size());
    printf("Size of shape[%ld].positions: %ld\n", i, shapes[i].mesh.positions.size()/3);
    assert((shapes[i].mesh.indices.size() % 3) == 0);
    }
    printf("done\n");*/

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(0);

    glBegin(GL_TRIANGLES);
    // for each shape
    for (size_t i = 0; i < 20; i++)
    {
        // for each triangle in shape
        for (size_t ti = 0; ti < shapes[i].mesh.indices.size() / 3; ti++)
        {
            float k = 0.001f;
            vertex a, b, c;

            a.id = shapes[i].mesh.indices[3 * ti + 0];
            b.id = shapes[i].mesh.indices[3 * ti + 1];
            c.id = shapes[i].mesh.indices[3 * ti + 2];

            a.x = shapes[i].mesh.positions[3 * a.id + 0] * k;
            a.y = shapes[i].mesh.positions[3 * a.id + 1] * k;
            a.z = shapes[i].mesh.positions[3 * a.id + 2] * k;
            a.n.x = shapes[i].mesh.normals[3 * a.id + 0];
            a.n.y = shapes[i].mesh.normals[3 * a.id + 1];
            a.n.z = shapes[i].mesh.normals[3 * a.id + 2];
            glColor3f(a.n.x, a.n.y, a.n.z);
            glVertex3f(a.x, a.y, a.z);

            b.x = shapes[i].mesh.positions[3 * b.id + 0] * k;
            b.y = shapes[i].mesh.positions[3 * b.id + 1] * k;
            b.z = shapes[i].mesh.positions[3 * b.id + 2] * k;
            b.n.x = shapes[i].mesh.normals[3 * b.id + 0];
            b.n.y = shapes[i].mesh.normals[3 * b.id + 1];
            b.n.z = shapes[i].mesh.normals[3 * b.id + 2];
            glColor3f(b.n.x, b.n.y, b.n.z);
            glVertex3f(b.x, b.y, b.z);

            c.x = shapes[i].mesh.positions[3 * c.id + 0] * k;
            c.y = shapes[i].mesh.positions[3 * c.id + 1] * k;
            c.z = shapes[i].mesh.positions[3 * c.id + 2] * k;
            c.n.x = shapes[i].mesh.normals[3 * c.id + 0];
            c.n.y = shapes[i].mesh.normals[3 * c.id + 1];
            c.n.z = shapes[i].mesh.normals[3 * c.id + 2];
            glColor3f(c.n.x, c.n.y, c.n.z);
            glVertex3f(c.x, c.y, c.z);
        }
    }
    glEnd();
    SwapBuffersBackend();

    //for (size_t i = 0; i < materials.size(); i++) {
    //    printf("material[%ld].name = %s\n", i, materials[i].name.c_str());
    //    //printf("  material.Kd = (%f, %f ,%f)\n", materials[i].diffuse[0], materials[i].diffuse[1], materials[i].diffuse[2]);
    //    printf("  material.map_Kd = %s\n", materials[i].diffuse_texname.c_str()); // diffuse
    //    printf("  material.Ns = %f\n", materials[i].shininess); // specular exponent
    //    printf("  material.map_Ks = %s\n", materials[i].specular_texname.c_str()); // specular
    //}
}

#endif