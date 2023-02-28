#include "system/globals.h"
#include "entities/Skeleton.h"
#include "interactables/Tree.h"
#include "system/GameManager.h"
#include "system/debug.h"
#include "system/Input.h"

int main(int argc, char *argv[])
{
	/* ############## */
	/* #### INIT #### */
	/* ############## */

	//Seed RNG
	srand((unsigned int)time(NULL));

	//Initiate Game Manager
	GameManager GM;

	//Init SDL Components
	SDL_Window* window;
	SDL_Renderer* renderer;

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
	draw_set_color(renderer, c_black);
	SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
	//Init PNG loading.
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL_Image Error", IMG_GetError(), window);
	}
	SDL_SetHint(SDL_HINT_RENDER_LOGICAL_SIZE_MODE, "letterbox");
	SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_SCALING, "0");
	SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
	SDL_RenderSetLogicalSize(renderer,w_width,w_height);
	GM.window = window;
	GM.renderer = renderer;




	//Init gamestate variables
	bool gameActive = true;
	SDL_Event event;


	//Init framerate counter
	//CLEANUP: Probably should move into DEBUG
	const Uint32 framenum = 20;
	Uint32 framestart;
	Uint32 frametimes[framenum];
	Uint32 frametimelast = SDL_GetTicks();
	Uint32 framecount = 0;
	Uint32 framespersecond = 0;
	memset(frametimes, 0, sizeof(frametimes)); // Essentially shorthand for looping though frametimes and setting to 0. Very C-like.
	Uint32 frametimesindex = framecount % framenum;
	Uint32 getticks = SDL_GetTicks();
	Uint32 count;


	//Viewport or Camera
	SDL_Rect worldBox = { 0,0,world_width,world_height };
	SDL_Rect camera = { 0,0,w_width,w_height };
	SDL_Rect port = { 0,0,w_width,w_height };
	GM.camera = &camera;

	//Setup Camera Render Texture
	SDL_Texture* gameTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, world_width, world_height);


	//Debug Text
	DebugText debug_text(renderer, &camera);


	//INIT INPUT HANDLER
	Input input(&GM); // Init input handler.
	GM.input = &input;


	//INIT PLAYER
	Player player;
	player.init(renderer, window, &camera);
	player.get_input(&input);
	GM.player = &player;


	//Draw Tiled Background //@TODO: Make into its own class.
	SDL_Surface* tile = SDL_LoadBMP("img/tile2.bmp");
	int winW, winH;
	SDL_GetWindowSize(window, &winW, &winH); // Get size of resized windows
	SDL_Texture* tilesrcTex = SDL_CreateTextureFromSurface(renderer, tile);
	SDL_Texture* preTileTex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, world_width, world_height);
	SDL_Rect tileRect;

	SDL_SetRenderTarget(renderer, preTileTex);
	for (int i = 0; i < (world_width / 32) + 1; i++)
	{
		for (int j = 0; j < (world_height / 32) + 1; j++)
		{
			tileRect = SDL_Rect{ i * 32 , j * 32 , 32 , 32 };
			SDL_RenderCopy(renderer, tilesrcTex, NULL, &tileRect);
		}
	}
	SDL_SetRenderTarget(renderer, NULL);


	//@TEMP: GENERATE SKELETONS
	const int numSkel = 500;
	Skeleton originalskel;
	originalskel.init(renderer, window, &camera);
	std::vector<Skeleton> skeletons;

	for (int i = 0; i < numSkel; i++)
	{
		skeletons.push_back(originalskel);
		skeletons[i].pos.x = rand() % world_width;
		skeletons[i].pos.y = rand() % world_height;
		//skeletons[i].init(renderer, window);
	}

	//@TEMP: Init Walls
	std::vector<Wall> testBuilding;

	//Init Trees
	const int forest_size = 1000;
	std::list<std::unique_ptr<Tree>> forest;

	for (int i = 0; i < forest_size; ++i)
	{
		forest.push_back(std::make_unique<Tree>()); // Creates new Tree.
		std::list<std::unique_ptr<Tree>>::iterator it = forest.begin(); 
		std::advance(it, i);
		it->get()->init(&GM, &(*it)); // Initialises  Tree and hands it a pointer to itself.
		it->get()->pos = { rand() % world_width,rand() % world_height }; // Sets position of tree.
	}

	//y-depth sort //@CLEANUP: Move to update in class or think of some other wrapper
	forest.sort([](const std::unique_ptr<Tree>& a, const std::unique_ptr<Tree>& b) {return a.get()->pos.y < b.get()->pos.y; });

	









	/* ################### */
	/* #### GAME LOOP #### */
	/* ################### */
	while (gameActive)
	{

		/* ####################### */
		/* #### LOGIC UPDATES #### */
		/* ####################### */

		//Calculate Average Framerate
		framestart = SDL_GetTicks();
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
	
		//Update Trees
		for (int i = 0; i < forest.size(); i++)
		{
			std::list<std::unique_ptr<Tree>>::iterator it = forest.begin();
			std::advance(it, i);
			if (*it == nullptr)
			{
				//Clean up Forest
				forest.erase(it);
			}
			else
			{

				//Update Tree
				it->get()->update();
			}
			
		}

		//Pump Event Queue
		SDL_PumpEvents();












		// ####################
		// #### Draw Calls ####
		// ####################

		//Clear Screen
		SDL_RenderClear(renderer);

		//Set Render Target to Camera Texture
		//SDL_RenderSetClipRect(renderer, &camera);
		SDL_SetRenderTarget(renderer, gameTexture);
		
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
		
		//if (KEYS[SDLK_b]) //@CLEANUP old implementation with bad key stuff
		if (input.keystate[SDL_SCANCODE_B])
		{
			bool isUnique = true;
			int placeX = ((input.mouse_x + camera.x) / 32) * 32;
			int placeY = ((input.mouse_y + camera.y) / 32) * 32;

			if (testBuilding.size() > 0)
			{
				for (int i = 0; i < testBuilding.size(); i++)
				{
					if (testBuilding[i].pos.x == placeX)
					{
						if (testBuilding[i].pos.y == placeY)
						{
							isUnique = false;
						}
					}
				}
			}
			if (isUnique)
			{
				testBuilding.emplace_back(Wall());
				Wall* temp = &testBuilding[testBuilding.size() - 1];
				temp->init(&GM);
				temp->pos.x = placeX;
				temp->pos.y = placeY;
			}
			
			//Make entries unique
			//std::sort(testBuilding.begin(), testBuilding.end());
			//testBuilding.erase(unique(testBuilding.begin(), testBuilding.end()), testBuilding.end());
		}
	
		//Draw Trees
		for (int i = 0; i < forest.size(); i++)
		{
			std::list<std::unique_ptr<Tree>>::iterator it = forest.begin();
			std::advance(it, i);
			if (*it != nullptr)
			{
				it->get()->draw_self();
			}
		}

		//Draw Walls
		for (int i = 0; i < testBuilding.size(); i++)
		{
			//skeletons[i].update();
			testBuilding[i].update();
			testBuilding[i].draw_self();
			//SDL_RenderDrawRect(renderer, &testBuilding[i].box);
		}

		//Update Camera Position
		camera.x = player.pos.x - camera.w / 2;
		camera.y = player.pos.y - camera.h / 2;
			//Restrain Camera
			if (camera.x < 0) camera.x = 0;
			if (camera.x + camera.w > world_width) camera.x = world_width - camera.w;
			if (camera.y < 0) camera.y = 0;
			if (camera.y + camera.h > world_height) camera.y = world_height - camera.y;
		draw_set_color(renderer, c_red);
		SDL_RenderDrawRect(renderer, &camera);
		draw_set_color(renderer, c_blue);
		SDL_RenderDrawRect(renderer, &worldBox);
		draw_reset_color(renderer);
		
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
					debug_text.draw_text("x: " + std::to_string(s->pos.x) + " y:" + std::to_string(s->pos.y), s->pos.x - s->w, s->box.y - 64);
					debug_text.draw_text("id:  " + std::to_string(i), s->pos.x - s->w, s->box.y - 32);
				}

				//@DEBUG
				debug_text.draw_text(std::to_string(i), s->pos.x - debug_text.w / 2, s->pos.y - debug_text.h / 2); // Show id
			}
		}

		//Reset renderer to main texture
		SDL_SetRenderTarget(renderer, NULL);
		SDL_RenderCopy(renderer, gameTexture, &camera, &port);













		/* ################### */
		/* #### GUI LAYER #### */
		/* ################### */

		//Draw Framerate
		std::string tempFPSstring =
			"FPS: " + std::to_string(framespersecond) +
			" ms: " + std::to_string(SDL_GetTicks() - frametimelast);

		//@BUG: Should probably have post-modifiable debug-text entries for X, Y, W and H. Since this is causing the bug of the text moving. I cannot call the debug-text width or position in the call itself, since it is still set to that of the previous string.
		//@CLEANUP: Take this off of debug_text and make a new GUI text object.
		debug_text.draw_gui_text(tempFPSstring, w_width - 200, w_height - 32);
		debug_text.draw_gui_text(" ", 0, 0);









		/* ################### */
		/* #### PAGE FLIP #### */
		/* ################### */
		
		//Push to Screen
		SDL_RenderPresent(renderer);

		//Delay Before Next Frame
		//SDL_Delay(1000/gameFramerate);
		if (1000 / gameFramerate > SDL_GetTicks() - framestart)
		{
			SDL_Delay(1000 / gameFramerate - (SDL_GetTicks() - framestart));
		}
	}






	/* ############## */
	/* #### QUIT #### */
	/* ############## */

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