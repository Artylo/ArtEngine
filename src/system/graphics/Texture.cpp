#include "Texture.h"
#include "Renderer.h"

Texture::Texture(const std::string& path) : filepath(path)
{
	//Generate Texture
	surface = IMG_Load(path.c_str());
	//FlipSurface(surface);
	assert(surface != nullptr);
	GLCALL(glGenTextures(1, &Texture_ID));

	Bind();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //Scaling Filtering?
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Wrapping around?
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	int mode = GL_RGB;
	if (surface->format->BytesPerPixel == 4) mode = GL_RGBA; //Does Texture have alpha?

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

	//@TODO: Other glTex* stuff here.

	if(surface != nullptr) SDL_FreeSurface(surface); // Do this at some point. Or replace with smart pointer.
}

Texture::~Texture()
{
	glDeleteTextures(1, &Texture_ID);
}

void Texture::Bind(unsigned int slot)
{
	texture_slot = slot;
	GLCALL(glActiveTexture(GL_TEXTURE0 + slot));
	GLCALL(glBindTexture(GL_TEXTURE_2D, Texture_ID));
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

//Flips a SDL_Surface vertically, so that its pixel data is in the correct format for being converted into an OpenGL texture.
void Texture::FlipSurface(SDL_Surface* surface)
{
	SDL_LockSurface(surface);
	int pitch = surface->pitch;
	char* temp = new char[pitch]; // Intermediate buffer.
	char* pixels = (char*)surface->pixels;

	for (int i = 0; i < surface->h / 2; ++i)
	{
		//Get pointers to rows that need to be swapped.
		char* row1 = pixels + (static_cast<int64_t>(i) * pitch);
		char* row2 = pixels + ((static_cast<int64_t>(surface->h) - i - 1) * pitch);

		//Swap rows.
		memcpy(temp, row1, pitch);
		memcpy(row1, row2, pitch);
		memcpy(row2, temp, pitch);
	}
	delete[] temp;

	SDL_UnlockSurface(surface);
}
