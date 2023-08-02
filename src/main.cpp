#include "system/Game.h"
#include <iostream>

int main(int argc, char *argv[])
{
	std::cout << "Pre-init Test." << std::endl;

	//Initiate Game
	Game game;

	std::cout << "Game Initialised." << std::endl;

	game.glinit();

	
	/* ################### */
	/* #### GAME LOOP #### */
	/* ################### */
	while (game.gameActive)
	{
		/* ####################### */
		/* #### LOGIC UPDATES #### */
		/* ####################### */

		game.glupdate();
		
		// ####################
		// #### Draw Calls ####
		// ####################

		game.gldraw();
		game.gldraw_gui();
		game.glpage_flip();
	}
	return 0;
}