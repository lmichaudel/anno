$input v_position, v_normal, v_uv

#include "common.sh"

SAMPLER2D(s_albedo, 0);

uniform vec4 u_pick_id;

void main()
{
    gl_FragData[0] = vec4(v_position, 1.0);
    gl_FragData[1] = vec4(v_normal, 1.0);
    gl_FragData[2] = vec4(texture2D(s_albedo, v_uv).rgb, 0.0);
    gl_FragData[3] = vec4(u_pick_id.x);
}
