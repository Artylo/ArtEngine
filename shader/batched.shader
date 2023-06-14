#shader vertex
#version 330 core

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec4 a_VertexColour;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TextureID;

out vec4 v_VertColour;
out vec2 v_TexCoord;
out float v_TexID;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * a_Position;
    v_TexCoord = a_TexCoord;
    v_TexID = a_TextureID;
    v_VertColour = a_VertexColour;
};




#shader fragment
#version 330 core

out vec4 colour;

in vec2 v_TexCoord;
in float v_TexID;
in vec4 v_VertColour;

uniform sampler2D u_Texture[2];

void main()
{
    int texture_index = int(v_TexID);
    vec4 tex_col = texture(u_Texture[texture_index], v_TexCoord);
    vec4 vert_col = vec4(v_VertColour.r, v_VertColour.g, v_VertColour.b, tex_col.a);
    //colour = vert_col;
    //colour = v_VertColour;
    //colour = vec4(v_TexID, v_TexID, v_TexID, 1.0f);
    colour = mix(tex_col, vert_col, 0.5);
};