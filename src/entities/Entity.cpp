#include "Entity.h"

#include "../system/graphics/VertexBufferLayout.h"

void Entity::init()
{
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

	projection_matrix = GM->projection_matrix;
	view_matrix = GM->view_matrix;
}

void Entity::update(float deltaTime)
{
	shader->SetUniform1f("u_Time", deltaTime); // Current Time in Ticks
	shader->SetUniform1i("u_Texture", texture->texture_slot); // Slot of Texture

	model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0));
	model_view_projection = (*projection_matrix) * (*view_matrix) * model_matrix;
	shader->SetUniformMat4f("u_MVP", model_view_projection);
}

void Entity::draw()
{
	renderer.Draw(*vertex_array, *index_buffer, *shader);
}

Entity::Entity()
{

}

Entity::~Entity()
{
	vertex_array->Unbind();
	vertex_buffer->Unbind();
	index_buffer->Unbind();
	shader->Unbind();
	texture->Unbind();
}
