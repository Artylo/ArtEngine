#include "../system/debug.h"
#include "../entities/Entity.h"



void Entity::init(SDL_Renderer* renderer, SDL_Window* window, SDL_Rect* camera)
{
	gameRenderer = renderer; // Get pointer reference of main game renderer.
	gameWindow = window; // Get  pointer reference of main game window.
	gameCamera = camera; // Get pointer reference for main game camera.

	box.x = pos.x - origin.x; // Set coordinates of bounding box to object coordinates.
	box.y = pos.y - origin.y;

	//box.x -= camera.x;
	//box.y -= camera.y;

	if (sprite != NULL)
	{
		box = SDL_Rect{ pos.x,pos.y,sprite->w,sprite->h }; // Define bounding box width and height based off of sprite data.
	}

	if (sprite != NULL)
	{
		texture = std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(gameRenderer, sprite.get()), SDL_DestroyTexture); // Generate hardware texture from sprite.

		if (texture.get() == NULL)
		{
			SDL_Log("Failed to create skele");
		}
	}
}

void Entity::draw_shadow()
{
	Uint8 shadowAlpha = 128;
	SDL_Colour shadowColour = { 0,0,0,shadowAlpha };

	//Draw Shadow
	draw_set_color(gameRenderer, shadowColour); // Set shadow alpha to 50%
	//draw_fillcircle2(gameRenderer, x, y + (h / 4) + 4, w / 2);
	draw_fillcircle(gameRenderer, pos.x, pos.y + (h / 4) + 4, w / 2);
	draw_reset_color(gameRenderer); // Reset shadow alpha change.
}

void Entity::draw_self()
{
	//draw_shadow();
	if (SDL_PointInRect(&pos,gameCamera))
	{
		if (texture != NULL)
		{
			//SDL_RenderCopy(gameRenderer,texture,NULL,&box);
			SDL_RenderCopyEx(gameRenderer, texture.get(), NULL, &box, 0, &origin, SDL_FLIP_NONE);
		}
		else SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL ERROR!", "Failed to render entity texture.", gameWindow);
	}
}

void Entity::update()
{
	pos.x += hspeed;
	pos.y += vspeed;

	box.x = pos.x - origin.x; // Set coordinates of bounding box to object coordinates.
	box.y = pos.y - origin.y;

	//box.x -= gameCamera->x;
	//box.y -= gameCamera->y;
}

Entity::~Entity()
{
	/*
	SDL_FreeSurface(sprite.get());
	sprite = nullptr;
	
	SDL_DestroyTexture(texture);
	texture = nullptr;
	*/
}