#include "TestDrawPrimitives.h"

namespace test
{
	TestDrawPrimitives::TestDrawPrimitives(GameManager* GM) : Test(GM), test_line({ 0,0 }, { 32,32 })
	{
		lines.push_back(test_line);
	}
	TestDrawPrimitives::~TestDrawPrimitives()
	{
	}
	void TestDrawPrimitives::OnUpdate()
	{
		lines.back().setPositions({ positions[0],positions[1] }, { positions[2],positions[3] });
	
		positions[2] = IM->mouse_x / 2;
		positions[3] = IM->mouse_y / 2;
		
		if (IM->event_ptr->type == SDL_MOUSEBUTTONDOWN)
		{
			if (IM->event_ptr->button.button == SDL_BUTTON_LEFT)
			{
				positions[0] = IM->mouse_x / 2;
				positions[1] = IM->mouse_y / 2;
			}
		}
		if (IM->event_ptr->type == SDL_MOUSEBUTTONUP)
		{
			if (IM->event_ptr->button.button == SDL_BUTTON_RIGHT)
			{
				lines.back().getPositions(NULL, NULL, &positions[0], &positions[1]);
				lines.push_back(test_line);
				std::cout << "New line!" << std::endl;

				line_colour = { random(1.0f), random(1.0f), random(1.0f), 1 };
				lines.back().setColour(line_colour);
			}
		}

		lines.back().Update();
	}
	void TestDrawPrimitives::OnRender()
	{
		//Simple Line
		for (auto line : lines)
		{
			line.Update();
			line.Draw();
		}
	}
	void TestDrawPrimitives::OnGUIRender()
	{
		//Simple Line
		//ImGui::DragInt("Thickness", &line_thickness, 0.1f, 0, 0);
		//ImGui::DragFloat("Thickness", &line_thickness);

		ImGui::Text("Lines: %i", lines.size());

		for (auto line : lines)
		{
			float line_pos[4] = {0,0,0,0};
			line.getPositions(&line_pos[0], &line_pos[1], &line_pos[2], &line_pos[3]);
			ImGui::DragFloat4("Position", line_pos);
		}

		
		//ImGui::ColorEdit3("Line Colour", line_colour);
	}
}