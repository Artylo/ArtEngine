#pragma once

#include "globals.h"

//Collision
bool checkCollision(SDL_Rect a, SDL_Rect b);

//Loose Math Functions
Uint8 random(int max);
int random_range(int min, int max);
double degtorad(double deg);
double radtodeg(double rad);
double lengthdir_x(int dist, double angle);
double lengthdir_y(int dist, double angle);
double point_distance(int x1, int y1, int x2, int y2);
double point_direction(int x1, int y1, int x2, int y2);

//Loose Draw Functions
void draw_set_color(SDL_Renderer* renderer, SDL_Color color);
void draw_set_texture_color_blend(SDL_Color col, SDL_Texture* tex);
void draw_reset_color(SDL_Renderer* renderer);
void draw_grid(SDL_Renderer* renderer, SDL_Color color, Uint8 alpha);
void draw_circle(SDL_Renderer* renderer, int centerX, int centerY, int radius);
void draw_fillcircle(SDL_Renderer* renderer, int x, int y, int radius);
void draw_fillcircle2(SDL_Renderer* renderer, int centerX, int centerY, int radius);

//Debug Elements
class DebugText
{
public:
	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;
	std::string text = "ERROR";
	const char* textChar = text.c_str();
	SDL_Surface* textSurface = NULL;
	SDL_Texture* textTexture = NULL;
	SDL_Rect textBox = SDL_Rect{ 0,0,0,0 };
	TTF_Font* font = NULL;
	TTF_Font* font_outline = NULL;
	Uint16 outline_thickness = 1;
	SDL_Renderer* rend = NULL;

	DebugText(SDL_Renderer* renderer);
	~DebugText();
	void create_surface();
	void draw_text(std::string inputText, int posX, int posY);
	void draw_outline(SDL_Colour colour);
};

//Templates
//@CLEANUP: This is super ugly but if defined in source it causes a compiler error - https://stackoverflow.com/questions/456713/why-do-i-get-unresolved-external-symbol-errors-when-using-templates

template<class T>
T choose(std::initializer_list<T> list)
{
	//SDL_Log("Choosing from %i choices.\n", list.size());
	std::vector<T> choices;
	for (auto n : list)
	{
		choices.push_back(n);
	}
	return choices[random(choices.size() - 1)];
}