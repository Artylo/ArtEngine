#include "Player.h"

void Player::init()
{
	sprite_filepath = "img/player.png";
	shader_filepath = "shader/base.shader";
	Entity::init();
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
