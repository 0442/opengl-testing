#version 330 core

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 vertCol;
layout (location = 2) in vec2 texPos;

out vec3 vertColorAttrib;
out vec2 texPosFrag;

uniform float xOffset;
uniform float yOffset;
uniform float changingNum;

uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;

void main() {
    gl_Position = projection * camera * model * vec4(vertPos.x + xOffset*changingNum, vertPos.y + yOffset*changingNum, vertPos.z, 1.0f);
    vertColorAttrib = vertCol;
    texPosFrag = texPos;
}
