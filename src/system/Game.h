#pragma once
#include "../system/globals.h"
#include "../system/GameManager.h"
#include "../system/InputManager.h"
#include "../system/debug.h"
//#include "../system/Input.h"

#include "../BountyHunter.h"
//#include "../entities/Skeleton.h"
//#include "../interactables/Tree.h"

#include "../system/graphics/OpenGLTest.h"

#include "../tests/TestClearColour.h" //@TEMP @CLEANUP: Remove later.
#include "../tests/TestTexture2D.h" //@TEMP @CLEANUP: Remove later.
#include "../tests/TestInstancedRendering.h" //@TEMP @CLEANUP: Remove later.
#include "../tests/TestBatchedRendering.h" //@TEMP @CLEANUP: Remove later.
#include "../tests/TestDrawPrimitives.h" //@TEMP @CLEANUP: Remove later.


class Game
{
	public:
		//Init SDL Components
		SDL_Window* window;
		SDL_Renderer* renderer;

		//Initiate Game Manager
		GameManager GM;

		//Initiate Input Manager
		InputManager IM;

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

		//Viewport or Camera
		SDL_Rect worldBox = { 0,0,world_width,world_height };
		SDL_Rect camera = { 0,0,w_width,w_height };
		SDL_Rect port = { 0,0,w_width,w_height };

		float camera_scale = 1.0f;
		float camera_rotation = 0.0f;

		//Projection Matrix
		glm::mat4 projection_matrix = glm::ortho(0.0f, (float)w_width / 2, (float)w_height / 2, 0.0f, -1.0f, 1.0f); // Coordinate System

		glm::mat4 identity_matrix = glm::mat4(1.0f);
		glm::mat4 scale_matrix = glm::scale(identity_matrix, glm::vec3(camera_scale, camera_scale, camera_scale));
		//glm::mat4 rotation_matrix = glm::rotate(identity_matrix, glm::radians(camera_rotation),glm::vec3(0.0,0.0,1.0));
		glm::mat4 transformation_matrix = scale_matrix * identity_matrix;


		glm::mat4 view_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)); // Camera - moved 100 to the right
		glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)); // Translate all vertecies by an amount.
		glm::mat4 model_view_projection = projection_matrix * view_matrix * model_matrix;

		//Misc. Variables
		bool wireframeMode = false;

		//Setup Camera Render Texture
		SDL_Texture* gameTexture;

		//Debug Text
		DebugText debug_text;

		//INIT INPUT HANDLER
		//Input input;

		//Player
		//Player player;

		//Draw Tiled Background
		SDL_Texture* preTileTex;

		//@TEMP:Init Skeletons
		const int numSkel = 500;
		//std::vector<Skeleton> skeletons;

		//@TEMP: Init Walls
		//std::vector<Wall> testBuilding;

		//@TEMP: Init Trees
		const int forest_size = 1000;
		//std::list<std::unique_ptr<Tree>> forest;

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

		void glinit();
		void glupdate();
		void gldraw();
		void gldraw_gui();
		void glpage_flip();

};

