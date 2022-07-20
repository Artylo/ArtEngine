#include "Entity.h"

Entity::Entity()
{
	
}

Entity::~Entity()
{
	//if (sprite != NULL)  SDL_FreeSurface(sprite);
	if (texture != NULL) SDL_DestroyTexture(texture);
}

void Entity::draw_self(SDL_Renderer* renderer, SDL_Window* window)
{

	if (sprite != NULL)
	{
		texture = SDL_CreateTextureFromSurface(renderer, sprite);
		if (texture == NULL) SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL ERROR!", "Failed to create entity texture.", window);
	}
	
	if (texture != NULL)
	{
		//SDL_RenderCopyEx(renderer, texture, NULL, box, 0, new SDL_Point{ 16,16 }, SDL_FLIP_NONE);
		SDL_RenderCopy(renderer,texture,NULL,box);
	}
	else SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL ERROR!", "Failed to render entity texture.", window);
}

void Entity::update()
{
	if (sprite != NULL)
	{
		box = new SDL_Rect{ x,y,sprite->w,sprite->h };
	}

	x += hspeed;
	y += vspeed;

	box->x = x;
	box->y = y;
}