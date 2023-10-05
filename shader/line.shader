#shader vertex
#version 330 core

layout (location = 0) in vec2 aPos;
uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * vec4(aPos, 1.0, 1.0);
}


#shader fragment
#version 330 core

out vec4 FragColor;
uniform vec4 u_Colour;

void main()
{
    FragColor = u_Colour;
}