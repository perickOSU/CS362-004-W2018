/*******************************************************************************
* Student: Brian Bruckner
* Course:  CS_362_400_W2018
*
* File: unittest3.c
* 
* This file tests the function numHandCard
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
	printf("handCount = %d, numHandCards = %d\n", handCount, numHandCards(&G));
	
	// add to hand
	gainCard(0, &G, 2, 0);
	
	handCount = G.handCount[0];
	printf("handCount = %d, numHandCards = %d\n", handCount, numHandCards(&G));
	
}