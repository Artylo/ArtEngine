#pragma once

class Pong
{
	struct gameObject
	{
		int x = 0, y = 0;
		int w = 0, h = 0;
		int speed = 1;
	};

	bool debug = false;

	gameObject Player;
	gameObject Enemy;
	gameObject BallTarget;

	int ballDirection = 0;
	int difficulty = 100;
	float interpRatio = 0.0f;
	int elapsedFrames = 0;

	int oldEnemyY = 0;

public:
	gameObject Ball;
	Pong();
	~Pong();
	void play(SDL_Renderer* renderer, const Uint8* keystate);
	void update(const Uint8* keystate);
	void draw(SDL_Renderer* renderer);
};
