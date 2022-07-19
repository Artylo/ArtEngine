#include "globals.h"

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