#version 330 core
out vec4 FragColor;

in vec4 ourColor;
in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture1;

// Directional light (sun)
uniform vec3 lightDir;      // direction FROM light (normalized)
uniform vec3 lightColor;    // light color (e.g., 1.0, 0.95, 0.85 for warm sun)
uniform vec3 ambientColor;  // ambient light color (e.g., 0.25, 0.25, 0.3)

// Camera position for specular
uniform vec3 viewPos;

void main() {
    vec4 texColor = texture(texture1, TexCoord) * ourColor;
    vec3 objectColor = texColor.rgb;

    // Normalize interpolated normal
    vec3 norm = normalize(Normal);

    // --- Ambient ---
    vec3 ambient = ambientColor * objectColor;

    // --- Diffuse (Lambertian) ---
    // lightDir points FROM the light, so negate it for the dot product
    vec3 lightDirNorm = normalize(-lightDir);
    float diff = max(dot(norm, lightDirNorm), 0.0);
    vec3 diffuse = diff * lightColor * objectColor;

    // --- Specular (Blinn-Phong) ---
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDirNorm + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 32.0);
    vec3 specular = spec * lightColor * 0.3; // moderate specular intensity

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, texColor.a);
}
