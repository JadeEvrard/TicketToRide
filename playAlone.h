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
File: playAlone.h 
*/

#ifndef __T2R_TEST_ALONE_H__
#define __T2R_TEST_ALONE_H__
#include "TicketToRideAPI.h"
#include "jeu.h"

void shortTrack(int Prec[], int src, int dest, int** G, t_game game, t_track* tracks);

int distanceMini(int D[], int Visite[], int N);

int nbTracksToDest(int src, int dest, int Prec[]);

void tabTracksToDest(int src, int dest, int Prec[], t_game game, t_track* tracks, t_track* tracksToDest);

int playAlone(int fisrtPlay, int* previousReturn, t_move* move, t_move* lastMove, t_game game, int Prec[], int** G, t_track* tracks);

#endif