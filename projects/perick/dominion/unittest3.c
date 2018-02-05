/*
 * unittest3.c
 *
 * 	@author
 * 		Kruno Peric
 *
 *	@desc
 *		Tests the whoseTurn() function.
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
int assertTrue(int turnExpected, int turnValue)
{
#ifdef NOISY_TEST
	printf("(turnExpected, turnValue) = (%d, %d)\n", turnExpected, turnValue);
#endif

	if (turnExpected != turnValue) {
		printf("Expected %d, but got %d instead.\n", turnExpected, turnValue);
		return -1;
	} else {
		return 0;
	}
}




int main()
{
	int i;
	int seed = 					1000;		/* for random number generator */
	int k[10] = {adventurer, council_room, feast, gardens, mine
								, remodel, smithy, village, baron, great_hall};
	struct gameState G;
	int res = 0;							/* for test results */
	int turnValue;


	/* setup turn configurations for test cases. */
	struct turnConfig tConf[] = { {0,2}, {1,2}, {0,3}, {2,3}, 
							{0, MAX_PLAYERS}, {MAX_PLAYERS-1, MAX_PLAYERS} };


	/* initiate tests... */
	printf ("TESTING whoseTurn():\n");


	/* test whoseTurn() for each configuration in tConf. */
	for (i=0; i<ARR_LEN(tConf); i++) {
		/*
		 * intialize the game state to reset its values.  Set the number
		 * which corresponds to who's turn it is and how many players 
		 * for the ith test configuration.
		 */
		memset(&G, 0, sizeof(struct gameState));
		initializeGame(tConf[i].numPlayers, k, seed, &G);
		G.whoseTurn = tConf[i].whoseTurn;


		/* run whoseTurn() test-configured game state. */
		turnValue = whoseTurn(&G);

		
		/* check that the return value is correct. */
		if (assertTrue(G.whoseTurn, turnValue) < 0) { res = -1; }	
	}


	if (res == 0) { printf("All tests passed!\n"); }

	return 0;
}
