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
				current_test->GM = GM; //@CLEANUP: Sets the current test's GM to the global one. Kind of a gamble whether or not this works.
			}
			//ImGui::SameLine();
		}
	}
}

