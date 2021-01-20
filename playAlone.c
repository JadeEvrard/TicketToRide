#include <stdio.h>
#include <stdlib.h>
#include "TicketToRideAPI.h"
#include "playAlone.h"
#include "jeu.h"
#include <limits.h>

void shortTrack(int Prec[], int src, int dest, int** G, t_game game, t_track* tracks){
	int N = game.board.nbCities;
	int* Visite = malloc(sizeof(int)*N);
	int u=src;
	int* D = malloc(sizeof(int)*N);

	/* initialisation des tableaux */
	for(int i=0; i<N; i++){
		D[i] = INT_MAX;	/* aucune distance calculée entre src et i */
		Visite[i] = 0; 	/* aucune ville n'a encore été visitée */
	}

	/* la distance de la src à la src est nulle */
	D[src] = 0;

	while(u!=dest){
		/* on cherche la ville non visitée la plus proche */
		u = distanceMini(D, Visite, N);
		/* on marque la ville précédente comme ayant été visitée */
		Visite[u] = 1;

		for(int k=0; k<N; k++){
			if((Visite[k]==0)&&(G[u][k]!=INT_MAX)&&(D[u]+G[u][k]<D[k])){
				D[k] = D[u] + G[u][k];
				Prec[k] = u;
			}
		}
	}
}

int distanceMini(int D[], int Visite[], int N){
	int indice_min;
	int min = INT_MAX;
	for(int i=0; i<N; i++){
		if(Visite[i]==0 && D[i]<min){
			min = D[i];
			indice_min = i;
		}
	}
	return indice_min;
}

int nbTracksToDest(int src, int dest, int Prec[]){
	int nbTracksToDest=0;
	int u=dest;
	while(u!=src){
		u=Prec[u];
		nbTracksToDest++;
	}
	return nbTracksToDest;
}

void tabTracksToDest(int src, int dest, int Prec[], t_game game, t_track* tracks, t_track* tracksToDest){
	int u=dest;
	int indice=0;
	while(u!=src){
		for(int i=0; i<game.board.nbTracks; i++){
			if((tracks[i].city1==u && tracks[i].city2==Prec[u])||(tracks[i].city1==Prec[u] && tracks[i].city2==u)){
				tracksToDest[indice]=tracks[i];
			}
		}
		u=Prec[u];
		indice++;
	}
}

int playAlone(int fisrtPlay, int* previousReturn, t_move* move, t_move* lastMove, t_game game, int Prec[], int** G, t_track* tracks){
	int ret;
	int scoreMin=INT_MAX;
	int indiceMin=0;
	int src[20];
	int dest[20];
	int color;
	int locos;
	int objectifPioche=1;
	int routePosee=0;
	int N=0;
	int currentSrc;
	int currentDest;
	int cartePioche=0;

	if(fisrtPlay==0){
		move->type=lastMove->type=DRAW_OBJECTIVES;
		ret=1;
		previousReturn[0]=1;
	}
	else{
		if(lastMove->type==DRAW_OBJECTIVES){
			if((previousReturn[0]==0)||(game.players[0].objectives[game.players[0].nbObjectives-3].score>=20 && game.players[0].objectives[game.players[0].nbObjectives-2].score>=20)||(game.players[0].objectives[game.players[0].nbObjectives-2].score>=20 && game.players[0].objectives[game.players[0].nbObjectives-1].score>=20)||(game.players[0].objectives[game.players[0].nbObjectives-1].score>=20 && game.players[0].objectives[game.players[0].nbObjectives-3].score>=20)||(game.players[0].objectives[game.players[0].nbObjectives-3].score<=12 && game.players[0].objectives[game.players[0].nbObjectives-2].score<=12)||(game.players[0].objectives[game.players[0].nbObjectives-2].score<=12 && game.players[0].objectives[game.players[0].nbObjectives-1].score<=12)||(game.players[0].objectives[game.players[0].nbObjectives-1].score<=12 && game.players[0].objectives[game.players[0].nbObjectives-3].score<=12)){
				move->type=CHOOSE_OBJECTIVES;
				for(int i=0; i<3; i++){
					move->chooseObjectives.chosen[i]=1;
					if(game.players[0].objectives[i].score<scoreMin){
						scoreMin=game.players[0].objectives[i].score;
						indiceMin=i;
					}
				}
				move->chooseObjectives.chosen[indiceMin]=0;
			}
			else{
				move->type=CHOOSE_OBJECTIVES;
				for(int i=0; i<3; i++){
					move->chooseObjectives.chosen[i]=1;
				}
			}
			lastMove->type=CHOOSE_OBJECTIVES;
		}
		else if(lastMove->type==DRAW_BLIND_CARD || lastMove->type==DRAW_CARD){
			//verification des objectifs 
			int previousN=0;
			int testObjectives[200];
			for(int s=0; s<200; s++){
				testObjectives[s]=1;
			}
			t_track* test_tracksToDest = malloc(sizeof(t_track)*1);
			for(int i=0; i<game.players[0].nbObjectives; i++){
				src[i]=game.players[0].objectives[i].city1;
				dest[i]=game.players[0].objectives[i].city2;
				shortTrack(Prec,src[i],dest[i],G,game,tracks);
				N=nbTracksToDest(src[i],dest[i],Prec);
				test_tracksToDest = realloc(test_tracksToDest, sizeof(t_track)*N);
				tabTracksToDest(src[i],dest[i],Prec,game,tracks,test_tracksToDest);
				for(int j=0; j<N; j++){
					if(test_tracksToDest[j].length!=0){
						currentSrc=src[i];
						currentDest=dest[i];
						testObjectives[i*(previousN)+j]=0;
					}
					else{
						testObjectives[i*(previousN)+j]=1;
					}
				}
				previousN=N;
			}
			for(int r=0; r<200; r++){
				if(testObjectives[r]==0){
					objectifPioche=0;
				}
			}

			//si on pioche parce qu'on a fini les objectifs
			if(objectifPioche==1){
				move->type=DRAW_BLIND_CARD;
				lastMove->type=CLAIM_ROUTE;
			}
			//si on pioche parce qu'on a pas fini les objectifs mais qu'on ne peut pas poser de route
			else{
				shortTrack(Prec,currentSrc,currentDest,G,game,tracks);
				N=nbTracksToDest(currentSrc,currentDest,Prec);
				t_track tracksToDest[N];
				tabTracksToDest(currentSrc,currentDest,Prec,game,tracks,tracksToDest);

				for(int k=0; k<N; k++){			 
					for(int i=0; i<5; i++){	
						if(game.faceUp[i]!=MULTICOLOR){
							if(tracksToDest[k].color1==game.faceUp[i]){
								color=tracksToDest[k].color1;							
							}
							else if(tracksToDest[k].color2==game.faceUp[i]){
								color=tracksToDest[k].color2;
							}
							if(tracksToDest[k].color1==game.faceUp[i] || tracksToDest[k].color2==game.faceUp[i]){
								move->type=DRAW_CARD;
								move->drawCard.card=color;
								lastMove->type=CLAIM_ROUTE;
								cartePioche=1;
							}
						}
					}
				}
				if(cartePioche==0){
					move->type=DRAW_BLIND_CARD;
					lastMove->type=CLAIM_ROUTE;
				}
			}
		}
		else{
			/* on doit traiter les objectifs 
			on vérifie d'abord si des objectifs sont déjà atteints */
			int previousN=0;
			int testObjectives[200];
			for(int s=0; s<200; s++){
				testObjectives[s]=1;
			}
			t_track* test_tracksToDest = malloc(sizeof(t_track)*1);
			for(int i=0; i<game.players[0].nbObjectives; i++){
				src[i]=game.players[0].objectives[i].city1;
				dest[i]=game.players[0].objectives[i].city2;
				shortTrack(Prec,src[i],dest[i],G,game,tracks);
				N=nbTracksToDest(src[i],dest[i],Prec);
				test_tracksToDest = realloc(test_tracksToDest, sizeof(t_track)*N);
				tabTracksToDest(src[i],dest[i],Prec,game,tracks,test_tracksToDest);
				for(int j=0; j<N; j++){
					if(test_tracksToDest[j].length!=0){
						currentSrc=src[i];
						currentDest=dest[i];
						testObjectives[i*(previousN)+j]=0;
					}
					else{
						testObjectives[i*(previousN)+j]=1;
					}
				}
				previousN=N;
			}
			for(int r=0; r<200; r++){
				if(testObjectives[r]==0){
					objectifPioche=0;
				}
			}

			if(objectifPioche==0){
				shortTrack(Prec,currentSrc,currentDest,G,game,tracks);
				N=nbTracksToDest(currentSrc,currentDest,Prec);
				t_track tracksToDest[N];

				tabTracksToDest(currentSrc,currentDest,Prec,game,tracks,tracksToDest);

				/* on teste en priorité si l'on peut poser une route de couleur */
				for(int k=0; k<N; k++){
					if(tracksToDest[k].taken==1 && tracksToDest[k].color1!=MULTICOLOR && tracksToDest[k].color2!=MULTICOLOR){
						/* si on a suffisamment de cartes de la couleur color1 pour poser une des routes */
						if(game.players[0].cards[tracksToDest[k].color1]+game.players[0].cards[MULTICOLOR] >= tracksToDest[k].length){
							if(game.players[0].cards[tracksToDest[k].color1]<tracksToDest[k].length){
								color=tracksToDest[k].color1;
								locos=tracksToDest[k].length-game.players[0].cards[tracksToDest[k].color1];
							}
							else{
								color=tracksToDest[k].color1;
								locos=0;
							}
						}
						/* si on a suffisamment de cartes de la couleur color2 pour poser une des routes */
						else if(game.players[0].cards[tracksToDest[k].color2]+game.players[0].cards[MULTICOLOR] >= tracksToDest[k].length){
							if(game.players[0].cards[tracksToDest[k].color2]<tracksToDest[k].length){
								color=tracksToDest[k].color2;
								locos=tracksToDest[k].length-game.players[0].cards[tracksToDest[k].color2];
							}
							else{
								color=tracksToDest[k].color2;
								locos=0;
							}
						}
						/* si on a suffisamment de cartes dans une des 2 couleurs du chemin, on pose la route */
						if(game.players[0].cards[tracksToDest[k].color1]+game.players[0].cards[MULTICOLOR]>=tracksToDest[k].length || game.players[0].cards[tracksToDest[k].color2]+game.players[0].cards[MULTICOLOR]>=tracksToDest[k].length){
							if(game.players[0].nbWagons >= tracksToDest[k].length){	
								move->type=CLAIM_ROUTE;
								move->claimRoute.city1=tracksToDest[k].city1;
								move->claimRoute.city2=tracksToDest[k].city2;
								move->claimRoute.color=color;
								move->claimRoute.nbLocomotives=locos;
								lastMove->type=CLAIM_ROUTE;
								routePosee=1;
								break;
							}
						}
					}
				}

				/* on teste ensuite si on peut poser une route multicolor */
				if(routePosee==0){
					for(int k=0; k<N; k++){
						if(tracksToDest[k].color1==MULTICOLOR){
							if(tracksToDest[k].taken==1){
								for(int m=1; m<9; m++){
									if(game.players[0].cards[m]+game.players[0].cards[MULTICOLOR]>=tracksToDest[k].length){
										if(game.players[0].cards[m]<tracksToDest[k].length){
											color=m;
											locos=tracksToDest[k].length-game.players[0].cards[color];
										}
										else{
											color=m;
											locos=0;
										}
										if(game.players[0].nbWagons >= tracksToDest[k].length){
											move->type=CLAIM_ROUTE;
											move->claimRoute.city1=tracksToDest[k].city1;
											move->claimRoute.city2=tracksToDest[k].city2;
											move->claimRoute.color=color;
											move->claimRoute.nbLocomotives=locos;
											lastMove->type=CLAIM_ROUTE;
											routePosee=1;
											break;
										}
									}
								}
							}
						}
					}
				}

				/* si on ne peut pas poser de cartes, on pioche */
				if(routePosee==0){
					for(int k=0; k<N; k++){			 
						for(int i=0; i<5; i++){		
							if(tracksToDest[k].color1==game.faceUp[i]){
								color=tracksToDest[k].color1;							
							}
							else if(tracksToDest[k].color2==game.faceUp[i]){
								color=tracksToDest[k].color2;
							}
							if(tracksToDest[k].color1==game.faceUp[i] || tracksToDest[k].color2==game.faceUp[i]){
								move->type=DRAW_CARD;
								move->drawCard.card=color;
								if(color==MULTICOLOR){
									lastMove->type=CLAIM_ROUTE;
								}
								else{
									lastMove->type=DRAW_CARD;
								}
								cartePioche=1;
							}
						}
					}
				}

				if(routePosee==0 && cartePioche==0){
					move->type=DRAW_BLIND_CARD;
					lastMove->type=DRAW_BLIND_CARD;
				}
			}
			else{
				//j'ai fini mes objectifs et je repioche de nouveaux objectifs
				if((game.players[0].nbWagons > 20 && game.players[1].nbWagons > 15)){
					move->type=DRAW_OBJECTIVES;
					lastMove->type=DRAW_OBJECTIVES;
				}
				//j'ai fini mes objectifs et je prefere ne pas en reprendre (trop tard)
				else{
					//j'essaie de placer des routes non occupées 
					for(int i=0; i<game.board.nbTracks; i++){
						if(tracks[i].taken==1 && tracks[i].color1!=MULTICOLOR && tracks[i].color2!=MULTICOLOR){
							if(game.players[0].cards[tracks[i].color1]+game.players[0].cards[MULTICOLOR] >= tracks[i].length){
								if(game.players[0].cards[tracks[i].color1] >= tracks[i].length){
									color=tracks[i].color1;
									locos=0;
								}
								else{
									color=tracks[i].color1;
									locos=tracks[i].length-game.players[0].cards[color];
								}
							}
							else if(game.players[0].cards[tracks[i].color2]+game.players[0].cards[MULTICOLOR] >= tracks[i].length){
								if(game.players[0].cards[tracks[i].color2] >= tracks[i].length){
									color=tracks[i].color2;
									locos=0;
								}
								else{
									color=tracks[i].color2;
									locos=tracks[i].length-game.players[0].cards[color];
								}
							}
							if(game.players[0].cards[tracks[i].color1]+game.players[0].cards[MULTICOLOR] >= tracks[i].length || game.players[0].cards[tracks[i].color2]+game.players[0].cards[MULTICOLOR] >= tracks[i].length){
								if(game.players[0].nbWagons >= tracks[i].length){
									move->type=CLAIM_ROUTE;
									move->claimRoute.city1=tracks[i].city1;
									move->claimRoute.city2=tracks[i].city2;
									move->claimRoute.color=color;
									move->claimRoute.nbLocomotives=locos;
									lastMove->type=CLAIM_ROUTE;
									routePosee=1;
								}
							}
						}
					}

					if(routePosee==0){
						for(int i=0; i<game.board.nbTracks; i++){
							if(tracks[i].taken==1 && tracks[i].color1==MULTICOLOR){
								for(int j=1; j<9; j++){
									if(game.players[0].cards[j]+game.players[0].cards[MULTICOLOR] >= tracks[i].length){
										if(game.players[0].cards[j] >= tracks[i].length){
											color=j;
											locos=0;
										}
										else{
											color=j;
											locos=tracks[i].length-game.players[0].cards[j];
										}
										if(game.players[0].nbWagons >= tracks[i].length){
											move->type=CLAIM_ROUTE;
											move->claimRoute.city1=tracks[i].city1;
											move->claimRoute.city2=tracks[i].city2;
											move->claimRoute.color=color;
											move->claimRoute.nbLocomotives=locos;
											lastMove->type=CLAIM_ROUTE;
											routePosee=1;
										}
									}
								}
							}
						}
					}

					//sinon je pioche une carte
					if(routePosee==0){
						move->type=DRAW_BLIND_CARD;
						lastMove->type=DRAW_BLIND_CARD;
					}
				}
			}
		}
		ret=1;
		previousReturn[0]=0;
	}
	return ret;
} 