#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 TransformMat;

void main() {
    TexCoord = aTexCoord;
    gl_Position = TransformMat * vec4(aPos.x, aPos.y, 0.0f, 1.0);
}
