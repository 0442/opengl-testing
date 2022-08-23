#version 330 core

out vec4 fragColor;

in vec3 vertColorAttrib;
in vec2 texPosFrag;

uniform float changingNum;
uniform sampler2D texture1;

void main() {
    vec4 newVect = vec4(vertColorAttrib, 1.0f);
    newVect.x = changingNum;
    fragColor = texture(texture1, texPosFrag, 1.0f) * newVect;
}
