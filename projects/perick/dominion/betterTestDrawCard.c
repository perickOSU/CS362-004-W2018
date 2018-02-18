#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

#define DEBUG 0
#define NOISY_TEST 1

int checkDrawCard(int p, struct gameState *post) {
	int r;

	r = drawCard (p, post);
	
	/*
	 * the test oracle:
	 *
	 * 	all this does is check that drawCard returns 0...
	 *
	 * 		not a very good test oracle...  All drawCard has to do is return 0
	 * 		and it will pass the test...
	 */
	assert (r == 0);
}

int main () {

	int i, n, r, p, deckCount, discardCount, handCount;

	int k[10] = {adventurer, council_room, feast, gardens, mine,
		remodel, smithy, village, baron, great_hall};

	struct gameState G;

	printf ("Testing drawCard.\n");

	printf ("RANDOM TESTS.\n");

	SelectStream(2);
	PutSeed(3);

	for (n = 0; n < 2000; n++) {
		for (i = 0; i < sizeof(struct gameState); i++) {
			/*
			 * still making a state of random bytes...
			 */
			((char*)&G)[i] = floor(Random() * 256);
		}

		/*
		 * but now we limit to only player in [0..1], we we know we'll have a 
		 * valid player.
		p = floor(Random() * 2);

		/*
		 * limit deck count of player p
		 */
		G.deckCount[p] = floor(Random() * MAX_DECK);

		/*
		 * limit discardCount to not be breater than MAX_DECK
		 */
		G.discardCount[p] = floor(Random() * MAX_DECK);

		/*
		 * limit handCount to not be greater than MAX_HAND
		 */
		G.handCount[p] = floor(Random() * MAX_HAND);

		/* call draw card */
		checkDrawCard(p, &G);

		/*
		 * So, G is still largely random garbage.  However, the critical things
		 * that drawCard should care about are valid values.
		 *
		 * We've limited our random inputs to inputs which satisfy pre
		 * conditions of drawCard.
		 */
	}

	printf ("ALL TESTS OK\n");

	exit(0);

	printf ("SIMPLE FIXED TESTS.\n");
	for (p = 0; p < 2; p++) {
		for (deckCount = 0; deckCount < 5; deckCount++) {
			for (discardCount = 0; discardCount < 5; discardCount++) {
				for (handCount = 0; handCount < 5; handCount++) {
					memset(&G, 23, sizeof(struct gameState)); 
					r = initializeGame(2, k, 1, &G);
					G.deckCount[p] = deckCount;
					memset(G.deck[p], 0, sizeof(int) * deckCount);
					G.discardCount[p] = discardCount;
					memset(G.discard[p], 0, sizeof(int) * discardCount);
					G.handCount[p] = handCount;
					memset(G.hand[p], 0, sizeof(int) * handCount);
					checkDrawCard(p, &G);
				}
			}
		}
	}

	return 0;
}
