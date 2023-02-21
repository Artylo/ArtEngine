#include "Wall.h"

Wall::Wall()
{
	sprite = std::shared_ptr<SDL_Surface>(IMG_Load("img/tile.bmp"), SDL_FreeSurface);
	assert(sprite.get() != NULL);

	box.w = sprite.get()->w;
	box.h = sprite.get()->h;

}

void Wall::init(GameManager GM)
{
	gameRenderer = GM.renderer;
	gameWindow = GM.window;
	player = GM.player;
	input = GM.input;
	generate_texture();
}

Wall::~Wall()
{

}

void Wall::update()
{
	if(box.x != pos.x) box.x = pos.x;
	if(box.y != pos.y) box.y = pos.y;

	verticalCollisionBox   = box;
	horizontalCollisionBox = box;

	if (player->vspeed)
	{
		verticalCollisionBox.y += -player->vspeed;
		draw_set_color(gameRenderer,c_blue);
		SDL_RenderDrawRect(gameRenderer, &verticalCollisionBox);
	}
	if (player->hspeed)
	{
		horizontalCollisionBox.x += -player->hspeed;
		draw_set_color(gameRenderer, c_red);
		SDL_RenderDrawRect(gameRenderer, &horizontalCollisionBox);
	}
	draw_reset_color(gameRenderer);


	if (checkCollision(verticalCollisionBox, player->verticalMovementBox))
	{
		player->verticallyColliding = true;
		player->wallColliding = this;
		//player->boxCollidingList.push_back(&box); // Inserts every frame, which is bad.
		player->boxCollidingList.insert(&box);
	}
	if (checkCollision(horizontalCollisionBox, player->horizontalMovementBox))
	{
		player->horizontallyColliding = true;
		player->wallColliding = this;
		//player->boxCollidingList.push_back(&box); // Inserts every frame, which is bad.
		player->boxCollidingList.insert(&box);
	}
}

void Wall::draw_self()
{
	//@TEMP:

	if (texture.get() != NULL)
	{
		//SDL_SetTextureAlphaMod(texture.get(), 10); //@DEBUG: This doesn't work for some reason.
		SDL_RenderCopy(gameRenderer, texture.get(), NULL, &box);
	}
}

void Wall::generate_texture()
{
	if (texture.get() == NULL)
	{
		texture = std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(gameRenderer,sprite.get()),SDL_DestroyTexture);
		assert(texture.get() != NULL);
	}
}