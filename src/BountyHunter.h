#pragma once

#include "tests/Test.h"

#include "system/graphics/Texture.h";

namespace test
{
	class BountyHunter : public Test
	{
	public:
		ImVec2 position = { (float)w_width / 4 -32, (float)w_height / 4 -32};
	private:
		float vertices[16] = {
			//position		//texture coords
			 0.0f, 0.0f,	0.0f, 0.0f,		//Top Left
			 32.0f, 0.0f,	1.0f, 0.0f,		//Top Right
			 0.0f, 32.0f,	0.0f, 1.0f,		//Bottom Left
			 32.0f, 32.0f,	1.0f, 1.0f		//Bottom Right

		};
		unsigned int indices[6] = {
			0, 1, 2,
			2, 1, 3
		};

		glm::mat4 projection_matrix = glm::ortho(0.0f, (float)w_width / 2, (float)w_height / 2, 0.0f, -1.0f, 1.0f); // Coordinate System
		glm::mat4 view_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)); // Camera
		glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(32, 32, 0));
		glm::mat4 model_view_projection = projection_matrix * view_matrix * model_matrix;

		Renderer renderer;
		std::unique_ptr<VertexArray> vertex_array;
		std::unique_ptr<VertexBuffer> vertex_buffer;
		std::unique_ptr<VertexBufferLayout> vertex_buffer_layout;
		std::unique_ptr<IndexBuffer> index_buffer;
		std::unique_ptr<Shader> shader;
		std::unique_ptr<Texture> texture;

	public:
		BountyHunter();
		~BountyHunter() override;

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnGUIRender() override;
	private:
		//float clear_colour[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	};
}


