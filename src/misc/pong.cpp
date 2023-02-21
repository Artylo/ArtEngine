#include "../system/globals.h"
#include "../system/debug.h"
#include "pong.h"
#include "stdlib.h"
#include <ctime>
#include <cmath>

float lerp(float a, float b, float f);

Pong::Pong()
{
	srand((unsigned int)time(NULL));
	Player = { 32,32,32,128 };
	Enemy = { w_width - 64,32,32,128 };
	Ball = {w_width/2, w_height/2, 32, 32,};
	BallTarget = {Ball.x,Ball.y,0,0};

	Player.speed = 5;
	Ball.speed = 5;

	BallTarget.x = 0;
	BallTarget.y = rand() % w_height + 1;
	oldEnemyY = Enemy.y;
}

Pong::~Pong()
{

}

void Pong::play(SDL_Renderer* renderer, const Uint8* keystate)
{
	update(keystate);
	draw(renderer);
}

void Pong::draw(SDL_Renderer* renderer)
{
	draw_set_color(renderer, c_white);
	SDL_RenderFillRect(renderer, new SDL_Rect{ Player.x,Player.y,32,128 });
	SDL_RenderFillRect(renderer, new SDL_Rect{ Ball.x - 16,Ball.y - 16,32,32 });
	if (debug) SDL_RenderDrawRect(renderer, new SDL_Rect{ BallTarget.x - 16,BallTarget.y - 16,32,32 });
	if (debug) if (Enemy.y == oldEnemyY) draw_set_color(renderer, c_red);
	SDL_RenderFillRect(renderer, new SDL_Rect{ Enemy.x,Enemy.y,32,128 });
	if (debug) draw_set_color(renderer, c_red);
	if (debug) SDL_RenderDrawRect(renderer, new SDL_Rect{ Enemy.x,oldEnemyY,32,128 });
	if (debug) draw_set_color(renderer, c_green);
	if (debug) SDL_RenderDrawRect(renderer, new SDL_Rect{ Enemy.x,(int)lerp((float)Enemy.y, (float)Ball.y, 1.0f) - Enemy.h / 2,32,128 });
	
	draw_reset_color(renderer);
}

void Pong::update(const Uint8* keystate)
{
	//Game Update
	if (keystate[SDL_SCANCODE_SPACE]) debug = true;
	else debug = false;

	//Player Update
	if (keystate[SDL_SCANCODE_S] || keystate[SDL_SCANCODE_DOWN])
	{
		if (Player.y + Player.h < w_height)
		{
			Player.y += Player.speed;
		}
	}
	if (keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_UP])
	{
		if (Player.y > 0)
		{
			Player.y -= Player.speed;
		}
	}

	//Enemy Update

	if (ballDirection)
	{
		if (elapsedFrames != difficulty)
		{
			interpRatio = (float)elapsedFrames / difficulty;
			Enemy.y = (int)lerp((float)oldEnemyY + Enemy.h / 2, (float)Ball.y, interpRatio) - Enemy.h / 2;
			if (elapsedFrames < difficulty) elapsedFrames++;
		}
	}


	//Ball Update

	if (Ball.x < BallTarget.x) Ball.x += Ball.speed;
	if (Ball.x > BallTarget.x) Ball.x -= Ball.speed;
	if (Ball.y < BallTarget.y) Ball.y += Ball.speed;
	if (Ball.y > BallTarget.y) Ball.y -= Ball.speed;


	if (!ballDirection) // std::bounce();
	{
		if (Ball.x - 16 <= Player.x + Player.w && Ball.y >= Player.y && Ball.y <= Player.y + Player.h)
		{
			BallTarget.x = w_width;
			BallTarget.y = rand() % w_height + 1;

			ballDirection = 1;
		}
	}
	else
	{
		if (Ball.x + 16 >= Enemy.x && Ball.y >= Enemy.y && Ball.y <= Enemy.y + Player.h)
		{
			BallTarget.x = 0;
			BallTarget.y = rand() % w_height + 1;

			oldEnemyY = Enemy.y;
			elapsedFrames = 0;

			ballDirection = 0;
		}
	}
}


float lerp(float a, float b, float f)
{
	return a + f * (b - a);
}