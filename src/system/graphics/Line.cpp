#include "Line.h"

Line::Line(glm::vec2 start, glm::vec2 end)
{
	GLCALL(glEnable(GL_BLEND));
	GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	vertices = { start.x, start.y, end.x, end.y};

	vertex_array = std::make_unique<VertexArray>();
	vertex_buffer = std::make_unique<VertexBufferDynamic>();

	vertex_buffer_layout = std::make_unique<VertexBufferLayout>();
	vertex_buffer_layout->Push<glm::vec2>(1);	// Position Coordinates

	vertex_array->AddBuffer(*vertex_buffer, *vertex_buffer_layout);

	shader = std::make_unique<Shader>("shader/line.shader");
	shader->Bind();
}

Line::~Line()
{
	vertex_array->Unbind();
	vertex_buffer->Unbind();
	shader->Unbind();
}


void Line::setPositions(glm::vec2 start, glm::vec2 end)
{
	vertices = { start.x, start.y, end.x, end.y };
}

void Line::setMVP(glm::mat4 mvp)
{
}

void Line::setColour(glm::vec4 colour)
{
	line_colour = colour;
}

void Line::setColour(float colour[4])
{
	glm::vec4 temp_colour = { colour[0], colour[1], colour[2], colour[3]};
	line_colour = temp_colour;
}

void Line::Update(float deltaTime)
{
	vertex_buffer->Bind();

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), &vertices.front());

	shader->Bind();

	model_view_projection = projection_matrix * view_matrix * model_matrix;
	shader->SetUniformMat4f("u_MVP", model_view_projection);
	shader->SetUniformVec4("u_Colour", line_colour);
}

void Line::Draw()
{
	shader->Bind();
	vertex_array->Bind();

	GLCALL(glDrawArrays(GL_LINES, 0, 2));
}
