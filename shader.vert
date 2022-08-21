#version 330 core

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 vertCol;

out vec3 vertColorAttrib;

uniform float xOffset;
uniform float yOffset;

void main() {
    gl_Position = vec4(vertPos.x + xOffset, vertPos.y + yOffset, vertPos.z, 1.0f);
    vertColorAttrib = vertCol;
}
