#pragma once
#include "globals.h"
#include "../entities/Player.h"

class Player;
class Input;

class GameManager
{
public:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	Player* player = nullptr;
	Input* input = nullptr;
};

