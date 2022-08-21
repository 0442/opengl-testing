#version 330 core

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 vertCol;

out vec3 vertColorAttrib;

void main() {
    gl_Position = vec4(vertPos.xyz, 1.0f);
    vertColorAttrib = vertCol;
}
