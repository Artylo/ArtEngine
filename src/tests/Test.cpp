#include "Test.h"
#include "imgui/imgui.h"

namespace test
{
	Test::Test()
	{
	}

	Test::~Test()
	{
	}

	TestMenu::TestMenu(Test*& current_test_ptr) : current_test(current_test_ptr)
	{

	}

	TestMenu::~TestMenu()
	{

	}

	void TestMenu::OnGUIRender()
	{
		for (auto& test : tests)
		{
			if (ImGui::Button(test.first.c_str()))
			{
				current_test = test.second();
			}
		}
	}
}

