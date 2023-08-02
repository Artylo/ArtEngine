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

		vertex_array->AddBuffer(*vertex_buffer, *vertex_buffer_layout);
		index_buffer = std::make_unique<IndexBuffer>(indices, (sizeof(indices)/sizeof(unsigned int)));

		shader = std::make_unique<Shader>("shader/batched.shader");
		shader->Bind();

		texture1 = std::make_unique<Texture>("img/tile.png");
		texture1->Bind(1);
		texture2 = std::make_unique<Texture>("img/tile2.png");
		texture2->Bind(2);

		// @CLEANUP @DEBUG @TODO FUCKING DISGUSTING - Make into a vector or some shit
		texture_select = { 0,0,0,0 };
		q0 = CreateQuad(64.0f, 0.0f,  texture_select[0]);
		q1 = CreateQuad(96.0f, 0.0f,  texture_select[1]);
		q2 = CreateQuad(128.0f, 0.0f, texture_select[2]);
		q3 = CreateQuad(160.0f, 0.0f, texture_select[3]);
		memcpy(vertices, q0.data(), q0.size() * sizeof(Vertex));
		memcpy(vertices + q0.size(), q1.data(), q1.size() * sizeof(Vertex));
		memcpy(vertices + q0.size() + q1.size(), q2.data(), q2.size() * sizeof(Vertex));
		memcpy(vertices + q0.size() + q1.size() + q2.size(), q3.data(), q3.size() * sizeof(Vertex));

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
		//@TEST
		// @CLEANUP @DEBUG @TODO FUCKING DISGUSTING - Make into a vector or some shit
		
		q0 = CreateQuad(64.0f, 0.0f,  texture_select[0]);
		q1 = CreateQuad(96.0f, 0.0f,  texture_select[1]);
		q2 = CreateQuad(128.0f, 0.0f, texture_select[2]);
		q3 = CreateQuad(160.0f, 0.0f, texture_select[3]);
		memcpy(vertices, q0.data(), q0.size() * sizeof(Vertex));
		memcpy(vertices + q0.size(), q1.data(), q1.size() * sizeof(Vertex));
		memcpy(vertices + q0.size() + q1.size(), q2.data(), q2.size() * sizeof(Vertex));
		memcpy(vertices + q0.size() + q1.size() + q2.size(), q3.data(), q3.size() * sizeof(Vertex));


		vertex_buffer->Bind();
		//GLCALL(glMapBuffer());
		//GLCALL(glUnmapBuffer());
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);


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
		ImGui::DragFloat("Offset", &offset);

		if (ImGui::Button("T1")) texture_select[0] = !texture_select[0];
		ImGui::SameLine();
		if (ImGui::Button("T2")) texture_select[1] = !texture_select[1];
		ImGui::SameLine();
		if (ImGui::Button("T3")) texture_select[2] = !texture_select[2];
		ImGui::SameLine();
		if (ImGui::Button("T4")) texture_select[3] = !texture_select[3];

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