$input v_uv

#include "common.sh"

SAMPLER2D(s_main_framebuffer_color, 0);

uniform vec4 u_camera_shift;

void main()
{
    vec2 uv = vec2(v_uv.x * u_camera_shift.z, v_uv.y * u_camera_shift.w) + u_camera_shift.xy;
    gl_FragColor = texture2D(s_main_framebuffer_color, uv);
}