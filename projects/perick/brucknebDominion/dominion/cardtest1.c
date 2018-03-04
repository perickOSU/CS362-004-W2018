/*******************************************************************************
* Student: Brian Bruckner
* Course:  CS_362_400_W2018
*
* File: cardtest1.c
* 
* This file tests the Smithy card
*******************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "dominion.h"
//#include "dominion_helpers.h"
#include "rngs.h"

int main() {
	struct gameState G;
	int k[10] = {adventurer, gardens, embargo, village, minion, mine, council_room, sea_hag, tribute, smithy};
	int retval, hc, dc, na, error;
	
	retval = initializeGame(2, k, 1, &G);
	
	assert(retval == 0);
	
	// Force card being tested into the hand of player 0
	G.hand[0][0] = village;
	hc = G.handCount[0];
	dc = G.deckCount[0];
	
	// Initialize numActions to 1 (start of turn)
	na = G.numActions = 1;
	
	// choice1, choice2, choice3 are irrelevant for village
	playCard(0, 0, 0, 0, &G);
	
	error = 0;
	
	// Smithy gives +3 cards
	
	// Assert that handCount[0] has gone down by 1
	if (G.handCount[0] != (hc - 1 + 3) ) {
		error = 1;
		printf("Error: handCount is %d but should be %d\n", G.handCount[0], (hc - 1 + 3));
	}
	
	// Assert that numActions is decreased by 1)
	if (G.numActions != (na - 1)) {
		error = 1;
		printf("Error: numActions is %d but should be %d\n", G.numActions, (na - 1));
	}
	
	// Assert that discardCount[0] = 1
	if (G.discardCount[0] != 1) {
		error = 1;
		printf("Error: discardCount is %d but should be %d\n", G.discardCount[0], (1));
	}
	
	// Assert that deckCount has reduced by 3
	if (G.deckCount[0] != (dc - 3)) {
		error = 1;
		printf("Error: deckCount is %d but should be %d\n", G.deckCount[0], (dc - 3));
	}

}
