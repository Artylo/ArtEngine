#include "globals.h"

SDL_Window* window;
SDL_Renderer* renderer;

#include "debug.h"
#include "Player.h"
#include "Input.h"
#include "Skeleton.h"
#include "Wall.h"


int main(int argc, char *argv[])
{
	/* ############## */
	/* #### INIT #### */
	/* ############## */
	//Seed RNG
	srand((unsigned int)time(NULL));

	//Predefined Colours
	SDL_Color colBackground = c_black;
	SDL_Color colLines = c_white;

	//Init SDL Components
	TTF_Init(); // Init TTF font loading.
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		SDL_Log("Successfully loaded SDL: %s", SDL_GetError());
	}
	else
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return 1;
	}

	//Init SDL Window and Renderer
	window = SDL_CreateWindow("ArtENGINE v0.1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w_width, w_height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);
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
	//Init PNG loading.
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL_Image Error", IMG_GetError(), window);
	}
	SDL_SetHint(SDL_HINT_RENDER_LOGICAL_SIZE_MODE, "letterbox");
	SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_SCALING, "0");
	SDL_RenderSetLogicalSize(renderer,w_width,w_height);

	//Init gamestate variables
	bool gameActive = true;
	SDL_Event event;

	//Init important objects
	Input input(renderer, window); // Init input handler.

	//INIT PLAYER
	Player player;
	player.init(renderer, window);
	player.get_input(&input);

	//Draw Tiled Background //@TODO: Make into its own class.
	SDL_Surface* tile = SDL_LoadBMP("img/tile2.bmp");
	int winW, winH;
	SDL_GetWindowSize(window, &winW, &winH); // Get size of resized windows.
	//@CLEANUP background update code for fullscreen scaling

	SDL_Texture* tilesrcTex = SDL_CreateTextureFromSurface(renderer, tile);
	//SDL_FreeSurface(tile);
	SDL_Texture* preTileTex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, w_width, w_height);
	//SDL_Surface* tileBlit = SDL_CreateRGBSurface(NULL, w_width, w_height, 32, 0, 0, 0, 0);
	SDL_Rect tileRect;
	SDL_SetRenderTarget(renderer, preTileTex);
	for (int i = 0; i < (winW / 32) + 1; i++)
	{
		for (int j = 0; j < (winH / 32) + 1; j++)
		{
			tileRect = SDL_Rect{ i * 32 , j * 32 , 32 , 32 };
			SDL_RenderCopy(renderer, tilesrcTex, NULL, &tileRect);
		}
	}
	SDL_SetRenderTarget(renderer, NULL);
	//@CLEANUP where background draw update was

	//@TEMP: GENERATE SKELETONS
	const int numSkel = 50;
	Skeleton originalskel;
	originalskel.init(renderer,window);
	std::vector<Skeleton> skeletons;

	for (int i = 0; i < numSkel; i++)
	{
		skeletons.push_back(originalskel);
		skeletons[i].x = rand() % w_width;
		skeletons[i].y = rand() % w_height;
		//skeletons[i].init(renderer, window);
	}

	//@TEMP: Generate Walls
	const int wallSize = 5;
	//Wall testPreBuilding[wallSize][wallSize];
	std::vector<Wall> testBuilding;
	for (int i = 0; i < wallSize; i++)
	{
		for (int j = 0; j < wallSize; j++)
		{
			if (i == 0 || i == wallSize - 1)
			{
				Wall w;
				testBuilding.push_back(w);
				testBuilding[testBuilding.size() - 1].init(renderer, window, &player);
				testBuilding[testBuilding.size() - 1].pos.x = j * 32;
				testBuilding[testBuilding.size() - 1].pos.y = i * 32;
			}
			else if (j == 0 || j == wallSize - 1)
			{
				Wall w;
				testBuilding.push_back(w);
				testBuilding[testBuilding.size() - 1].init(renderer, window, &player);
				testBuilding[testBuilding.size() - 1].pos.x = j * 32;
				testBuilding[testBuilding.size() - 1].pos.y = i * 32;
			}
		}
	}

	//Debug Text
	
	//std::vector<DebugText> skeletonDebugs;
	//skeletonDebugs.push_back();

	DebugText debug_text(renderer);

	//DebugText skeletonCords(renderer, "x: " + std::to_string(s.x) + "y:" + std::to_string(s.y), s.x - s.w, s.box.y - 64, s.w * 4, s.h);

	//Init framerate counter
	//CLEANUP: Probably should move into DEBUG
	const Uint32 framenum = 20;
	Uint32 frametimes[framenum];
	Uint32 frametimelast = SDL_GetTicks();
	Uint32 framecount = 0;
	Uint32 framespersecond = 0;
	memset(frametimes, 0, sizeof(frametimes)); // Essentially shorthand for looping though frametimes and setting to 0. Very C-like.
	Uint32 frametimesindex = framecount % framenum;
	Uint32 getticks = SDL_GetTicks();
	Uint32 count;


	/* ################### */
	/* #### GAME LOOP #### */
	/* ################### */
	while (gameActive)
	{
		/* ####################### */
		/* #### LOGIC UPDATES #### */
		/* ####################### */

		//Calculate Average Framerate
		frametimesindex = framecount % framenum;
		getticks = SDL_GetTicks();
		count = 0;

		frametimes[frametimesindex] = getticks - frametimelast;
		frametimelast = getticks;
		framecount++;

		if (framecount < framenum) count = framecount;
		else count = framenum;

		for (Uint32 i = 0; i < count; i++)
		{
			framespersecond += frametimes[i];
		}
		framespersecond /= count;
		if (framespersecond > 0) framespersecond = 1000 / framespersecond;
		else framespersecond = 0;

		//Handle Inputs
		input.update(&gameActive, &event, &player);

		//Clear Screen
		SDL_RenderClear(renderer);
		SDL_PumpEvents();

		// ####################
		// #### Draw Calls ####
		// ####################
		
		//Draw Background
		SDL_RenderCopy(renderer, preTileTex, NULL, NULL);

		//Draw Grid
   		if (input.keystate[SDL_SCANCODE_SPACE])
		{
			draw_grid(renderer, c_white, 128);
		}
		
		//Draw Skeletons
		for (int i = 0; i < numSkel; i++)
		{
			skeletons[i].update();
			skeletons[i].draw_self();
			//SDL_RenderCopy(renderer,skeletons[i].texture.get(),NULL,&skeletons[i].box);
		}

		/*
		* This is a glorious first step in creating new things at runtime.
		* I screeched at my screen like a banshee when it made new walls 
		* appear on screen and they worked like they should have.
		*/

		if (input.keystate[SDL_SCANCODE_B])
		{
			testBuilding.emplace_back(Wall());
			Wall* temp = &testBuilding[testBuilding.size() - 1];
			temp->init(renderer, window, &player);
			temp->pos.x = input.mouse_x;
			temp->pos.y = input.mouse_y;
		}

		//Draw Walls
		for (int i = 0; i < testBuilding.size(); i++)
		{
			//skeletons[i].update();
			testBuilding[i].draw_self();
			//SDL_RenderDrawRect(renderer, &testBuilding[i].box);
		}

		//Update Player
		player.update();
		//Draw Player
		player.draw_self();

		//Draw Skeleton Debug
		for (int i = 0; i < numSkel; i++)
		{
			Skeleton* s = &skeletons[i];
			if (input.keystate[SDL_SCANCODE_SPACE]) // @DEBUG: Squares around skeleton positions.
			{
				if (input.mouseIsHovering(skeletons[i]))
				{
					//@DEBUG
					debug_text.draw_text("x: " + std::to_string(s->x) + " y:" + std::to_string(s->y), s->x - s->w, s->box.y - 64);
					debug_text.draw_text("id:  " + std::to_string(i), s->x - s->w, s->box.y - 32);
				}

				//@DEBUG
				debug_text.draw_text(std::to_string(i), s->x - debug_text.w / 2, s->y - debug_text.h / 2); // Show id
			}
		}

		//Draw Framerate
		std::string tempFPSstring = 
		"FPS: " + std::to_string(framespersecond) +
		" ms: " + std::to_string(SDL_GetTicks() - frametimelast);

		debug_text.draw_text(tempFPSstring, debug_text.w, w_height - debug_text.h);

		/* ################### */
		/* #### PAGE FLIP #### */
		/* ################### */
		
		//Push to Screen
		SDL_RenderPresent(renderer);

		//Delay Before Next Frame
		SDL_Delay(1000/gameFramerate);
	}

	//Quit
	SDL_Log("Destroying Renderer");
	SDL_DestroyRenderer(renderer);
	SDL_Log("Destroying Window");
	SDL_DestroyWindow(window);
	SDL_Log("Unloading SDL_IMG");
	std::atexit(IMG_Quit);
	SDL_Log("Unloading SDL_TTF");
	std::atexit(TTF_Quit);
	SDL_Log("Unloading SDL2");
	std::atexit(SDL_Quit);
	SDL_Log("SUCCESSFULLY SHUT DOWN");

	return 0;
}