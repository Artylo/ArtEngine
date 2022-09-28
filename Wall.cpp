#include "globals.h"
#include "Entity.h"
#include "Player.h"
#include "debug.h"
#include "Wall.h"

Wall::Wall()
{
	
	sprite = std::shared_ptr<SDL_Surface>(IMG_Load("img/tile.bmp"), SDL_FreeSurface);
	assert(sprite.get() != NULL);

	box.w = sprite.get()->w;
	box.h = sprite.get()->h;
}

void Wall::init(SDL_Renderer* renderer, SDL_Window* window, Player* plr)
{
	gameRenderer = renderer;
	gameWindow = window;
	player = plr;
}

Wall::~Wall()
{

}

void Wall::update()
{
	if(box.x != pos.x) box.x = pos.x;
	if(box.y != pos.y) box.y = pos.y;

	if (checkCollision(box, player->premovementBox))
	{
		player->isColliding = true;
		player->boxColliding = &box;
	}
}

void Wall::draw_self()
{
	update();
	generate_texture();
	if (texture.get() != NULL)
	{
		SDL_RenderCopy(gameRenderer, texture.get(), NULL, &box);
	}
}

void Wall::generate_texture()
{
	if (texture.get() == NULL)
	{
		texture = std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(gameRenderer,sprite.get()),SDL_DestroyTexture);
		assert(texture.get() != NULL);
	}
}