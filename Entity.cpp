#include "Entity.h"

Entity::Entity()
{
	
}

void Entity::draw_self(SDL_Renderer* renderer, SDL_Window* window)
{
	if (texture != NULL)
	{
		//SDL_RenderCopyEx(renderer, texture, NULL, box, 0, new SDL_Point{ 16,16 }, SDL_FLIP_NONE);
		SDL_RenderCopy(renderer,texture,NULL,&box);
	}
	else SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL ERROR!", "Failed to render entity texture.", window);

	//if (texture != NULL) SDL_DestroyTexture(texture);
}

void Entity::init(SDL_Renderer* renderer)
{
	if (sprite != NULL)
	{
		box = SDL_Rect{ x,y,sprite->w,sprite->h };
	}

	box.x = x;
	box.y = y;

	x += hspeed;
	y += vspeed;

	if (sprite != NULL)
	{
		texture = SDL_CreateTextureFromSurface(renderer, sprite);
		//if (texture == NULL) SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL ERROR!", "Failed to create entity texture.", window);
	}
	update();
}

void Entity::update()
{

}

Entity::~Entity()
{
	SDL_FreeSurface(sprite);
	SDL_DestroyTexture(texture);
}