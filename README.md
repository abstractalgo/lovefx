# lovefx
collection of opengl effects

###camera effects:
- [blur, glow](https://github.com/AbstractAlgorithm/lovefx/tree/master/examples/002_blur)
- bloom
- [vignette & filmgrain](https://github.com/AbstractAlgorithm/lovefx/tree/master/examples/005_vignette)
- dof
- lens flare
- motion blur
- anti-aliasing
- color grading & tonemapping & hdr

###object effects:
- refractive, diamond rendering
- pbr (glossy)
- cell shading, stroke
- local reflections
- sss

###lighting effects:
- shadows
- ssao
- godrays, volumetric fog
- gi

###other:
- parallax mapping
- object picking
- deferred decals
- compute
- instancing (perf)
- pipelines

--------

- mt system
- rendering backend abstraction
    - api
        - opengl
        - d3d
        - vulkan
    - shader compilation
- rendering thread queue

- model loading
- transformations
- event system
- clustered forward pipeline + deferred for post-processing
