#pragma once
#include "Player.h"

class Player; // Forward Declaration of Player for cyclic dependency;

class Input
{
	public:

		enum InputState
		{
			NOTPRESSED,
			PRESSED,
			RELEASED
		};

		Input(SDL_Renderer* renderer, SDL_Window* window);
		~Input();
		
		const Uint8* keystate = SDL_GetKeyboardState(NULL);
		SDL_Window* wind = NULL;
		SDL_Renderer* rend = NULL;
		int rawMouse_x = 0;
		int rawMouse_y = 0;
		int mouseScaleX = 0;
		int mouseScaleY = 0;
		int mouse_x = 0;
		int mouse_y = 0;
		Uint32 mouse_buttons = 0;
		
		void update(bool* gameState, SDL_Event* eventPtr, Player* player);
		bool mouseIsHovering(Entity entity);
		void setMouseScale();
};

