#version 330

//uniform sampler2D diff_tex;
in vec2 uv;
in vec3 nor;

out vec4 frag;

void main()
{
    frag = vec4(1);
    //frag = texture(diff_tex, uv);
    vec3 n = nor*0.5+vec3(.5);
    frag = vec4(n, 1.);
    frag = vec4(1);
}