#pragma once

#include "Test.h"

#include "../system/graphics/Renderer.h";
#include "../system/graphics/IndexBuffer.h"
#include "../system/graphics/VertexArray.h"
#include "../system/graphics/VertexBuffer.h"
#include "../system/graphics/VertexBufferLayout.h"
#include "../system/graphics/Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <memory>

namespace test
{
	class TestInstancedRendering : public Test
	{
	public:
		ImVec2 position = { 160,160 };
	private:

		//Batch Rendering
		glm::vec2 translations[100];
		int index = 0;
		float offset = 16.0f;


		//Generic OpenGL
		float vertices[32] = {
			//position		//vert colours		//tex coords   //tex ID
			 0.0f, 0.0f,	1.0f,0.0f,0.0f,		0.0f, 0.0f,		0.0f,	//Top Left
			 32.0f, 0.0f,	0.0f,1.0f,0.0f,		1.0f, 0.0f,		0.0f,	//Top Right
			 0.0f, 32.0f,	0.0f,0.0f,1.0f,		0.0f, 1.0f,		0.0f,	//Bottom Left
			 32.0f, 32.0f,	0.0f,1.0f,1.0f,		1.0f, 1.0f,		0.0f	//Bottom Right
		};

		unsigned int indices[6] = {
			0, 1, 2,
			2, 1, 3
		};

		//Projection Matrix
		//glm::mat4 projection_matrix = glm::ortho(0.0f, 640.0f, 480.0f, 0.0f, -1.0f, 1.0f); // Coordinate System
		glm::mat4 projection_matrix = glm::ortho(0.0f, (float)w_width / 2, (float)w_height / 2, 0.0f, -1.0f, 1.0f); // Coordinate System

		glm::mat4 view_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)); // Camera - moved 100 to the right
		glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(32, 32, 0)); // Translate all vertecies by an amount.
		glm::mat4 model_view_projection = projection_matrix * view_matrix * model_matrix;

		Renderer renderer;
		std::unique_ptr<VertexArray> vertex_array;
		std::unique_ptr<VertexBuffer> vertex_buffer;
		std::unique_ptr<VertexBufferLayout> vertex_buffer_layout;
		std::unique_ptr<IndexBuffer> index_buffer;
		std::unique_ptr<Shader> shader;
		std::unique_ptr<Texture> texture;

	public:
		TestInstancedRendering(GameManager* GM);
		~TestInstancedRendering();

		void OnUpdate() override;
		void OnRender() override;
		void OnGUIRender() override;
	private:
	};
}