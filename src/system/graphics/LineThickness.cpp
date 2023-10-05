#include "LineThickness.h"

LineThickness::LineThickness(glm::vec2 start, glm::vec2 end)
{
	GLCALL(glEnable(GL_BLEND));
	GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	vertex_array = std::make_unique<VertexArray>();
	//vertex_array->AddBuffer(*vertex_buffer, *vertex_buffer_layout); //@TODO

	shader = std::make_unique<Shader>("shader/line_thick.shader");
	shader->Bind();

	//Square
	glm::vec4 p0(32.0f, 32.0f, 0.0f, 1.0f);
	glm::vec4 p1(64.0f, 32.0f, 0.0f, 1.0f);
	glm::vec4 p2(128.0f, 128.0f, 0.0f, 1.0f);
	glm::vec4 p3(32.0f, 64.0f, 0.0f, 1.0f);
	varray1 = { p3, p0, p1, p2, p3, p0, p1 };
	

	//Circle
	for (int u = -8; u <= 368; u += 8)
	{
		double a = u * M_PI / 180.0;
		double c = cos(a), s = sin(a);
		varray2.emplace_back(glm::vec4(320.0+(float)c*32, 240.0+(float)s*32, 0.0f, 1.0f));
	}

	CreateSSBO(varray2);
}

LineThickness::~LineThickness()
{
	vertex_array->Unbind();
	//vertex_buffer->Unbind();
	shader->Unbind();
}

//@TODO: Make into its own class preferably.
GLuint LineThickness::CreateSSBO(std::vector<glm::vec4>& varray)
{
	GLCALL(glGenBuffers(1, &ssbo));
	GLCALL(glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo));
	GLCALL(glBufferData(GL_SHADER_STORAGE_BUFFER, varray.size() * sizeof(*varray.data()), varray.data(), GL_STATIC_DRAW));
	return ssbo;
}

void LineThickness::setPositions(glm::vec2 start, glm::vec2 end)
{
}

void LineThickness::setMVP(glm::mat4 mvp)
{
}

void LineThickness::setColour(glm::vec4 colour)
{
}

void LineThickness::setColour(float colour[4])
{
}

void LineThickness::setThickness(float param_thickness)
{
	thickness = param_thickness;
}

void LineThickness::Update(float deltaTime)
{
	shader->Bind(); // @TODO: Something unbound the shader before this point, making this necessary.
	model_view_projection = projection_matrix * view_matrix * model_matrix;
	shader->SetUniformMat4f("u_MVP", model_view_projection);
	shader->SetUniformVec2("u_resolution", resolution);
	shader->SetUniform1f("u_thickness", thickness);
}

void LineThickness::Draw()
{
	shader->Bind();
	vertex_array->Bind();

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
	GLsizei N1 = (GLsizei)varray2.size() - 2;
	glDrawArrays(GL_TRIANGLES, 0, 6 * (N1 - 1));
	//GLCALL(glDrawArrays(GL_LINES, 0, 2));
}
