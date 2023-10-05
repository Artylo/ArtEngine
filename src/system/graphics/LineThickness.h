#pragma once

#include "../globals.h"
#include "../graphics/Renderer.h" // implements Shader, VertexArray
//#include "../graphics/VertexBuffer.h"
//#include "../graphics/VertexBufferDynamic.h"
//#include "../graphics/VertexBufferLayout.h"

#include "glm/glm.hpp" // GL Maths
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h" // GUI

#include <memory>


class LineThickness
{
private:
	//Projection Matrix
	glm::mat4 projection_matrix = glm::ortho(0.0f, (float)w_width / 2, (float)w_height / 2, 0.0f, -1.0f, 1.0f); // World
	glm::mat4 view_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)); // Camera
	glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)); // Instance
	glm::mat4 model_view_projection;

	std::unique_ptr<Shader> shader;
	std::unique_ptr<VertexArray> vertex_array;
	GLuint ssbo; // Shared Storage Buffer Object

	glm::vec2 resolution = {w_width, w_height};
	float thickness = 20.0;
	std::vector<glm::vec4> varray1; // Square
	std::vector<glm::vec4> varray2; // Circle

public:
	LineThickness(glm::vec2 start, glm::vec2 end);
	~LineThickness();

	GLuint CreateSSBO(std::vector<glm::vec4>& varray);

	void setPositions(glm::vec2 start, glm::vec2 end);
	void setMVP(glm::mat4 mvp);
	void setColour(glm::vec4 colour);
	void setColour(float colour[4]);
	void setThickness(float param_thickness);

	void Update(float deltaTime);
	void Draw();
};

