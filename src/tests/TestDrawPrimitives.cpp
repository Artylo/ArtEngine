#include "TestDrawPrimitives.h"

namespace test
{
	TestDrawPrimitives::TestDrawPrimitives() : test_line({ 0,0 }, { 0,0 }), test_line_thickness({ 32,32 }, { 64,64 })
	{
		
	}
	TestDrawPrimitives::~TestDrawPrimitives()
	{
	}
	void TestDrawPrimitives::OnUpdate(float deltaTime)
	{
		//Simple Line
		test_line.setPositions({ positions[0],positions[1] }, { positions[2], positions[3] });
		test_line.setColour(line_colour);
		test_line.Update(1);

		//Thick Line
		test_line_thickness.setThickness(line_thickness);
		test_line_thickness.Update(1);
	}
	void TestDrawPrimitives::OnRender()
	{
		//Simple Line
		//test_line.Draw();

		//Thick Line
		test_line_thickness.Draw();
	}
	void TestDrawPrimitives::OnGUIRender()
	{
		//Simple Line
		//ImGui::DragInt("Thickness", &line_thickness, 0.1f, 0, 0);
		if (line_thickness < 0) line_thickness = 0;
		//ImGui::DragFloat("Thickness", &line_thickness);
		ImGui::DragFloat4("Position", positions);
		ImGui::ColorPicker4("Line Colour", line_colour);

		//Thick Line
		ImGui::DragFloat("Thickness", &line_thickness, 0.1f, 0, 0);
	}
}