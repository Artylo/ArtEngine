#include "globals.h"
#include "debug.h"
#include "Input.h"

void Input::update(bool* gameState, SDL_Event* eventPtr, std::string* str, Player* player)
{
	//Mouse Events
	mouse_buttons = SDL_GetMouseState(&mouse_x,&mouse_y);

	std::string msgTxt;
	//Keyboard Events
	while (SDL_PollEvent(eventPtr))
	{
		//Handle inputs.
		switch (eventPtr->type)
		{
		case SDL_KEYDOWN:
			switch (eventPtr->key.keysym.sym)
			{
			case SDLK_ESCAPE: // Press Esc to close game.
				*gameState = false;
				break;

			case SDLK_w: // Player Movement Binds
				*str = "UP";
				player->vspeed = -player->speed;
				break;
			case SDLK_a: // 
				*str = "LEFT";
				player->hspeed = -player->speed;
				break;
			case SDLK_s: // 
				*str = "DOWN";
				player->vspeed = player->speed;
				break;
			case SDLK_d: // 
				*str = "RIGHT";
				player->hspeed = player->speed;
				break;
			case SDLK_SPACE:
				break;
			default:
				break;
			}
			break;

		case SDL_KEYUP:
			switch (eventPtr->key.keysym.sym)
			{
			case SDLK_w: // Player Movement Binds
				player->vspeed = 0;
				break;
			case SDLK_a: // 
				player->hspeed = 0;
				break;
			case SDLK_s: // 
				player->vspeed = 0;
				break;
			case SDLK_d: // 
				player->hspeed = 0;
				break;
			default:
				break;
			}
			break;

		case SDL_QUIT: // Press X on window to close game.
			*gameState = false;
			break;
		}
	}
}

bool Input::mouseIsHovering(Entity entity)
{
	if (mouse_x < entity.box.x + entity.box.w && mouse_x > entity.box.x)
	{
		if (mouse_y < entity.box.y + entity.box.h && mouse_y > entity.box.y)
		{
			return true;
		}
		
	}
	return false;
}