#pragma once
#include "Player.h"

class Player; // Forward Declaration of Player for cyclic dependency;
class Wall; // Forward Declaration of Wall for cyclic dependency;

class Input
{
	public:

		enum InputState
		{
			NOTPRESSED,
			PRESSED,
			RELEASED
		};

		Input(SDL_Renderer* renderer, SDL_Window* window, SDL_Rect* camera);
		~Input();
		
		const Uint8* keystate = SDL_GetKeyboardState(NULL);
		SDL_Window* gameWindow = NULL;
		SDL_Renderer* gameRenderer = NULL;
		SDL_Rect* gameCamera = NULL;
		int rawMouse_x = 0;
		int rawMouse_y = 0;
		int mouseScaleX = 0;
		int mouseScaleY = 0;
		int mouse_x = 0;
		int mouse_y = 0;
		Uint32 mouse_buttons = 0;
		
		void update(bool* gameState, SDL_Event* eventPtr, Player* player, bool (*keys)[322]);
		bool mouseIsHovering(Entity entity);
		bool mouseIsHovering(Wall wall);
		void setMouseScale();
};

