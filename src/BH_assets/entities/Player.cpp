#include "Player.h"

void Player::init(GameManager* gamemanager)
{
	sprite_filepath = "img/player.png";
	shader_filepath = "shader/base.shader";
	Entity::init(gamemanager);
}

void Player::update(float deltaTime)
{
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
