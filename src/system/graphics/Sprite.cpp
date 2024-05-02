#include "Sprite.h"



void Sprite::init(GameManager* gamemanager)
{
	//Graphics
	GLCALL(glEnable(GL_BLEND));
	GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	vertex_array = std::make_unique<VertexArray>();
	vertex_buffer = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
	vertex_buffer_layout = std::make_unique<VertexBufferLayout>();
	vertex_buffer_layout->Push<float>(2); // Position Coordinates
	vertex_buffer_layout->Push<float>(2); //  Texture Coordinates
	vertex_array->AddBuffer(*vertex_buffer, *vertex_buffer_layout);
	index_buffer = std::make_unique<IndexBuffer>(indices, 6);

	shader = std::make_unique<Shader>(shader_filepath);
	shader->Bind();

	texture = std::make_unique<Texture>(sprite_filepath);
	texture->Bind(0);

	projection_matrix = gamemanager->projection_matrix;
	view_matrix = gamemanager->view_matrix;
}

void Sprite::update(float deltaTime, glm::vec2 position)
{
	shader->Bind(); //@TODO: The way this is currently handled is that everything breaks the moment your shader doesn't need these uniforms. Either have them in there and do nothing, or find a more elegant per entitiy system.

	shader->SetUniform1f("u_Time", deltaTime); // Current Time in Ticks
	shader->SetUniform1i("u_Texture", texture->texture_slot); // Slot of Texture

	updateMVP(position);
}

void Sprite::draw()
{
	renderer.Draw(*vertex_array, *index_buffer, *shader);
}

void Sprite::updateMVP(glm::vec2 position)
{
	model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0));
	model_view_projection = (*projection_matrix) * (*view_matrix) * model_matrix;
	shader->SetUniformMat4f("u_MVP", model_view_projection);
}

Sprite::Sprite()
{
	
}

Sprite::~Sprite()
{
	vertex_array->Unbind();
	vertex_buffer->Unbind();
	index_buffer->Unbind();
	shader->Unbind();
	texture->Unbind();
}
