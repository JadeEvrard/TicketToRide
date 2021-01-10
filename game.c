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
#include "game.h"
#include "jeu.h"

/* creates the game */
void createGame(t_board* board,t_player* playerMe){
	char* serverName = "li1417-56.members.linode.com";
	unsigned int port = 1234;
	playerMe->name = "Radé";
	char gameName[20];

	/* connection to server */
	connectToServer(serverName,port,playerMe->name);
	/* wait for a game */
	waitForT2RGame("TRAINING DO_NOTHING timeout=10000 start=0 map=USA",gameName,&board->nbCities,&board->nbTracks);
	board->arrayTracks = malloc(5*board->nbTracks*sizeof(int));
}

/* creates the map */
int createMap(t_board board, t_game* game, t_player* player){
	return getMap(board.arrayTracks,game->faceUp,player->initialCards);
}

/* creates the players */
void createPlayers(t_player* playerMe, t_player* playerOpponent){
	playerMe->nbWagons = playerOpponent->nbWagons = 45;
	playerMe->nbCards = playerOpponent->nbWagons = 4;
	playerMe->nbObjectives = 0;
	for(int i=0; i<=10; i++){
		switch(playerMe->initialCards[i]){
			case PURPLE:
				playerMe->cards[PURPLE]++;
				break;
			case WHITE:
				playerMe->cards[WHITE]++;
				break;
			case BLUE:
				playerMe->cards[BLUE]++;
				break;
			case YELLOW:
				playerMe->cards[YELLOW]++;
				break;
			case ORANGE:
				playerMe->cards[ORANGE]++;
				break;
			case BLACK:
				playerMe->cards[BLACK]++;
				break;
			case RED:
				playerMe->cards[RED]++;
				break;
			case GREEN: 
				playerMe->cards[GREEN]++;
				break;
			case MULTICOLOR:
				playerMe->cards[MULTICOLOR]++;
				break;
		}
	}
}

/* displays the board game */
void displayGame(t_board board, t_game game, t_player player){
	printf("NbCities :%d\nNbTracks :%d\n",board.nbCities,board.nbTracks);
}