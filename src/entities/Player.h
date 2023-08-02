#pragma once
#include "Entity.h"

class Player : public Entity
{
public:
	void init();
	void update();
	void draw();

	Player();
	~Player();
};

