#pragma once
#include "globals.h"
#include "glm/glm.hpp"

#include "InputManager.h"

class GameManager
{
public:
	SDL_Window* window = nullptr;

	InputManager* input_manager = nullptr;

	float* camera_scale;
	float* camera_rotation;

	float deltaTime = 0.0f;

	//Projection Matrix
	glm::mat4* projection_matrix; // Coordinate System
	glm::mat4* identity_matrix;
	glm::mat4* scale_matrix;
	glm::mat4* transformation_matrix;
	glm::mat4* view_matrix;
	glm::mat4* model_matrix;
	glm::mat4* model_view_projection;

	/*
	SDL_Rect* camera = nullptr;
	Player* player = nullptr;
	Input* input = nullptr;
	*/
};

