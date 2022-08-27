#include "globals.h"
#include "debug.h"
#include "Entity.h"

double degtorad(double deg)
{
	return deg * (M_PI / 180);
}

double radtodeg(double rad)
{
	//return tan(rad * M_PI / 180);
	return rad * (180 / M_PI);
}

double lengthdir_x(int dist, double angle)
{
	return dist * cos(degtorad(angle));
}

double lengthdir_y(int dist, double angle)
{
	return dist * -sin(degtorad(angle));
}

double point_distance(int x1, int y1, int x2, int y2)
{
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

double point_direction(int x1, int y1, int x2, int y2)
{
	return -radtodeg(atan2(y1 - y2, x1 - x2)) + 180;
}

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

void draw_circle(SDL_Renderer* renderer, int centerX, int centerY, int radius) // @FIX: Line-based drawing (has overlapping lines which mess with alpha blending)
{
	const int diameter = radius * 2;
	int x = radius - 1;
	int y = 0;
	int tx = 1;
	int ty = 1;
	int error = tx - diameter;

	while (x >= y)
	{
		SDL_RenderDrawPoint(renderer, centerX + x, centerY - y);
		SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
		SDL_RenderDrawPoint(renderer, centerX - x, centerY - y);
		SDL_RenderDrawPoint(renderer, centerX - x, centerY + y);
		SDL_RenderDrawPoint(renderer, centerX + y, centerY - x);
		SDL_RenderDrawPoint(renderer, centerX + y, centerY + x);
		SDL_RenderDrawPoint(renderer, centerX - y, centerY - x);
		SDL_RenderDrawPoint(renderer, centerX - y, centerY + x);

		if (error <= 0)
		{
			++y;
			error += ty;
			ty += 2;
		}
		if (error > 0)
		{
			--x;
			tx += 2;
			error += (tx - diameter);
		}
	}
}

void draw_fillcircle(SDL_Renderer* renderer, int x, int y, int radius) // Point-based fill (works as presumed)
{
	for (int w = 0; w < radius * 2; w++)
	{
		for (int h = 0; h < radius * 2; h++)
		{
			int dx = radius - w; // horizontal offset
			int dy = radius - h; // vertical offset
			if ((dx * dx + dy * dy) <= (radius * radius))
			{
				SDL_RenderDrawPoint(renderer, x + dx, y + dy);
			}
		}
	}
}

void draw_fillcircle2(SDL_Renderer* renderer, int x, int y, int radius)
{
	int offsetx, offsety, d;

	offsetx = 0;
	offsety = radius;
	d = radius - 1;

	while (offsety >= offsetx)
	{
		SDL_RenderDrawLine(renderer, x - offsety, y + offsetx, x + offsety, y + offsetx);
		SDL_RenderDrawLine(renderer, x - offsetx, y + offsety, x + offsetx, y + offsety);
		SDL_RenderDrawLine(renderer, x - offsetx, y - offsety, x + offsetx, y - offsety);
		SDL_RenderDrawLine(renderer, x - offsety, y - offsetx, x + offsety, y - offsetx);

		if (d >= 2 * offsetx)
		{
			d -= 2 * offsetx + 1;
			offsetx += 1;
		}
		else if (d < 2 * (radius - offsety))
		{
			d += 2 * offsety - 1;
			offsety -= 1;
		}
		else
		{
			d += 2 * (offsety - offsetx - 1);
			offsety -= 1;
			offsetx += 1;
		}
	}
}

DebugText::DebugText(SDL_Renderer* renderer)
{
	rend = renderer;
}

DebugText::~DebugText()
{
	
	// Asserts always come back negative and all of this is prone to exceptions. If this causes a memory leak, I don't know how to fix it.

	/*
	assert(textSurface != NULL);
	if (surfaceExists)
	{
		SDL_FreeSurface(textSurface);
		textSurface = NULL;
	}
	assert(textTexture != NULL);
	if (textureExists)
	{
		SDL_DestroyTexture(textTexture);
		textTexture = NULL;
	}
	assert(font != NULL);
	if (fontExists)
	{
		TTF_CloseFont(font);
	}
	*/
	
}

void DebugText::create_surface()
{
	if (surfaceExists)
	{
		SDL_FreeSurface(textSurface);
		textSurface = NULL;
		assert(textSurface == NULL);
		surfaceExists = false;
	}
	if (!surfaceExists)
	{
		textSurface = TTF_RenderText_Solid(font, textChar, c_white);
		assert(textSurface != NULL);
		surfaceExists = true;
	}
}

void DebugText::draw_text(std::string inputText, int posX, int posY, int width, int height)
{	
   	x = posX;
	y = posY;
	w = width;
	h = height;
	textBox.x = x;
	textBox.y = y;
	textBox.w = w;
	textBox.h = h;

	if(inputText != text)
	{
		text = inputText;
		textChar = text.c_str();

		create_surface();

		if (textureExists)
		{
			SDL_DestroyTexture(textTexture);
			textureExists = false;
		}
		if (!textureExists)
		{
			textTexture = SDL_CreateTextureFromSurface(rend, textSurface);
			textureExists = true;
		}
	}

	if (textureExists)
	{
		draw_outline(2, c_black);
		SDL_RenderCopy(rend, textTexture, NULL, &textBox);
	}
}

void DebugText::draw_outline(unsigned int thickness, SDL_Colour colour)
{
	int offset = 1;
	SDL_Colour col = c_black;

	offset = thickness;
	col = colour;

	SDL_Rect textBoxCopy = SDL_Rect{x, y, w, h};

	SDL_Surface* outlineSurface = TTF_RenderText_Solid(font, textChar, col);
	SDL_Texture* outlineTexture = SDL_CreateTextureFromSurface(rend,outlineSurface);

		textBoxCopy.x -= offset;
			SDL_RenderCopy(rend, outlineTexture, NULL, &textBoxCopy);
		textBoxCopy.y -= offset;
			SDL_RenderCopy(rend, outlineTexture, NULL, &textBoxCopy);
		textBoxCopy.x += 2*offset;
			SDL_RenderCopy(rend, outlineTexture, NULL, &textBoxCopy);
		textBoxCopy.y += 2*offset;
			SDL_RenderCopy(rend, outlineTexture, NULL, &textBoxCopy);

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