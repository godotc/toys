#version 330 core

in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec3 spriteColor = vec3(0, 1, 0);
uniform bool bPureColor = false;

void main()
{
    if (bPureColor)
    {
        color = vec4(spriteColor, 1.0);
    }
    else
    {
        vec4 texColor = texture(image, TexCoords);
        color = vec4(spriteColor, 1.0) * texColor;
    }
}

