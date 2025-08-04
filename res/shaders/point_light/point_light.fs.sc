$input v_uv

#include "common.sh"

SAMPLER2D(g_position, 0);
SAMPLER2D(g_normal, 1);
SAMPLER2D(g_albedo, 2);

uniform vec4 u_camera_position;

uniform vec4 u_lightPos;      // xyz = position, w = radius
uniform vec4 u_lightColor;    // rgb = color, a = intensity

void main()
{
    // Unpack G-Buffer data
    vec3 albedo = texture2D(g_albedo, v_uv).rgb;
    vec3 normal = normalize(texture2D(g_normal, v_uv).rgb);
    vec3 worldPos = texture2D(g_position, v_uv).rgb;

    // Calculate lighting vectors
    vec3 lightPos = u_lightPos.xyz;
    float lightRadius = u_lightPos.w;
    vec3 lightColor = u_lightColor.rgb * u_lightColor.a;

    vec3 lightDir = lightPos - worldPos;
    float distance = length(lightDir);
    lightDir = normalize(lightDir);

    // Calculate distance attenuation
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);

    // Apply light radius (cut off beyond radius)
    if (distance > lightRadius) {
        attenuation = 0.0;
    }

    // Simple Lambertian diffuse lighting
    float NdotL = max(dot(normal, lightDir), 0.0);

    // Simple specular (Blinn-Phong)
    vec3 viewDir = normalize(u_camera_position.xyz - worldPos);
    vec3 halfDir = normalize(lightDir + viewDir);
    float NdotH = max(dot(normal, halfDir), 0.0);
    float specular = pow(NdotH, 32.0);

    // Combine lighting
    vec3 diffuse = albedo * lightColor * NdotL * attenuation;
    vec3 spec = vec3(texture2D(g_albedo, v_uv).a) * lightColor * specular * attenuation;

    // Simple ambient
    vec3 ambient = vec3(0.005) * albedo;

    vec3 finalColor = ambient + diffuse + spec;

    gl_FragColor = vec4(finalColor, 1.0);
}