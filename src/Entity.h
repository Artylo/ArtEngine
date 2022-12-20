#pragma once

#include "globals.h"

class Entity
{
private:

public:
	SDL_Point pos = {0,0};
	int w = 32;
	int h = 32;
	int speed = 5;
	int hspeed = 0;
	int vspeed = 0;
	SDL_Renderer* gameRenderer = NULL;
	SDL_Window* gameWindow = NULL;
	SDL_Rect* gameCamera = NULL;
	std::shared_ptr<SDL_Surface> sprite = nullptr;
	std::shared_ptr <SDL_Texture> texture = nullptr;
	SDL_Rect box = {0,0,1,1};
	SDL_Point origin = {16,16};

	//Entity();

	void init(SDL_Renderer* renderer, SDL_Window* window, SDL_Rect* camera);
	void draw_shadow();
	void draw_self();
	void update();

	~Entity();
};

