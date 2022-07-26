#pragma once
#include "globals.h"

class Entity
{
private:

public:
	int x = 0;
	int y = 0;
	int w = 32;
	int h = 32;
	int speed = 5;
	int hspeed = 0;
	int vspeed = 0;
	SDL_Surface* sprite = SDL_LoadBMP("img/tile.bmp");
	SDL_Texture* texture = NULL;
	SDL_Rect box = {0,0,1,1};
	SDL_Point origin = {16,16};

	Entity();

	void draw_self(SDL_Renderer* renderer, SDL_Window* window);
	void init(SDL_Renderer* renderer);
	void update();

	~Entity();
};

