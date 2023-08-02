#pragma once
#include "globals.h"
#include "glm/glm.hpp"

class Player;
class Input;

class GameManager
{
public:
	SDL_Window* window = nullptr;

	float* camera_scale;
	float* camera_rotation;

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

