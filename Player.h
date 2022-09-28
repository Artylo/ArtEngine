#pragma once
#include "Entity.h"
#include "Input.h"

class Input; // Forward Declaration of Input for cyclic dependency;

class Player : public Entity
{
public:

	Input* input = NULL;
	double deg = 0;

	bool isColliding = false;
	SDL_Rect premovementBox = { 0,0,1,1 };
	SDL_Rect* boxColliding = NULL;

	Player();
	//void init(SDL_Renderer* renderer, SDL_Window* window, Input* inpt);
	void get_input(Input* inpt);
	void update();
	void draw_self();

	
};