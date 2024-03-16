#version 330 core
in vec3 ourPosition;

out vec4 fragColor;

void main() {
    fragColor = vec4(ourPosition, 1.0f);
}