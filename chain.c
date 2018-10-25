#include <stdio.h>
#include <SDL2/SDL.h>
#include "chain.h"

int rows, columns;

int main(int argc, char *argv[]) {	
	/* Initialize SDL */
	if(SDL_Init(SDL_INIT_VIDEO)) {
		fprintf(stderr, "Could not Initialize SDL : %s\n", SDL_GetError());
		exit(1);
	}
	
	/* Declare human players and computer players variables, a board and player row */
	int players_number, computer_players_number;
	board b = NULL;
	player *pl, *pl2;
	 
	/* First ask user whether to start a new game or resume a game. For new game, define table rows, columns, 		 * human and computer players in a window.
	 * Also, ask if user wants to quit.
	 */
	switch(startmenu(&players_number, &computer_players_number)) {
		case QUIT:
			return 0;
			break;
		case NEW_GAME:
			/* Initiate a board and players row */
			b = Initiate_My_Board();
			pl = Create_Player_Row(players_number, computer_players_number); 
			pl2 = pl;
			break;
		case RESUME:
			//code to extract game information from file	
			break;
		default:
			break;	
	} 
		
	/* Call a function to start the game */
	START_THE_GAME(&b, &pl, players_number, computer_players_number);
	
	/* Free the malloced Resources */
	DestroyPlayer(&pl2);
	DestroyBoard(&b);
	SDL_Quit();
	
	/* Successfully exit the game */
	return 0;
}


