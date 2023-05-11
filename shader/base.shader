#shader vertex
#version 330 core

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec2 a_TexCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP;

void main()
{
	gl_Position = a_Position * u_MVP;
    v_TexCoord = a_TexCoord;
};




#shader fragment
#version 330 core

out vec4 colour;

in vec2 v_TexCoord;

uniform vec4 u_Colour = vec4(255.0f, 0.0f, 220.f, 1.0f);
uniform float u_Time;
uniform sampler2D u_Texture;

void main()
{
    vec4 tex_col = texture(u_Texture, v_TexCoord);

    vec4 col_shift;
    float time = u_Time/500.0f;
    float r = (sin(time) / 2.0f) + 0.5f;
    float g = (sin(time + 2.0f * 3.14159f / 3.0f) / 2.0f) + 0.5f;
    float b = (sin(time + 4.0f * 3.14159f / 3.0f) / 2.0f) + 0.5f;
    col_shift = vec4(r, g, b, tex_col.a);

    colour = mix(tex_col, col_shift, sin(time));
};