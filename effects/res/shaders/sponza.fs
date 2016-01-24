#version 330

//uniform sampler2D diff_tex;
in vec2 uv;
in vec3 nor;

out vec4 frag;

void main()
{
    frag = vec4(1);
    //frag = texture(diff_tex, uv);
    frag = vec4(uv, 0.5, 1.);
}