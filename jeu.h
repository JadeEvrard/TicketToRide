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
File: jeu.h 
*/

#ifndef __T2R_TEST_JEU_H__
#define __T2R_TEST_JEU_H__
#include "TicketToRideAPI.h"

typedef struct{
	int nbCities;				/* number of cities */
	int nbTracks;				/* number of tracks */
	inht* arrayTracks;			/* tracks */
}t_board;

typedef struct{
	int city1, city2; 			/* id of the cities */
	int length;					/* length of the track */
	t_color color1, color2;		/* colors */
	int taken;					/* tell if taken */
}t_track;

typedef struct{
	char name[20];				/* name of the player */
	int nbWagons;				/* number of wagons */
	int nbCards;				/* number of cards */
	int cards[10];				/* number of cards of each color, ie cards[YELLOW] is the number of YELLOW cards */
	int nbObjectives;			/* number of objectives */
	t_objective objectives[20];	/* objectives */	
}t_player;

typedef struct{
	char name[20];				/* name of the game */
	t_color faceUp[5];			/* face up cards */
	int player;					/* player who plays: 0 -> I play, 1 -> the opponent plays */
	t_player players[2];		
	t_board board;				/* board of the game */
}t_game;

#endif