#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "TicketToRideAPI.h"
#include "move.h"

/* plays the move choosen by the player */
t_return_code playOurMove(t_move* move, t_color* lastCard){
	t_return_code ret;

	switch (move->type) {
		case CLAIM_ROUTE:
			ret = claimRoute(move->claimRoute.city1, move->claimRoute.city2, move->claimRoute.color, move->claimRoute.nbLocomotives);
			*lastCard = NONE;
			break;
		case DRAW_CARD:
			ret = drawCard(move->drawCard.card, move->drawCard.faceUp);
			*lastCard = (*lastCard==NONE && move->drawCard.card!= MULTICOLOR) ? move->drawCard.card : NONE;
			break;
		case DRAW_BLIND_CARD:
			ret = drawBlindCard(&move->drawBlindCard.card);
			*lastCard = (*lastCard==NONE) ? move->drawBlindCard.card : NONE;
			break;
		case DRAW_OBJECTIVES:
			ret = drawObjectives(move->drawObjectives.objectives);
			for(int i=0; i<3; i++){
				printf("%d. %d (", i, move->drawObjectives.objectives[i].city1);
				printCity(move->drawObjectives.objectives[i].city1);
				printf(") -> (");
				printCity(move->drawObjectives.objectives[i].city2);
				printf(") %d (%d pts)\n", move->drawObjectives.objectives[i].city2, move->drawObjectives.objectives[i].score);
			}
			*lastCard = NONE;
			break;
		case CHOOSE_OBJECTIVES:
			ret = chooseObjectives(move->chooseObjectives.chosen);
			*lastCard = NONE;
			break;
	}
	return ret;
}

/* asks to the player which move he would like to play */
void askMove(t_move* move){
	/* ask for the type */
	printf("Choose a move\n");
	printf("1. claim a route\n");
	printf("2. draw a blind card\n");
	printf("3. draw a card\n");
	printf("4. draw objectives\n");
	printf("5. choose objectives\n");
	scanf("%d", &move->type);

	/* ask for details */
	if (move->type == CLAIM_ROUTE) {
		printf("Give city1, city2, color and nb of locomotives: ");
		scanf("%d %d %d %d", &move->claimRoute.city1, &move->claimRoute.city2, &move->claimRoute.color,
			  &move->claimRoute.nbLocomotives);
	}
	else if (move->type == DRAW_CARD) {
		printf("Give the color: ");
		scanf("%d", &move->drawCard.card);
	}
	else if (move->type == CHOOSE_OBJECTIVES){
		printf("For each objective, give 0 or 1:");
		scanf("%d %d %d", &move->chooseObjectives.chosen[0], &move->chooseObjectives.chosen[1], &move->chooseObjectives.chosen[2]);
	}
}

/* return if we have to replay or not */
int needReplay(t_move* move, t_color lastCard){
	int replay = 0;

	if (move->type == DRAW_OBJECTIVES)
		replay = 1;
	else if (move->type == DRAW_BLIND_CARD && lastCard == NONE)
		replay = 1;
	else if (move->type == DRAW_CARD && lastCard == NONE && move->drawCard.card != MULTICOLOR)
		replay = 1;

	return replay;
}

/* updates the game after a move from me */
void updateMove(t_move move, t_track* tracks, t_game* game, int** G){
	switch(move.type){
		case CLAIM_ROUTE:
			for(int i=0; i<game->board.nbTracks; i++){
				if((move.claimRoute.city1==tracks[i].city1)&&(move.claimRoute.city2==tracks[i].city2)){
					tracks[i].taken=0;
					game->players[0].nbWagons=game->players[0].nbWagons-tracks[i].length;
					game->players[0].nbCards=game->players[0].nbCards-tracks[i].length;
					game->players[0].cards[move.claimRoute.color]=game->players[0].cards[move.claimRoute.color]-tracks[i].length+move.claimRoute.nbLocomotives;
					game->players[0].cards[MULTICOLOR]=game->players[0].cards[MULTICOLOR]-move.claimRoute.nbLocomotives;
					tracks[i].length=0;	
					G[move.claimRoute.city1][move.claimRoute.city2]=0;
					G[move.claimRoute.city2][move.claimRoute.city1]=0;
				}
			}
			break;
		case DRAW_CARD:
			game->players[0].nbCards++;
			game->players[0].cards[move.drawCard.card]++;
			for(int i=0; i<5; i++){
				game->faceUp[i] = move.drawCard.faceUp[i];
			}
			break;
		case DRAW_BLIND_CARD:
			game->players[0].nbCards++;
			game->players[0].cards[move.drawBlindCard.card]++;
			break;
		case DRAW_OBJECTIVES:
			for(int i=0; i<3; i++){
				game->players[0].objectives[game->players[0].nbObjectives+i]=move.drawObjectives.objectives[i];
			}
			game->players[0].nbObjectives=game->players[0].nbObjectives+3;
			break;
		case CHOOSE_OBJECTIVES:
			for(int i=game->players[0].nbObjectives-3; i<game->players[0].nbObjectives; i++){
				if(move.chooseObjectives.chosen[i-game->players[0].nbObjectives+3]==0){
					game->players[0].objectives[i].city1=0;
					game->players[0].objectives[i].city2=0;
					game->players[0].objectives[i].score=0;
					for(int j=i; j<game->players[0].nbObjectives; j++){
						game->players[0].objectives[j]=game->players[0].objectives[j+1];
					}
					game->players[0].nbObjectives--;
				}
			}
			break;
	}
}

/* updates the game after a move from the opponent */
void updateOpponent(t_move move,t_track* tracks, t_game* game, int** G){
	switch(move.type){
		case CLAIM_ROUTE:
			for(int i=0; i<game->board.nbTracks; i++){
				if((move.claimRoute.city1==tracks[i].city1)&&(move.claimRoute.city2==tracks[i].city2)){
					tracks[i].taken=INT_MAX;
					game->players[1].nbWagons=game->players[1].nbWagons-tracks[i].length;
					game->players[1].nbCards=game->players[1].nbCards-tracks[i].length;
					tracks[i].length=INT_MAX;
					G[move.claimRoute.city1][move.claimRoute.city2]=INT_MAX;
					G[move.claimRoute.city2][move.claimRoute.city1]=INT_MAX;
				}
			}
			break;
		case DRAW_CARD:
			game->players[1].nbCards++;
			for(int i=0; i<5; i++){
				game->faceUp[i] = move.drawCard.faceUp[i];
			}
			break;
		case DRAW_BLIND_CARD:
			game->players[1].nbCards++;
			break;
		case DRAW_OBJECTIVES:
			game->players[1].nbObjectives=game->players[1].nbObjectives+3;
			break;
		case CHOOSE_OBJECTIVES:
			for(int i=game->players[1].nbObjectives-3; i<game->players[1].nbObjectives; i++){
				if(move.chooseObjectives.chosen[i-game->players[1].nbObjectives+3]==0){
					game->players[1].nbObjectives--;
				}
			}
			break;
	}
}