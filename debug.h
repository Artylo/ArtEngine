#pragma once

void draw_set_color(SDL_Renderer* renderer, SDL_Color color);
void draw_reset_color(SDL_Renderer* renderer);
void draw_grid(SDL_Renderer* renderer, SDL_Color color, Uint8 alpha);

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