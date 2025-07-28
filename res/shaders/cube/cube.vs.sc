$input a_position, a_normal, a_texcoord0
$output v_position, v_normal, v_uv

#include "common.sh"

void main()
{
    v_position = mul(u_model[0], vec4(a_position, 1.0)).xyz;
    v_normal = mul(u_model[0], vec4(a_normal, 1.0)).xyz;
    v_uv = a_texcoord0;

    gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));
}
