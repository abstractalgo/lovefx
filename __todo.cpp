typedef GLuint LFX_fullscreen_quad;

typedef struct LFX_rendertarget_tag
{
    GLuint fbo, rb, col0, col1, col2, col3;
} LFX_rendertarget;

typedef struct LFX_blur_pass_tag
{
private:
    GLuint blur_program;
    GLuint uloc_tex, uloc_resolution, uloc_filter;
public:
    GLint res[2];
    GLuint from, to;
    LFX_fullscreen_quad quad;
    float xy[2];
} LFX_blur_pass;

typedef struct LFX_vignette_pass_tag
{
private:
    GLuint program;
    GLuint uloc_tex;
public:
    LFX_fullscreen_quad quad;
    GLuint from, to;
} LFX_vignette_pass;

typedef struct LFX_gbuffer_pass_tag {} LFX_gbuffer_pass;
typedef struct LFX_deferred_light_pass_tag {} LFX_deferred_light_pass;

typedef struct LFX_deferred_pipeline_tag
{
private:
    LFX_rendertarget GbufferRT, finalRT;
    LFX_gbuffer_pass gbufferPass;
    LFX_deferred_light_pass finalPass;
public:
    Scene* scene;
} LFX_deferred_pipeline;

namespace lovefx
{
    namespace rt
    {
        // rendertarget
        void init(LFX_rendertarget rt, GLint width, GLint height, GLuint col_cnt=1);
        void destroy(LFX_rendertarget rt);
    }
    namespace pp
    {
        // blur
        void init(LFX_blur_pass bp, LFX_fullscreen_quad quad=0);
        void draw(LFX_blur_pass bp);
        void destroy(LFX_blur_pass bp);

        // vignette
        void init(LFX_vignette_pass vp, LFX_fullscreen_quad quad=0);
        void draw(LFX_vignette_pass vp);
        void destroy(LFX_vignette_pass vp);
    }
    namespace pipeline
    {
        // 
        void init(LFX_deferred_pipeline dp);
        void draw(LFX_deferred_pipeline dp);
        void destroy(LFX_deferred_pipeline dp);
    }
}

// ------------------------------------------------ VAR ------------------------
LFX_rendertarget sceneRT, blurHorRT, blurVertRT;
LFX_blur_pass blurPass;
LFX_vignette_pass vignettePass;
// ------------------------------------------------ INIT -----------------------
// init rendertargets
GLint width, height;
lovefx::utils::getResolution(width, height);
lovefx::rt::init(sceneRT, width, height);
lovefx::rt::init(blurHorRT, width, height);
lovefx::rt::init(blurVertRT, width, height);

// init postprocessing
lovefx::pp::init(blurPass, /*using quad*/0);
blurPass.res        = {width, height};
lovefx::pp::init(vignettePass, /*using quad*/blurPass.quad);
vignettePass.from   = blurVertRT;
vignettePass.to     = 0;
// ------------------------------------------------ RENDER ---------------------
// draw scene
lovefx::utils::drawSampleScene();

// horizontal blur
blurPass.from      = sceneRT.col0;
blurPass.to        = blurHorRT;
blurPass.xy        = {1,0};
lovefx::pp::draw(blurPass);

// vertical blur
blurPass.from      = blurHorRT.col0;
blurPass.to        = blurVertRT;
blurPass.xy        = {0,1};
lovefx::pp::draw(blurPass);

// vignette pass
lovefx::pp::draw(vignettePass);
// ------------------------------------------------ END ------------------------
lovefx::rt::destroy(sceneRT);
lovefx::rt::destroy(blurHorRT);
lovefx::rt::destroy(blurVertRT);
lovefx::pp::destroy(blurPass);
lovefx::pp::destroy(vignettePass);
// -----------------------------------------------------------------------------