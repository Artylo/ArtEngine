#include "InputManager.h"
#include <iostream>

bool InputManager::ProcessEvent(const SDL_Event* event)
{
	event_ptr = event;

	//@TODO: Maybe I can have the system work so I can have rebindable keys and it just references a bunch of preset actions.
	//e.g. player_move_left = SDLK_LEFT | SDLK_W;
	// And then every time I want some action to be done from some other object I find a way to send a request for player_move_left.

	switch (event->type)
	{
	case SDL_KEYDOWN:
		switch (event->key.keysym.sym)
		{
		case SDLK_LEFT:
			//std::cout << "Left key pressed." << std::endl;
			break;
		}
		break;
	}
	return true;
}



