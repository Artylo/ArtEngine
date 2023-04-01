#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
void main()
{
	gl_Position = position;
};




#shader fragment
#version 330 core

out vec4 colour;

uniform vec4 u_Colour = vec4(255.0f, 0.0f, 220.f, 1.0f);

void main()
{
	colour = u_Colour;
};