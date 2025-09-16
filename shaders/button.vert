#version 330 core

layout(location = 0) in vec2 aPos; // quad vertices in [0..1] space

uniform vec2 u_position;   // bottom-left corner in screen space
uniform vec2 u_size;       // width and height in screen space
uniform mat4 u_projection; // orthographic projection

void main()
{
    vec2 scaled = aPos * u_size;          // scale unit quad
    vec2 world = scaled + u_position;     // translate to position
    gl_Position = u_projection * vec4(world, 0.0, 1.0);
}

