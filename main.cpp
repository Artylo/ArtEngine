#include "globals.h"

SDL_Window* window;
SDL_Renderer* renderer;

#include "debug.h"
#include "Input.h"
#include "Skeleton.h"


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
	SDL_RenderSetLogicalSize(renderer,w_width,w_height);

	//Init gamestate variables
	bool gameActive = true;
	SDL_Event event;
	const Uint8* keystate = SDL_GetKeyboardState(NULL);

	//Init important objects
	Input input; // Init input handler.
	Player player; // Init player.
	player.init(renderer, window);

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

	//@DEBUG: Text Initis
	SDL_Surface* msg = NULL;
	std::string msgTxt = "COCK AND BALLS";
	TTF_Font* msgFont = default_font;
	const char* c = msgTxt.c_str();
	SDL_Rect msgRect = SDL_Rect{0,0,640,128};

	const int numSkel = 50;
	Skeleton originalskel;
	originalskel.init(renderer,window);
	std::vector<Skeleton> skeletons;

	//@TEMP: GENERATE SKELETONS
	for (int i = 0; i < numSkel; i++)
	{
		skeletons.push_back(originalskel);
		skeletons[i].x = rand() % w_width;
		skeletons[i].y = rand() % w_height;
		//skeletons[i].init(renderer, window);
	}

	//Debug Text
	
	//std::vector<DebugText> skeletonDebugs;
	//skeletonDebugs.push_back();

	DebugText skeletonDebug(renderer);

	//DebugText skeletonCords(renderer, "x: " + std::to_string(s.x) + "y:" + std::to_string(s.y), s.x - s.w, s.box.y - 64, s.w * 4, s.h);

	//Init framerate counter
	//CLEANUP: Probably should move into DEBUG
	Uint32 frametimes[10];
	Uint32 frametimelast = SDL_GetTicks();
	Uint32 framecount = 0;
	Uint32 framespersecond = 0;
	memset(frametimes, 0, sizeof(frametimes)); // Essentially shorthand for looping though frametimes and setting to 0. Very C-like.


	/* ################### */
	/* #### GAME LOOP #### */
	/* ################### */
	while (gameActive)
	{
		/* ####################### */
		/* #### LOGIC UPDATES #### */
		/* ####################### */

		//Handle Inputs
		input.update(&gameActive, &event, &msgTxt, &player);

		//Clear Screen
		SDL_RenderClear(renderer);
		SDL_PumpEvents();

		// ####################
		// #### Draw Calls ####
		// ####################

		
		//Draw Background
		SDL_RenderCopy(renderer, preTileTex, NULL, NULL);

		//Draw Grid
   		if (keystate[SDL_SCANCODE_SPACE])
		{
			draw_grid(renderer, c_white, 128);
		}

		//Draw Message
		c = msgTxt.c_str();
		msg = TTF_RenderText_Solid(msgFont, c, c_white);
		SDL_Texture* msgTex = SDL_CreateTextureFromSurface(renderer, msg);
		SDL_RenderCopy(renderer, msgTex, NULL, &msgRect);
		SDL_FreeSurface(msg);
		SDL_DestroyTexture(msgTex);
		

		
		//Draw Skeletons
		for (int i = 0; i < numSkel; i++)
		{
			skeletons[i].update();
			skeletons[i].draw_self();
			SDL_RenderCopy(renderer,skeletons[i].texture.get(),NULL,&skeletons[i].box);
			if (keystate[SDL_SCANCODE_SPACE]) // @DEBUG: Squares around skeleton positions.
			{
				//Render Box
				Skeleton s = skeletons[i];
				draw_set_color(renderer, c_red);
				SDL_RenderDrawRect(renderer, &skeletons[i].box);
				draw_set_color(renderer, c_default);

				//Bounding Box
				draw_set_color(renderer, c_green);
				SDL_RenderDrawLine(renderer,s.box.x,s.box.y,s.box.x+s.box.w,s.box.y);
				SDL_RenderDrawLine(renderer, s.box.x + s.box.w, s.box.y, s.box.x+s.box.w, s.box.y+s.box.h);
				draw_set_color(renderer, c_default);
			}
		}

		//Draw Skeleton Debug
		for (int i = 0; i < numSkel; i++)
		{
			if (keystate[SDL_SCANCODE_SPACE]) // @DEBUG: Squares around skeleton positions.
			{
				if (input.mouseIsHovering(skeletons[i]))
				{
					Skeleton s = skeletons[i];
					//@DEBUGTEXT
					skeletonDebug.draw_text("x: " + std::to_string(s.x) + " y:" + std::to_string(s.y), s.x - s.w, s.box.y - 64, s.w * 4, s.h);
					skeletonDebug.draw_text("id:  " + std::to_string(i), s.x - s.w, s.box.y - 32, s.w * 4, s.h);
				}
			}
		}

		//Update Player
		player.update();
		//Draw Player
		player.draw_self();

		//Draw Framerate
		//DebugText frameCounter(renderer, std::to_string(framespersecond), w_width - 74, 0, 64, 64);
		std::string tempFPSstring = "FPS: " + std::to_string(framespersecond);
		const char* tempFPSchar = tempFPSstring.c_str();
		SDL_Surface* tempFPSsurface = TTF_RenderText_Solid(msgFont, tempFPSchar, c_white);
		SDL_Texture* tempFPStexture = SDL_CreateTextureFromSurface(renderer, tempFPSsurface);
		SDL_Rect tempFPSrect = SDL_Rect{ w_width - 74, w_height - 64, 64, 64 };
		SDL_RenderCopy(renderer, tempFPStexture, NULL, &tempFPSrect);
		SDL_FreeSurface(tempFPSsurface);
		SDL_DestroyTexture(tempFPStexture);

		/* ################### */
		/* #### PAGE FLIP #### */
		/* ################### */
		
		//Push to Screen
		SDL_RenderPresent(renderer);

		//Delay Before Next Frame
		SDL_Delay(1000/gameFramerate);

		//Calculate Average Framerate
		Uint32 frametimesindex = framecount % 10;
		Uint32 getticks = SDL_GetTicks();
		Uint32 count;
		
		frametimes[frametimesindex] = getticks - frametimelast;
		frametimelast = getticks;
		framecount++;

		if (framecount < 10) count = framecount;
		else count = 10;

		for (Uint32 i = 0; i < count; i++)
		{
			framespersecond += frametimes[i];
		}
		framespersecond /= count;
		framespersecond = 1000 / framespersecond;
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