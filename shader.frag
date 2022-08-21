#version 330 core

in vec4 aColor;
uniform vec4 changingColor;

out vec4 fragColor;

void main() {
    fragColor = changingColor;
}
