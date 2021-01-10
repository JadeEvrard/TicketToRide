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
File: game.h 
*/

#ifndef __T2R_TEST_GAME_H__
#define __T2R_TEST_GAME_H__
#include "TicketToRideAPI.h"
#include "jeu.h"

/* creates the game */
void createGame(t_board* board, t_player* playerMe);

/* creates the map */
int createMap(t_board board, t_game* game, t_player* player);

/* creates the players */
void createPlayers(t_player* playerMe, t_player* playerOpponent);

/* displays the board game */
void displayGame(t_board board, t_game game, t_player player);

#endif