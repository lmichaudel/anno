$input a_position, a_normal, a_texcoord0
$output v_position, v_normal, v_uv

#include "common.sh"

void main()
{
    vec4 world_pos = mul(u_model[0], vec4(a_position, 1.0));
    v_position = world_pos.xyz;

	vec3 normal = a_normal;
	vec3 wnormal = mul(u_model[0], vec4(normal, 0.0) ).xyz;
    vec3 viewNormal = normalize(mul(u_view, vec4(wnormal, 0.0) ).xyz);
    v_normal = wnormal;

    v_uv = a_texcoord0;

    gl_Position = mul(u_viewProj, world_pos);
}
