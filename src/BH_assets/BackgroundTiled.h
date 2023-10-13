#pragma once

#include "../system/GameManager.h"

#include "../system/graphics/Renderer.h"
#include "../system/graphics/VertexArray.h"
#include "../system/graphics/VertexBufferDynamic.h"
#include "../system/graphics/VertexBufferLayout.h"
#include "../system/graphics/IndexBufferDynamic.h"
#include "../system/graphics/Texture.h"

#include "imgui/imgui.h"
#include "glm/glm.hpp"

#include <array>
#include <memory>

class BackgroundTiled
{
public:
	GameManager* GM;
	ImVec2 background_position = { 0,0 };
	float tilesize = 32.0f;
	int triangles = 1;
private:

	//Batch Rendering

	std::vector<std::array<Vertex, 4>> vertices;
	std::vector<int> texture_select;
	std::vector<std::array<unsigned int, 6>> indices; // @TODO: Every set increases by 4. But keep in mind you're not reusing the last pair of indecies, you're making 2 extra ones you could be saving. That's 72 bytes per pair, so you could save the size of a quad every three quads.

	//Projection Matrix
	glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)); // Translate all vertecies by an amount.
	glm::mat4 model_view_projection;

	Renderer renderer;
	std::unique_ptr<VertexArray> vertex_array;
	std::unique_ptr<VertexBufferLayout> vertex_buffer_layout;
	std::unique_ptr<IndexBufferDynamic> index_buffer;
	std::unique_ptr<Shader> shader;
	std::unique_ptr<VertexBufferDynamic> vertex_buffer; 
	std::unique_ptr<Texture> texture1; // Checkerboard Texture
	std::unique_ptr<Texture> texture2; // Grass Texture

public:
	BackgroundTiled(GameManager* gamemanager);
	~BackgroundTiled();

	void Update(float deltaTime);
	void Draw();
	void GUIDraw();
private:
	std::array<Vertex, 4> CreateQuad(float x, float y, float textureID);
};

