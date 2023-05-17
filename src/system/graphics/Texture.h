#pragma once

#include "../globals.h"
#include "Renderer.h"
#include <cstring>

class Texture
{
public:
	unsigned int texture_slot = 0;
private:
	unsigned int Texture_ID = 0;
	std::string filepath;
	SDL_Surface* surface = nullptr;
	int width = 0,
		height = 0,
		BPP = 0;
public:
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0);
	void Unbind() const;

	inline int GetWidth() const { return width; }
	inline int GetHeight() const { return height; }
private:
	void FlipSurface(SDL_Surface* surface);
};

