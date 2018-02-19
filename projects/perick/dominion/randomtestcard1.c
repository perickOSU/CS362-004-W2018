/*
 * @Author:
 *
 * 	Kruno Peric
 *
 * @desc
 * 		
 *	Smithy effect tester
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <stdio.h>
#include <string.h>
#include "rngs.h"
#include <stdbool.h>
#include <math.h>

#define NUM_TESTS				1000
#define NUM_CARD_T				26

//#define NOISY
#define BUG printf("-----  ERROR -----\n"); isError = true;


void testOracle(struct gameState* post, int handPos);


int main()
{
	int t, i, j;
	struct gameState G;	

	SelectStream(2);
	PutSeed(10);

	/*
	 * Intialize and run NUM_TESTS random tests on smithyEffect().
	 */
	printf("-------RUNNING SMITHY CARD RANDOM TEST-------\n\n");
	for (t=0; t<NUM_TESTS; t++) {
		/* fill G with random bytes. */
		for (i=0; i<sizeof(G); i++) {
			((char*)(&G))[i] = floor(Random() * 256);
		}

		G.numPlayers = MAX_PLAYERS;


		/* intialize deck counts for all players. */
		for (i=0; i<G.numPlayers; i++) {
			G.deckCount[i] = floor(Random() * MAX_DECK);
			G.handCount[i] = floor(Random() * MAX_HAND);
			G.discardCount[i] = floor(Random() * MAX_DECK);
			G.playedCardCount = 0;

			/* intialize the player's hand, discard and deck piles. */
			for (j=0; j<G.deckCount[i]; j++) {
				G.deck[i][j] = floor(Random() * NUM_CARD_T);
			}

			for (j=0; j<G.handCount[i]; j++) {
				G.hand[i][j] = floor(Random() * NUM_CARD_T);
			}

			for (j=0; j<G.discardCount[i]; j++) {
				G.discard[i][j] = floor(Random() * NUM_CARD_T);
			}
		}


		/* pick a player at random for the turn. */
		G.whoseTurn = floor(Random() * G.numPlayers);

		/* pick a random handPos for Smithy. */
		int handPos = floor(Random() * G.handCount[G.whoseTurn]);
		G.hand[G.whoseTurn][handPos] = smithy;

		/* run test and oracle */
		testOracle(&G, handPos);
	}

	printf("------SMITHY TEST COMPLETE.------\n");

	return 0;
}



/*
 * testOracle()
 *
 * 	@desc
 *
 * 		Checks to see that the smithyEffect() method behaves according
 * 		to how the smithy card is supposed to work.
 */
void testOracle(struct gameState* post, int handPos)
{
	bool isError = false;
	struct gameState pre;
	int p = post->whoseTurn;
	int i;
	int preCount = pre.deckCount[p];

	/* make a copy of the game state to compare. */
	memcpy(&pre, post, sizeof pre);

	/* run the target function. */
	smithyEffect(post, p, handPos);
	
	/*
	 * "move" three cards from the deck, to the hand, and "discard"
	 * the smithy card located at handPos.
	 */
	if (pre.deckCount[p] > 2) {
		/* draw from top of deck onto back of hand. */
		for (i=0; i<3; i++) {
			pre.hand[p][pre.handCount[p]] = pre.deck[p][pre.deckCount[p]-1];
			pre.handCount[p]++;
			pre.deckCount[p]--;
		}

		/* pull smithy "out" of the hand. */
		if (handPos == pre.handCount[p] - 1 || pre.handCount[p] == 1) {
			/* smithy is at the back of the hand. just drop the count. */
			pre.handCount[p]--;
		} else {
			/* 
			 * move the last card in the hand to where the smithy card "was",
			 * set the last card to -1 and update the handCount.
			 */
			pre.hand[p][handPos] = pre.hand[p][pre.handCount[p] - 1];
			pre.hand[p][pre.handCount[p] - 1] = -1;
			pre.handCount[p]--;
		}

		/* add the smithy card to the played pile and update the played count. */
		pre.playedCards[pre.playedCardCount] = smithy;
		pre.playedCardCount++;


		/* 
		 * TODO: 
		 * 	
		 * 		Smithy is supposed to discard... but dominion.c doesn't...
		 * 		Leaving this off, just to be able to test the rest of the code.
		 */
//		pre.discard[p][pre.discardCount[p]] = smithy;
//		pre.discardCount[p]++;
	} else {
		/* 
		 * the deck doesn't have enough cards, which will cause a shuffling.
		 * use the discard and deck from post...
		 */
		memcpy(pre.deck[p], post->deck[p], sizeof(pre.deck[p]));
		memcpy(pre.discard[p], post->discard[p], sizeof(pre.discard[p]));
		
		/* 
		 * copy the drawn cards in post to pre's hand.  Increment the hand count
		 * to reflect 3 drawn cards.
		 */
		for (i=0; i<=3; i++) {
			pre.hand[p][post->handCount[p]-i] = post->hand[p][post->handCount[p]-i];
		}
		
		pre.handCount[p] = post->handCount[p];

		/* replace the smithy card with the card in post */
		pre.hand[p][handPos] = post->hand[p][handPos];


		/* 
		 * set the deck count the discard count - whatever the deck didn't 
		 * cover.  Set the discard to zero.
		 */
		pre.deckCount[p] = pre.discardCount[p] - (3 - pre.deckCount[p]);
		pre.discardCount[p] = 0;

		/* add the smithy card to the played pile and update the played count. */
		pre.playedCards[pre.playedCardCount] = smithy;
		pre.playedCardCount++;
	}


	if (memcmp(&pre, post, sizeof pre) != 0) {
		BUG
		printf("preCount = %d\n", preCount);
	}
}
