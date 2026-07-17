#version 450

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

out vec4 Fragcolor;

uniform sampler2D Tex1;

void main()
{
    vec4 texColor = texture(Tex1,TexCoord);
    //fragment 색상 결정 
    Fragcolor=vec4(texColor);
}