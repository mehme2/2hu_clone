#version 330 core
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D Texture;

void main()
{
    FragColor = texture(Texture, TexCoord) * vec4(TexCoord.x, 1.0f, TexCoord.y, 1.0f);
} 
