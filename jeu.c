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
#include <limits.h>
#include "TicketToRideAPI.h"
#include "move.h"
#include "jeu.h"
#include "game.h"
#include "playAlone.h"

int main(){

	t_return_code returnCode;
	t_move move;         /* current move */
	int replay = 0;      /* boolean, tells if the player replays or not */
	t_color lastCard = NONE; 	 /* last card taken if we need to replay else NONE */
	int firstPlay=0;				/* boolean, tells if we play for the first time or not */
	t_move lastMove;				/* last move done */

	/* create a game, a map and players */
	t_board board;
	t_game game;
	t_player playerMe;
	t_player playerOpponent;


	createGame(&board,&game,&playerMe);
	game.player = createMap(board,&game,&playerMe);
	createPlayers(&playerMe,&playerOpponent);
	game.players[0]=playerMe;
	game.players[1]=playerOpponent;
	game.board=board;
	displayGame(game);

	t_track tracks[game.board.nbTracks];
	/* initialization of all the track structures */
	for(int i=0; i<game.board.nbTracks; i++){
		tracks[i].city1=board.arrayTracks[i*5];
		tracks[i].city2=board.arrayTracks[i*5+1];
		tracks[i].length=board.arrayTracks[i*5+2];
		tracks[i].color1=board.arrayTracks[i*5+3];
		tracks[i].color2=board.arrayTracks[i*5+4];
		tracks[i].taken=1;
	}

	for(int i=0; i<20; i++){
		for(int j=0; j<2; j++){
			game.players[j].objectives[i].city1=0;
			game.players[j].objectives[i].city2=0;
			game.players[j].objectives[i].score=0;
		}
	}

	int** G = malloc(sizeof(int*)*game.board.nbCities);
	for(int i=0; i<game.board.nbCities; i++){
		G[i]=malloc(sizeof(int)*game.board.nbCities);
	}
	for(int i=0; i<game.board.nbCities; i++){
		for(int j=0; j<game.board.nbCities; j++){
			G[i][j] = INT_MAX;
			for(int k=0; k<game.board.nbTracks; k++){
				if((tracks[k].city1==i && tracks[k].city2==j)||(tracks[k].city1==j && tracks[k].city2==i)){
					G[i][j]=tracks[k].length;
					G[j][i]=tracks[k].length;
				}
			}
		}
	}

	int Prec[game.board.nbCities];
	for(int i=0; i<game.board.nbCities; i++){
		Prec[i]=0;
	}

	int previousReturn[1]={0};

	/* game loop */
	do {
		/* display map */
		if(!replay)
			printMap();

		/* the opponent plays */
		if(game.player == 1){
			returnCode=getMove(&move, &replay);
			updateOpponent(move,tracks,&game,G);
		}
		else {
			/* I play */
			firstPlay = playAlone(firstPlay,previousReturn,&move,&lastMove,game,Prec,G,tracks);
			replay = needReplay(&move,lastCard);
			returnCode = playOurMove(&move,&lastCard);
			updateMove(move,tracks,&game,G);		
		}

		/* change player */
		if(returnCode == NORMAL_MOVE && !replay)
			game.player=!game.player;

	} while (returnCode == NORMAL_MOVE);

	/* check who wins */
	if((returnCode==WINNING_MOVE && game.player==0)||(returnCode==LOOSING_MOVE && game.player==1))
		printf("We are the best !\n");
	else 
		printf("The opponent was lucky\n");

	/* server shutdown */
	closeConnection();

	for(int i=0; i<game.board.nbCities; i++){
		free(G[i]);
	}
	free(G);

	return EXIT_SUCCESS;
}