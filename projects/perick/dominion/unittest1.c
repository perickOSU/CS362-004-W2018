/*
 * unittest1.c
 *
 * 	@author
 * 		Kruno Peric
 *
 *	@desc
 *		Tests the updateCoins() function.
 *
 *	@inputs
 *
 *	@outputs
 */
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"


/* set NOISY_TEST to 0 to remove printfs from output. */
#define NOISY_TEST 1

#define ARR_LEN(x) ( sizeof(x) / sizeof(x[0]) )


struct playerConfig {
	int player;
	int numPlayers;
};

int main()
{
	int i, j, h, bonus;
	int seed = 					1000;		/* for random number generator */
	const int MAX_BONUS = 		10;
	int k[10] = {adventurer, council_room, feast, gardens, mine
								, remodel, smithy, village, baron, great_hall};
	struct gameState G;


	/* 
	 * setup hand configurations for test cases.
	 * 	config 1:	all copper
	 * 	config 2:	all silver
	 * 	config 3:	all gold
	 * 	config 4:	only kingdom cards, of all kinds
	 */
	int hands[4][MAX_HAND];
	for (i=0; i<ARR_LEN(hands); i++) { memset(hands[i], 0, sizeof(hands[i])); }

	for (i=0; i<ARR_LEN(hands[0]); i++) { hands[0][i] = copper; }	/* conf 1 */
	for (i=0; i<ARR_LEN(hands[1]); i++) { hands[1][i] = silver; }	/* conf 2 */
	for (i=0; i<ARR_LEN(hands[2]); i++) { hands[2][i] = gold; }		/* conf 3 */
	for (i=0; i<ARR_LEN(hands[3]); i++) { hands[3][i] = k[i % 10]; }/* conf 4 */

	int coinValue[] = { 1, 2, 3, 0 };


	/* setup hand size configurations for test cases. */
	int handSize[] = { 0, 1, 2, 3, 4, 5, MAX_HAND };


	/* setup player configurations for test cases. */
	struct playerConfig pConf[] = {
		{0,2}, {1,2}, 
		{0,3}, {1,3}, {2,3},
		{0,MAX_PLAYERS}, {1,MAX_PLAYERS}, {2,MAX_PLAYERS}, 
												{MAX_PLAYERS-1, MAX_PLAYERS}
	};

	
	/* initiate tests... */
	printf ("TESTING updateCoins():\n");

	for (h=0; h<ARR_LEN(hands); h++) {
		for (i=0; i<ARR_LEN(pConf); i++) {
			for (j=0; j<ARR_LEN(handSize); j++) {
				for (bonus=0; bonus<MAX_BONUS; bonus++) {
					/*
					 * intialize the game state to reset its values.  Set the 
					 * number of cards in the players hand to handSize.
					 */
					memset(&G, 0, sizeof(struct gameState));
					initializeGame(pConf[i].numPlayers, k, seed, &G);
					G.handCount[pConf[i].player] = handSize[j];


					/*
					 * run the hand with player, G.numPlayers, bonus, and
					 * G.handCount[player] as a configuration in the game
					 * state.
					 */
					memcpy(G.hand[pConf[i].player], hands[h], sizeof(hands[h]));
					updateCoins(pConf[i].player, &G, bonus);

					
					/*
					 * assert that the game state is altered such that
					 * G.coins = handSize * currency_value + bonus for the
					 * respective handCount and currency value of the 
					 * treasury being tested.
					 */
					assert(G.coins == handSize[j] * coinValue[h] + bonus);
//					printf("G.coins = %d, expected = %d\n", G.coins, handSize[j] * 1 + 2);
					}
			}
		}
	}
	
	
	printf("All tests passed!\n");
	return 0;
}
