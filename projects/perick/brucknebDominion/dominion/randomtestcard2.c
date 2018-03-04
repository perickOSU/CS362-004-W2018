// TESTING: council_room
//
// INJECTED BUG:
// In the section of cardEffect_CouncilRoom where each other player draws a 
// card, I have commented out the logic that restricts the drawing to other
// players ( if ( i != currentPlayer) ) so that the current player also draws a
// card, in violation of the rules, which will affect the outcome of the game,
// and which will be difficult to detect without careful review of the gameplay.

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "randomtestdriver.h"
#include "dominion.h"
#include "dominion_helpers.h"

void testCard_council_room(struct gameState *gs_pre, struct gameState *gs_post) {
	int choice1, choice2, choice3, handPos, bonus;
	
	// choice should not matter for council_room; random values
	choice1 = rand_int(curse, treasure_map);
	choice2 = rand_int(curse, treasure_map);
	choice3 = rand_int(curse, treasure_map);
	
	// handPos should not matter for council_room; random value
	handPos = rand_int(0, gs_pre->handCount[gs_pre->whoseTurn]);
	// put council_room at handPos for next play
	gs_pre->hand[gs_pre->whoseTurn][handPos] = council_room;
	if (DEBUG)
		printf("rts.1\n");
	// dumpGameState(gs_pre);
	// copy pre to post gameState
	memcpy(gs_post, gs_pre, sizeof(struct gameState));
	if (DEBUG)
		printf("rts.2\n");
	
	// now play the card
	cardEffect(council_room, choice1, choice2, choice3, gs_post, handPos, &bonus);
	if (DEBUG)
		printf("rts.3\n");
	
	return;
}

void validate_council_room(struct gameState *gs_pre, struct gameState *gs_post, struct validations *v) {
	int i;
	/* compare gs_pre and gs_post, looking for the following */
	/*********************************************************/
	/* whoseTurn shouldn't change */
	if (gs_pre->whoseTurn != gs_post->whoseTurn)
		v->errcount_whoseTurn++;
	
	/* gs_post->hand increased by 4*/
	/* additional cards */
	if (gs_post->handCount[gs_pre->whoseTurn] != gs_pre->handCount[gs_pre->whoseTurn] + 4) {
		v->errcount_playerHandCount++;
		if (DEBUG) {
			printf("gs_post->handCount=%d, gs_pre->handCount=%d\n", gs_post->handCount[gs_pre->whoseTurn], gs_pre->handCount[gs_pre->whoseTurn]);
			// exit(1);
		}
	}
	
	/* gs_post->deck reduction by 4 cards */
	if (gs_post->deckCount[gs_pre->whoseTurn] != gs_pre->deckCount[gs_pre->whoseTurn] - 4) {
		v->errcount_playerDeckCount++;
		if (DEBUG) {
			printf("gs_post->deckCount=%d, gs_pre->deckCount=%d\n", gs_post->deckCount[gs_pre->whoseTurn], gs_pre->deckCount[gs_pre->whoseTurn]);
			// exit(1);
		}
	}
	
	/* gs_post->discard equal to pre (cardEffect doesn't actually discard) */
	if (gs_post->discardCount[gs_pre->whoseTurn] != gs_pre->discardCount[gs_pre->whoseTurn]) {
		v->errcount_playerDiscardCount++;
		if (DEBUG) {
			printf("gs_post->discardCount=%d, gs_pre->discardCount=%d\n", gs_post->discardCount[gs_pre->whoseTurn], gs_pre->discardCount[gs_pre->whoseTurn]);
			// exit(1);
		}
	}
	
	/* playedCardCount incremented by 1 */
	if (gs_post->playedCardCount != gs_pre->playedCardCount + 1) {
		if (DEBUG) {
			printf("gs_post->playedCardCount=%d, gs_pre->playedCardCount=%d\n", gs_post->playedCardCount, gs_pre->playedCardCount);
			// exit(1);
		}
		v->errcount_playedCards++;
		
	}
	
	/* buy increased by 1 */
	if (gs_post->numBuys != gs_pre->numBuys + 1)
		v->errcount_numBuys++;
	
	/* opponents draw a card*/
	for (i = 0; i < gs_pre->numPlayers; i++) {
		if (i != gs_pre->whoseTurn) {
			if ( gs_post->deckCount[i] != gs_pre->deckCount[i] - 1)
				v->errcount_opponentDeckCount++;
			if (gs_post->handCount[i] != gs_pre->handCount[i] + 1)
				v->errcount_opponentHandCount++;
			if (gs_post->discardCount[i] != gs_pre->discardCount[i])
				v->errcount_opponentDiscardCount++;
		}
	}
	
	/* TO DO: what if we have to shuffle discard back into deck? */
	
}