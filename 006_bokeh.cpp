#include "..\\startupfx.h"
#include <iostream>
#if (STARTUPFX_ID==6)
#include "..\/lovefx.hpp"
#include "thirdparty\/tiny_obj_loader.hpp"
#include "thirdparty\/nv_dds.hpp"


//TwBar *myBar;
GLuint quad;
GLuint prog;
GLint texloc;
GLuint texobj;

using namespace lovefx;

void InitApp()
{
    //myBar = TwNewBar("Simple sample bar");
    glClearColor(0.0f, 0.2f, 0.4f, 1.0f);

    glGenTextures(1, &texobj);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texobj);

    CDDSImage image;
    image.load("effects\\res\\textures\\sponza_flagpole_spec.dds");
    image.upload_texture2D();
    /*bool d = image.is_valid();
    unsigned frmt = image.get_format();
    unsigned wid = image.get_width();
    unsigned heig = image.get_height();
    unsigned sz = image.get_size();*/
    
    // -------------------------------------------------------------------------
    /*glCompressedTexImage2DARB(GL_TEXTURE_2D, 0, image.get_format(),
        image.get_width(), image.get_height(), 0, image.get_size(),
        image);

    for (int i = 0; i < image.get_num_mipmaps(); i++)
    {
        CSurface mipmap = image.get_mipmap(i);
    
        glCompressedTexImage2DARB(GL_TEXTURE_2D, i+1, image.get_format(),
            mipmap.get_width(), mipmap.get_height(), 0, mipmap.get_size(),
            mipmap);
    }*/
    // -------------------------------------------------------------------------
    /*glTexImage2D(GL_TEXTURE_2D, 0, image.get_components(), image.get_width(),
        image.get_height(), 0, image.get_format(), GL_UNSIGNED_BYTE, image);*/

    /*for (int i = 0; i < image.get_num_mipmaps(); i++)
    {
        glTexImage2D(GL_TEXTURE_2D, i+1, image.get_components(),
            image.get_mipmap(i).get_width(), image.get_mipmap(i).get_height(),
            0, image.get_format(), GL_UNSIGNED_BYTE, image.get_mipmap(i));
    }*/
    // -------------------------------------------------------------------------

    lovefx::utils::initFSQuad(quad);
    program::createFromFiles(prog, "effects\\res\\shaders\\fs.vs", 0, 0, 0, "effects\\res\\shaders\\fs.fs", 0);
    program::location(prog, "tex", texloc);
    

    /*std::string inputfile = "effects\\res\\sponza.obj";
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string err;
    bool ret = tinyobj::LoadObj(shapes, materials, err, inputfile.c_str(), "effects\\res\\");

    if (!err.empty()) {  std::cerr << err << std::endl; }
    if (!ret) {  exit(1); }

    std::cout << "# of shapes    : " << shapes.size() << std::endl;
    std::cout << "# of materials : " << materials.size() << std::endl;

    for (size_t i = 0; i < shapes.size(); i++) {
        printf("shape[%ld].name = %s\n", i, shapes[i].name.c_str());
        printf("Size of shape[%ld].indices: %ld\n", i, shapes[i].mesh.indices.size());
        printf("Size of shape[%ld].material_ids: %ld\n", i, shapes[i].mesh.material_ids.size());
        assert((shapes[i].mesh.indices.size() % 3) == 0);
    }

    for (size_t i = 0; i < materials.size(); i++) {
        printf("material[%ld].name = %s\n", i, materials[i].name.c_str());
    }*/
}

void RenderApp()
{
    glClear(GL_COLOR_BUFFER_BIT);
    program::use(prog);
    glBindTexture(GL_TEXTURE_2D, texobj);
    glUniform1i(texloc, 0);
    utils::drawFSQuad(quad);
}

void CleanupApp()
{

}

#endif