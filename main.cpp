#include "globals.h"

SDL_Window* window;
SDL_Renderer* renderer;

#include "debug.h"
#include "Input.h"
#include "Skeleton.h"


int main(int argc, char *argv[])
{
	//Seed RNG
	srand((unsigned int)time(NULL));

	//Predefined Colours
	SDL_Color colBackground = c_black;
	SDL_Color colLines = c_white;

	//Init SDL Components
	TTF_Init();
	SDL_Init(SDL_INIT_EVERYTHING);

	//Init SDL Window and Renderer
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

	//Init gamestate variables
	bool gameActive = true;
	SDL_Event event;
	const Uint8* keystate = SDL_GetKeyboardState(NULL);

	//Init important objects
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

	//Text Initis
	SDL_Surface* msg = NULL;
	std::string msgTxt = "COCK AND BALLS";
	const char* c = msgTxt.c_str();

	const int numSkel = 50;
	//Skeleton skeletons[numSkel];
	std::vector<Skeleton> skeletons;

	//GENERATE SKELETONS
	for (int i = 0; i < numSkel-1; i++)
	{
		skeletons.push_back(Skeleton());
		skeletons[i].x = rand() % w_width;
		skeletons[i].y = rand() % w_height;
	}

	while (gameActive)
	{
		//Handle Inputs
		input.update(&gameActive, &event, &msgTxt, &player);

		//Clear Screen
		SDL_RenderClear(renderer);
		SDL_PumpEvents();

		/****Draw Calls****/

		//Draw Background
		SDL_RenderCopy(renderer, preTileTex, NULL, NULL);

		//Draw Grid
   		if (keystate[SDL_SCANCODE_SPACE])
		{
			draw_grid(renderer, c_white, 128);
		}

		//Update Player
		player.update();
		//Draw Player
		player.draw_self(renderer, window);

		//Draw Message
		c = msgTxt.c_str();
		msg = TTF_RenderText_Solid(default_font, c, c_white);
		SDL_Texture* msgTex = SDL_CreateTextureFromSurface(renderer, msg);
		SDL_RenderCopy(renderer, msgTex, NULL, new SDL_Rect{0,0,640,128});

		//Draw Skeletons
		for (int i = 0; i < numSkel-1; i++)
		{
			skeletons[i].update();
			skeletons[i].draw_self(renderer,window);
			SDL_RenderCopy(renderer,skeletons[i].texture,NULL,skeletons[i].box);

			if (keystate[SDL_SCANCODE_SPACE]) // @DEBUG: Squares around skeleton positions.
			{
				SDL_Rect* temp = skeletons[i].box;
				draw_set_color(renderer, c_red);
				SDL_RenderDrawRect(renderer, temp);
				draw_set_color(renderer, c_default);

				if (input.mouseIsHovering(skeletons[i]))
				{
					DebugText skeletonID(renderer, "0x" + std::to_string((unsigned)std::addressof(skeletons[i])), skeletons[i].x - skeletons[i].w, skeletons[i].box->y - 32, skeletons[i].w * 4, skeletons[i].h);
					//DebugText skeletonID(renderer, "x: " + std::to_string(skeletons[i].x) + " y: " + std::to_string(skeletons[i].y), skeletons[i].x - skeletons[i].w, skeletons[i].box->y - 32, skeletons[i].w * 4, skeletons[i].h);
				}
			}
		}		

		//Push to Screen
		SDL_RenderPresent(renderer);

		//Delay Before Next Frame
		SDL_Delay(1000/gameFramerate);
	}

	//Quit
	SDL_DestroyRenderer(renderer);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
	//SDL_DestroyWindow(window);

	return 0;
}