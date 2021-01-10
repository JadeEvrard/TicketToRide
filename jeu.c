/*
+----------------------------------------------------+
|                                                    |
|        #######                                     |
|           #    #  ####  #    # ###### #####        |
|           #    # #    # #   #  #        #          |
|           #    # #      ####   #####    #          |
|           #    # #      #  #   #        #          |
|           #    # #    # #   #  #        #          |
|           #    #  ####  #    # ######   #          |
|                                                    |
|                      ######                        |
|      #####  ####     #     # # #####  ######       |
|        #   #    #    #     # # #    # #            |
|        #   #    #    ######  # #    # #####        |
|        #   #    #    #   #   # #    # #            |
|        #   #    #    #    #  # #    # #            |
|        #    ####     #     # # #####  ######       |
|                                                    |
|                                                    |
+----------------------------------------------------+
*/

#include <stdio.h>
#include <stdlib.h>
#include "TicketToRideAPI.h"
#include "move.h"
#include "jeu.h"
#include "game.h"

int main(){

	int playerPlays; 		 /* boolean, tells if we play (0) or if the opponent plays (1) */
	t_return_code returnCode;
	t_move move;         /* current move */
	int replay = 0;      /* boolean, tells if the player replays or not */
	t_color lastMove = NONE; 	 /* last card taken if we need to replay else NONE */

	/* create a game, a map and players */
	t_board board;
	t_game game;
	t_player playerMe;
	t_player playerOpponent;

	createGame(&board,&playerMe);
	playerPlays = createMap(board,&game,&playerMe);
	createPlayers(&playerMe,&playerOpponent);
	displayGame(board,game,playerMe);

	/* game loop */
	do {
		/* display map */
		if(!replay)
			printMap();

		/* the opponent plays */
		if(playerPlays == 1){
			returnCode=getMove(&move, &replay);
		}
		else {
			/* I play */
			askMove(&move);
			replay = needReplay(&move,lastMove);
			returnCode = playOurMove(&move,&lastMove);
		}

		/* change player */
		if(returnCode == NORMAL_MOVE && !replay)
			playerPlays=!playerPlays;

	} while (returnCode == NORMAL_MOVE);

	/* check who wins */
	if((returnCode==WINNING_MOVE && playerPlays==0)||(returnCode==LOOSING_MOVE && playerPlays==1))
		printf("We are the best !\n");
	else 
		printf("The opponent was lucky\n");

	/* server shutdown */
	closeConnection();

	return EXIT_SUCCESS;
}