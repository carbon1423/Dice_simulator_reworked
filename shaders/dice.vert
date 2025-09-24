#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal; // normal attribute

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;  // position in world space (for lighting calculations)
out vec3 Normal;   // normal vector (for lighting)

void main()
{
    // Transform the vertex position to world space
    FragPos = vec3(view * model * vec4(aPos, 1.0));

    // Transform the normal vector with the inverse transpose of the model matrix
    Normal = mat3(transpose(inverse(view * model))) * aNormal;

    // Final position on screen
    gl_Position = projection * view * model * vec4(aPos, 1.0);

}