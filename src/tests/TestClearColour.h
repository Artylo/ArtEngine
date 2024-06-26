#pragma once

#include "Test.h"
//#include "glm/glm.hpp"

namespace test
{
	class TestClearColour : public Test
	{
	public:
		TestClearColour(GameManager* GM);
		~TestClearColour() override;

		void OnUpdate() override;
		void OnRender() override;
		void OnGUIRender() override;
	private:
		float clear_colour[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	};
}


