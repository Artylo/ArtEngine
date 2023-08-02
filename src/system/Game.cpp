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

	/* ImGui Quits*/
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	SDL_Log("ImGui Shutdown");

	/* SDL Quits */
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
	std::cout << "TTF_init successful!" << std::endl;

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		SDL_Log("Successfully loaded SDL: %s", SDL_GetError());
	}
	else
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		init_success = false;
	}
	std::cout << "SDL_INIT_EVERYTHING successful!" << std::endl;

	//Init SDL Window and Renderer

	/* SDL2 Window */

	//Uint32 WindowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_OPENGL; // SDL Configuration

	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	Uint32 WindowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL; // OpenGL Configuration
	std::string baseTitle = "ArtENGINE v0.3 - OPENGL"; // Window Title
	window = SDL_CreateWindow(baseTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w_width, w_height, WindowFlags);
	if (window == NULL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL ERROR!", "You did not init your SDL window!", window);
	}
	SDL_SetHint(SDL_HINT_RENDER_LOGICAL_SIZE_MODE, "letterbox");
	SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_SCALING, "0");
	SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
	std::cout << "SDL_CreateWindow successful!" << std::endl;

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

	
	std::string OpenGLVersion = (const char*)glGetString(GL_VERSION);
	std::string GLSLVersion = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
	std::string title = baseTitle + " v." + OpenGLVersion + " - GLSL v." + GLSLVersion;
	SDL_Log("OPENGL LOADED: v.%s", glGetString(GL_VERSION)); // Get and print OpenGL version to test if latest drivers are loaded.
	SDL_SetWindowTitle(window, title.c_str());

	/* OpenGL Vsync */
	if (SDL_GL_SetSwapInterval(1) < 0)
	{
		SDL_Log("Warning: Unable to set Vsync! SDL Error: %s\n", SDL_GetError());
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

	//Init ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io_ptr = &io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad  Controls

	ImGui::StyleColorsDark(); //ImGui Style
	
	ImGui_ImplSDL2_InitForOpenGL(window, gl_renderer); // Setup Platform/Renderer backends
	//ImGui_ImplOpenGL3_Init(GLSLVersion.c_str());
	ImGui_ImplOpenGL3_Init("#version 330"); //@DEBUG: I think I should find a way to parse this from the GLSLVersion string because this will probably break.

	return init_success;
}

void Game::glinit()
{
	//Game Manages Setup
	GM.window = window;
	GM.projection_matrix = &projection_matrix;

	//testShader.init();

	//Init Test Menu
	current_test = test_menu;
	test_menu->AddTest<test::BountyHunter>("Main Game");
	test_menu->AddTest<test::TestClearColour>("Clear Colour");
	test_menu->AddTest<test::TestTexture2D>("Texture Test");
	test_menu->AddTest<test::TestInstancedRendering>("Instanced Rendering Test");
	test_menu->AddTest<test::TestBatchedRendering>("Batched Rendering Test");

	//testShader.GM = &GM;
}

void Game::glupdate()
{
	

	while (SDL_PollEvent(&event)) //@TEMP: Close window
	{
		ImGui_ImplSDL2_ProcessEvent(&event); // This handles mouse input for ImGui.
		switch (event.type) 
		{
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				gameActive = false;
				break;
			}
			break;
		case SDL_WINDOWEVENT:
			switch (event.window.event)
			{
			case SDL_WINDOWEVENT_CLOSE:
				gameActive = false;
				break;
			case SDL_WINDOWEVENT_RESIZED:
				//glViewport(0, 0, (int)io_ptr->DisplaySize.x, (int)io_ptr->DisplaySize.y);

				break;
			}
			break;
		}
	}
	
	//testShader.update();
	if (current_test != nullptr) current_test->OnUpdate((float)SDL_GetTicks());
}

void Game::gldraw()
{
	GLCALL(glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w));
	GLCALL(glClear(GL_COLOR_BUFFER_BIT));

	if (wireframeMode)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	//Setup Matricies
	int winW, winH;
	SDL_GetWindowSize(GM.window, &winW, &winH);
	projection_matrix = glm::ortho(0.0f, (float)winW / 2, (float)winH / 2, 0.0f, -1.0f, 1.0f);

	//@TEST
	//@CLEANUP: Test ImGui Frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	//testShader.renderer.Clear();
	if (current_test != nullptr) current_test->OnRender();
	//testShader.draw();
}

void Game::gldraw_gui()
{	
	//@TEST

	//if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window); //@CLEANUP: Built in documentation and demo window. Remove before release.

	ImGui::Begin("ArtEngine Developer Menu v0.1"); // Create a window - also bind to it.

	ImGui::Text("Remember thee the acceptance of being unoriginal\nand bringing to bear external libraries,\nas a sin of the highest order.	 - Arty, 2023"); // Display some text (you can use a format strings too)

	ImGui::ColorEdit3("Clear Colour", (float*)&clear_color); // Edit Clear colour - @DEBUG cannot work in this scope

	//testShader.model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0));
	//ImGui::DragFloat2("ModelProj Position", (float*)&position);

	//ImGui::DragFloat("Camera Zoom", (float*)&testShader.camera_scale, 0.01f);
	//testShader.scale_matrix = glm::scale(testShader.identity_matrix, glm::vec3(testShader.camera_scale, testShader.camera_scale, testShader.camera_scale));
	//ImGui::DragFloat("Camera Rotation", (float*)&testShader.camera_rotation, 0.1f);

	//if (ImGui::Button("Button")) counter++;
	ImGui::SameLine();
	ImGui::Text("counter = %d", counter);

	if (ImGui::Button("Wireframe Mode")) wireframeMode = !wireframeMode;
	ImGui::SameLine();
	if (wireframeMode) ImGui::Text("ON");
	else ImGui::Text("OFF");


	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io_ptr->Framerate, io_ptr->Framerate);
	ImGui::End();

	if (current_test)
	{
		ImGui::Begin("Tests");
			if (current_test != test_menu && ImGui::Button("<--"))
			{
				delete current_test;
				current_test = test_menu;
			}
			current_test->OnGUIRender();
		ImGui::End();
	}

	//@CLEANUP: Test ImGui Frame
	ImGui::Render();
	//glViewport(0, 0, (int)io_ptr->DisplaySize.x, (int)io_ptr->DisplaySize.y);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Game::glpage_flip()
{
	//Swap Buffers - page-flip
	SDL_GL_SetSwapInterval(1);
	SDL_GL_SwapWindow(window);

	//Pump Event Queue
	SDL_PumpEvents();
}