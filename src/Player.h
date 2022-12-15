#pragma once
#include "Entity.h"
#include "Input.h"
#include "Wall.h"
#include <set>

class Input; // Forward Declaration of Input for cyclic dependency;
class Wall; // Forward Declaration of Wall for cyclic dependency;

class Player : public Entity
{
public:

	Input* input = NULL;
	double deg = 0;

	bool izColliding           = false;
	bool verticallyColliding   = false;
	bool horizontallyColliding = false;

	SDL_Rect verticalMovementBox   = { 0,0,1,1 };
	SDL_Rect horizontalMovementBox = { 0,0,1,1 };

	Wall* wallColliding;
	std::set<SDL_Rect*> boxCollidingList;

	Player();
	//void init(SDL_Renderer* renderer, SDL_Window* window, Input* inpt);
	void get_input(Input* inpt);
	void update();
	void draw_self();

	
};
