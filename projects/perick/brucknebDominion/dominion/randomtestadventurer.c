// TESTING: adventurer
//
// injected bug: 
// In the section of cardEffect_Adventurer where all cards in play that have
// been drawn are discarded, I have modified the boundary check on the while
// statement from z-1>0 to z>0. This will affect the outcome of the game and 
// is difficult to detect without careful review of the gameplay.

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "randomtestdriver.h"
#include "dominion.h"
#include "dominion_helpers.h"

void testCard_adventurer(struct gameState *gs_pre, struct gameState *gs_post) {
	int choice1, choice2, choice3, handPos, bonus;
	
	// choice should not matter for adventurer; random values
	choice1 = rand_int(curse, treasure_map);
	choice2 = rand_int(curse, treasure_map);
	choice3 = rand_int(curse, treasure_map);
	
	// handPos should not matter for adventurer; random value
	handPos = rand_int(0, gs_pre->handCount[gs_pre->whoseTurn]);
	// put adventurer at handPos for next play
	gs_pre->hand[gs_pre->whoseTurn][handPos] = adventurer;
	if (DEBUG)
		printf("rta.1\n");
	
	// copy pre to post gameState
	memcpy(gs_post, gs_pre, sizeof(struct gameState));
	if (DEBUG)
		printf("rta.2\n");
	
	// now play the card
	cardEffect(adventurer, choice1, choice2, choice3, gs_post, handPos, &bonus);
	if (DEBUG)
		printf("rta.3\n");
	
	return;
}

void validate_adventurer(struct gameState *gs_pre, struct gameState *gs_post, struct validations *v) {
	/* compare gs_pre and gs_post, looking for the following */
	/*********************************************************/
	/* whose turn shouldn't change */
	if (gs_pre->whoseTurn != gs_post->whoseTurn)
		v->errcount_whoseTurn++;
	
	/* gs_post->discard equal to pre (cardEffect doesn't actually discard) */
	if (gs_post->discardCount[gs_pre->whoseTurn] != gs_pre->discardCount[gs_pre->whoseTurn]) {
		v->errcount_playerDiscardCount++;
		if (DEBUG) {
			printf("gs_post->discardCount=%d, gs_pre->discardCount=%d\n", gs_post->discardCount[gs_pre->whoseTurn], gs_pre->discardCount[gs_pre->whoseTurn]);
			// exit(1);
		}
	}
	
	/* gs_post->hand increased by 2 (treasure cards*/
	if (gs_post->handCount[gs_pre->whoseTurn] != gs_pre->handCount[gs_pre->whoseTurn] + 2) {
		v->errcount_playerHandCount++;
		if (DEBUG) {
			printf("gs_post->handCount=%d, gs_pre->handCount=%d\n", gs_post->handCount[gs_pre->whoseTurn], gs_pre->handCount[gs_pre->whoseTurn]);
			// exit(1);
		}
	}
	
	/* gs_post->hand should have 2 additional treasure cards */
	/* NB: use pre->whoseTurn for indexing into both pre and post */
	int i, c, pre_treasureCount = 0, post_treasureCount = 0;
	for (i = 0; i < gs_pre->handCount[gs_pre->whoseTurn]; i++) {
		c = gs_pre->hand[gs_pre->whoseTurn][i];
		if (c == copper || c == silver || c == gold)
			pre_treasureCount++;
	}
	for (i = 0; i < gs_post->handCount[gs_pre->whoseTurn]; i++) {
		c = gs_post->hand[gs_pre->whoseTurn][i];
		if (c == copper || c == silver || c == gold)
			post_treasureCount++;
	}
	if (post_treasureCount != pre_treasureCount + 2)
		v->errcount_adventurer_treasure++;
	
	/* gs_post->deck reduction = gs_post->discard increment + 1 */
	/* NB: use pre->whoseTurn for indexing into both pre and post */
	int preDeckCount = gs_pre->deckCount[gs_pre->whoseTurn];
	int postDeckCount = gs_post->deckCount[gs_pre->whoseTurn];
	int preDiscardCount = gs_pre->discardCount[gs_pre->whoseTurn];
	int postDiscardCount = gs_post->discardCount[gs_pre->whoseTurn];
	if ((preDeckCount - postDeckCount) != (postDiscardCount - preDiscardCount + 1)) {
		v->errcount_adventurer_deckhanddiscard++;
		// printf("preDeckCount=%d, postDeckCount=%d, preDiscardCount=%d, postDiscardCount=%d\n", preDeckCount, postDeckCount, preDiscardCount, postDiscardCount);
		// exit(1);
	}
	
	/* additional cards in gs_post->discard should not be treasure */
	for (i = preDiscardCount-1; i < postDiscardCount; i++) {
		c = gs_post->discard[gs_pre->whoseTurn][i];
		if (c == copper || c == silver || c == gold)
			v->errcount_adventurer_treasureindiscard++;
	}
	/* playedCardCount incremented by 1 */
	if (gs_post->playedCardCount != gs_pre->playedCardCount) {
		if (DEBUG)
			printf("gs_post->playedCardCount=%d, gs_pre->playedCardCount=%d\n", gs_post->playedCardCount, gs_pre->playedCardCount);
		v->errcount_playedCards++;
		exit(1);
	}
	
	/* opponents' cards unchanged */
	for (i = 0; i < gs_pre->numPlayers; i++) {
		if (i != gs_pre->whoseTurn) {
			if (gs_pre->deckCount[i] != gs_post->deckCount[i])
				v->errcount_opponentDeckCount++;
			if (gs_pre->handCount[i] != gs_post->handCount[i])
				v->errcount_opponentHandCount++;
			if (gs_pre->discardCount[i] != gs_post->discardCount[i])
				v->errcount_opponentDiscardCount++;
		}
	}
	
	/* TO DO: what if we have to shuffle discard back into deck? */
	
}