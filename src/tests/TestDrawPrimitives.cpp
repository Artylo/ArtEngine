#include "TestDrawPrimitives.h"

namespace test
{
	TestDrawPrimitives::TestDrawPrimitives(GameManager* GM) : Test(GM), test_line({ 0,0 }, { 0,0 })
	{
		test_line_thickness = new LineThickness({ 0,0 }, { 32, 32 });
	}
	TestDrawPrimitives::~TestDrawPrimitives()
	{
	}
	void TestDrawPrimitives::OnUpdate()
	{
		//Simple Line
		test_line.setPositions({ positions[0],positions[1] }, { positions[2], positions[3] });
		test_line.setColour(line_colour);
		test_line.Update(1);

		//Thick Line
		test_line_thickness->Update(Test::testGM->deltaTime);
	}
	void TestDrawPrimitives::OnRender()
	{
		//Simple Line
		//test_line.Draw();

		//Thick Line
		test_line_thickness->Draw();
	}
	void TestDrawPrimitives::OnGUIRender()
	{
		//Simple Line
		//ImGui::DragInt("Thickness", &line_thickness, 0.1f, 0, 0);
		//ImGui::DragFloat("Thickness", &line_thickness);
		ImGui::DragFloat4("Position", positions);
		ImGui::ColorEdit3("Line Colour", line_colour);

		//Thick Line
		test_line_thickness->DrawGUI();
	}
}