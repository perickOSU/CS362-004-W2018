/*
 * cardtest3.c
 *
 * 	@author
 * 		Kruno Peric
 *
 *	@desc
 *		Tests the great hall card.
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
int assertTrue(struct gameState* preState, struct gameState* postState)
{
	int res, i;
	int p = preState->whoseTurn;
	/*
	 * check that num actions increased accordingly
	 */
	if ( postState->numActions != preState->numActions + 1) {
		printf("ERROR numActions in correct!\n");
	}

	/* check the deck count  for the player */
	int preDeckCount = preState->deckCount[p];
	int preDiscardCount = preState->discardCount[p];
	if (preDeckCount > 0) {
		/*
		 * if the deck has at least 1 cards, re-shuffling
		 * the discard pile won't be necessary and the deck
		 * should just have three less cards than before the call
		 * to cardEffect.
		 */
		if (postState->deckCount[p] != preDeckCount - 1) {
			printf("player deckCount mismatch!\n");
			res = -1;
		}
		if (postState->discardCount[p] != preDiscardCount + 1) {
			/*
			 * the discard pile should be incrased by one, since the
			 * great_hall card gets discarded after it's used.
			 */
			printf("Discard pile did not incease by 1 card!\n");
			res = -1;
		}
	} else {
		/*
		 * here the deck will have to be replenished with the shuffled
		 * discard pile.  The discard pile should then only contain the
		 * great_hall card.
		 */
		if (postState->deckCount[p] != preDiscardCount - 1) {
			printf("player deckCount mismatch after discard pile shuffle!\n");
			res = -1;
		}
		/* the discard count should have just the great_hall card. */
		if (postState->discardCount[p] != 1) {
			printf("player discardCount doesn't contain one card!\n");
		}
	}

	/*
	 * check that no other players decks and hands were effected...
	 */
	for (i=0; i<preState->numPlayers; i++) {
		if (i != p) {
			if ( preState->deckCount[i] != postState->deckCount[i] ) {
				printf("other player's deck modified!\n");
				res = -1;
			}
			if ( preState->handCount[i] != postState->handCount[i] ) {
				printf("other player's numActions modified!\n");
				res = -1;
			}
		}
	}

	if (res < 0) { return -1; } else { return 0; }
}


int main()
{
	int j,z;
	int seed = 					1000;		/* for random number generator */
	int k[10] = {adventurer, council_room, feast, gardens, mine
								, remodel, smithy, village, baron, great_hall};
	struct gameState preState, postState;
	int res = 0;							/* for test results */
	const int handPos = 2;
	int deck[MAX_DECK];


	/* setup deck size test cases. */
	int deckSize[] = {2,5,10,100};

	
	/* initiate tests... */
	printf ("TESTING great hall card:\n");

	/* 
	 * test cardEffect() for great_hall with each configuration in Conf.
	 */

	/* configure the pre state of the game and copy to the post state. */
	memset(&preState, 0, sizeof(struct gameState));
	memset(&postState, 0, sizeof(struct gameState));
	initializeGame(2, k, seed, &preState);

	/* replace one of the player's initial cards with a great_hall hard */
	preState.hand[preState.whoseTurn][handPos] = great_hall;

	for (j=0; j<ARR_LEN(deckSize); j++) {	
		/* load the test deck with kingdom cards */
		for (z=0; z<ARR_LEN(deck); z++) { deck[z] = k[z % 10]; }

		/* over-write the current player's deck with the cooked up deeck. */
		memcpy(preState.deck[preState.whoseTurn], deck, sizeof(deck));
		preState.deckCount[preState.whoseTurn] = deckSize[j] + 1;
		preState.discardCount[preState.whoseTurn] = 0;

		/* save the state. */
		memcpy(&postState, &preState, sizeof(struct gameState));

		/* run card effect on the configured state. */
		cardEffect(great_hall, 0, 0, 0, &postState, handPos, 0);

		/* check if the post state was altered correctly */
		if (assertTrue(&preState, &postState) < 0) { res = -1; }
	}
	
	if (res == 0) { printf("All tests passed!\n"); }

	return 0;
}
