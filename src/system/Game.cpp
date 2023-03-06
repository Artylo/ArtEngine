#include "Game.h"

Game::Game()
{
	init_lib();
}

Game::~Game()
{
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
}

bool Game::init_lib()
{
	bool init_success = true;

	TTF_Init(); // Init TTF font loading.
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		SDL_Log("Successfully loaded SDL: %s", SDL_GetError());
	}
	else
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		init_success = false;
	}

	//Init SDL Window and Renderer

	/* SDL2 Window */
	//Uint32 WindowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_OPENGL; // SDL Configuration
	Uint32 WindowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL; // OpenGL Configuration
	window = SDL_CreateWindow("ArtENGINE v0.3 - OPENGL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w_width, w_height, WindowFlags);
	if (window == NULL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL ERROR!", "You did not init your SDL window!", window);
	}
	SDL_SetHint(SDL_HINT_RENDER_LOGICAL_SIZE_MODE, "letterbox");
	SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_SCALING, "0");
	SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

	/* OpenGL Renderer */
	SDL_GLContext gl_renderer = SDL_GL_CreateContext(window); // Generate OpenGL render context.
	if (gl_renderer == NULL)
	{
		SDL_Log("Failed to create OpenGL context! SDL Error: %s\n", SDL_GetError());
		init_success = false;
	}

	/* Init GLEW */
	if (glewInit() != GLEW_OK) // glewInit - requires an existing OpenGL render context at some point before being called
	{
		SDL_Log("Could not initialise GLEW!");
		init_success = false;
	}

	SDL_Log("OPENGL LOADED: v.%s",glGetString(GL_VERSION)); // Get and print OpenGL version to test if latest drivers are loaded.

	/* OpenGL Vsync */
	if (SDL_GL_SetSwapInterval(1) < 0)
	{
		SDL_Log("Warning: Unable to set Vsync! SDL Error: %s\n", SDL_GetError());
	}

	/* Init OpenGL */
	if (!initGL())
	{
		SDL_Log("Unable to initialize OpenGL!\n");
		init_success = false;
	}

	/* SDL2 Renderer */
	//@CLEANUP: Depricated in switch to OpenGL
	Uint32 RendererFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	renderer = SDL_CreateRenderer(window, -1, RendererFlags);
	if (renderer == NULL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL ERROR!", "You did not init your SDL renderer!", window);
		init_success = false;
	}
	draw_set_color(renderer, c_black);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_RenderSetLogicalSize(renderer, w_width, w_height);

	//Init PNG loading.
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL_Image Error", IMG_GetError(), window);
		init_success = false;
	}

	return init_success;
}

void Game::init()
{
	/* ############## */
	/* #### INIT #### */
	/* ############## */

	//Seed RNG
	srand((unsigned int)time(NULL));

	GM.window = window;
	GM.renderer = renderer;

	//Init framerate counter
	frametimelast = SDL_GetTicks();
	framecount = 0;
	framespersecond = 0;
	memset(frametimes, 0, sizeof(frametimes)); // Essentially shorthand for looping though frametimes and setting to 0. Very C-like.
	frametimesindex = framecount % framenum;
	getticks = SDL_GetTicks();


	//Viewport or Camera
	GM.camera = &camera;

	//Setup Camera Render Texture
	gameTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, world_width, world_height);

	//INIT INPUT HANDLE
	input.init(&GM);
	GM.input = &input;

	//INIT PLAYER
	player.init(renderer, window, &camera);
	player.get_input(&input);
	GM.player = &player;

	//Init Debug Text
	debug_text.init(&GM);

	//Draw Tiled Background //@TODO: Make into its own class.
	SDL_Surface* tile = SDL_LoadBMP("img/tile2.bmp");
	int winW, winH;
	SDL_GetWindowSize(window, &winW, &winH); // Get size of resized windows
	SDL_Texture* tilesrcTex = SDL_CreateTextureFromSurface(renderer, tile);
	preTileTex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, world_width, world_height);
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
	Skeleton originalskel;
	originalskel.init(renderer, window, &camera);

	for (int i = 0; i < numSkel; i++)
	{
		skeletons.push_back(originalskel);
		skeletons[i].pos.x = rand() % world_width;
		skeletons[i].pos.y = rand() % world_height;
		//skeletons[i].init(renderer, window);
	}

	

	//Init Trees
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

}

void Game::update()
{
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
}

void Game::draw()
{
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
}

void Game::draw_gui()
{
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
	//debug_text.draw_gui_text(" ", 0, 0);
}

void Game::page_flip()
{
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

//Shader loading utility programs
void Game::printProgramLog(GLuint program)
{
	//@COPYPASTE: Figure out what this does
	//Make sure name is shader
	if (glIsProgram(program))
	{
		//Program log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		//Get info string length
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		//Allocate string
		char* infoLog = new char[maxLength];

		//Get info log
		glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);
		if (infoLogLength > 0)
		{
			//Print Log
			printf("%s\n", infoLog);
		}

		//Deallocate string
		delete[] infoLog;
	}
	else
	{
		printf("Name %d is not a program\n", program);
	}
}
void Game::printShaderLog(GLuint shader)
{
	//@COPYPASTE: Figure out what this does
	//Make sure name is shader
	if (glIsShader(shader))
	{
		//Shader log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		//Get info string length
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		//Allocate string
		char* infoLog = new char[maxLength];

		//Get info log
		glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
		if (infoLogLength > 0)
		{
			//Print Log
			printf("%s\n", infoLog);
		}

		//Deallocate string
		delete[] infoLog;
	}
	else
	{
		printf("Name %d is not a shader\n", shader);
	}
}

bool Game::initGL()
{
	gProgramID = glCreateProgram();

	//Create Vertex Shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//Load Vertex Shader
	const GLchar* vertexShaderSource[] =
	{
		"#version 140\nin vec2 LVertexPos2D; void main() { gl_Position = vec4( LVertexPos2D.x, LVertexPos2D.y, 0, 1 ); }"
	};

	//Set Vertex Source
	glShaderSource(vertexShader, 1, vertexShaderSource, NULL);

	//Compile Vertex Source
	glCompileShader(vertexShader);

	//Check Vertex Shader for Errors
	GLint vShaderCompiled = GL_FALSE;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
	if (vShaderCompiled != GL_TRUE)
	{
		SDL_Log("Unable to compile vertex shader %d!\n", vertexShader);
	}
	else
	{
		//Attach Vertex Shader to Program
		glAttachShader(gProgramID, vertexShader);

		//Create Fragment Shader
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		//Load Fragment Shader
		const GLchar* fragmentShaderSource[] =
		{
			"#version 140\nout vec4 LFragment; void main() { LFragment = vec4( 1.0, 1.0, 1.0, 1.0 ); }"
		};

		//Set Fragment Shader
		glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);

		//Compile Fragment Shader
		glCompileShader(fragmentShader);

		//Check fragment shader for errors
		GLint fShaderCompiled = GL_FALSE;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
		if (fShaderCompiled != GL_TRUE)
		{
			SDL_Log("Unable to compile fragment shader %d!\n", fragmentShader);
		}
		else
		{
			//Attach fragment shader to program
			glAttachShader(gProgramID, fragmentShader);

			//Link Program
			glLinkProgram(gProgramID);

			//Check for errors
			GLint programSuccess = GL_TRUE;
			glGetProgramiv(gProgramID, GL_LINK_STATUS, &programSuccess);
			if (programSuccess != GL_TRUE)
			{
				SDL_Log("Error linking program %d!\n", gProgramID);
			}
			else
			{
				//Get vertex attribute location
				gVertexPos2DLocation = glGetAttribLocation(gProgramID, "LVertexPos2D");
				if (gVertexPos2DLocation == -1)
				{
					SDL_Log("LVertexPos2D is not a valid glsl program variable!\n");
				}
				else
				{
					//Init clear colour
					glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

					//VBO data
					GLfloat vertexData[] =
					{
						-0.5f, -0.5f,
						 0.5f, -0.5f,
						 0.5f,  0.5f,
						-0.5f,  0.5f
					};

					//IBO data
					GLuint indexData[] = { 0,1,2,3 };

					//Create VBO
					glGenBuffers(1, &gVBO);
					glBindBuffer(GL_ARRAY_BUFFER, gVBO);
					glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);

					//Create IBO
					glGenBuffers(1, &gIBO);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW);
				}
			}
		}
	}
	return 1;
}

void Game::glinit()
{
	
}

void Game::glupdate()
{

}

void Game::gldraw()
{
	//Clear Colour Buffer
	glClear(GL_COLOR_BUFFER_BIT);

	//Render Quad
	if (true)
	{
		//Bind Program
		glUseProgram(gProgramID);

		//Enable vertex position
		glEnableVertexAttribArray(gVertexPos2DLocation);

		//Set Vertex Data
		glBindBuffer(GL_ARRAY_BUFFER, gVBO);
		glVertexAttribPointer(gVertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);

		//Set Index Data and Render
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
		glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);

		//Disable Vertex Position
		glDisableVertexAttribArray(gVertexPos2DLocation);

		//Unbind Program
		glUseProgram(NULL);
	}
}

void Game::gldraw_gui()
{

}

void Game::glpage_flip()
{
	SDL_GL_SwapWindow(window);
}