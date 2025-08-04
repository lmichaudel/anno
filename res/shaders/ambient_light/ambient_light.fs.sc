$input v_uv

#include "common.sh"

SAMPLER2D(g_albedo, 0);

uniform vec4 u_light_color; // xyz = color; a = intensity

void main()
{
    vec3 albedo = texture2D(g_albedo, v_uv).rgb * (u_light_color.xyz * u_light_color.w);
    gl_FragColor = vec4(albedo, 1.0);
}