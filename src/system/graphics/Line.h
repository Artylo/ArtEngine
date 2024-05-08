#pragma once

#include "../graphics/Renderer.h";
#include "../graphics/IndexBuffer.h"
#include "../graphics/VertexArray.h"
#include "../graphics/VertexBuffer.h"
#include "../graphics/VertexBufferDynamic.h"
#include "../graphics/VertexBufferLayout.h"
#include "../graphics/Texture.h"

#include "glm/glm.hpp"
#include "imgui/imgui.h";


class Line
{
public:
	
private:

	std::vector<float> vertices;
	glm::vec4 line_colour = {1.0,1.0,1.0,1.0};
	float line_thickness = 1.0f;

	//Projection Matrix
	glm::mat4 projection_matrix = glm::ortho(0.0f, (float)w_width / 2, (float)w_height / 2, 0.0f, -1.0f, 1.0f); // World
	glm::mat4 view_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)); // Camera
	glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)); // Instance
	glm::mat4 model_view_projection;

	Renderer renderer;
	std::unique_ptr<VertexArray> vertex_array;
	std::unique_ptr<VertexBufferDynamic> vertex_buffer;
	std::unique_ptr<VertexBufferLayout> vertex_buffer_layout;
	std::unique_ptr<Shader> shader;
	

public:
	Line();
	Line(const Line& other);
	Line(glm::vec2 start, glm::vec2 end);
	~Line();

	void setPositions(glm::vec2 start, glm::vec2 end);
	void setMVP(glm::mat4 mvp);
	void setColour(glm::vec4 colour);
	void setColour(float colour[4]);
	void setColour(std::vector<float> colour);

	void getPositions(float* Ax, float* Ay, float* Bx, float* By);

	void Update();
	void Draw();
	
};

