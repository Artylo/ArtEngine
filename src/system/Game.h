#pragma once
#include "../system/globals.h"
#include "../system/GameManager.h"
#include "../system/debug.h"
#include "../system/Input.h"

#include "../entities/Skeleton.h"
#include "../interactables/Tree.h"
#include "../system/graphics/OpenGLTest.h"

#include "../tests/TestClearColour.h" //@TEMP @CLEANUP: Remove later.

class Game
{
	public:
		//Init SDL Components
		SDL_Window* window;
		SDL_Renderer* renderer;

		//Initiate Game Manager
		GameManager GM;

		//Init gamestate variables
		SDL_Event event;
		bool gameActive = true;

		//Init GUI variables
		ImGuiIO* io_ptr = nullptr; //@DEBUG: This feels very hacky.
		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
		float f = 0.0f;
		int counter = 0;
		ImVec2 position;

		//Tests
		test::Test* current_test = nullptr;
		test::TestMenu* test_menu = new test::TestMenu(current_test);

		//Framerate counter
		//CLEANUP: Probably should move into DEBUG or own class
		static const Uint32 framenum = 20;
		Uint32 frametimes[20];
		Uint32 framestart;
		Uint32 frametimelast;
		Uint32 framecount;
		Uint32 framespersecond;
		Uint32 frametimesindex;
		Uint32 getticks;
		Uint32 count;

		//Viewport or Camera
		SDL_Rect worldBox = { 0,0,world_width,world_height };
		SDL_Rect camera = { 0,0,w_width,w_height };
		SDL_Rect port = { 0,0,w_width,w_height };

		//Setup Camera Render Texture
		SDL_Texture* gameTexture;

		//Debug Text
		DebugText debug_text;

		//INIT INPUT HANDLER
		Input input;

		//Player
		Player player;

		//Draw Tiled Background
		SDL_Texture* preTileTex;

		//@TEMP:Init Skeletons
		const int numSkel = 500;
		std::vector<Skeleton> skeletons;

		//@TEMP: Init Walls
		std::vector<Wall> testBuilding;

		//@TEMP: Init Trees
		const int forest_size = 1000;
		std::list<std::unique_ptr<Tree>> forest;

		//Member Functions
		Game();
		~Game();

		bool init_lib();
		void init();
		void update();
		void draw();
		void draw_gui();
		void page_flip();

		//OpenGL Conversion
		OpenGLTest testShader;

		void glinit();
		void glupdate();
		void gldraw();
		void gldraw_gui();
		void glpage_flip();

};

