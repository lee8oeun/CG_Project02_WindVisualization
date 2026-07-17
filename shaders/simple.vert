#version 450 

layout(location=0) in vec3 pos;  
layout(location=1) in vec3 normal;
layout(location=2) in vec2 vertextexCoord;

out vec3 Position;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 mvp;

void main()
{
    TexCoord=vertextexCoord;
    
    gl_Position = mvp*vec4(pos,1.0f); //클립 coordinate : 무조건 설정. 
}
