#include "TestTexture2D.h"



namespace test
{
	TestTexture2D::TestTexture2D()
	{
		//@TODO: Setup desired blending for current text. Probably best if renderer does this in the future.
		GLCALL(glEnable(GL_BLEND));
		GLCALL(glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA));

		vertex_array = std::make_unique<VertexArray>();
		vertex_buffer = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
		vertex_buffer_layout = std::make_unique<VertexBufferLayout>();
		vertex_buffer_layout->Push<float>(2);
		vertex_buffer_layout->Push<float>(2);
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
		//ImGui::SliderFloat3("Translation")
	}
}