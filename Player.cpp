#include "Player.h"
#include "debug.h"

Player::Player()
{
	sprite = IMG_Load("img/player.png");
}

void Player::update()
{
	x += hspeed;
	y += vspeed;
	box.x = x - origin.x;
	box.y = y - origin.y;
}

void Player::draw_self()
{
	draw_shadow();
	if (texture != NULL)
	{
		SDL_RenderCopyEx(gameRenderer, texture, NULL, &box, (((double)x / w_width) * 3600.0), &origin, SDL_FLIP_NONE);
	}
	else SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL ERROR!", "Failed to render player texture.", gameWindow);

	
}

/*Player::Player(SDL_Renderer* renderer)
{
	//sprite = SDL_LoadBMP("img/player.bmp");
	sprite = IMG_Load("img/player.png");
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
	//Draw Shadow
	draw_set_color(renderer, {0,0,0,128}); // Set shadow alpha to 50%
	draw_fillcircle(renderer,x,y+h/4,w/2);
	draw_reset_color(renderer); // Reset shadow alpha change.

	if (texture != NULL)
	{
		SDL_RenderCopyEx(renderer, texture, NULL, &box, (((double)x / w_width) * 3600.0), &origin, SDL_FLIP_NONE);
	}
	else SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL ERROR!", "Failed to render player texture.", window);
}

*/