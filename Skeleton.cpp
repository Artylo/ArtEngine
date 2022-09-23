#include "Skeleton.h"
#include "debug.h"

Skeleton::Skeleton()
{
	//sprite = IMG_Load("img/skeleton.png");
	//std::shared_ptr<SDL_Surface> shared_sprite(IMG_Load("img/skeleton.png"), SDL_FreeSurface);
	sprite = std::shared_ptr<SDL_Surface>(IMG_Load("img/skeleton.png"), SDL_FreeSurface);
}

void Skeleton::update()
{
	Entity::update();
}