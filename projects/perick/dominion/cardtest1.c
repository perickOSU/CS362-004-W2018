/*
 * unittest1.c
 *
 * 	@author
 * 		Kruno Peric
 *
 *	@desc
 *		Tests the smithy card.
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
	int numPlayers;
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
	int preSmithyNum = 0;
	int postSmithyNum = 0;
	int i;
	int res = 0;
	int preDeckCount;
	int preDiscardCount;


	/*
	 * grab who's turn it was, and assert that this did not change.
	 * This would be HIGHLY unlikely to change here.
	 */
	int whoseTurn = preState->whoseTurn;
	assert(whoseTurn == postState->whoseTurn);


	/* check if two cards were added to the current player's hand */
	if (postState->handCount[whoseTurn] != preState->handCount[whoseTurn] + 2) {
		printf("Incorrect hand count!\n");
		printf("postState.handCount = %i\n", postState->handCount[whoseTurn]);
		res = -1;
	}


	/* check that there is one less smithy card in post state */
	for (i=0; i<preState->handCount[whoseTurn]; i++) {
		if (preState->hand[whoseTurn][i] == smithy) { preSmithyNum++; }
	}

	for (i=0; i<postState->handCount[whoseTurn]; i++) {
		if (postState->hand[whoseTurn][i] == smithy) { postSmithyNum++; }
	}

	if (postSmithyNum != preSmithyNum - 1) {
		printf("discard error!\n");
		printf("number of smithy in hand [PRE] = %i\n", preSmithyNum);
		printf("number of smithy in hand [POST] = %i\n", postSmithyNum);
		res = -1;
	}


	/* check the deck and discard counts for the player */
	preDeckCount = preState->deckCount[whoseTurn];
	preDiscardCount = preState->discardCount[whoseTurn];
	if (preDeckCount > 2) {
		/*
		 * if the deck has at least three cards, re-shuffling
		 * the discard pile won't be necessary and the deck
		 * should just have three less cards than before the call
		 * to cardEffect.
		 */
		if (postState->deckCount[whoseTurn] != preDeckCount - 3) {
			printf("player deckCount mismatch!\n");
			printf("postState.deckCount = %i\n", postState->deckCount[whoseTurn]);
			res = -1;
		}
		if (postState->discardCount[whoseTurn] != preDiscardCount + 1) {
			/*
			 * the discard pile should be incrased by one, since the
			 * smithy card gets discarded after it's used.
			 */
			printf("Discard pile did not incease by 1 card!\n");
			printf("postState.discardCount = %i\n", postState->discardCount[whoseTurn]);
			res = -1;
		}

	} else {
		/*
		 * here the deck will have to be replenished with the shuffled
		 * discard pile.  The discard pile should then only contain the
		 * smithy card.
		 */
		if (postState->deckCount[whoseTurn] != preDiscardCount - (3-preDeckCount)) {
			printf("player deckCount mismatch after discard pile shuffle!\n");
			printf("pre state discard count: %d\n", preDiscardCount);
			printf("post state deck count: %d\n", postState->deckCount[whoseTurn]);
			res = -1;
		}
		/* the discard count should have just the smithy card. */
		if (postState->discardCount[whoseTurn] != 1) {
			printf("player discardCount doesn't contain one card after a discard pile shuffle/transfer!\n");
		}
	}


	if (res < 0) { return -1; } else { return 0; }
}




int main()
{
	int i;
	int seed = 					1000;		/* for random number generator */
	int k[10] = {adventurer, council_room, feast, gardens, mine
								, remodel, smithy, village, baron, great_hall};
	struct gameState preState, postState;
	int res = 0;							/* for test results */
	const int handPos = 2;


	/* setup player configurations for test cases. */
	struct testConf Conf[] = { {2}, {3}, {MAX_PLAYERS} };

	
	/* initiate tests... */
	printf ("TESTING smithy card:\n");

	/* 
	 * test cardEffect() for smithy with each configuration in Conf.
	 */
	for (i=0; i<ARR_LEN(Conf); i++) {
		/*
		 * configure the pre state of the game and copy to the post state.
		 */
		memset(&preState, 0, sizeof(struct gameState));
		memset(&postState, 0, sizeof(struct gameState));
		initializeGame(Conf[i].numPlayers, k, seed, &preState);

		/* replace one of the places initial cards with a smithy hard */
		preState.hand[preState.whoseTurn][handPos] = smithy;

		/* save the state. */
		memcpy(&postState, &preState, sizeof(struct gameState));


		/* run card effect on the configured state. */
		cardEffect(smithy, 0, 0, 0, &postState, handPos, 0);

	
		/* check if the post state was altered correctly */
		if (assertTrue(&preState, &postState) < 0) {
			printf("failed test case was:\n\n");
			printf("numPlayers = %i\n", i);
			printf("handPos = %i\n", handPos);
			printf("deckCount = %i\n", preState.deckCount[preState.whoseTurn]);
			printf("handCount = %i\n", preState.handCount[preState.whoseTurn]);
			printf("discardCount = %i\n", preState.discardCount[preState.whoseTurn]);
			printf("-----END CASE----\n\n\n");
			res = -1;
		}
	}

	for (i=0; i<3; i++) {
		/*
		 * test case of empty deck....
		 */
		memset(&preState, 0, sizeof(struct gameState));
		memset(&postState, 0, sizeof(struct gameState));
		initializeGame(2, k, seed, &preState);

		/* replace one of the places initial cards with a smithy hard */
		preState.hand[preState.whoseTurn][handPos] = smithy;

		/* empty the deck */
		preState.deckCount[preState.whoseTurn] = i;
		preState.discardCount[preState.whoseTurn] = 10;

		/* save the state. */
		memcpy(&postState, &preState, sizeof(struct gameState));

		/* run card effect on the configured state. */
		cardEffect(smithy, 0, 0, 0, &postState, handPos, 0);

		/* check if the post state was altered correctly */
		if (assertTrue(&preState, &postState) < 0) {
			printf("\nfailed test case was:\n\n");
			printf("numPlayers = %i\n", i);
			printf("handPos = %i\n", handPos);
			printf("deckCount = %i\n", preState.deckCount[preState.whoseTurn]);
			printf("handCount = %i\n", preState.handCount[preState.whoseTurn]);
			printf("discardCount = %i\n", preState.discardCount[preState.whoseTurn]);
			printf("-----END CASE----\n\n\n");
			res = -1;
		}


	}
	
	if (res == 0) { printf("All tests passed!\n"); }


	return 0;
}
