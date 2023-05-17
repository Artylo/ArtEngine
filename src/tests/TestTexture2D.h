#pragma once

#include "Test.h"

namespace test
{
	class TestTexture2D : public Test
	{
	public:
		
	private:
		float vertices[16] = {
			//position		//texture coords
			 0.0f, 0.0f,	0.0f, 0.0f,		//Top Left
			 32.0f, 0.0f,	1.0f, 0.0f,		//Top Right
			 0.0f, 32.0f,	0.0f, 1.0f,		//Bottom Left
			 32.0f, 32.0f,	1.0f, 1.0f		//Bottom Right

		};
		unsigned int indices[6] = {
			0, 1, 2,
			2, 1, 3
		};
	public:
		TestTexture2D();
		~TestTexture2D();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnGUIRender() override;
	private:
	};
}


