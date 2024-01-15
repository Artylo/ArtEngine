#pragma once
#include "Test.h"

#include "../system/graphics/Renderer.h";
#include "../system/graphics/IndexBufferDynamic.h"
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

		unsigned int quad_number = 250; //@TODO: DEBUG: THIS BREAKS AFTER 250
		float quad_hori_offset = 0.0f;
		float quad_vert_offset = 0.0f;

		std::vector<std::array<Vertex, 4>> vertices;
		std::vector<bool> texture_select;
		std::vector<std::array<unsigned int, 6>> indices; // @TODO: Every set increases by 4. But keep in mind you're not reusing the last pair of indecies, you're making 2 extra ones you could be saving. That's 72 bytes per pair, so you could save the size of a quad every three quads.

		//Projection Matrix
		glm::mat4 projection_matrix = glm::ortho(0.0f, (float)w_width / 2, (float)w_height / 2, 0.0f, -1.0f, 1.0f); // Coordinate System
		glm::mat4 view_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)); // Camera - moved 100 to the right
		glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)); // Translate all vertecies by an amount.
		glm::mat4 model_view_projection;

		Renderer renderer;
		std::unique_ptr<VertexArray> vertex_array;
		std::unique_ptr<VertexBufferLayout> vertex_buffer_layout;
		std::unique_ptr<IndexBufferDynamic> index_buffer;
		std::unique_ptr<Shader> shader;

		//Batch Rendering-related members
		std::unique_ptr<VertexBufferDynamic> vertex_buffer; // The thing we're actually testing here.
		std::unique_ptr<Texture> texture1; // Checkerboard Texture
		std::unique_ptr<Texture> texture2; // Grass Texture

	public:
		TestBatchedRendering(GameManager* GM);
		~TestBatchedRendering();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnGUIRender() override;
	private:
		std::array<Vertex, 4> CreateQuad(float x, float y, float textureID);
    };
}
