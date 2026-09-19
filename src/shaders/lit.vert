#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aNormal;

out vec4 ourColor;
out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    // Transform position to world space for lighting
    FragPos = vec3(model * vec4(aPos, 1.0));

    // Transform normal to world space (use transpose of inverse for non-uniform scaling)
    // For uniform scaling, (mat3(model) * aNormal) is sufficient and cheaper
    Normal = mat3(transpose(inverse(model))) * aNormal;

    gl_Position = projection * view * vec4(FragPos, 1.0);
    ourColor = aColor;
    TexCoord = aTexCoord;
}
