#include "globals.h"
#include "debug.h"
#include "Input.h"

//#include "pong.h" //@TEMP

SDL_Window* window;
SDL_Renderer* renderer;

int main(int argc, char *argv[])
{
	SDL_Color colBackground = c_black;
	SDL_Color colLines = c_white;

	TTF_Init();
	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow("ArtENGINE v0.1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w_width, w_height, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL ERROR!", "You did not init your SDL window!", window);
	}
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == NULL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL ERROR!", "You did not init your SDL renderer!", window);
	}
	SDL_SetRenderDrawColor(renderer, colBackground.r, colBackground.g, colBackground.b, colBackground.a);
	SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);

	bool gameActive = true;
	SDL_Event event;
	const Uint8* keystate = SDL_GetKeyboardState(NULL);


	Input input; // Init input handler.
	Player player(renderer); // Init player.




	//Draw Tiled Background
	//@TEMP Attempt at tile system with repeating texture genereted from file on disk to RAM before generating texture with blitted surface.
	//@CLEANUP Commented out sections are in-RAM surface manipulation, probably useful in the future. Used parts are 100% hardware transformation in VRAM. 
	SDL_Surface* tile = SDL_LoadBMP("img/tile2.bmp");
	SDL_Texture* tilesrcTex = SDL_CreateTextureFromSurface(renderer,tile);
	SDL_FreeSurface(tile);
	SDL_Texture* preTileTex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, w_width, w_height);
	//SDL_Surface* tileBlit = SDL_CreateRGBSurface(NULL, w_width, w_height, 32, 0, 0, 0, 0);

	SDL_SetRenderTarget(renderer,preTileTex);

	for (int i = 0; i < w_width / 32; i++)
	{
		for (int j = 0; j < w_height / 32; j++)
		{
			SDL_RenderCopy(renderer, tilesrcTex, NULL, new SDL_Rect{ i * 32,j * 32,32,32 });
		}
	}
	
	SDL_SetRenderTarget(renderer, NULL);

	/* @CLEANUP Software tile minupulation in RAM.
	for (int i = 0; i < w_width / 32; i++)
	{
		for (int j = 0; j < w_height / 32; j++)
		{
			SDL_BlitSurface(tile, NULL, tileBlit, new SDL_Rect{ i*32,j*32,32,32 });
		}
	}
	*/

	//@TEMP @CLEANUP Deallocate textures and free surfaces.
	//SDL_Texture* tileTex = SDL_CreateTextureFromSurface(renderer,tileBlit);
	//SDL_FreeSurface(tileBlit);

	//SDL_DestroyTexture(tileTex); @CLEANUP Run in tile engine destructor to allow for dynamic texture changes at runtime.
	//SDL_DestroyTexture(tilesrcTex); 
	//SDL_DestroyTexture(preTileTex); 

	//Pong pong; //@TEMP



	SDL_Surface* msg = NULL;
	std::string msgTxt = "COCK AND BALLS";
	const char* c = msgTxt.c_str();

	while (gameActive)
	{
		//Clear Screen
		SDL_RenderClear(renderer);
		SDL_PumpEvents();

		/****Draw Calls****/
		// @TEMP Render blitted tile surface.
		SDL_RenderCopy(renderer, preTileTex, NULL, NULL);

		//@TEMP
		//pong.play(renderer,keystate);

		//Draw Grid
   		if (keystate[SDL_SCANCODE_SPACE])
		{
			draw_grid(renderer, c_white, 128);
		}

		//Update Player
		player.update();
		
		//Draw Player
		player.draw_player(renderer, window);

		//Draw Message
		c = msgTxt.c_str();
		msg = TTF_RenderText_Solid(default_font, c, c_white);
		SDL_Texture* msgTex = SDL_CreateTextureFromSurface(renderer, msg);
		SDL_RenderCopy(renderer, msgTex, NULL, new SDL_Rect{0,0,640,128});


		//Push to Screen
		SDL_RenderPresent(renderer);

		//Handle Inputs
		input.update(&gameActive, &event, &msgTxt, &player);

		SDL_Delay(1000/gameFramerate);
	}

	//Quit
	SDL_DestroyRenderer(renderer);
	//SDL_DestroyWindow(window);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();

	return 0;
}