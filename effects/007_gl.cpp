#include "../startupfx.h"
#if (STARTUPFX_ID==7)
#include "../lovefx.hpp"
#include "../thirdparty/glm/glm.hpp"
#include "../thirdparty/glm/gtc/matrix_transform.hpp"
#include "../thirdparty/glm/gtc/type_ptr.hpp"
#include "../thirdparty/SOIL.h"

// ----------------------------------------------------------------------------- VARIABLES

// time
float time = 0, delta = 0;
// env cube
struct
{
    GLuint cubemap, geom, program;
    GLint uloc_skybox, uloc_view, uloc_proj;
} env;
void setupEnvCube();
void renderEnvCube();
void destroyEnvCube();
// fulscreen quad
GLuint fsquad;
// camera
struct
{
    glm::mat4 view, proj;
    glm::vec3 dir, position;
    float yaw, pitch;
    bool fwdMv, bckMv, leftMv, rightMv;
    float moveSpeed, rotateSpeed;
    float lastX, lastY;
    bool firstMouse;
    float sensitivity;
    float fov;
} cam;
void updateCamera(unsigned int x, unsigned int y);
void updateCameraMatrix();

// ----------------------------------------------------------------------------- EVENT HANDLERS

void h_resize(unsigned int x, unsigned int y)
{
    lovefx::utils::setResolution(x, y);
    float ar; lovefx::utils::getAspectRatio(ar);
    cam.proj = glm::perspective(cam.fov, ar, 0.1f, 100.0f);
}
void h_mouseMove(int a, int b, unsigned int x, unsigned int y)
{
    static int q = 0;
    printf("mm: %d\n", q++);
    updateCamera(x, y);
}
void h_keyUp(unsigned int k)
{
    switch (k)
    {
    case INPUT_KEY_ESC:
        PostQuitMessage(0);
        break;
    case INPUT_KEY_W:
        cam.fwdMv = false;
        break;
    case INPUT_KEY_S:
        cam.bckMv = false;
        break;
    case INPUT_KEY_A:
        cam.leftMv = false;
        break;
    case INPUT_KEY_D:
        cam.rightMv = false;
        break;
    }
}
void h_keyDown(unsigned int k)
{
    static int q = 0;
    printf("kd: %d\n", q++);
    switch (k)
    {
    case INPUT_KEY_W:
        cam.fwdMv = true;
        cam.bckMv = false;
        break;
    case INPUT_KEY_S:
        cam.bckMv = true;
        cam.fwdMv = false;
        break;
    case INPUT_KEY_A:
        cam.leftMv = true;
        cam.rightMv = false;
        break;
    case INPUT_KEY_D:
        cam.rightMv = true;
        cam.leftMv = false;
        break;
    }
}

// ----------------------------------------------------------------------------- VARIOUS FUNCTIONS

void setupEnvCube()
{
    // env cubemap
    glGenTextures(1, &env.cubemap);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, env.cubemap);
    
    const char* faces[] =
    {
        "effects\\res\\sf_skybox\\posx.bmp",
        "effects\\res\\sf_skybox\\negx.bmp",
        "effects\\res\\sf_skybox\\posy.bmp",
        "effects\\res\\sf_skybox\\negy.bmp",
        "effects\\res\\sf_skybox\\posz.bmp",
        "effects\\res\\sf_skybox\\negz.bmp"
    };
    int width, height;
    unsigned char* image;
    for (GLuint i = 0; i < 6; i++)
    {
        image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        delete[] image;
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // env shader
    const char* vs = LFX_GLSL(330,
        layout(location = 0) in vec3 position;
        out vec3 TexCoords;
        uniform mat4 view;
        uniform mat4 proj;
        void main()
        {
            vec4 pos = proj * view * vec4(position, 1.0);
            //pos.xyz = pos.xyz * 0.2;
            gl_Position = pos.xyww;
            TexCoords = position;
        }
    );
    const char* fs = LFX_GLSL(330,
        in vec3 TexCoords;
        out vec4 color;

        uniform samplerCube skybox;

        void main()
        {
            color = texture(skybox, vec3(TexCoords.x, TexCoords.y, TexCoords.z));
        }
    );
    lovefx::program::createFromSource(env.program, vs, 0, 0, 0, fs, 0);
    lovefx::program::log(env.program);
    lovefx::program::location(env.program, "skybox", env.uloc_skybox);
    lovefx::program::location(env.program, "view", env.uloc_view);
    lovefx::program::location(env.program, "proj", env.uloc_proj);

    // env cube geometry
    const GLfloat verts[3*3*2*6] = { -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f };
    glGenBuffers(1, &env.geom);
    glBindBuffer(GL_ARRAY_BUFFER, env.geom);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    LFX_ERRCHK();
}
void renderEnvCube()
{
    // setup depth test nicely
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);
    // use shader
    lovefx::program::use(env.program);
    // uniforms
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, env.cubemap);
    glUniform1i(env.uloc_skybox, 0);
    glm::mat4 view_noTranslate = cam.view;
    view_noTranslate[3][0] = 0.0f;
    view_noTranslate[3][1] = 0.0f;
    view_noTranslate[3][2] = 0.0f;
    glUniformMatrix4fv(env.uloc_view, 1, GL_FALSE, glm::value_ptr(view_noTranslate));
    glUniformMatrix4fv(env.uloc_proj, 1, GL_FALSE, glm::value_ptr(cam.proj));
    // render cube's vao
    glBindBuffer(GL_ARRAY_BUFFER, env.geom);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // depth reset
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
    LFX_ERRCHK();
}
void destroyEnvCube()
{
    glDeleteTextures(1, &env.cubemap);
    lovefx::program::destroy(env.program);
    glDeleteBuffers(1, &env.geom);
    LFX_ERRCHK();
}

void setupCamera()
{
    float ar; lovefx::utils::getAspectRatio(ar);
    cam.fov = 45.0f;
    cam.yaw = 0.0f;
    cam.pitch = 0.0f;
    cam.moveSpeed = 2.2f;
    cam.sensitivity = 0.25f;
    cam.position = glm::vec3(0, 0, 0);
    cam.dir = glm::vec3(0, 0, 1);
    
    cam.fwdMv = cam.bckMv = cam.rightMv = cam.leftMv = false;
    cam.firstMouse = true;
    cam.proj = glm::perspective(cam.fov, ar, 0.1f, 100.0f);
    int w,h;
    lovefx::utils::getResolution(w,h);
    cam.lastX = w/2.0f;
    cam.lastY /= h/2.0f;
    // set cursor TODO
}
void updateCamera(unsigned int x, unsigned int y)
{
    if (cam.firstMouse)
    {
        cam.lastX = x;
        cam.lastY = y;
        cam.firstMouse = false;
    }

    GLfloat xoffset = (float)x - cam.lastX;
    GLfloat yoffset = cam.lastY - (float)y;

    cam.lastX = x;
    cam.lastY = y;

    // update angles
    cam.yaw += xoffset * cam.sensitivity;
    cam.pitch += yoffset * cam.sensitivity;

    // clamp pitch
    if (cam.pitch> 89.0f)
        cam.pitch = 89.0f;
    if (cam.pitch < -89.0f)
        cam.pitch = -89.0f;

    // update vectors
    glm::vec3 front;
    front.x = cos(glm::radians(cam.yaw)) * cos(glm::radians(cam.pitch));
    front.y = sin(glm::radians(cam.pitch));
    front.z = sin(glm::radians(cam.yaw)) * cos(glm::radians(cam.pitch));
    cam.dir = glm::normalize(front);
    updateCameraMatrix();
}
void updateCameraMatrix()
{
    glm::vec3 right = glm::cross(cam.dir, glm::vec3(0.0f, 1.0f, 0.0f));
    // update position
    if (cam.fwdMv) { cam.position += cam.dir * cam.moveSpeed * delta; }
    else if (cam.bckMv) { cam.position -= cam.dir * cam.moveSpeed * delta; }
    if (cam.rightMv) { cam.position += right * cam.moveSpeed * delta; }
    else if (cam.leftMv) { cam.position -= right * cam.moveSpeed * delta; }
    // update view matrix
    glm::vec3 target = cam.position+cam.dir;
    cam.view = glm::lookAt(cam.position, target, glm::vec3(0.0f, 1.0f, 0.0f));
}

// ----------------------------------------------------------------------------- MAIN APP

void InitApp()
{
    // setup handlers
    INPUT_f_mouseMove(h_mouseMove);
    INPUT_f_keyDown(h_keyDown);
    INPUT_f_keyUp(h_keyUp);
    INPUT_f_resize(h_resize);
    // setup timer
    lovefx::timer::init();
    // setup fullscreen quad
    lovefx::utils::initFSQuad(fsquad);
    // setup cam
    setupCamera();
    // setup env cube
    setupEnvCube();
}

void RenderApp()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // update timer
    lovefx::timer::update();
    delta = lovefx::timer::DIFF*0.000001f;
    time += delta;
    // update camera
    updateCameraMatrix();
    // render env cube
    renderEnvCube();
}

void CleanupApp()
{
    destroyEnvCube(); // destroy env cube
    lovefx::utils::destroyFSQuad(fsquad); // delete fullscreen quad
}

#endif