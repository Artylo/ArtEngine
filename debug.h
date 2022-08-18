#pragma once

double degtorad(double deg);
double radtodeg(double rad);
double lengthdir_x(int dist, double angle);
double lengthdir_y(int dist, double angle);
double point_distance(int x1, int y1, int x2, int y2);
double point_direction(int x1, int y1, int x2, int y2);

void draw_set_color(SDL_Renderer* renderer, SDL_Color color);
void draw_reset_color(SDL_Renderer* renderer);
void draw_grid(SDL_Renderer* renderer, SDL_Color color, Uint8 alpha);
void draw_circle(SDL_Renderer* renderer, int centerX, int centerY, int radius);
void draw_fillcircle(SDL_Renderer* renderer, int x, int y, int radius);
void draw_fillcircle2(SDL_Renderer* renderer, int centerX, int centerY, int radius);

class DebugText
{
public:
	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;
	std::string text = "ERROR";
	const char* constChar = NULL;
	SDL_Surface* textSurface = NULL;
	SDL_Texture* textTexture = NULL;
	SDL_Rect textBox = SDL_Rect{ 0,0,0,0 };
	TTF_Font* font = TTF_OpenFont("arial.ttf", 24);

	DebugText(SDL_Renderer* renderer, std::string inputText, int x, int y, int w, int h);
	~DebugText();
	void create_surface();
	void draw_text(SDL_Renderer* renderer);
	void draw_outline(SDL_Renderer* renderer, unsigned int thickness, SDL_Colour colour);
};