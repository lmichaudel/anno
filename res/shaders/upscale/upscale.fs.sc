$input v_uv

#include "common.sh"

SAMPLER2D(s_albedo, 0);

uniform vec4 u_texel_error;

void main()
{
    vec2 final_uv = vec2(v_uv.x * u_texel_error.z, v_uv.y * u_texel_error.w) + u_texel_error.xy;
    gl_FragColor = texture2D(s_albedo, final_uv);
}