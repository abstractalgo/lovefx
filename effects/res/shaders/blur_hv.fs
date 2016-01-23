#version 330

in vec2 uv;

uniform vec3 uFilter;   // x-size, y-size, filter type (box, gaussian, triangle)
uniform vec2 uRes;      // resolution
uniform sampler2D tex;

out vec4 fragCol;

void main()
{
    vec2 rures = vec2(1.0/uRes.x, 1.0/uRes.y);

    vec4 _out = vec4(0);
    for(float j=-uFilter.x/2.0; j<uFilter.x/2.0; j+=1.0)
    {
        for (float i=-uFilter.y/2.0; i<uFilter.y/2.0; i+=1.0)
        {
            _out += texture(tex, uv + vec2(rures.x*j,rures.y*i));
        }
    }
    fragCol = _out / (uFilter.x*uFilter.y);
}