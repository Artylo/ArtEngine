#include "system/Game.h"

int main(int argc, char *argv[])
{
	//Initiate Game
	Game game;
	//game.init_SDL();
	game.init();

	/* ################### */
	/* #### GAME LOOP #### */
	/* ################### */
	while (game.gameActive)
	{
		/* ####################### */
		/* #### LOGIC UPDATES #### */
		/* ####################### */

		game.update();

		// ####################
		// #### Draw Calls ####
		// ####################

		game.draw();
		game.draw_gui();
		game.page_flip();
	}

	return 0;
}