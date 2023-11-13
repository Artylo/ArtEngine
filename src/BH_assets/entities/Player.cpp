#include "Player.h"

void Player::init(GameManager* gamemanager)
{
	sprite_filepath = "img/player.png";
	shader_filepath = "shader/base.shader";
	Entity::init(gamemanager);
}

void Player::update(float deltaTime)
{
	if (IM != nullptr)
	{
		switch (IM->event_ptr->type)
		{
		case SDL_KEYDOWN:
			switch (IM->event_ptr->key.keysym.sym)
			{
			case SDLK_RIGHT:
				std::cout << "RIGHT key pressed! Success!" << std::endl;
				break;
			}
			break;

		}
	}

	Entity::update(deltaTime);
}

void Player::draw()
{
	Entity::draw();
}

Player::Player()
{
}

Player::~Player()
{
}
