#include "Entity.h"

#include "../src/system/graphics/VertexBufferLayout.h"

void Entity::init(GameManager* gamemanager)
{
	//Set Variables
	GM = gamemanager;
	sprite.init(gamemanager);
}

void Entity::update(float deltaTime)
{
	sprite.update(deltaTime, position);
}

void Entity::draw()
{
	sprite.draw();
}

Entity::Entity()
{

}

Entity::~Entity()
{
	
}
