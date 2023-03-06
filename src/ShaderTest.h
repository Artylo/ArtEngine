#pragma once
#include "system/globals.h"

#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/glu.h>

class ShaderTest
{
public:
	// Constructor 
	ShaderTest(const std::string& id, const std::string& v_str, const std::string& f_str);

	// Destructor
	~ShaderTest() {}

	// Add an image
	void addImg(std::string path);

	// Free the image used in the shader
	void freeImg();

	// Add a variable to the vector 
	void addVariable(std::string idV);

	// Get the location of a variable
	Uint32 getVar(std::string idV);

	// Getter for the id
	std::string getId() { return id; }

	// Set the image to the shader
	void setImgShader();

	// Activate the shader
	void activate();


private:
	// Shader Id
	std::string id;

	// v vertice shader, f fragment shader, p shader program
	Uint32 v, f, p;

	// Array of pair (id variable, location in the shader)
	std::vector<std::pair<std::string, Uint32>> variables;

	// Shader attributes and uniform locations
	GPU_ShaderBlock block;

	// Image for the shader
	GPU_Image* img;
};

