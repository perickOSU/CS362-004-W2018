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
			 * create completely random game state.  Assigns each byte of G
			 * to a random value.  This fails pre conditions for game state...
			 */
			((char*)&G)[i] = floor(Random() * 256);
		}
		/*
		 * set player number to random value in [0..999].  This fails pre conditions
		 * for number of players...
		 */
		p = floor(Random() * 1000);
		
		/* call the draw card. */
		checkDrawCard(p, &G);
	}

	printf ("ALL TESTS OK\n");

	exit(0);
}
