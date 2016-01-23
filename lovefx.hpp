#pragma once

#include "backend.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <hash_map>

#define LFX_GLSL(version, shader)  "#version " #version "\n" #shader
#define LFX_ERRCHK(glFn) \
do { \
glFn; \
GLenum err = glGetError(); \
const GLubyte* errmsg = gluErrorString(err); \
if (err != GL_NO_ERROR)    \
    printf("ERROR: 0x%x (%s)\n%s : %d\n", err, errmsg, __FILE__, __LINE__); \
} while (0)

typedef GLuint LFXshader;
typedef GLuint LFXprogram;
typedef GLuint LFXfbo;
typedef GLuint LFXrb;
typedef GLuint LFXtex2d;
typedef GLuint LFXtex2d;
typedef GLuint LFX_quad;

typedef struct LFX_rt_tag
{
    GLuint fbo, rb, col0, col1, col2, col3;
} LFX_rt;

typedef struct LFX_blur_pass_tag
{
    GLuint program;
    GLint uloc_tex, uloc_resolution, uloc_filter;
    LFX_quad quad;
    GLuint uTex;
    float uXY[2];
    GLint uRes[2];
} LFX_blur_pass;

typedef struct LFX_vignette_pass_tag
{
    enum UNIFORMS {TEX, CNT};
    GLint location[CNT];
    GLuint program;
    GLint uloc_tex;
    LFX_quad quad;
    GLuint uTex;
} LFX_vignette_pass;

typedef struct LFX_filmgrain_pass_tag
{
private:
    GLuint program;
    GLint uloc_tex, uloc_tSize, uloc_colored, uloc_coloramount, uloc_grainsize, uloc_lumamount, uloc_grainamount, uloc_timer;
public:
    LFX_quad quad;
    GLuint from, to;
    float uColored = 1;
    float uColorAmount = 0.25;
    float uGrainSize = 1.72;
    float uLumAmount = 0.64;
    float uGrainAmount = 0.1;
    float uTimer = 0;
} LFX_filmgrain_pass;



namespace lovefx
{
    namespace file
    {
        HRESULT load(const char* filename, std::string& content);
        HRESULT loadBMP(const char* filename, char*& data);
        //HRESULT loadPNG(const char* filename, char*& data);
    }
    namespace shader
    {
        void createFromSource(GLuint& shdr, GLenum type, const char* source);
        void createFromFile(GLuint& shdr, GLenum type, const char* filename);
        void log(GLuint shdr);
        void destroy(GLuint& shdr);
    }
    namespace program
    {
        void create(GLuint& pr, GLuint vs, GLuint ts, GLuint es, GLuint gs, GLuint fs, GLuint cs);
        void createFromSource(GLuint& pr, const char* vs, const char* ts, const char* es, const char* gs, const char* fs, const char* cs);
        void createFromFiles(GLuint& pr, const char* vs, const char* ts, const char* es, const char* gs, const char* fs, const char* cs);
        void log(GLuint pr);
        void location(GLuint pr, const char* name, GLint& loc);
        void use(GLuint pr);
        void destroy(GLuint& pr);
    }
    namespace texture2d
    {
        void create(GLuint& tex, GLsizei width, GLsizei height, GLvoid* data, GLenum type = GL_UNSIGNED_BYTE, GLenum minFilter = GL_NEAREST, GLenum magFilter = GL_NEAREST, GLint intFormat = GL_RGBA, GLenum format = GL_RGBA, GLenum wrapS = GL_REPEAT, GLenum wrapT = GL_REPEAT);
        void destroy(GLuint& tex);
    }
    namespace rb
    {
        void create(GLuint& rb, GLsizei width, GLsizei height, GLenum format = GL_DEPTH_COMPONENT16);
        void destroy(GLuint& rb);
    }
    namespace fbo
    {
        void create(GLuint& fbo);
        void create(GLuint& fbo, GLuint rb, GLuint color0, GLuint color1 = 0, GLuint color2 = 0, GLuint color3 = 0);
        void create(GLsizei width, GLsizei height, GLuint& fbo, GLuint& rb, GLuint& color0);
        void create(GLsizei width, GLsizei height, GLuint& fbo, GLuint& rb, GLuint& color0, GLuint& color1);
        void create(GLsizei width, GLsizei height, GLuint& fbo, GLuint& rb, GLuint& color0, GLuint& color1, GLuint& color2);
        void create(GLsizei width, GLsizei height, GLuint& fbo, GLuint& rb, GLuint& color0, GLuint& color1, GLuint& color2, GLuint& color3);
        void attachments(GLuint fbo, GLuint rb, GLuint color0, GLuint color1=0, GLuint color2=0, GLuint color3=0);
        void bind(GLenum target, GLuint fbo=0);
        void use(GLuint fbo=0);
        void bind_clear(GLuint fbo, GLbitfield mask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        void destroy(GLuint& fbo);
        void destroy(GLuint& fbo, GLuint& rb, GLuint& color0);
        void destroy(GLuint& fbo, GLuint& rb, GLuint& color0, GLuint& color1);
        void destroy(GLuint& fbo, GLuint& rb, GLuint& color0, GLuint& color1, GLuint& color2);
        void destroy(GLuint& fbo, GLuint& rb, GLuint& color0, GLuint& color1, GLuint& color2, GLuint& color3);
    }
    namespace rt
    {
        void create(LFX_rt& rt, GLint width, GLint height, GLuint col_cnt = 1);
        void destroy(LFX_rt& rt);
    }
    namespace pp
    {
        // blur
        void create(LFX_blur_pass& bp, LFX_quad quad = 0);
        void draw(LFX_blur_pass& bp);
        void destroy(LFX_blur_pass& bp);

        // vignette
        void create(LFX_vignette_pass& vp, LFX_quad = 0);
        void draw(LFX_vignette_pass& vp);
        void destroy(LFX_vignette_pass& vp);

        // filmgrain
        void create(LFX_filmgrain_pass& fp, LFX_quad = 0);
        void draw(LFX_filmgrain_pass& fp);
        void destroy(LFX_filmgrain_pass& fp);
    }
    namespace utils
    {
        void getResolution(GLint& w, GLint& h);
        void setResolution(GLsizei w, GLsizei h);
        void getAspectRatio(float& ar);
        void initFSQuad(GLuint& quad);
        void drawFSQuad(GLuint quad);
        void destroyFSQuad(GLuint& quad);
        void drawTris2f(GLfloat* coord, int n);
    }
    namespace timer
    {
        static LARGE_INTEGER qpFrequency;
        static LARGE_INTEGER prevFrameTime, nowTime;
        static long long unsigned FRAMECNT;
        extern double DIFF;
        extern double FPS;
        void init();
        void update();
        long long unsigned getFrame();
    }
}

HRESULT lovefx::file::load(const char* filename, std::string& content)
{
    if (!filename)
        return E_FAIL;

    FILE* file;
    if (!(file = fopen(filename, "r")))
        return E_FAIL;

    int n = 0;
    char *str = (char*)malloc(sizeof(char));
    if (NULL == str)
        return E_FAIL;

    char c;
    while (true)
    {
        c = fgetc(file);
        str[n] = c;
        n++;
        str = (char*)realloc(str, sizeof(char)* n + sizeof(char));
        if (NULL == str)
            return E_FAIL;
        if (c == EOF)
            break;
    }
    str[n - 1] = '\0';
    fclose(file);

    content = (str);
    return S_OK;
}
HRESULT lovefx::file::loadBMP(const char* filename, char*& data)
{
    unsigned char header[54];
    unsigned int dataPos;
    unsigned int width, height;
    unsigned int imageSize;
    data = 0;

    // Open the file
    FILE * file = fopen(filename, "rb");
    if (!file)
        return E_FAIL;
    if (fread(header, 1, 54, file) != 54)
        return E_FAIL;

    if (header[0] != 'B' || header[1] != 'M')
        return E_FAIL;

    // Read ints from the byte array
    dataPos     = *(int*)&(header[0x0A]);
    imageSize   = *(int*)&(header[0x22]);
    width       = *(int*)&(header[0x12]);
    height      = *(int*)&(header[0x16]);

    if (imageSize == 0)    imageSize = width*height * 3;    // 3 : one byte for each Red, Green and Blue component
    if (dataPos == 0)      dataPos = 54;                    // The BMP header is done that way
    data = new char[imageSize];
    if (!data)
        return E_FAIL;
    fread(data, 1, imageSize, file);
    fclose(file);

    return S_OK;
}

void lovefx::shader::createFromSource(GLuint& shdr, GLenum type, const char* source)
{
    shdr = glCreateShader(type);
    glShaderSource(shdr, 1, &source, NULL);
    glCompileShader(shdr);
}
void lovefx::shader::createFromFile(GLuint& shdr, GLenum type, const char* filename)
{
    std::string source;
    lovefx::file::load(filename, source);
    lovefx::shader::createFromSource(shdr, type, source.c_str());
}
void lovefx::shader::log(GLuint shdr)
{
    int infologLen = 0;
    int charsWritten = 0;
    GLchar *infoLog = NULL;
    glGetShaderiv(shdr, GL_INFO_LOG_LENGTH, &infologLen);
    if (infologLen > 0)
    {
        infoLog = (GLchar*)malloc(infologLen);
        if (infoLog == NULL)
            return;
        glGetShaderInfoLog(shdr, infologLen, &charsWritten, infoLog);
    }
    printf("%s", infoLog);
    delete[] infoLog;
}
void lovefx::shader::destroy(GLuint& shdr)
{
    glDeleteShader(shdr);
    shdr = 0;
}

void lovefx::program::create(GLuint& pr, GLuint vs, GLuint ts, GLuint es, GLuint gs, GLuint fs, GLuint cs)
{
    pr = glCreateProgram();
    if (vs) glAttachShader(pr, vs);
    if (ts) glAttachShader(pr, ts);
    if (es) glAttachShader(pr, es);
    if (gs) glAttachShader(pr, gs);
    if (fs) glAttachShader(pr, fs);
    if (cs) glAttachShader(pr, cs);
    glLinkProgram(pr);
}
void lovefx::program::createFromSource(GLuint& pr, const char* vs, const char* ts, const char* es, const char* gs, const char* fs, const char* cs)
{
    GLuint _vs, _ts, _es, _gs, _fs, _cs;
    if (vs) shader::createFromSource(_vs, GL_VERTEX_SHADER, vs);          else _vs = 0;
    if (ts) shader::createFromSource(_ts, GL_TESS_CONTROL_SHADER, ts);    else _ts = 0;
    if (es) shader::createFromSource(_es, GL_TESS_EVALUATION_SHADER, es); else _es = 0;
    if (gs) shader::createFromSource(_gs, GL_GEOMETRY_SHADER, gs);        else _gs = 0;
    if (fs) shader::createFromSource(_fs, GL_FRAGMENT_SHADER, fs);        else _fs = 0;
    if (cs) shader::createFromSource(_cs, GL_COMPUTE_SHADER, cs);         else _cs = 0;
    lovefx::program::create(pr, _vs, _ts, _es, _gs, _fs, _cs);
    shader::destroy(_vs);
    shader::destroy(_ts);
    shader::destroy(_es);
    shader::destroy(_gs);
    shader::destroy(_fs);
    shader::destroy(_cs);
}
void lovefx::program::createFromFiles(GLuint& pr, const char* vs, const char* ts, const char* es, const char* gs, const char* fs, const char* cs)
{
    GLuint _vs, _ts, _es, _gs, _fs, _cs;
    if (vs) shader::createFromFile(_vs, GL_VERTEX_SHADER, vs);          else _vs = 0;
    if (ts) shader::createFromFile(_ts, GL_TESS_CONTROL_SHADER, ts);    else _ts = 0;
    if (es) shader::createFromFile(_es, GL_TESS_EVALUATION_SHADER, es); else _es = 0;
    if (gs) shader::createFromFile(_gs, GL_GEOMETRY_SHADER, gs);        else _gs = 0;
    if (fs) shader::createFromFile(_fs, GL_FRAGMENT_SHADER, fs);        else _fs = 0;
    if (cs) shader::createFromFile(_cs, GL_COMPUTE_SHADER, cs);         else _cs = 0;
    lovefx::program::create(pr, _vs, _ts, _es, _gs, _fs, _cs);
    shader::destroy(_vs);
    shader::destroy(_ts);
    shader::destroy(_es);
    shader::destroy(_gs);
    shader::destroy(_fs);
    shader::destroy(_cs);
}
void lovefx::program::log(GLuint pr)
{
    int infologLen = 0;
    int charsWritten = 0;
    GLchar *infoLog = NULL;
    glGetProgramiv(pr, GL_INFO_LOG_LENGTH, &infologLen);
    if (infologLen > 0)
    {
        infoLog = (GLchar*)malloc(infologLen);
        if (infoLog == NULL)
            return;
        glGetProgramInfoLog(pr, infologLen, &charsWritten, infoLog);
    }
    printf("%s", infoLog);
    delete[] infoLog;
}
void lovefx::program::location(GLuint pr, const char* name, GLint& loc)
{
    loc = glGetUniformLocation(pr, name);
}
void lovefx::program::use(GLuint pr)
{
    glUseProgram(pr);
}
void lovefx::program::destroy(GLuint& pr)
{
    glDeleteProgram(pr);
    pr = 0;
}

void lovefx::texture2d::create(GLuint& tex, GLsizei width, GLsizei height, GLvoid* data, GLenum type, GLenum minFilter, GLenum magFilter, GLint intFormat, GLenum format, GLenum wrapS, GLenum wrapT)
{
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    glTexImage2D(GL_TEXTURE_2D, 0, intFormat, width, height, 0, format, type, data);
    glBindTexture(GL_TEXTURE_2D, 0);
}
void lovefx::texture2d::destroy(GLuint& tex)
{
    glDeleteTextures(1, &tex);
    tex = 0;
}

void lovefx::rb::create(GLuint& rb, GLsizei width, GLsizei height, GLenum format)
{
    glGenRenderbuffers(1, &rb);
    glBindRenderbuffer(GL_RENDERBUFFER, rb);
    glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
void lovefx::rb::destroy(GLuint& rb)
{
    glDeleteRenderbuffers(1, &rb);
    rb = 0;
}

void lovefx::fbo::create(GLuint& fbo)
{
    glGenFramebuffers(1, &fbo);
}
void lovefx::fbo::create(GLuint& fbo, GLuint rb, GLuint color0, GLuint color1, GLuint color2, GLuint color3)
{
    glGenFramebuffers(1, &fbo);
    lovefx::fbo::attachments(fbo, rb, color0, color1, color2, color3);
}
void lovefx::fbo::create(GLsizei width, GLsizei height, GLuint& fbo, GLuint& rb, GLuint& color0)
{
    glGenFramebuffers(1, &fbo);
    rb::create(rb, width, height);
    texture2d::create(color0, width, height, 0);
    fbo::attachments(fbo, rb, color0);
}
void lovefx::fbo::create(GLsizei width, GLsizei height, GLuint& fbo, GLuint& rb, GLuint& color0, GLuint& color1)
{
    glGenFramebuffers(1, &fbo);
    rb::create(rb, width, height);
    texture2d::create(color0, width, height, 0);
    texture2d::create(color1, width, height, 0);
    fbo::attachments(fbo, rb, color0, color1);
}
void lovefx::fbo::create(GLsizei width, GLsizei height, GLuint& fbo, GLuint& rb, GLuint& color0, GLuint& color1, GLuint& color2)
{
    glGenFramebuffers(1, &fbo);
    rb::create(rb, width, height);
    texture2d::create(color0, width, height, 0);
    texture2d::create(color1, width, height, 0);
    texture2d::create(color2, width, height, 0);
    fbo::attachments(fbo, rb, color0, color1, color2);
}
void lovefx::fbo::create(GLsizei width, GLsizei height, GLuint& fbo, GLuint& rb, GLuint& color0, GLuint& color1, GLuint& color2, GLuint& color3)
{
    glGenFramebuffers(1, &fbo);
    rb::create(rb, width, height);
    texture2d::create(color0, width, height, 0);
    texture2d::create(color1, width, height, 0);
    texture2d::create(color2, width, height, 0);
    texture2d::create(color3, width, height, 0);
    fbo::attachments(fbo, rb, color0, color1, color2, color3);
}
void lovefx::fbo::attachments(GLuint fbo, GLuint rb, GLuint color0, GLuint color1, GLuint color2, GLuint color3)
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rb);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color0, 0);
    if (color1) glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, color1, 0);
    if (color2) glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, color2, 0);
    if (color3) glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, color3, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void lovefx::fbo::bind(GLenum target, GLuint fbo)
{
    glBindFramebuffer(target, fbo);
}
void lovefx::fbo::use(GLuint fbo)
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}
void lovefx::fbo::bind_clear(GLuint fbo, GLbitfield mask)
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glClear(mask);
}
void lovefx::fbo::destroy(GLuint& fbo)
{
    lovefx::fbo::use(0);
    glDeleteFramebuffers(1, &fbo);
    fbo = 0;
}
void lovefx::fbo::destroy(GLuint& fbo, GLuint& rb, GLuint& color0)
{
    lovefx::fbo::use(0);
    glDeleteRenderbuffers(1, &rb);
    rb = 0;
    glDeleteTextures(1, &color0);
    color0 = 0;
    glDeleteFramebuffers(1, &fbo);
    fbo = 0;
}
void lovefx::fbo::destroy(GLuint& fbo, GLuint& rb, GLuint& color0, GLuint& color1)
{
    lovefx::fbo::use(0);
    glDeleteRenderbuffers(1, &rb);
    rb = 0;
    glDeleteTextures(1, &color0);
    color0 = 0;
    glDeleteTextures(1, &color1);
    color1 = 0;
    glDeleteFramebuffers(1, &fbo);
    fbo = 0;
}
void lovefx::fbo::destroy(GLuint& fbo, GLuint& rb, GLuint& color0, GLuint& color1, GLuint& color2)
{
    lovefx::fbo::use(0);
    glDeleteRenderbuffers(1, &rb);
    rb = 0;
    glDeleteTextures(1, &color0);
    color0 = 0;
    glDeleteTextures(1, &color1);
    color1 = 0;
    glDeleteTextures(1, &color2);
    color2 = 0;
    glDeleteFramebuffers(1, &fbo);
    fbo = 0;
}
void lovefx::fbo::destroy(GLuint& fbo, GLuint& rb, GLuint& color0, GLuint& color1, GLuint& color2, GLuint& color3)
{
    lovefx::fbo::use(0);
    glDeleteRenderbuffers(1, &rb);
    rb = 0;
    glDeleteTextures(1, &color0);
    color0 = 0;
    glDeleteTextures(1, &color1);
    color1 = 0;
    glDeleteTextures(1, &color2);
    color2 = 0;
    glDeleteTextures(1, &color3);
    color3 = 0;
    glDeleteFramebuffers(1, &fbo);
    fbo = 0;
}

void lovefx::rt::create(LFX_rt& rt, GLint width, GLint height, GLuint col_cnt)
{
    rt.col0 = rt.col1 = rt.col2 = rt.col3 = rt.fbo = rt.rb = 0;
    if (1 == col_cnt)
    {
        lovefx::fbo::create(width, height, rt.fbo, rt.rb, rt.col0);
    }
    else if (2 == col_cnt)
    {
        lovefx::fbo::create(width, height, rt.fbo, rt.rb, rt.col0, rt.col1);
    }
    else if (3 == col_cnt)
    {
        lovefx::fbo::create(width, height, rt.fbo, rt.rb, rt.col0, rt.col1, rt.col2);
    }
    else if (4 == col_cnt)
    {
        lovefx::fbo::create(width, height, rt.fbo, rt.rb, rt.col0, rt.col1, rt.col2, rt.col3);
    }
}
void lovefx::rt::destroy(LFX_rt& rt)
{
    lovefx::fbo::destroy(rt.fbo, rt.rb, rt.col0, rt.col1, rt.col2, rt.col3);
}

// blur
void lovefx::pp::create(LFX_blur_pass& bp, LFX_quad quad)
{
    program::createFromFiles(bp.program, "..\\..\\res\\shaders\\fs.vs", 0, 0, 0, "..\\..\\res\\shaders\\blur_separate_g11.fs", 0);
    program::log(bp.program);
    program::location(bp.program, "tex", bp.uloc_tex);
    program::location(bp.program, "uRes", bp.uloc_resolution);
    program::location(bp.program, "uFilter", bp.uloc_filter);
    bp.quad = quad;
    if (!bp.quad)
        utils::initFSQuad(bp.quad);
}
void lovefx::pp::draw(LFX_blur_pass& bp)
{
    program::use(bp.program);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, bp.uTex);
    glUniform1i(bp.uloc_tex, 0);
    glUniform2f(bp.uloc_resolution, (float)bp.uRes[0], (float)bp.uRes[1]);
    glUniform2f(bp.uloc_filter, bp.uXY[0], bp.uXY[1]);
    lovefx::utils::drawFSQuad(bp.quad);
}
void lovefx::pp::destroy(LFX_blur_pass& bp)
{
    program::destroy(bp.program);
    utils::destroyFSQuad(bp.quad);
}
// vignette
void lovefx::pp::create(LFX_vignette_pass& vp, LFX_quad quad)
{
    program::createFromFiles(vp.program, "..\\..\\res\\shaders\\fs.vs", 0, 0, 0, "..\\..\\res\\shaders\\vignette.fs", 0);
    lovefx::program::location(vp.program, "tex", vp.location[vp.TEX]);
    vp.quad = quad;
    if (!vp.quad)
        utils::initFSQuad(vp.quad);
}
void lovefx::pp::draw(LFX_vignette_pass& vp)
{
    program::use(vp.program);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUniform1i(vp.location[vp.TEX], 0);
    lovefx::utils::drawFSQuad(vp.quad);
}
void lovefx::pp::destroy(LFX_vignette_pass& vp)
{

}
// filmgrain
void lovefx::pp::create(LFX_filmgrain_pass& fp, LFX_quad)
{

}
void lovefx::pp::draw(LFX_filmgrain_pass& fp)
{

}
void lovefx::pp::destroy(LFX_filmgrain_pass& fp)
{

}

void lovefx::utils::getResolution(GLint& w, GLint& h)
{
    GLint viewportDims[4];
    glGetIntegerv(GL_VIEWPORT, viewportDims);
    w = viewportDims[2];
    h = viewportDims[3];
}
void lovefx::utils::setResolution(GLsizei w, GLsizei h)
{
    glViewport(0, 0, w, h);
}
void lovefx::utils::getAspectRatio(float& ar)
{
    GLint viewportDims[4];
    glGetIntegerv(GL_VIEWPORT, viewportDims);
    ar = (float)viewportDims[2] / (float)viewportDims[3];
}
void lovefx::utils::initFSQuad(GLuint& quad)
{
    GLfloat verts[12] = { -1, -1, 1, 1, -1, 1, -1, -1, 1, -1, 1, 1 };
    glGenBuffers(1, &quad);
    glBindBuffer(GL_ARRAY_BUFFER, quad);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void lovefx::utils::drawFSQuad(GLuint quad)
{
    glBindBuffer(GL_ARRAY_BUFFER, quad);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
void lovefx::utils::destroyFSQuad(GLuint& quad)
{
    glDeleteBuffers(1, &quad);
    quad = 0;
}
void lovefx::utils::drawTris2f(GLfloat* coord, int n)
{
    glUseProgram(0);
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < 3*n; i++)
    {
        glVertex2f(coord[2 * i], coord[2 * i + 1]);
    }
        
    glEnd();
}

void lovefx::timer::init()
{
    QueryPerformanceFrequency(&qpFrequency);
    QueryPerformanceCounter(&prevFrameTime);
    FRAMECNT = 0;
}
void lovefx::timer::update()
{
    QueryPerformanceCounter(&nowTime);
    LARGE_INTEGER elapsed;
    elapsed.QuadPart = nowTime.QuadPart - prevFrameTime.QuadPart;
    elapsed.QuadPart *= 1000000;
    elapsed.QuadPart /= qpFrequency.QuadPart;
    DIFF = (double)elapsed.QuadPart;
    FPS = 1000000.0 / DIFF;
    prevFrameTime = nowTime;
    FRAMECNT += 1;
}
long long unsigned lovefx::timer::getFrame()
{
    return FRAMECNT;
}
double lovefx::timer::DIFF = 1.0;
double lovefx::timer::FPS = 0.0;