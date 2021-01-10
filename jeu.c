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

int main(){

	char* serverName = "li1417-56.members.linode.com";
	unsigned int port = 1234;
	char* name = "Radé";
	char gameName[20];
	int nbCities;
	int nbTracks;
	int* arrayTracks; 
	t_color faceUp[5]; /* turned over cards */
	t_color myCards[4];	 /* my cards */
	int player; 		 /* boolean, tells if we play (0) or if the opponent plays (1) */
	t_return_code returnCode;
	t_move move;         /* current move */
	int replay = 0;      /* boolean, tells if the player replays or not */
	t_color lastMove = NONE; 	 /* last card taken if we need to replay else NONE */

	/* connection to server */
	connectToServer(serverName,port,name);

	/* wait for a game */
	waitForT2RGame("TRAINING DO_NOTHING timeout=10000 start=0 map=USA",gameName,&nbCities,&nbTracks);

	arrayTracks = malloc(5*nbTracks*sizeof(int));
	player = getMap(arrayTracks, faceUp, myCards);

	/* game loop */
	do {
		/* display map */
		if(!replay)
			printMap();

		/* the opponent plays */
		if(player == 1){
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
			player=!player;

	} while (returnCode == NORMAL_MOVE);

	/* check who wins */
	if((returnCode==WINNING_MOVE && player==0)||(returnCode==LOOSING_MOVE && player==1))
		printf("We are the best !\n");
	else 
		printf("The opponent was lucky\n");

	/* server shutdown */
	closeConnection();

	return EXIT_SUCCESS;
}