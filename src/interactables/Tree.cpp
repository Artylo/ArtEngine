#include "Tree.h"

Tree::Tree()
{
	sprite = std::shared_ptr<SDL_Surface>(IMG_Load("img/tree.png"), SDL_FreeSurface);
	assert(sprite.get() != NULL);

	box.w = sprite.get()->w;
	box.h = sprite.get()->h;
}

void Tree::init(GameManager* GM, std::unique_ptr<Tree>* sp)
{
	Wall::init(GM);
	assert(sp != nullptr);
	selfptr = sp;
	assert(selfptr != nullptr);
}

void Tree::update()
{
	if (durability <= 0)
	{
		//this->~Tree(); //Has the desired effect but does not release memory.
		selfptr->reset(nullptr);
		//delete this;
	}
	else
	{
		Wall::update();

		
		if(input->mouseIsHovering(*this))
		{
			if (this->durability != 0)
			{
				if (input->mouse_left == 1)
				{
					this->durability -= 100;
					//input.mouse_left = 0;
				}
			}
		}
	}
}

void Tree::draw_self()
{
	
	if (durability <= 0)
	{
		//delete this;
	}
	else
	{
		Wall::draw_self();
		//Healthbar
		//(CURRENT_Value / MAXIMUM_value) * 100)
		if (durability != initial_durability)
		{
			SDL_RenderDrawLine( // Healthbar Backgound
				gameRenderer,
				box.x,
				box.y + box.w + 6,
				box.x + box.w, //Value
				box.y + box.w + 6
			);

			if (durability != 0)
			{
				draw_set_color(gameRenderer, c_green);
				SDL_RenderDrawLine( // Healthbar
					gameRenderer,
					box.x,
					box.y + box.w + 6,
					box.x + (box.w * (durability / (float)initial_durability)), //Value @HARDCODED: initial durability value hardcoded
					box.y + box.w + 6
				);
				draw_reset_color(gameRenderer);
			}
		}
	}
}
