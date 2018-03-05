/*
 * cardtest2.c
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


#define ARR_LEN(x) ( sizeof(x) / sizeof(x[0]) )


struct testConf {
	int coin1;
	int coin2;
};




/*
 * assertTrue()
 *
 * 	@desc
 *
 *	@params
 *		struct 	gameState preState		state before cardEffect
 *		struct 	gameState postState		state after cardEffect
 *
 *	@outputs
 *		prints test failure report
 */
int assertTrue(struct gameState* preState, struct gameState* postState, struct testConf coins)
{
	int i;
	int p = preState->whoseTurn;
	int res = 0;
	/*
	 * check the the player's hand has X more treasure that what they
	 * started with.
	 */
	int X = 0;
	if (coins.coin1 > 0) { X += coins.coin1 - 3; }
	if (coins.coin2 > 0) { X += coins.coin2 - 3; }

	/* get pre and post amount. */
	updateCoins(p, preState, 0);
	updateCoins(p, postState, 0);

	int preTreasure = preState->coins;
	int posTreasure = postState->coins;

	if (posTreasure != preTreasure + X) {
	printf("Incorrect treasure in post hand!\n");
		printf("(%d,%d)\n", coins.coin1, coins.coin2);
		printf("pre: %d teasure, post: %d treasure.\n", 
				preTreasure, posTreasure);

		printf("pre hand:\n");
		for (i=0; i<preState->handCount[p]; i++) {
			printf("%d", preState->hand[p][i]);
		}
		printf("\n");

		printf("post hand:\n");
		for (i=0; i<postState->handCount[p]; i++) {
			printf("%d", postState->hand[p][i]);
		}
		printf("\n");

		printf("pre deck:\n");
		for (i=0; i<preState->deckCount[p]; i++) {
			printf("%d", preState->deck[p][i]);
		}
		printf("\n");

		printf("post deck:\n");
		for (i=0; i<postState->deckCount[p]; i++) {
			printf("%d", postState->deck[p][i]);
		}
		printf("\n");

		res = -1;
	}

	/*
	 * check that no other players decks and hands were effected...
	 */
	for (i=0; i<preState->numPlayers; i++) {
		if (i != p) {
			/* check the coins */
			updateCoins(i, preState, 0);
			updateCoins(i, postState, 0);
			if (preState->coins != postState->coins) {
				printf("other player's treaure modified!\n");
				res = -1;
			}
			/* check the deck counts */
			if ( preState->deckCount[i] != postState->deckCount[i] ) {
				printf("other player's deck modified!\n");
				res = -1;
			}
		}
	}

	if (res < 0) { return -1; } else { return 0; }
}


void placeCoin(struct testConf coins, int *deck, int len) {
	int i,j;
	if (len > 2) {
		i = (int)(Random() * (len - 1));
		do { j = (int)( Random() * (len -1 )); } while (j == i);
	} else {
		i=0;
		j=1;
	}

	deck[i] = coins.coin1;
	if (coins.coin2 != 0) { deck[j] = coins.coin2; }
}


int main()
{
	int i,j,z;
	int seed = 					1000;		/* for random number generator */
	int k[10] = {adventurer, council_room, feast, gardens, mine
								, remodel, smithy, village, baron, great_hall};
	struct gameState preState, postState;
	int res = 0;							/* for test results */
	const int handPos = 2;
	int deck[MAX_DECK];


	/* setup player configurations for test cases. */
	struct testConf Conf[] = {
		{copper, copper}, 	{copper, 0},
		{silver, silver}, 	{silver, 0},
		{gold, gold},		{gold, 0},
		{gold, silver},
		{gold, copper},
		{silver, copper}
	};

	/* setup deck size test cases. */
	int deckSize[] = {2,5,10,100};

	
	/* initiate tests... */
	printf ("TESTING adventurer card:\n");

	/* 
	 * test cardEffect() for adventurer with each configuration in Conf.
	 */
	for (i=0; i<ARR_LEN(Conf); i++) {
		/*
		 * configure the pre state of the game and copy to the post state.
		 */
		memset(&preState, 0, sizeof(struct gameState));
		memset(&postState, 0, sizeof(struct gameState));
		initializeGame(2, k, seed, &preState);

		/* replace one of the player's initial cards with a adventurer hard */
		preState.hand[preState.whoseTurn][handPos] = adventurer;

		for (j=0; j<ARR_LEN(deckSize); j++) {	
			/* load the test deck with kingdom cards */
			for (z=0; z<ARR_LEN(deck); z++) { deck[z] = k[i % 10]; }

			/* add coins of current config to the test deck. */
			placeCoin(Conf[i], deck, deckSize[j]);

			/* over-write the current player's deck with the cooked up deeck. */
			memcpy(preState.deck[preState.whoseTurn], deck, sizeof(deck));
			preState.deckCount[preState.whoseTurn] = deckSize[j] + 1;
			preState.discardCount[preState.whoseTurn] = 0;

			/* save the state. */
			memcpy(&postState, &preState, sizeof(struct gameState));

			/* run card effect on the configured state. */
			cardEffect(adventurer, 0, 0, 0, &postState, 0, 0);

			/* check if the post state was altered correctly */
			if (assertTrue(&preState, &postState, Conf[i]) < 0) { res = -1; }
		}
	}
	
	if (res == 0) { printf("All tests passed!\n"); }

	return 0;
}
