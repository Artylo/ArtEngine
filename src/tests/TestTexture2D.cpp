#include "TestTexture2D.h"

#include "../system/graphics/Renderer.h";
#include "../system/graphics/IndexBuffer.h"
#include "../system/graphics/VertexArray.h"
#include "../system/graphics/VertexBuffer.h"
#include "../system/graphics/VertexBufferLayout.h"
#include "../system/graphics/Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test
{
	TestTexture2D::TestTexture2D()
	{
		//@TODO: Setup desired blending for current text. Probably best if renderer does this in the future.
		GLCALL(glEnable(GL_BLEND));
		GLCALL(glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA));

		VertexArray vertex_array;
		VertexBuffer vertex_buffer(vertices, sizeof(vertices));
		VertexBufferLayout vertex_buffer_layout;
		vertex_buffer_layout.Push<float>(2);
		vertex_buffer_layout.Push<float>(2);
		vertex_array.AddBuffer(vertex_buffer, vertex_buffer_layout);
		IndexBuffer index_buffer(indices, 6);

		//Projection Matrix
		glm::mat4 projection_matrix = glm::ortho(0.0f, 640.0f, 480.0f, 0.0f, -1.0f, 1.0f); // Coordinate System
		glm::mat4 view_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)); // Camera - moved 100 to the right
		glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)); // Translate all vertecies by an amount.
		glm::mat4 model_view_projection = projection_matrix * view_matrix * model_matrix;

		Shader shader("shader/base.shader");
		shader.Bind();

		Texture texture("img/skeleton.png");
		texture.Bind();

		shader.SetUniform1f("u_Time", ((float)SDL_GetTicks())); // Current Time in Ticks
		shader.SetUniform1i("u_Texture", 0); // Slot of Texture

		model_view_projection = projection_matrix * view_matrix * model_matrix;
		shader.SetUniformMat4f("u_MVP", model_view_projection);

		vertex_array.Unbind();
		vertex_buffer.Unbind();
		index_buffer.Unbind();
		shader.Unbind();
		texture.Unbind();
	}

	TestTexture2D::~TestTexture2D()
	{

	}

	void TestTexture2D::OnUpdate(float deltaTime)
	{

	}

	void TestTexture2D::OnRender()
	{

	}

	void TestTexture2D::OnGUIRender()
	{

	}
}