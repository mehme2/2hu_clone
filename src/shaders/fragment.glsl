#version 330 core

in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D tex;

void main()
{
    vec4 color = texture(tex, texCoord);
    //if(color.r > 0.9 && color.g < 0.1 && color.b > 0.9)
        //discard;
    if(color.a < 0.1)
        discard;
    //fragColor = color * vec4(texCoord.x, 1.0f, texCoord.y, 1.0f);
    fragColor = color;
} 
