#shader vertex
#version 330 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec3 a_VertexColour;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TextureID;

out vec4 v_VertColour;
out vec2 v_TexCoord;
out float v_TexID;

uniform vec2 offsets[100];
uniform mat4 u_MVP;

void main()
{
    vec2 offset = offsets[gl_InstanceID];
    vec4 offseted_position = vec4((a_Position + offset), 0.0, 1.0);
    gl_Position = u_MVP * offseted_position;
    v_TexCoord = a_TexCoord;
    v_TexID = a_TextureID;
    v_VertColour = vec4(a_VertexColour, 1.0);
};




#shader fragment
#version 330 core

out vec4 colour;

in vec2 v_TexCoord;
in float v_TexID;
in vec4 v_VertColour;

uniform sampler2D u_Texture;

void main()
{
	vec4 tex_col = texture(u_Texture, v_TexCoord);
    vec4 vert_col = vec4(v_VertColour.r, v_VertColour.g, v_VertColour.b, tex_col.a);
    colour = vert_col;
};