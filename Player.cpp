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
	int collisionOffset = 0;
	verticalMovementBox = box;
	horizontalMovementBox = box;

	verticalMovementBox.x -= collisionOffset/2;
	verticalMovementBox.y -= collisionOffset/2;
	verticalMovementBox.w += collisionOffset;
	verticalMovementBox.h += collisionOffset;

	horizontalMovementBox.x -= collisionOffset / 2;
	horizontalMovementBox.y -= collisionOffset / 2;
	horizontalMovementBox.w += collisionOffset;
	horizontalMovementBox.h += collisionOffset;

	//Player Inputs if Colliding
	if (wallColliding != nullptr)
	{
		if (input->keystate[SDL_SCANCODE_W])
		{
			verticalMovementBox.y -= speed + collisionOffset;
			if (!checkCollision(verticalMovementBox, wallColliding->verticalCollisionBox))
			{
				vspeed = -speed;
			}
			else vspeed = 0;
		}
		if (input->keystate[SDL_SCANCODE_A])
		{
			horizontalMovementBox.x -= speed + collisionOffset;
			if (!checkCollision(horizontalMovementBox, wallColliding->horizontalCollisionBox))
			{
				hspeed = -speed;
			}
			else hspeed = 0;
		}
		if (input->keystate[SDL_SCANCODE_D])
		{
			horizontalMovementBox.x += speed + collisionOffset;
			if (!checkCollision(horizontalMovementBox, wallColliding->horizontalCollisionBox))
			{
				hspeed = speed;
			}
			else hspeed = 0;
		}
		if (input->keystate[SDL_SCANCODE_S])
		{
			verticalMovementBox.y += speed + collisionOffset;
			if (!checkCollision(verticalMovementBox, wallColliding->verticalCollisionBox))
			{
				vspeed = speed;
			}
			else vspeed = 0;
		}
	}
	else // Just normal movement.
	{
		if (input->keystate[SDL_SCANCODE_A])
		{
			hspeed = -speed;

		}
		if (input->keystate[SDL_SCANCODE_D])
		{
			hspeed = speed;
		}
		if (input->keystate[SDL_SCANCODE_W])
		{
			vspeed = -speed;
		}
		if (input->keystate[SDL_SCANCODE_S])
		{
			vspeed = speed;
		}
	}


	//Debug Stuff and Freeing Up Collider List.
	draw_set_color(gameRenderer, c_red);
	SDL_RenderDrawRect(gameRenderer, &verticalMovementBox);
	draw_set_color(gameRenderer, c_blue);
	SDL_RenderDrawRect(gameRenderer, &horizontalMovementBox);

	draw_set_color(gameRenderer, c_green);
	if (boxCollidingList.size() > 0)
	{
		//for (int i = 0; i < boxCollidingList.size()-1; i++)
		for (auto it = boxCollidingList.begin(); it != boxCollidingList.end();)
		{
			SDL_RenderDrawRect(gameRenderer, *it);
			if (!checkCollision(box, **it))
			{
				//boxCollidingList.erase(std::next(boxCollidingList.begin(), i));
				it = boxCollidingList.erase(it++);
				if (boxCollidingList.size() == 0)
				{
					//std::iter_swap(it, boxCollidingList.begin());
					//boxCollidingList.clear();
					break;
				}
			}
			else ++it;
		}
		if (boxCollidingList.size() == 0)
		{
			boxCollidingList.clear();
		}
	}
	else
	{
		boxCollidingList.clear();
	}


	draw_reset_color(gameRenderer);

	if (wallColliding != nullptr)
	{
		if (!checkCollision(verticalMovementBox, wallColliding->verticalCollisionBox)) // Vertical Check
		{
			verticallyColliding = true;
		}
		if (!checkCollision(horizontalMovementBox, wallColliding->horizontalCollisionBox)) // Horizontal Check
		{
			horizontallyColliding = true;
		}
		if (verticallyColliding && horizontallyColliding)
		{
			izColliding = true;
		}
		else izColliding = false;
	}
	if (!izColliding) wallColliding = NULL;

	Entity::update();
}

void Player::draw_self()
{
	//draw_shadow();
	if (texture != NULL)
	{
		SDL_RenderCopyEx(gameRenderer, texture.get(), NULL, &box, deg, &origin, SDL_FLIP_NONE);
	}
	else SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL ERROR!", "Failed to render player texture.", gameWindow);
}