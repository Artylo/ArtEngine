#include "BountyHunter.h"

#include "imgui/imgui.h"

#include "system/graphics/VertexBufferLayout.h"

namespace test
{
	test::BountyHunter::BountyHunter()
	{
		GM.projection_matrix = &projection_matrix;
		GM.view_matrix = &view_matrix;

		player.GM = &GM;
		player.init();
	}

	test::BountyHunter::~BountyHunter()
	{

	}

	void test::BountyHunter::OnUpdate(float deltaTime)
	{
		player.update(deltaTime);
	}

	void test::BountyHunter::OnRender()
	{
		player.draw();
	}

	void test::BountyHunter::OnGUIRender()
	{
		ImGui::DragFloat2("Position", (float*)&player.position);
	}
}
