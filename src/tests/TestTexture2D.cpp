#include "TestTexture2D.h"

namespace test
{
	TestTexture2D::TestTexture2D(GameManager* GM) : Test(GM)
	{
		//@TODO: Setup desired blending for current test. Probably best if renderer does this in the future.
		GLCALL(glEnable(GL_BLEND));
		GLCALL(glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA));

		vertex_array = std::make_unique<VertexArray>();
		vertex_buffer = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
		vertex_buffer_layout = std::make_unique<VertexBufferLayout>();
		vertex_buffer_layout->Push<float>(2); // Position Coordinates
		vertex_buffer_layout->Push<float>(2); //  Texture Coordinates
		vertex_array->AddBuffer(*vertex_buffer, *vertex_buffer_layout);
		index_buffer = std::make_unique<IndexBuffer>(indices, 6);

		shader = std::make_unique<Shader>("shader/base.shader");
		shader->Bind();

		texture = std::make_unique<Texture>("img/skeleton.png");
		texture->Bind(1);
	}

	TestTexture2D::~TestTexture2D()
	{
		vertex_array->Unbind();
		vertex_buffer->Unbind();
		index_buffer->Unbind();
		shader->Unbind();
		texture->Unbind();
	}

	void TestTexture2D::OnUpdate(float deltaTime)
	{
		shader->Bind();
		//texture->Bind(texture->texture_slot); //@CLEANUP: Might not be necessary

		shader->SetUniform1f("u_Time", deltaTime); // Current Time in Ticks

		shader->SetUniform1i("u_Texture", texture->texture_slot); // Slot of Texture

		model_view_projection = projection_matrix * view_matrix * model_matrix;
		shader->SetUniformMat4f("u_MVP", model_view_projection);
	}

	void TestTexture2D::OnRender()
	{
		renderer.Draw(*vertex_array,*index_buffer,*shader);
	}

	void TestTexture2D::OnGUIRender()
	{
		model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0));
		ImGui::DragFloat2("Position", (float*)&position);
	}
}