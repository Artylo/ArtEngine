#pragma once

#include "system/globals.h"
#include "tests/Test.h"
#include "system/GameManager.h"
#include "BH_assets/entities/Player.h"
#include "BH_assets/BackgroundTiled.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

namespace test
{
	class BountyHunter : public Test
	{
	public:
		//ImVec2 position = { (float)w_width / 4 -32, (float)w_height / 4 -32};

		GameManager GM;
		Player player;

		BackgroundTiled* background;

	private:

		glm::mat4 identity_matrix = glm::mat4(1.0f);
		glm::mat4 zoom_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(camera_zoom, camera_zoom, camera_zoom));
		glm::mat4 transform_matrix = zoom_matrix * identity_matrix;
		float camera_zoom_temp = 1.0f;
		float camera_zoom = 1.0f;



		glm::mat4 projection_matrix = glm::ortho(0.0f, (float)w_width / 2, (float)w_height / 2, 0.0f, -1.0f, 1.0f); // Coordinate System
		glm::mat4 view_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)); // Camera
		glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(32, 32, 0));
		glm::mat4 model_view_projection = projection_matrix * view_matrix * model_matrix;

		//Renderer renderer;

	public:
		BountyHunter();
		~BountyHunter() override;

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnGUIRender() override;
	private:
		//float clear_colour[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	};
}


