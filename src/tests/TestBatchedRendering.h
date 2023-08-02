#pragma once
#include "Test.h"

#include "../system/graphics/Renderer.h";
#include "../system/graphics/IndexBuffer.h"
#include "../system/graphics/VertexArray.h"
#include "../system/graphics/VertexBuffer.h"
#include "../system/graphics/VertexBufferDynamic.h"
#include "../system/graphics/VertexBufferLayout.h"
#include "../system/graphics/Texture.h"

#include "glm/glm.hpp"
#include "imgui/imgui.h";

#include <array>

namespace test
{
    class TestBatchedRendering :
        public Test
    {
	public:
		ImVec2 position = { 0,0 };
	private:

		//Batch Rendering
		glm::vec2 translations[100];
		int index = 0;
		float offset = 16.0f;


		Vertex vertices[16];
		std::array<bool, 4> texture_select;
		std::array<Vertex, 4> q0; //@CLEANUP
		std::array<Vertex, 4> q1;
		std::array<Vertex, 4> q2;
		std::array<Vertex, 4> q3;


		//Generic OpenGL

		unsigned int indices[24] = { // @TODO: Make Dynamic as well. Every set increases by 4. But keep in mind you're not reusing the last pair of indecies, you're making 2 extra ones you could be saving. That's 72 bytes per pair, so you could save the size of a quad every three quads.
			0, 1, 2,
			2, 1, 3,

			4, 5, 6,
			6, 5, 7,

			8, 9, 10,
			10, 9, 11,

			12, 13, 14,
			14, 13, 15
		};

		//Projection Matrix
		//glm::mat4 projection_matrix = glm::ortho(0.0f, 640.0f, 480.0f, 0.0f, -1.0f, 1.0f); // Coordinate System
		
		

		glm::mat4 projection_matrix = glm::ortho(0.0f, (float)w_width / 2, (float)w_height / 2, 0.0f, -1.0f, 1.0f); // Coordinate System

		glm::mat4 view_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)); // Camera - moved 100 to the right
		glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(32, 32, 0)); // Translate all vertecies by an amount.
		glm::mat4 model_view_projection;

		Renderer renderer;
		std::unique_ptr<VertexArray> vertex_array;
		std::unique_ptr<VertexBufferLayout> vertex_buffer_layout;
		std::unique_ptr<IndexBuffer> index_buffer;
		std::unique_ptr<Shader> shader;

		//Batch Rendering-related members
		std::unique_ptr<VertexBufferDynamic> vertex_buffer; // The thing we're actually testing here.
		std::unique_ptr<Texture> texture1; // Checkerboard Texture
		std::unique_ptr<Texture> texture2; // Grass Texture

	public:
		TestBatchedRendering();
		~TestBatchedRendering();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnGUIRender() override;
	private:
		std::array<Vertex, 4> CreateQuad(float x, float y, float textureID);
    };
}
