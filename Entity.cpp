#include "Entity.h"
#include "debug.h"

Entity::Entity()
{
	
}

void Entity::init(SDL_Renderer* renderer, SDL_Window* window)
{
	gameRenderer = renderer; // Get pointer reference of main game renderer.
	gameWindow = window; // Get  pointer reference of main game window.

	box.x = x - origin.x; // Set coordinates of bounding box to object coordinates.
	box.y = y - origin.y;

	if (sprite != NULL)
	{
		box = SDL_Rect{ x,y,sprite->w,sprite->h }; // Define bounding box width and height based off of sprite data.
	}

	if (sprite != NULL)
	{
		texture = SDL_CreateTextureFromSurface(gameRenderer, sprite); // Generate hardware texture from sprite.
	}
}

void Entity::draw_shadow()
{
	int shadowAlpha = 128;
	SDL_Colour shadowColour = { 0,0,0,shadowAlpha };

	//Draw Shadow
	draw_set_color(gameRenderer, shadowColour); // Set shadow alpha to 50%
	draw_fillcircle(gameRenderer, x, y + (h / 4) + 4, w / 2);
	draw_reset_color(gameRenderer); // Reset shadow alpha change.
}

void Entity::draw_self()
{
	draw_shadow();
	if (texture != NULL)
	{
		//SDL_RenderCopy(gameRenderer,texture,NULL,&box);
		SDL_RenderCopyEx(gameRenderer, texture, NULL, &box, 0, &origin, SDL_FLIP_NONE);
	}
	else SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL ERROR!", "Failed to render entity texture.", gameWindow);
}

void Entity::update()
{
	x += hspeed;
	y += vspeed;
	box.x = x - origin.x; // Set coordinates of bounding box to object coordinates.
	box.y = y - origin.y;
}

Entity::~Entity()
{
	//SDL_FreeSurface(sprite);
	//if(texture != NULL) SDL_DestroyTexture(texture);
}