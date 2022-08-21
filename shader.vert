#version 330 core

layout (location = 0) in vec3 vertPos;

out vec4 aColor;

void main() {
    gl_Position = vec4(vertPos.x, vertPos.y, vertPos.z, 1.0);
    aColor = vec4(1.0f, 0.6f, 0.5f, 1.0f);
}
