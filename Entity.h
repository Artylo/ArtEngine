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
	SDL_Rect* box = NULL;

	Entity();
	~Entity();

	void draw_self(SDL_Renderer* renderer, SDL_Window* window);
	void update();
};

