#include "globals.h"
#include "Player.h"
#include "debug.h"

Player::Player()
{
	//sprite = IMG_Load("img/player.png");
	sprite = std::shared_ptr<SDL_Surface>(IMG_Load("img/player.png"), SDL_FreeSurface);

	x = 320;
	y = 240;

	x += hspeed;
	y += vspeed;

	box.x = x - origin.x; // Set coordinates of bounding box to object coordinates.
	box.y = y - origin.y;
}

void Player::get_input(Input* inpt)
{
	input = inpt;
}

void Player::update()
{
	premovementBox = box;
	//Player Inputs
	if (input->keystate[SDL_SCANCODE_W])
	{
		premovementBox.y -= speed;
		if (boxColliding != nullptr)
		{
			if (!checkCollision(premovementBox, *boxColliding))
			{
				vspeed = -speed;
			}
			else vspeed = 0;
		}
		else vspeed = -speed;
		
	}
	if (input->keystate[SDL_SCANCODE_A])
	{
		premovementBox.x -= speed;
		if (boxColliding != nullptr)
		{
			if (!checkCollision(premovementBox, *boxColliding))
			{
				hspeed = -speed;
			}
			else hspeed = 0;
		}
		else hspeed = -speed;
	}
	if (input->keystate[SDL_SCANCODE_D])
	{
		premovementBox.x += speed;
		if (boxColliding != nullptr)
		{
			if (!checkCollision(premovementBox, *boxColliding))
			{
				hspeed = speed;
			}
			else hspeed = 0;
		}
		else hspeed = speed;
	}
	if (input->keystate[SDL_SCANCODE_S])
	{
		premovementBox.y += speed;
		if (boxColliding != nullptr)
		{
			if (!checkCollision(premovementBox, *boxColliding))
			{
				vspeed = speed;
			}
			else vspeed = 0;
		}
		else vspeed = speed;
	}

	if (boxColliding != nullptr)
	{
		if (!checkCollision(premovementBox, *boxColliding)) isColliding = false;
	}
	if (!isColliding) boxColliding = NULL;

	SDL_RenderDrawRect(gameRenderer,&premovementBox);

	Entity::update();
}

void Player::draw_self()
{
	//draw_shadow();
	if (texture != NULL)
	{
		SDL_RenderCopyEx(gameRenderer, texture.get(), NULL, &box, (((double)x / w_width) * 3600.0), &origin, SDL_FLIP_NONE);
	}
	else SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL ERROR!", "Failed to render player texture.", gameWindow);
}