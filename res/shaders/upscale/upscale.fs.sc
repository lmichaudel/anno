$input v_uv

#include "common.sh"

SAMPLER2D(s_main_framebuffer_color, 0);

uniform vec4 u_camera_shift;

void main()
{
    vec2 uv = vec2(v_uv.x * u_camera_shift.z, v_uv.y * u_camera_shift.w) + u_camera_shift.xy;
    float gamma = 1.0;

    vec3 hdrColor = texture2D(s_main_framebuffer_color, uv).xyz;

    vec3 mapped = hdrColor / (hdrColor + vec3(1.0));
    mapped = pow(mapped, vec3(1.0 / gamma));

    gl_FragColor = vec4(mapped, 1.0);
}