#pragma once

#include "Test.h"
//#include "glm/glm.hpp"

namespace test
{
	class TestClearColour : public Test
	{
	public:
		TestClearColour();
		~TestClearColour() override;

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnGUIRender() override;
	private:
		float clear_colour[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	};
}


