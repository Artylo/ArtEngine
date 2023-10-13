#include "TestBatchedRendering.h"

namespace test
{
	TestBatchedRendering::TestBatchedRendering()
	{
		GLCALL(glEnable(GL_BLEND));
		GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		vertex_array = std::make_unique<VertexArray>();
		vertex_buffer = std::make_unique<VertexBufferDynamic>();

		vertex_buffer_layout = std::make_unique<VertexBufferLayout>();
		/*
		vertex_buffer_layout->Push<float>(2); // Position Coordinates
		vertex_buffer_layout->Push<float>(4); //   Vertex Colour
		vertex_buffer_layout->Push<float>(2); //  Texture Coordinates
		vertex_buffer_layout->Push<float>(1); //  Texture ID
		*/
		vertex_buffer_layout->Push<glm::vec2>(1);	// Position Coordinates
		vertex_buffer_layout->Push <glm::vec4>(1);	// Vertex Colour
		vertex_buffer_layout->Push<glm::vec2>(1);	// Texture Coordinates
		vertex_buffer_layout->Push<float>(1);		// Texture ID

		//Create Quads
		for (int i = 0; i < quad_number; i++)
		{
			texture_select.push_back(0);

			std::array<unsigned int, 6> temp_index_array =
			{0 + i * 4 , 1 + i * 4, 2 + i * 4, 2 + i * 4, 1 + i * 4, 3 + i * 4 };
			indices.push_back(temp_index_array);

			if (i % 25 == 0)
			{
				quad_vert_offset += 32.0f;
				quad_hori_offset = 0.0f;
			}
			vertices.push_back(CreateQuad(quad_hori_offset, quad_vert_offset, texture_select[i]));
			quad_hori_offset += 32.0f;
		}

		vertex_array->AddBuffer(*vertex_buffer, *vertex_buffer_layout);
		index_buffer = std::make_unique<IndexBufferDynamic>(&indices.front(), 6*indices.size()); //@TODO: The count of the indices is currently only ever set here, so it you make new quads, it cannot know about them without regenerating the whole index buffer. Find a way to change the count at runtime, like the dynamic vertex buffer.

		shader = std::make_unique<Shader>("shader/batched.shader");
		shader->Bind();

		texture1 = std::make_unique<Texture>("img/tile.png");
		texture1->Bind(1);
		texture2 = std::make_unique<Texture>("img/tile2.png");
		texture2->Bind(2);

	}

	std::array<Vertex, 4> TestBatchedRendering::CreateQuad(float x, float y , float textureID)
	{
		float size = 32.0f;

		Vertex v0;
		v0.position = {x, y};
		v0.vertex_colour = { 1.0f, 0.0f, 0.0f, 1.0f };
		v0.texture_coordinates = { 0.0f, 0.0f };
		v0.texture_ID = textureID;

		Vertex v1;
		v1.position = { x + size, y};
		v1.vertex_colour = { 0.0f, 1.0f, 0.0f, 1.0f };
		v1.texture_coordinates = { 1.0f, 0.0f };
		v1.texture_ID = textureID;

		Vertex v2;
		v2.position = { x, y + size};
		v2.vertex_colour = { 0.0f,0.0f,1.0f, 1.0f };
		v2.texture_coordinates = { 0.0f, 1.0f };
		v2.texture_ID = textureID;

		Vertex v3;
		v3.position = { x + size, y + size};
		v3.vertex_colour = { 0.0f,1.0f, 1.0f, 1.0f};
		v3.texture_coordinates = { 1.0f, 1.0f };
		v3.texture_ID = textureID;

		return {v0, v1, v2, v3};
	}

	void TestBatchedRendering::OnUpdate(float deltaTime)
	{
		vertex_buffer->Bind();
		//GLCALL(glMapBuffer());
		//GLCALL(glUnmapBuffer());
		//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(o_vertices), o_vertices);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size()*sizeof(std::array<Vertex, 4>), &vertices.front());

		shader->Bind();
		int samplers[2] = { texture1->texture_slot , texture2->texture_slot };
		shader->SetUniform1iv("u_Texture", 2, samplers); // Array of TextureIDs
	
		model_view_projection = projection_matrix * view_matrix * model_matrix;
		shader->SetUniformMat4f("u_MVP", model_view_projection);
	}

	void TestBatchedRendering::OnRender()
	{
		shader->Bind();
		vertex_array->Bind();
		index_buffer->Bind();

		GLCALL(glDrawElements(GL_TRIANGLES, index_buffer->GetCount(), GL_UNSIGNED_INT, nullptr));

	}

	void TestBatchedRendering::OnGUIRender()
	{
		model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0));
		ImGui::DragFloat2("Position", (float*)&position);

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
			if((i+1)%5!=0) ImGui::SameLine();
		}
	}


	TestBatchedRendering::~TestBatchedRendering()
	{
		vertex_array->Unbind();
		vertex_buffer->Unbind();
		index_buffer->Unbind();
		shader->Unbind();
		texture1->Unbind();
		texture2->Unbind();
	}
}