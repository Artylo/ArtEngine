#include "globals.h"

Player::Player(SDL_Renderer* renderer)
{
	sprite = SDL_LoadBMP("img/player.bmp");
	if (sprite != NULL)
	{
		texture = SDL_CreateTextureFromSurface(renderer, sprite);
		box = SDL_Rect{ x,y,sprite->w,sprite->h };
	}
}

Player::~Player()
{
	SDL_DestroyTexture(texture);
}

void Player::draw_self(SDL_Renderer* renderer, SDL_Window* window)
{
	if (texture != NULL)
	{
		SDL_RenderCopyEx(renderer, texture, NULL, &box, (((double)x / w_width) * 3600.0), &origin, SDL_FLIP_NONE);
	}
	else SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL ERROR!", "Failed to render player texture.", window);
}

void Player::update()
{
	x += hspeed;
	y += vspeed;
	box.x = x;
	box.y = y;
}