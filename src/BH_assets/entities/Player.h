#pragma once
#include "Entity.h"
#include "../../system/InputManager.h"

class Player : public Entity
{
public:
	int speed = 5;
private:
	InputManager* IM;

public:
	void init(GameManager* gamemanager);
	void update(float deltaTime);
	void draw();

	Player();
	~Player();
};

