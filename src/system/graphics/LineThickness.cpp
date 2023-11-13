#include "LineThickness.h"

LineThickness::LineThickness(glm::vec2 start, glm::vec2 end)
{
	GLCALL(glEnable(GL_BLEND));
	GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	vertex_array = std::make_unique<VertexArray>();
	//vertex_array->AddBuffer(*vertex_buffer, *vertex_buffer_layout); //@TODO

	shader = std::make_unique<Shader>("shader/line_thick.shader");
	shader->Bind();

	//Line
	glm::vec4 c0 = { start.x, start.y, 0.0f, 1.0f };
	glm::vec4 c1 = { end.x,   end.y,   0.0f, 1.0f };
	varray_line.push_back(c0);
	varray_line.push_back(c1);

	//Square
	glm::vec4 p0(0.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 p1(32.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 p2(32.0f, 32.0f, 0.0f, 1.0f);
	glm::vec4 p3(0.0f, 32.0f, 0.0f, 1.0f);
	varray_square = { p3, p0, p1, p2, p3, p0, p1 };
	//varray_square = { p0, p1, p2, p3 };
	
	//Circle
	for (int u = -8; u <= 368; u += 8)
	{
		double a = u * M_PI / 180.0;
		double c = cos(a), s = sin(a);
		varray_circle.emplace_back(glm::vec4(start.x+(float)c*32, start.y+(float)s*32, 0.0f, 1.0f));
	}

	current_varray = &varray_line; // Change Array Type Here
	CreateSSBO(*current_varray);
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
	line_thickness = param_thickness;
}

void LineThickness::Update(float deltaTime)
{
	model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(position[0], position[1], 0.0f));

	shader->Bind(); // @TODO: Something unbound the shader before this point, making this necessary.
	model_view_projection = projection_matrix * view_matrix * model_matrix;
	shader->SetUniformMat4f("u_MVP", model_view_projection);
	shader->SetUniformVec2("u_resolution", resolution);
	shader->SetUniform1f("u_thickness", line_thickness);
}

void LineThickness::Draw()
{
	shader->Bind();
	vertex_array->Bind();

	GLCALL(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo));
	//GLsizei N = (GLsizei)current_varray->size() - 2; //Why is this -2?
	GLsizei N = (GLsizei)current_varray->size();
	//GLCALL(glDrawArrays(GL_TRIANGLES, 0, 6 * (N - 1))); // This too?
	GLCALL(glDrawArrays(GL_TRIANGLES, 0, 6 * (N)));
	//GLCALL(glDrawArrays(GL_LINES, 0, 2));
}

void LineThickness::DrawGUI()
{
	//Thick Line
	ImGui::SeparatorText("Debug Line w/ Thickness");
	ImGui::DragFloat("Thickness", &line_thickness, 0.1f, 1.0f, 256.0f);
	ImGui::DragFloat2("Line Position", position, 0.1f);
}
