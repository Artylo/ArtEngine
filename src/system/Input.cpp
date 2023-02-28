#include "globals.h"
#include "debug.h"
#include "../entities/Entity.h"
#include "Input.h"

Input::Input(GameManager* gm)
{
	gameWindow = gm->window;
	gameRenderer = gm->renderer;
	gameCamera = gm->camera;
}

Input::~Input()
{

}

void Input::update(bool* gameState, SDL_Event* eventPtr, Player* player)
{
	/*
	//Mouse Events
	mouse_buttons = SDL_GetMouseState(&rawMouse_x,&rawMouse_y); // Gets Mouse coords relative to window, not renderer. Needs scaling to work properly but doesn't fully account for non 16:9 aspect ratios.
	*/
	setMouseScale();
	//Reset mouse buttons.
	mouse_left = NOT_PRESSED;
	mouse_right = NOT_PRESSED;

	//Single Press
	while (SDL_PollEvent(eventPtr))
	{
		//Handle inputs - Single Press.
		switch (eventPtr->type)
		{
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

		case SDL_KEYDOWN:
			switch (eventPtr->key.keysym.sym)
			{
			case SDLK_ESCAPE: // Press Esc to close game.
				*gameState = false;
				break;
			case SDLK_SPACE:
				break;

			default:
				break;
			}
			break;

		case SDL_MOUSEMOTION: // Alternative Mouse Coordinates (this is renderer-relative not window-relative)
			rawMouse_x = eventPtr->motion.x;
			rawMouse_y = eventPtr->motion.y;
			break;

		case SDL_MOUSEBUTTONDOWN:
			switch (eventPtr->button.button)
			{
				case SDL_BUTTON_LEFT:
						mouse_left = PRESSED;
					break;
				case SDL_BUTTON_RIGHT:
						mouse_right = PRESSED;
					break;
			}
			break;

		case SDL_QUIT: // Press X on window to close game.
			*gameState = false;
			break;
		}
	}
}

bool Input::mouseIsHovering(Entity entity) // Entity-version
{
	SDL_Point mouse = { mouse_x + gameCamera->x, mouse_y + gameCamera->y };
	SDL_Rect boxHover = { entity.box.x,entity.box.y,entity.box.w,entity.box.h };
	
	if (SDL_EnclosePoints(&mouse, 1, &boxHover, nullptr))
	{
		return true;
	}
	return false;

}

bool Input::mouseIsHovering(Wall wall) // Wall-version
{
	//@TODO: Convert to template for easier access.
	SDL_Point mouse = { mouse_x + gameCamera->x, mouse_y + gameCamera->y};
	SDL_Rect boxHover = { wall.box.x,wall.box.y,wall.box.w,wall.box.h };

	if (SDL_EnclosePoints(&mouse, 1, &boxHover, nullptr))
	{
		return true;
	}
	return false;
}


void Input::setMouseScale()
{
	bool doScaling = false;

	/*
	//Debug
	SDL_Log("rMouseX = %d \n", rawMouse_x);
	SDL_Log("rMouseY = %d \n", rawMouse_y);
	SDL_Log("MouseX = %d \n", mouse_x);
	SDL_Log("MouseY = %d \n", mouse_y);
	*/

	if (doScaling) //@CLEANUP: This may come useful at some point, I do not fully anticipate needing it right now.
	{
		//Display Size
		SDL_DisplayMode DM;
		SDL_GetCurrentDisplayMode(0, &DM);
		int screenWidth = DM.w;
		int screenHeight = DM.h;

		int currentWindowWidth;
		int currentWindowHeight;

		/*
		int rendW, rendH;
		SDL_RenderGetLogicalSize(rend,&rendW,&rendH);
		*/

		SDL_GetWindowSize(gameWindow, &currentWindowWidth, &currentWindowHeight);

		mouseScaleX = (int)((float)currentWindowWidth / (float)w_width);
		mouseScaleY = (int)((float)currentWindowHeight / (float)w_height);

		int Xoffset = (DM.w - w_width * mouseScaleX) / 2;

		mouse_x = (int)(rawMouse_x / mouseScaleX);
		mouse_y = (int)(rawMouse_y / mouseScaleY);

		mouse_x += Xoffset;
	}
	else
	{
		mouse_x = rawMouse_x;
		mouse_y = rawMouse_y;
	}
}