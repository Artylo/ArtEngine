#include "TestInstancedRendering.h"

namespace test
{
	TestInstancedRendering::TestInstancedRendering()
	{

		//@TODO: Setup desired blending for current test. Probably best if renderer does this in the future.
		GLCALL(glEnable(GL_BLEND));
		GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		vertex_array = std::make_unique<VertexArray>();
		vertex_buffer = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
		vertex_buffer_layout = std::make_unique<VertexBufferLayout>();
		vertex_buffer_layout->Push<float>(2); // Position Coordinates
		vertex_buffer_layout->Push<float>(3); //   Vertex Colour
		vertex_buffer_layout->Push<float>(2); //  Texture Coordinates
		vertex_buffer_layout->Push<float>(1); //  Texture ID
		vertex_array->AddBuffer(*vertex_buffer, *vertex_buffer_layout);
		index_buffer = std::make_unique<IndexBuffer>(indices, 6);

		shader = std::make_unique<Shader>("shader/batch.shader");
		shader->Bind();

		texture = std::make_unique<Texture>("img/tree.png");
		texture->Bind(1);
	}

	void TestInstancedRendering::OnUpdate(float deltaTime)
	{
		shader->Bind();

		for (int y = -10; y < 10; y += 2)
		{
			for (int x = -10; x < 10; x += 2)
			{
				glm::vec2 translation;
				translation.x = x*offset;
				translation.y = y*offset;
				translations[index++] = translation;
			}
		}
		index = 0;

		//Batch Rendering
		for (unsigned int i = 0; i < 100; i++)
		{
			shader->SetUniformVec2(("offsets[" + std::to_string(i) + "]"), translations[i]);
		}

		//texture->Bind(texture->texture_slot); //@CLEANUP: Might not be necessary

		//shader->SetUniform1f("u_Time", deltaTime); // Current Time in Ticks

		shader->SetUniform1i("u_Texture", texture->texture_slot); // Slot of Texture

		model_view_projection = projection_matrix * view_matrix * model_matrix;
		shader->SetUniformMat4f("u_MVP", model_view_projection);
	}

	void TestInstancedRendering::OnRender()
	{
		shader->Bind();
		vertex_array->Bind();
		index_buffer->Bind();

		//GLCALL(glDrawElements(GL_TRIANGLES, index_buffer->GetCount(), GL_UNSIGNED_INT, nullptr));

		//GLCALL(glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100));

		GLCALL(glDrawElementsInstanced(GL_TRIANGLES, index_buffer->GetCount(), GL_UNSIGNED_INT, nullptr, 100));
	}

	void TestInstancedRendering::OnGUIRender()
	{
		model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0));
		ImGui::DragFloat2("Position", (float*)&position);
		ImGui::DragFloat("Offset", &offset);
	}

	TestInstancedRendering::~TestInstancedRendering()
	{
		vertex_array->Unbind();
		vertex_buffer->Unbind();
		index_buffer->Unbind();
		shader->Unbind();
		texture->Unbind();
	}
}