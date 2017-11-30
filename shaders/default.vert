#version 330 core

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

out vec2 frag_uv;

void main()
{
    gl_Position = proj * view * model * vec4(position, 1.0);
    frag_uv = uv;
}
