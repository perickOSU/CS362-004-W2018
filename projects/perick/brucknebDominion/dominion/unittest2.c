/*******************************************************************************
* Student: Brian Bruckner
* Course:  CS_362_400_W2018
*
* File: unittest2.c
* 
* This file tests the function gainCard
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
	
	retval = initializeGame(2, k, 2, &G);
	
	int handCount = G.handCount[0];
	int discCount = G.discardCount[0];
	int deckCount = G.deckCount[0];
	
	// add to discard
	gainCard(0, &G, 0, 0);
	// add to deck
	gainCard(0, &G, 1, 0);
	// add to hand
	gainCard(0, &G, 2, 0);
	
	printf("handCount = %d, expecting %d\n", G.handCount[0], handCount+1);
	printf("discCount = %d, expecting %d\n", G.discardCount[0], discCount+1);
	printf("deckCount = %d, expecting %d\n", G.deckCount[0], deckCount+1);
	
}