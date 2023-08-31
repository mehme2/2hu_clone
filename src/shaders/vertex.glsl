#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 texCoord;

uniform mat4 transformMat;

void main() {
    texCoord = aTexCoord;
    gl_Position = transformMat * vec4(aPos.x, aPos.y, 0.0f, 1.0);
}
