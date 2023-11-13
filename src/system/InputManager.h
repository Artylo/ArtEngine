#pragma once
#include "SDL.h"


class InputManager
{

public:
	const SDL_Event* event_ptr;
	bool ProcessEvent(const SDL_Event *event);
};

