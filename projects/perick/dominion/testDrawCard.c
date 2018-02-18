#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

#define DEBUG 0
#define NOISY_TEST 1

int checkDrawCard(int p, struct gameState *post) {
	struct gameState pre;

	/* 
	 * copy the pre came state to post.  This was we can see what the game state
	 * was prior to calling drawCard...
	 */
	memcpy (&pre, post, sizeof(struct gameState));

	int r;
	//  printf ("drawCard PRE: p %d HC %d DeC %d DiC %d\n",
	//	  p, pre.handCount[p], pre.deckCount[p], pre.discardCount[p]);

	/* run on post */
	r = drawCard (p, post);

	//printf ("drawCard POST: p %d HC %d DeC %d DiC %d\n",
	//      p, post->handCount[p], post->deckCount[p], post->discardCount[p]);

	/*
	 * much stronger test oracle...
	 *
	 * 	We are essentially going to do what drawCard should do.
	 */

	/*
	 * if the previous deck count was great than zero, then drawCard should
	 * draw from the player's deck...  We somewhat trust that drawCard handles
	 * other things like shuffling the deck, but we check that it gives the 
	 * correct results.
	 */
	if (pre.deckCount[p] > 0) {
		/* player draws a card from the deck... */
		pre.handCount[p]++;
		pre.hand[p][pre.handCount[p]-1] = pre.deck[p][pre.deckCount[p]-1];
		pre.deckCount[p]--;

		/*
		 * otherwise, if the discard count was greater than zero, draw from it...
		 */
	} else if (pre.discardCount[p] > 0) {
		/*
		 * copy post deck and discard to pre.
		 */
		memcpy(pre.deck[p], post->deck[p], sizeof(int) * pre.discardCount[p]);
		memcpy(pre.discard[p], post->discard[p], sizeof(int)*pre.discardCount[p]);

		/* set the hand count of pre */
		pre.hand[p][post->handCount[p]-1] = post->hand[p][post->handCount[p]-1];

		/* increment the hand count */
		pre.handCount[p]++;

		/* decriment the deck count */
		pre.deckCount[p] = pre.discardCount[p]-1;

		/* set the discardCount as empty */
		pre.discardCount[p] = 0;
	} /* if you deck and discount are BOTH EMPTY, 

		 drawCard does NOTHING.  we check this too...
		 */

	assert (r == 0);

	/*
	 * the pre and post should be the SAME.
	 */
	assert(memcmp(&pre, post, sizeof(struct gameState)) == 0);
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
			((char*)&G)[i] = floor(Random() * 256);
		}
		p = floor(Random() * 2);
		G.deckCount[p] = floor(Random() * MAX_DECK);
		G.discardCount[p] = floor(Random() * MAX_DECK);
		G.handCount[p] = floor(Random() * MAX_HAND);
		checkDrawCard(p, &G);
	}

	printf ("ALL TESTS OK\n");

	exit(0);


	/*
	 * fixed test to cover empty decks and empty discards...  but you can also
	 * just increase the number of tests...
	 */
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
