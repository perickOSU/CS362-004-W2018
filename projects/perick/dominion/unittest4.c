/*
 * unittest4.c
 *
 * 	@author
 * 		Kruno Peric
 *
 *	@desc
 *		Tests the numHandCards() function.
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


struct turnConfig {
	int whoseTurn;
	int numPlayers;
};




/*
 * assertTrue()
 *
 * 	@desc
 * 		Test that turnExpected = turnValue.
 *
 *	@params
 *		int turnExpected
 *		int turnValue
 *
 *	@outputs
 *		prints test failure report
 */
int assertTrue(int expectedCount, int handCount)
{
#ifdef NOISY_TEST
	printf("(expectedCount, handCount) = (%d, %d)\n", expectedCount, handCount);
#endif

	if (expectedCount != handCount) {
		printf("Expected %d, but got %d instead.\n", expectedCount, handCount);
		return -1;
	} else {
		return 0;
	}
}




int main()
{
	int i, j;
	int seed = 					1000;		/* for random number generator */
	int k[10] = {adventurer, council_room, feast, gardens, mine
								, remodel, smithy, village, baron, great_hall};
	struct gameState G;
	int res = 0;							/* for test results */
	int hCount;


	/* setup turn configurations for test cases. */
	struct turnConfig tConf[] = { {0,2}, {1,2}, {0,3}, {2,3}, 
							{0, MAX_PLAYERS}, {MAX_PLAYERS-1, MAX_PLAYERS} };

	/* setup hand size configurations for test cases. */
	int handSize[] = { 0, 1, 2, 3, 4, 5, MAX_HAND };


	/* initiate tests... */
	printf ("TESTING numHandCards():\n");


	/* 
	 * test numHandCards() for each configuration in tConf and for each
	 * hand count in handSize.
	 */
	for (i=0; i<ARR_LEN(tConf); i++) {
		/*
		 * intialize the game state to reset its values.  Set the number
		 * which corresponds to who's turn it is and how many players 
		 * for the ith test configuration.
		 */
		memset(&G, 0, sizeof(struct gameState));
		initializeGame(tConf[i].numPlayers, k, seed, &G);
		G.whoseTurn = tConf[i].whoseTurn;

		/* run the test for all hand sizes. */
		for (j=0; j<ARR_LEN(handSize); j++) {
			G.handCount[tConf[i].whoseTurn] = handSize[j];

			/* run numHandCards() test-configured game state. */
			hCount = numHandCards(&G);
		
			/* check that the return value is correct. */
			if (assertTrue(G.handCount[G.whoseTurn], hCount) < 0) { res = -1; }	
		}
	}


	if (res == 0) { printf("All tests passed!\n"); }

	return 0;
}
