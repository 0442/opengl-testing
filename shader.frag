#version 330 core

in vec3 vertColorAttrib;
uniform vec4 changingColor;

out vec4 fragColor;

void main() {
    vec4 newVect = vec4(vertColorAttrib, 1.0f);
    newVect.x = changingColor.z;
    fragColor = newVect;
}
