#pragma once
#include "Entity.h"
#include "Player.h"

class Input
{

	//Input();
	//~Input();

	public:
		enum InputState
		{
			NOTPRESSED,
			PRESSED,
			RELEASED
		};
		
		int mouse_x;
		int mouse_y;
		Uint32 mouse_buttons;
		
		void update(bool* gameState, SDL_Event* eventPtr,  std::string* str, Player* player);
		bool mouseIsHovering(Entity entity);
};

