/*
 * unittest2.c
 *
 * 	@author
 * 		Kruno Peric
 *
 *	@desc
 *		Tests the getCost() function.
 *
 *	@inputs
 *		none
 *
 *	@outputs
 *		test results
 */
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>


#define ARR_LEN(x) ( sizeof(x) / sizeof(x[0]) )


struct playerConfig {
	int player;
	int numPlayers;
};




/*
 * assertTrue()
 *
 * 	@desc
 *		check if the card cost is as expected.
 *
 *	@params
 *		int cardNumber
 *		int cost
 *
 *	@outputs
 *		prints test failure report
 */
int assertTrue(int cardNumber, int cost)
{
	int expectedCost[] = {0,2,5,8,0,3,6,6,5,4,4,5,4,4,3,4,3,5,3,5,3,4,2,5,4,4,4,-1};
	if (expectedCost[cardNumber] != cost) {
		printf("card number %d should cost %d but got %d instead\n",
				cardNumber, expectedCost[cardNumber], cost);
		return -1;
	} else {
		return 0;
	}
}




int main()
{
	int cardNumber;
	int res = 0;							/* for test results */
	int cost;



	/* initiate tests... */
	printf ("TESTING getCost():\n");

	for (cardNumber=0; cardNumber<28; cardNumber++) {
		cost = getCost(cardNumber);
		if (assertTrue(cardNumber, cost) < 0) { res = -1; }
	}


	if (res == 0) { printf("All tests passed!\n"); }

	return 0;
}
