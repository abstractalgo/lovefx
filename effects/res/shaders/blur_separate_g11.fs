#version 330

in vec2 uv;

uniform vec2 uFilter;   // x-delta, y-delta
uniform vec2 uRes;      // resolution
uniform sampler2D tex;

out vec4 fragCol;

// http://dev.theomader.com/gaussian-kernel-calculator/
const float weights[11] = {0.000003, 0.000229, 0.005977, 0.060598, 0.24173, 0.382925, 0.24173, 0.060598, 0.005977, 0.000229, 0.000003};

void main()
{
    vec2 rures = vec2(1.0/uRes.x, 1.0/uRes.y);

    vec4 _out = vec4(0);
    for(float k=-5.5; k<5.5; k+=1.0)
    {
        _out += texture(tex, uv + uFilter*k*rures);
    }
    fragCol = _out / 11.0;
}