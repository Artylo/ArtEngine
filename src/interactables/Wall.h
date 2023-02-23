#pragma once
#include "../system/GameManager.h"

class GameManager;
class Player; // Player reference for circular dependency.
class Input; // Forward Declaration of Input for cyclic dependency;

class Wall
{
public:
	SDL_Point pos = {0,0};
	GameManager* GM;
	SDL_Renderer* gameRenderer = nullptr;
	SDL_Window* gameWindow = nullptr;
	SDL_Rect* gameCamera = nullptr;
	std::shared_ptr<SDL_Surface> sprite = nullptr;
	std::shared_ptr <SDL_Texture> texture = nullptr;
	SDL_Rect box = { 0,0,1,1 };

	SDL_Rect verticalCollisionBox   = { 0,0,1,1 };
	SDL_Rect horizontalCollisionBox = { 0,0,1,1 };

	Player* player;
	Input* input;

	//Gameplay Elements
	int initial_durability = 100;
	int durability = initial_durability;

	Wall();
	~Wall();
	void init(GameManager* GM);
	void update();
	void draw_self();
	void generate_texture();
	
	//Operator Overloads
	friend bool operator==(const Wall& lhs, const Wall& rhs)
	{
		return lhs.pos.x < rhs.pos.x&& lhs.pos.y == rhs.pos.y;
	}
	friend bool operator<(const Wall& lhs, const Wall& rhs)
	{
		return std::tie(lhs.pos.x, lhs.pos.y) < std::tie(rhs.pos.x, rhs.pos.y);
	}
	friend bool operator>(const Wall& lhs, const Wall& rhs)
	{
		return std::tie(lhs.pos.x, lhs.pos.y) > std::tie(rhs.pos.x, rhs.pos.y);
	}

};

