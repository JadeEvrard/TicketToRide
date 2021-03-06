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
File: move.h 
*/

#ifndef __T2R_TEST_MOVE_H__
#define __T2R_TEST_MOVE_H__
#include "TicketToRideAPI.h"
#include "jeu.h"

/* plays the move chosen by the player*/
t_return_code playOurMove(t_move* move, t_color* lastMove);

/* asks to the player which move he would like to play */
void askMove(t_move* move);

/* return if we have to replay or not */
int needReplay(t_move* move, t_color lastCard);

/* updates the game */
void updateMove(t_move move, t_track* tracks, t_game* game, int** G);

/* updates the game after a move from the opponent */
void updateOpponent(t_move move,t_track* tracks, t_game* game, int** G);

#endif