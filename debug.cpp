#include "globals.h"
#include "debug.h"
#include "Entity.h"

void draw_set_color(SDL_Renderer* renderer, SDL_Color color)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void draw_reset_color(SDL_Renderer* renderer)
{
	SDL_Color color = c_default;
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void draw_grid(SDL_Renderer *renderer, SDL_Color color, Uint8 alpha)
{	
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, alpha);

	int hTimes = w_height/32;
	int wTimes = w_width/32;

	for (int i = 0; i < hTimes; i++)
	{
		SDL_RenderDrawLine(renderer, 0, i * 32, w_width, i * 32);
	}
	for (int j = 0; j < wTimes; j++)
	{
		SDL_RenderDrawLine(renderer, j * 32, 0, j * 32, w_height);
	}
	draw_reset_color(renderer);
}

DebugText::DebugText(SDL_Renderer* renderer, std::string inputText, int posX, int posY, int width, int height)
{
	x = posX;
	y = posY;
	w = width;
	h = height;
	text = inputText;
	constChar = text.c_str();
	create_surface();
	draw_text(renderer);
}

DebugText::~DebugText()
{
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
	Entity entity;
}

void DebugText::create_surface()
{
	textBox->x = x;
	textBox->y = y;
	if (textSurface == NULL)
	{
		textBox->w = w;
		textBox->h = h;
		TTF_Font* font = TTF_OpenFont("arial.ttf", w);
		textSurface = TTF_RenderText_Solid(font, constChar, c_white);
		
	}
}

void DebugText::draw_text(SDL_Renderer* renderer)
{	
	if (textTexture == NULL)
	{
		textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	}
	if (textTexture != NULL)
	{
		draw_outline(renderer,2,c_black);
		SDL_RenderCopy(renderer, textTexture, NULL, textBox);
	}
}

void DebugText::draw_outline(SDL_Renderer* renderer, unsigned int thickness, SDL_Colour colour)
{
	int offset = 1;
	SDL_Colour col = c_black;

	offset = thickness;
	col = colour;

	SDL_Rect* textBoxCopy = new SDL_Rect{x, y, w, h};

	SDL_Surface* outlineSurface = TTF_RenderText_Solid(TTF_OpenFont("arial.ttf", w), constChar, col);
	SDL_Texture* outlineTexture = SDL_CreateTextureFromSurface(renderer,outlineSurface);

		textBoxCopy->x -= offset;
			SDL_RenderCopy(renderer, outlineTexture, NULL, textBoxCopy);
		textBoxCopy->y -= offset;
			SDL_RenderCopy(renderer, outlineTexture, NULL, textBoxCopy);
		textBoxCopy->x += 2*offset;
			SDL_RenderCopy(renderer, outlineTexture, NULL, textBoxCopy);
		textBoxCopy->y += 2*offset;
			SDL_RenderCopy(renderer, outlineTexture, NULL, textBoxCopy);

	SDL_FreeSurface(outlineSurface);
	SDL_DestroyTexture(outlineTexture);
}

class DebugList
{
private: // Variables
	std::string preprocessedDebugList;
	DebugEntry<int> playerX = {"PlayerX", 1};
	DebugEntry<int> playerY = {"PlayerY", 2};
	DebugEntry<std::string> playerName = {"Name", "jimmothy"};
	DebugEntry<float> playerHeight = { "PlayerHeight", 5.11f };

public:
	//std::vector<std::string> arr;

	std::vector<DebugEntry<int>> ints;
	std::vector<DebugEntry<float>> floats;
	std::vector<DebugEntry<std::string>> strings;

private: // Functions
	DebugList()
	{
		ints.push_back(playerX);
		ints.push_back(playerY);
		strings.push_back(playerName);
		floats.push_back(playerHeight);
	}
	~DebugList()
	{

	}

public:
	void update()
	{
		preprocessedDebugList.clear();
		for (int i = 0; i < ints.size(); i++)
		{
			preprocessedDebugList += ints[i].EntryName + ": " + std::to_string(ints[i].value) + "\n";
		}
		for (int i = 0; i < floats.size(); i++)
		{
			preprocessedDebugList += floats[i].EntryName + ": " + std::to_string(floats[i].value) + "\n";
		}
		for (int i = 0; i < floats.size(); i++)
		{
			preprocessedDebugList += strings[i].EntryName + ": " + strings[i].value + "\n";
		}
	}
	void print()
	{
		std::cout << preprocessedDebugList << std::endl;
	}
};