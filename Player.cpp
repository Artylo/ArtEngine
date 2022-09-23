#include "Player.h"
#include "debug.h"

Player::Player()
{
	//sprite = IMG_Load("img/player.png");
	sprite = std::shared_ptr<SDL_Surface>(IMG_Load("img/player.png"), SDL_FreeSurface);
}

void Player::update()
{
	Entity::update();
}

void Player::draw_self()
{
	draw_shadow();
	if (texture != NULL)
	{
		SDL_RenderCopyEx(gameRenderer, texture.get(), NULL, &box, (((double)x / w_width) * 3600.0), &origin, SDL_FLIP_NONE);
	}
	else SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL ERROR!", "Failed to render player texture.", gameWindow);
}