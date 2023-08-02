#pragma once
#include "Entity.h"

class Player : public Entity
{
public:
private:

public:
	void init();
	void update(float deltaTime);
	void draw();

	Player();
	~Player();
};

