#pragma once

#include "../../system/graphics/Sprite.h"
#include "../../system/GameManager.h"

class Entity
{
public:
	GameManager* GM = nullptr;
	//std::string sprite_filepath = "img/tile.png";
	//std::string shader_filepath = "shader/base.shader";
	glm::vec2 position = { (float)w_width / 4 - 32, (float)w_height / 4 - 32 };

	Sprite sprite;


public:
	void init(GameManager* gamemanager);
	void update(float deltaTime);
	void draw();

	Entity();
	~Entity();
};

