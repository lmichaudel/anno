$input a_position, a_normal, a_texcoord0
$output v_uv

#include "../common/common.sh"

void main() {
    gl_Position = vec4(a_position, 1.0);
    v_uv = a_texcoord0;
}
