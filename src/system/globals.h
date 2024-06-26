#pragma once
//SDL includes
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include "SDL_gpu.h"

//OpenGL includes
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/glu.h>

//ImGUI includes
#include "../vendor/imgui/imgui.h"
#include "../vendor/imgui/imgui_impl_sdl2.h"
#include "../vendor/imgui/imgui_impl_opengl3.h"

//GLM includes
#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"

//STL incldes
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <assert.h>
#include <initializer_list>
#include <algorithm>
#include <tuple>

//Window Constants
#define gameFramerate 60
#define w_width  1790
#define w_height 1008
#define world_width  1920
#define world_height 1080

//Premade Colors
#define c_red {255,0,0,255}
#define c_green { 0,255,0,255 }
#define c_blue { 0,0,255,255 }
#define c_white { 255,255,255,255 }
#define c_black { 0,0,0,255 }
#define c_gray { 100,100,100,255 }

#define c_default c_black

//Fonts
#define default_font TTF_OpenFont("arial.ttf",128)

//Structures
template <typename T>
struct DebugEntry
{
	std::string EntryName = "NULL";
	T value;
};