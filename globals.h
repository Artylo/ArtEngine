#pragma once
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include <iostream>
#include <string>
#include <vector>
#include <list>

//Window Constants
#define gameFramerate 30
#define w_width  640
#define w_height 480

//Premade Colors
#define c_red {255,0,0,255}
#define c_green { 0,255,0,255 }
#define c_blue { 0,0,255,255 }
#define c_white { 255,255,255,255 }
#define c_black { 0,0,0,255 }
#define c_gray { 100,100,100,255 }

#define c_default c_black

//Fonts
#define default_font TTF_OpenFont("arial.ttf",128)

//Structures
template <typename T>
struct DebugEntry
{
	std::string EntryName = "NULL";
	T value;
};

//Classes
class Player
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
	SDL_Surface* sprite = NULL;
	SDL_Texture* texture = NULL;
	SDL_Rect box = { 0,0,1,1 };
	SDL_Point origin = { 16,16 };

	Player(SDL_Renderer* renderer);
	~Player();

	void draw_self(SDL_Renderer* renderer, SDL_Window* window);
	void update();
};