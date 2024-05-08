#pragma once
#include "SDL.h"
#include "glm/glm.hpp"


class InputManager
{

public:

	//Mouse
	Uint32 mouse_state = NULL;
	int mouse_x = NULL;
	int mouse_y = NULL;

	//System
	const SDL_Event* event_ptr;
	bool ProcessEvent(const SDL_Event *event);

	glm::vec2 GetMousePosition(int& x, int& y);
	glm::vec2 GetMousePosition();
};

