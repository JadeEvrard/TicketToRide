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

int main(){

	char* serverName = "li1417-56.members.linode.com";
	unsigned int port = 1234;
	char* name = "Radé";
	char gameName[20];
	int nbCities;
	int nbTracks;

	/* connection to server */
	connectToServer(serverName,port,name);

	/* wait for a game */
	waitForT2RGame("TRAINING DO_NOTHING timeout=10000 start=0 map=USA",gameName,&nbCities,&nbTracks);

	printf("Game: %s, nbCities = %d, nbTracks = %d\n", gameName, nbCities, nbTracks);

	/* server shutdown */
	closeConnection();

	return EXIT_SUCCESS;
}