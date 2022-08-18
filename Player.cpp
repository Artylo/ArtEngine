#include "Player.h"
#include "debug.h"

Player::Player()
{
	sprite = IMG_Load("img/player.png");
}

void Player::update()
{
	x += hspeed;
	y += vspeed;
	box.x = x - origin.x;
	box.y = y - origin.y;
}

void Player::draw_self()
{
	draw_shadow();
	if (texture != NULL)
	{
		SDL_RenderCopyEx(gameRenderer, texture, NULL, &box, (((double)x / w_width) * 3600.0), &origin, SDL_FLIP_NONE);
	}
	else SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL ERROR!", "Failed to render player texture.", gameWindow);

	// @CLEANUP: Debug for new maths functions.
	int debdist = 57;
	int acdist = 0;

	deg += 1;
	int a = x + (int)lengthdir_x(58, 90);
	int b = y + (int)lengthdir_y(58, 90);
	acdist = (int)point_distance(x,y,a,b);
	DebugText debugAngle0(gameRenderer, "90", a, b, w, h/2);

	draw_circle(gameRenderer, x, y, acdist);

	draw_set_color(gameRenderer, c_red);

		draw_circle(gameRenderer, a, b, 5);
		SDL_RenderDrawLine(gameRenderer, x, y, a, b);

		a = x + (int)lengthdir_x(58, 180);
		b = y + (int)lengthdir_y(58, 180);
		DebugText debugAngle1(gameRenderer, "180", a, b, w, h / 2);

		draw_circle(gameRenderer, a, b, 5);
		SDL_RenderDrawLine(gameRenderer, x, y, a, b);

		a = x + (int)lengthdir_x(58, 270);
		b = y + (int)lengthdir_y(58, 270);
		DebugText debugAngle2(gameRenderer, "270", a, b, w, h / 2);

		draw_circle(gameRenderer, a, b, 5);
		SDL_RenderDrawLine(gameRenderer, x, y, a, b);

		a = x + (int)lengthdir_x(58, 360);
		b = y + (int)lengthdir_y(58, 360);
		DebugText debugAngle3(gameRenderer, "360", a, b, w, h / 2);

		draw_circle(gameRenderer, a, b, 5);
		SDL_RenderDrawLine(gameRenderer, x, y, a, b);

		a = x + (int)lengthdir_x(58, 0);
		b = y + (int)lengthdir_y(58, 0);
		DebugText debugAngle4(gameRenderer, "0", a, b, w, h / 2);

	draw_set_color(gameRenderer, c_green);

		SDL_RenderDrawPoint(gameRenderer, a, b);

	draw_set_color(gameRenderer, c_blue);

		a = x + (int)lengthdir_x(58, deg);
		b = y + (int)lengthdir_y(58, deg);
		draw_circle(gameRenderer, a, b, 5);
		SDL_RenderDrawLine(gameRenderer, x, y, a, b);

	draw_reset_color(gameRenderer);
	//@CLEANUP: 
}

/*Player::Player(SDL_Renderer* renderer)
{
	//sprite = SDL_LoadBMP("img/player.bmp");
	sprite = IMG_Load("img/player.png");
	if (sprite != NULL)
	{
		texture = SDL_CreateTextureFromSurface(renderer, sprite);
		box = SDL_Rect{ x,y,sprite->w,sprite->h };
	}
}

Player::~Player()
{
	SDL_DestroyTexture(texture);
}

void Player::draw_self(SDL_Renderer* renderer, SDL_Window* window)
{
	//Draw Shadow
	draw_set_color(renderer, {0,0,0,128}); // Set shadow alpha to 50%
	draw_fillcircle(renderer,x,y+h/4,w/2);
	draw_reset_color(renderer); // Reset shadow alpha change.

	if (texture != NULL)
	{
		SDL_RenderCopyEx(renderer, texture, NULL, &box, (((double)x / w_width) * 3600.0), &origin, SDL_FLIP_NONE);
	}
	else SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL ERROR!", "Failed to render player texture.", window);
}

*/