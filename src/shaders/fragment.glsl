#version 330 core

in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D tex;

void main()
{
    fragColor = texture(tex, texCoord) * vec4(texCoord.x, 1.0f, texCoord.y, 1.0f);
} 
