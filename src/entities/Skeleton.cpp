#include "../entities/Skeleton.h"
#include "../system/globals.h"
#include "../system/debug.h"

Skeleton::Skeleton()
{
	//sprite = IMG_Load("img/skeleton.png");
	//std::shared_ptr<SDL_Surface> shared_sprite(IMG_Load("img/skeleton.png"), SDL_FreeSurface);
	sprite = std::shared_ptr<SDL_Surface>(IMG_Load("img/skeleton.png"), SDL_FreeSurface);

	surf_hair = std::shared_ptr<SDL_Surface>(IMG_Load("img/hair_skeleton.png"), SDL_FreeSurface);
	surf_shirt = std::shared_ptr<SDL_Surface>(IMG_Load("img/shirt_skeleton.png"), SDL_FreeSurface);
	surf_pants = std::shared_ptr<SDL_Surface>(IMG_Load("img/pants_skeleton.png"), SDL_FreeSurface);
}

void Skeleton::update()
{
	Entity::update();
}

void Skeleton::draw_self()
{
	Entity::draw_self(); // Draw The Skeleton
	generate_clothes_textures(); // Generate Textures for Clothes

	if (SDL_PointInRect(&pos, gameCamera))
	{
		//Draw The Clothes
		SDL_RenderCopyEx(gameRenderer, tex_pants.get(), NULL, &box, 0, &origin, SDL_FLIP_NONE);
		SDL_RenderCopyEx(gameRenderer, tex_shirt.get(), NULL, &box, 0, &origin, SDL_FLIP_NONE);
		SDL_RenderCopyEx(gameRenderer, tex_hair.get(), NULL, &box, 0, &origin, SDL_FLIP_NONE);

	}
	//SDL_RenderCopyEx(gameRenderer, texture.get(), NULL, &box, 0, &origin, SDL_FLIP_NONE);
}

void Skeleton::generate_clothes_textures()
{
	SDL_Color black = c_black;
	SDL_Color white = c_white;
	hairCol = choose<SDL_Color>({ c_black });
	//hairCol = { random(255),random(255),random(255),255 };
	shirtCol = { random(255),random(255),random(255),255 };
	pantsCol = { random(255),random(255),random(255),255 };

	SDL_SetSurfaceColorMod(surf_hair.get(), hairCol.r, hairCol.g, hairCol.b);
	SDL_SetSurfaceColorMod(surf_shirt.get(), shirtCol.r, shirtCol.g, shirtCol.b);
	SDL_SetSurfaceColorMod(surf_pants.get(), pantsCol.r, pantsCol.g, pantsCol.b);

	if (tex_hair.get() == NULL)
	{
		tex_hair = std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(gameRenderer, surf_hair.get()), SDL_DestroyTexture);
		if (tex_hair.get() == NULL) SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
	}
	
	if (tex_shirt.get() == NULL)
	{
		tex_shirt = std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(gameRenderer, surf_shirt.get()), SDL_DestroyTexture);
		if (tex_shirt.get() == NULL) SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
	}

	if (tex_pants.get() == NULL)
	{
		tex_pants = std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(gameRenderer, surf_pants.get()), SDL_DestroyTexture);
		if (tex_pants.get() == NULL) SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError());
	}
}