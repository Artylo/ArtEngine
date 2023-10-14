#include "BackgroundTiled.h"

BackgroundTiled::BackgroundTiled(GameManager* gamemanager)
{
	//Variables
	GM = gamemanager;

	//Graphics
	GLCALL(glEnable(GL_BLEND));
	GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	vertex_array = std::make_unique<VertexArray>();
	vertex_buffer = std::make_unique<VertexBufferDynamic>();

	vertex_buffer_layout = std::make_unique<VertexBufferLayout>();
	vertex_buffer_layout->Push<glm::vec2>(1);	// Position Coordinates
	vertex_buffer_layout->Push <glm::vec4>(1);	// Vertex Colour
	vertex_buffer_layout->Push<glm::vec2>(1);	// Texture Coordinates
	vertex_buffer_layout->Push<float>(1);		// Texture ID

	//Create Quads
	//for (int i = 0; i < quad_number; i++)
	//{
	//	texture_select.push_back(0);
	//
	//	std::array<unsigned int, 6> temp_index_array =
	//	{ 0 + i * 4 , 1 + i * 4, 2 + i * 4, 2 + i * 4, 1 + i * 4, 3 + i * 4 };
	//	indices.push_back(temp_index_array);
	//
	//	if (i % 5 == 0 && i != 0)
	//	{
	//		quad_vert_offset += 32.0f;
	//		quad_hori_offset = 0.0f;
	//	}
	//	vertices.push_back(CreateQuad(quad_hori_offset, quad_vert_offset, texture_select[i]));
	//	quad_hori_offset += 32.0f;
	//}

	for (int i = 0; i < w_height/2/tilesize; i++)
	{
		for (int j = 0; j < w_width/2/tilesize; j++)
		{
			texture_select.push_back(1);

			//vertices.push_back(CreateQuad(i, j, texture_select[i]));
			vertices.push_back(CreateQuad((float) (j * tilesize), (float) (i * tilesize), texture_select[i+j]));
		}
	}
	for (int i = 0; i < vertices.size(); i++)
	{
		// 0               1               2                  3					 // Quad
		// 0 1 2, 2 1 3 -> 4 5 6, 6 5 7 -> 8 9 10, 10 9 11 -> 12 13 14, 14 13,15 // Index
		//std::array<unsigned int, 6> temp_index_array = { 0 + i * 4, 1 + i * 4, 2 + i * 4, 2 + i * 4, 1 + i * 4, 3 + i * 4 };

		int id = i*4;
		std::array<unsigned int, 6> temp_index_array =
		{id, id+1, id+2, id+1, id+2, id+3};

		indices.push_back(temp_index_array);
	}

	GLCALL(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(std::array<Vertex, 4>), &vertices[0], GL_DYNAMIC_DRAW););

	GLint buffersize = 0;
	glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &buffersize);
	std::cout << "Buffer size: " << buffersize << std::endl;
	std::cout << "Size of data: " << vertices.size() * sizeof(std::array<Vertex, 4>) << std::endl;

	vertex_array->AddBuffer(*vertex_buffer, *vertex_buffer_layout);

	unsigned int indices_count = 6 * indices.size();
	triangles = indices_count;

	index_buffer = std::make_unique<IndexBufferDynamic>(&indices.front(), indices_count); //@TODO: The count of the indices is currently only ever set here, so it you make new quads, it cannot know about them without regenerating the whole index buffer. Find a way to change the count at runtime, like the dynamic vertex buffer.

	shader = std::make_unique<Shader>("shader/batched.shader");
	shader->Bind();

	texture1 = std::make_unique<Texture>("img/tile.png");
	texture1->Bind(1);
	texture2 = std::make_unique<Texture>("img/tile2.png");
	texture2->Bind(2);
}

BackgroundTiled::~BackgroundTiled()
{
	vertex_array->Unbind();
	vertex_buffer->Unbind();
	index_buffer->Unbind();
	shader->Unbind();
	texture1->Unbind();
	texture2->Unbind();
}

void BackgroundTiled::Update(float deltaTime)
{
	vertex_buffer->Bind();
	GLCALL(glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(std::array<Vertex, 4>), &vertices[0]));

	

	index_buffer->Bind();
	GLCALL(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(std::array<unsigned int, 6>), &indices[0]));

	shader->Bind();
	int samplers[2] = { texture1->texture_slot , texture2->texture_slot };
	shader->SetUniform1iv("u_Texture", 2, samplers); // Array of TextureIDs

	model_view_projection = *GM->projection_matrix * *GM->view_matrix * model_matrix;
	shader->SetUniformMat4f("u_MVP", model_view_projection);
}

void BackgroundTiled::Draw()
{
	shader->Bind();
	vertex_array->Bind();
	index_buffer->Bind();

	GLCALL(glDrawElements(GL_TRIANGLES, triangles, GL_UNSIGNED_INT, nullptr));
}

void BackgroundTiled::GUIDraw()
{
	ImGui::SeparatorText("Renderer");
	ImGui::DragInt("Elements Drawn", &triangles, 0.1f, 1, index_buffer->GetCount());

	ImGui::SeparatorText("Game");
	model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(background_position.x, background_position.y, 0));
	ImGui::DragFloat2("Background Position", (float*)&background_position);

	for (int i = 0; i < vertices.size(); i++)
	{
		std::string label_name;
		if (i > 8) label_name = std::to_string(i + 1);
		else label_name = std::to_string(0) + std::to_string(i + 1);

		if (ImGui::Button(label_name.c_str()))
		{
			texture_select[i] = !texture_select[i];
			for (int j = 0; j < vertices[i].size(); j++)
			{
				vertices[i]._Elems[j].texture_ID = texture_select[i];
			}
		}
		if ((i + 1) % (int)((w_width/2)/tilesize+1) != 0) ImGui::SameLine();
	}
}

std::array<Vertex, 4> BackgroundTiled::CreateQuad(float x, float y, float textureID)
{
	Vertex v0;
	v0.position = { x, y };
	v0.vertex_colour = { 1.0f, 0.0f, 0.0f, 1.0f };
	v0.texture_coordinates = { 0.0f, 0.0f };
	v0.texture_ID = textureID;

	Vertex v1;
	v1.position = { x + tilesize, y };
	v1.vertex_colour = { 0.0f, 1.0f, 0.0f, 1.0f };
	v1.texture_coordinates = { 1.0f, 0.0f };
	v1.texture_ID = textureID;

	Vertex v2;
	v2.position = { x, y + tilesize };
	v2.vertex_colour = { 0.0f,0.0f,1.0f, 1.0f };
	v2.texture_coordinates = { 0.0f, 1.0f };
	v2.texture_ID = textureID;

	Vertex v3;
	v3.position = { x + tilesize, y + tilesize };
	v3.vertex_colour = { 0.0f,1.0f, 1.0f, 1.0f };
	v3.texture_coordinates = { 1.0f, 1.0f };
	v3.texture_ID = textureID;

	return { v0, v1, v2, v3 };
}
