#include "Skeleton.h"

Skeleton::Skeleton()
{
	sprite = SDL_LoadBMP("img/enemy.bmp");
	if (sprite != NULL)
	{
		box = new SDL_Rect{ x,y,sprite->w,sprite->h };
	}
}