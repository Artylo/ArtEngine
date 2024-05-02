#pragma once

#include <string>
#include <memory> //std::unique_ptr

#include <glm/glm.hpp> //glm:mat4
#include <glm/ext.hpp> //glm::translate()

#include "../GameManager.h"
#include "Renderer.h"
#include "Texture.h"

#include "VertexBufferLayout.h"


class Sprite
{
public:

	std::string sprite_filepath = "img/tile.png";
	std::string shader_filepath = "shader/base.shader";

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

	glm::mat4* projection_matrix = nullptr;
	glm::mat4* view_matrix = nullptr;
	glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	glm::mat4 model_view_projection;

	Renderer renderer;
	std::unique_ptr<VertexArray> vertex_array;
	std::unique_ptr<VertexBuffer> vertex_buffer;
	std::unique_ptr<VertexBufferLayout> vertex_buffer_layout;
	std::unique_ptr<IndexBuffer> index_buffer;
	std::unique_ptr<Shader> shader;
	std::unique_ptr<Texture> texture;

public:
	void init(GameManager* gamemanager);
	void update(float deltaTime, glm::vec2 position);
	void draw();

	void updateMVP(glm::vec2 position);

	Sprite();
	~Sprite();
};

