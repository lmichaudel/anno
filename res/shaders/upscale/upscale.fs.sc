$input v_uv

#include "common.sh"

SAMPLER2D(s_albedo, 0);

void main()
{
    gl_FragColor = texture2D(s_albedo, v_uv);
}