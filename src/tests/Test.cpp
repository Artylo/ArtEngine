#include "Test.h"
#include "imgui/imgui.h"

namespace test
{
	Test::Test(GameManager* GM)
	{
	}

	Test::~Test()
	{
	}

	TestMenu::TestMenu(GameManager* GM, Test*& current_test_ptr) : current_test(current_test_ptr), Test(GM)  // Call the superclass constructor in the subclass' initialization list.
	{
		TestMenuGM = GM;
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
				current_test = test.second(TestMenuGM);
				//current_test->testGM = testGM; //@CLEANUP: Sets the current test's GM to the global one. Kind of a gamble whether or not this works.
			}
			//ImGui::SameLine();
		}
	}
}

