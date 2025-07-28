$input v_position, v_normal, v_uv

#include "common.sh"

SAMPLER2D(s_texture, 0);

uniform vec4 light_dir;

void main()
{
    vec3 view_pos = vec3(0.0, 0.0, -5.0);
    vec3 light_pos = vec3(3, 1.5, 0.0);

    vec3 color = texture2D(s_texture, v_uv).rgb;

    // ambient
    vec3 ambient = 0.05 * color;

    // diffuse
    vec3 light_dir = normalize(light_pos - v_position);
    vec3 normal = normalize(v_normal);
    float diff = max(dot(light_dir.xyz, normal), 0.0);
    vec3 diffuse = diff * color;

    // specular
    vec3 view_dir = normalize(view_pos - v_position);
    vec3 reflect_dir = reflect(-light_dir.xyz, normal);
    vec3 halfway_dir = normalize(light_dir.xyz + view_dir);
    float spec = pow(max(dot(normal, halfway_dir), 0.0), 32.0);
    vec3 specular = vec3(0.2) * spec;

    gl_FragData[0] = vec4(ambient + diffuse + specular, 1.0);
}
