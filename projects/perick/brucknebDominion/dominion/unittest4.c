/*******************************************************************************
* Student: Brian Bruckner
* Course:  CS_362_400_W2018
*
* File: unittest4.c
* 
* This file tests the function 
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
	printf("g.hand[0][0] = %d, handCard = %d\n", G.hand[0][0], handCard(0, &G));

}
