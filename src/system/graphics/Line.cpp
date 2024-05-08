#include "Line.h"

Line::Line() : vertices({ 0, 0, 32, 32 })
{
	GLCALL(glEnable(GL_BLEND));
	GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	vertex_array = std::make_unique<VertexArray>();
	vertex_buffer = std::make_unique<VertexBufferDynamic>();

	vertex_buffer_layout = std::make_unique<VertexBufferLayout>();
	vertex_buffer_layout->Push<glm::vec2>(1);	// Position Coordinates

	vertex_array->AddBuffer(*vertex_buffer, *vertex_buffer_layout);

	shader = std::make_unique<Shader>("shader/line.shader");
	shader->Bind();
}

Line::Line(const Line& other) : vertices(other.vertices)
{
	GLCALL(glEnable(GL_BLEND));
	GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	vertex_array = std::make_unique<VertexArray>();
	vertex_buffer = std::make_unique<VertexBufferDynamic>();

	vertex_buffer_layout = std::make_unique<VertexBufferLayout>();
	vertex_buffer_layout->Push<glm::vec2>(1);	// Position Coordinates

	vertex_array->AddBuffer(*vertex_buffer, *vertex_buffer_layout);

	shader = std::make_unique<Shader>("shader/line.shader");
	shader->Bind();
}

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

//Sets the position of points A and B for the line.
void Line::setPositions(glm::vec2 start, glm::vec2 end)
{
	vertices = { start.x, start.y, end.x, end.y };
}

void Line::setMVP(glm::mat4 mvp)
{
}

//Sets the colour and opacity of the line.
void Line::setColour(glm::vec4 colour)
{
	line_colour = colour;
}

//Sets the colour and opacity of the line.
void Line::setColour(float colour[4])
{
	glm::vec4 temp_colour = { colour[0], colour[1], colour[2], colour[3]};
	line_colour = temp_colour;
}

void Line::setColour(std::vector<float> colour)
{
	line_colour = { colour[0], colour[1], colour[2], colour[3] };
}

void Line::getPositions(float* Ax, float* Ay, float* Bx, float* By)
{
	if (Ax != NULL) *Ax = vertices[0];
	if (Ay != NULL) *Ay = vertices[1];
	if (Bx != NULL) *Bx = vertices[2];
	if (By != NULL) *By = vertices[3];
}

void Line::Update()
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
