#include "TestClearColour.h"
#include "../system/graphics/Renderer.h"
#include "imgui/imgui.h"

namespace test
{
	test::TestClearColour::TestClearColour()
	{

	}

	test::TestClearColour::~TestClearColour()
	{

	}

	void test::TestClearColour::OnUpdate(float deltaTime)
	{

	}

	void test::TestClearColour::OnRender()
	{
		GLCALL(glClearColor(clear_colour[0], clear_colour[1], clear_colour[2], clear_colour[3]));
		GLCALL(glClear(GL_COLOR_BUFFER_BIT));
	}

	void test::TestClearColour::OnGUIRender()
	{
		ImGui::ColorPicker4("Clear Colour", clear_colour);
	}
}
