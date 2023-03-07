#include "system/Game.h"
#include "Shader.h"

int main(int argc, char *argv[])
{
	bool UsingOpenGL = true;

	//Initiate Game
	Game game;

	if(UsingOpenGL) game.glinit();
	else game.init();
	
	/* ################### */
	/* #### GAME LOOP #### */
	/* ################### */
	while (game.gameActive)
	{
		/* ####################### */
		/* #### LOGIC UPDATES #### */
		/* ####################### */

		if (UsingOpenGL) game.glupdate();
		else game.update();
		
		// ####################
		// #### Draw Calls ####
		// ####################

		if (UsingOpenGL) game.gldraw();
		else game.draw();
		
		if (UsingOpenGL) game.gldraw_gui();
		else game.draw_gui();
		
		if (UsingOpenGL) game.glpage_flip();
		else game.page_flip();
	}

	return 0;
}