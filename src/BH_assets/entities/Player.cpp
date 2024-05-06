#include "Player.h"

void Player::init(GameManager* gamemanager)
{
	Entity::sprite.sprite_filepath = "img/player.png";
	Entity::sprite.shader_filepath = "shader/base.shader";
	IM = gamemanager->input_manager;
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
				position.x += speed;
				//std::cout << "RIGHT key pressed! Success!" << std::endl;
				break;
			case SDLK_LEFT:
				position.x -= speed;
				//std::cout << "LEFT key pressed! Success!" << std::endl;
				break;
			case SDLK_UP:
				position.y -= speed;
				//std::cout << "UP key pressed! Success!" << std::endl;
				break;
			case SDLK_DOWN:
				position.y += speed;
				//std::cout << "DOWN key pressed! Success!" << std::endl;
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
