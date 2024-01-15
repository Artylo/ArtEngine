#include "BountyHunter.h"

#include "imgui/imgui.h"

#include "system/graphics/VertexBufferLayout.h"

namespace test
{
	test::BountyHunter::BountyHunter(GameManager* GM) : Test(GM)
	{
		BH_GM.projection_matrix = &projection_matrix;
		BH_GM.view_matrix = &view_matrix;

		BH_GM.input_manager = GM->input_manager;

		background = new BackgroundTiled(&BH_GM); //@CLEANUP


		player.init(&BH_GM);
	}

	test::BountyHunter::~BountyHunter()
	{
		delete background;
	}

	void test::BountyHunter::OnUpdate(float deltaTime)
	{
		//glm::mat4 zoom_matrix = glm::identity() * glm::scale(camera_zoom, camera_zoom, 1.0f);
		//view_matrix = view_matrix * zoom_matrix;

		background->Update(deltaTime);
		player.update(deltaTime);

		//@TEMP: INPUT TEST
		switch (BH_GM.input_manager->event_ptr->type)
		{
		case SDL_KEYDOWN:
			switch (BH_GM.input_manager->event_ptr->key.keysym.sym)
			{
			case SDLK_RIGHT:
				//player.position.x += 1; //HUGE SUCCESS!!!!
				break;
			}
			break;
		}
	}

	void test::BountyHunter::OnRender()
	{
		background->Draw();
		player.draw();
	}

	void test::BountyHunter::OnGUIRender()
	{
		ImGui::SeparatorText("Camera");
		ImGui::DragFloat("Camera Zoom", &camera_zoom_temp, 0.001f);
		if (camera_zoom_temp != camera_zoom)
		{
			view_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)); // Camera
			camera_zoom = camera_zoom_temp;
			zoom_matrix = glm::scale(identity_matrix, glm::vec3(camera_zoom, camera_zoom, camera_zoom));
			transform_matrix = zoom_matrix * identity_matrix;
			//model_view_projection = transform_matrix * model_view_projection;
			view_matrix = transform_matrix * view_matrix;
		}

		ImGui::SeparatorText("Player");
		ImGui::DragFloat2("Player Position", (float*)&player.position);
		
		ImGui::SeparatorText("Background");
		background->GUIDraw();
	}
}
