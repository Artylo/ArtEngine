#include "InputManager.h"
#include <iostream>

//This function refreshes the event pointer from SLD2 and sends it to all Game Managers. This gets processed every frame in Game.cpp.
bool InputManager::ProcessEvent(const SDL_Event* event)
{
	event_ptr = event;

	//Mouse
	if (event->type == SDL_MOUSEMOTION)
	{
		//SDL_GetGlobalMouseState(&mouse_x, &mouse_y); //Mouse from 0,0 on desktop.
		mouse_state = SDL_GetMouseState(&mouse_x, &mouse_y);
	}
	//Single button down event.
	if (event->type == SDL_MOUSEBUTTONDOWN)
	{
		if (event->button.button == SDL_BUTTON_LEFT) std::cout << "LMB: " << mouse_x << "," << mouse_y << std::endl;
		if (event->button.button == SDL_BUTTON_RIGHT) std::cout << "RMB: " << mouse_x << "," << mouse_y << std::endl;
	}
	//Single releases.
	if (event->type == SDL_MOUSEBUTTONUP)
	{
		if (event->button.button == SDL_BUTTON_LEFT) std::cout << "-LMB: " << mouse_x << "," << mouse_y << std::endl;
		if (event->button.button == SDL_BUTTON_RIGHT) std::cout << "-RMB: " << mouse_x << "," << mouse_y << std::endl;
	}
	//Continuous mouse button updates (every frame).
	//if (mouse_state & SDL_BUTTON(SDL_BUTTON_Left)) std::cout << "LMB: " << mouse_x << "," << mouse_y << std::endl;
	
	//Mouse Wheel
	if (event->type == SDL_MOUSEWHEEL)
	{
		if(event->wheel.y > 0) std::cout << "Mouse wheel up." << std::endl;
		else std::cout << "Mouse wheel down." << std::endl;
	}

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



