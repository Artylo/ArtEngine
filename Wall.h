#pragma once

class Player; // Player reference for circular dependency.

class Wall
{
public:
	SDL_Point pos = {0,0};
	SDL_Renderer* gameRenderer = NULL;
	SDL_Window* gameWindow = NULL;
	std::shared_ptr<SDL_Surface> sprite = nullptr;
	std::shared_ptr <SDL_Texture> texture = nullptr;
	SDL_Rect box = { 0,0,1,1 };

	SDL_Rect verticalCollisionBox   = { 0,0,1,1 };
	SDL_Rect horizontalCollisionBox = { 0,0,1,1 };

	Player* player;

	Wall();
	~Wall();
	void init(SDL_Renderer* renderer, SDL_Window* window, Player* plr);
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

