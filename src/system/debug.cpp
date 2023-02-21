#include "debug.h"
#include "../entities/Entity.h"

bool checkCollision(SDL_Rect a, SDL_Rect b)
{
	return SDL_HasIntersection(&a,&b);
}

Uint8 random(int max)
{
	return rand() % (max+1);
}

int random_range(int min, int max)
{
	return (rand() % (max - min)) + min;
}

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

void draw_set_texture_color_blend(SDL_Color col, SDL_Texture* tex)
{
	SDL_SetTextureColorMod(tex, col.r, col.g, col.b);
}


void draw_grid(SDL_Renderer *renderer, SDL_Color color, Uint8 alpha)
{	
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, alpha);

	int hTimes = world_height/32;
	int wTimes = world_width/32;

	for (int i = 0; i < hTimes; i++)
	{
		SDL_RenderDrawLine(renderer, 0, i * 32, world_width, i * 32);
	}
	for (int j = 0; j < wTimes; j++)
	{
		SDL_RenderDrawLine(renderer, j * 32, 0, j * 32, world_height);
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

void draw_fillcircle(SDL_Renderer* renderer, int x, int y, int radius) // Point-based fill (works as presumed) - causes massive slow-down and FPS issues
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

DebugText::DebugText(SDL_Renderer* renderer, SDL_Rect* camera)
{
	rend = renderer;
	gameCamera = camera;

	font = TTF_OpenFont("arial.ttf", 24);

	font_outline = TTF_OpenFont("arial.ttf", 24);
	TTF_SetFontOutline(font_outline, outline_thickness);
}

DebugText::~DebugText()
{
	//Contents Font
	assert(font != NULL);
	TTF_CloseFont(font);
	font = NULL;

	//Outline Font
	assert(font_outline != NULL);
	TTF_CloseFont(font_outline);
	font_outline = NULL;
}

void DebugText::create_surface()
{
	if (textSurface != NULL)
	{
		SDL_FreeSurface(textSurface);
		textSurface = NULL;
		assert(textSurface == NULL);
	}
	if (textSurface == NULL)
	{
   		textSurface = TTF_RenderText_Solid(font, textChar, c_white);
		assert(textSurface != NULL);
	}
}

void DebugText::draw_text(std::string inputText, int posX, int posY)
{	
	
	pos.x = posX;
	pos.y = posY;

	if(inputText != text)
	{
		text = inputText;
		textChar = text.c_str();

		create_surface();

		if (textTexture != NULL)
		{
			SDL_DestroyTexture(textTexture);
			textTexture = NULL;
		}
		if (textTexture == NULL)
		{
			textTexture = SDL_CreateTextureFromSurface(rend, textSurface);
		}
	}

	TTF_SizeText(font, text.c_str(), &w, &h);

	//@CLEANUP: I think it's redundant to have independent coords and a SDL_Rect.
	textBox.x = pos.x;
	textBox.y = pos.y;
	textBox.w = w;
	textBox.h = h;

	//@CLEANUP: Debug text rectangle
	//SDL_RenderDrawRect(rend, &textBox);

	if (SDL_PointInRect(&pos,gameCamera))
	{
		if (textTexture != NULL)
		{
			draw_outline(c_black);
			SDL_RenderCopy(rend, textTexture, NULL, &textBox);
		}
	}
	
}

void DebugText::draw_gui_text(std::string inputText, int posX, int posY)
{

	pos.x = posX;
	pos.y = posY;

	if (inputText != text)
	{
		text = inputText;
		textChar = text.c_str();

		create_surface();

		if (textTexture != NULL)
		{
			SDL_DestroyTexture(textTexture);
			textTexture = NULL;
		}
		if (textTexture == NULL)
		{
			textTexture = SDL_CreateTextureFromSurface(rend, textSurface);
		}
	}

	TTF_SizeText(font, text.c_str(), &w, &h);

	//@CLEANUP: I think it's redundant to have independent coords and a SDL_Rect.
	textBox.x = pos.x;
	textBox.y = pos.y;
	textBox.w = w;
	textBox.h = h;

	//@CLEANUP: Debug text rectangle
	//SDL_RenderDrawRect(rend, &textBox);


	if (textTexture != NULL)
	{
		draw_outline(c_black);
		SDL_RenderCopy(rend, textTexture, NULL, &textBox);
	}

}

void DebugText::draw_outline(SDL_Colour colour)
{
	SDL_Color outline_colour = colour;

	SDL_Surface* outlineSurface = TTF_RenderText_Solid(font_outline, textChar, outline_colour);
	SDL_Texture* outlineTexture = SDL_CreateTextureFromSurface(rend,outlineSurface);

	SDL_Rect* rect = new SDL_Rect{ textBox.x - outline_thickness, textBox.y - outline_thickness, outlineSurface->w, outlineSurface->h};

	SDL_RenderCopy(rend, outlineTexture, NULL, rect);

	SDL_FreeSurface(outlineSurface);
	SDL_DestroyTexture(outlineTexture);
	delete rect;
}