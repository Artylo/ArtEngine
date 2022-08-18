#pragma once
#include "Entity.h"
class Player : public Entity
{
public:
	Player();
	void update();
	void draw_self();

	double deg = 0;
};