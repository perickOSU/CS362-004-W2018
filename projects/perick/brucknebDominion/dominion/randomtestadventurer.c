/*
 * @Author:
 *
 * 	Kruno Peric
 *
 * @desc
 * 		
 *	Adventurer effect tester
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <stdio.h>
#include <string.h>
#include "rngs.h"
#include <stdbool.h>
#include <math.h>

#define NUM_TESTS	5000
#define NUM_CARD_T	26

//#define NOISY
#define BUG printf("-----  ERROR -----\n"); isError = true;


/*
 * @pre
 * 	int i, j must be in scope.
 *
 * @post
 * 	i and j will be written to.  number of <type> cards in <pile> will be
 * 	counted.
 */
#define countTreasure(pile, type) \
{ \
	for (i=0; i<G->pile ## Count[G->whoseTurn]; i++) { \
		if (G->pile[G->whoseTurn][i] == type) { counts->pile.type++; } \
	} \
}

/*
 * @pre
 * 	counts array
 *
 * @post
 * 	none
 */
#define pileSum(counts, pile) \
	(\
	 counts.pile.copper + counts.pile.silver + counts.pile.gold \
	 )


/*
 * @pre
 * 	pre and post counts arrays
 *
 * @post
 * 	none
 */
#define diffTreasure(pile, type)\
	(\
	 countsPre.pile.type - countsPost.pile.type \
	 )


/*
 * @pre
 * 	populated struct treasureCounts with numTreasure()
 *
 * @post
 * 	none
 */
#define treasurePrinter(pile, pileStr)\
{\
	printf(pileStr "\t (copper, silver, gold) : (%3.d, \t%3.d, \t%3.d)\n", \
			counts->pile.copper, counts->pile.silver, counts->pile.gold); \
}




enum PILE { deck, hand, discard };

struct treasureCounts {
	int copper;
	int silver;
	int gold;
};
void initTreasureCounts(struct treasureCounts *c)
{
	c->copper = 0;
	c->silver = 0;
	c->gold = 0;
}

struct pileTreasureCounts {
	struct treasureCounts deck;
	struct treasureCounts hand;
	struct treasureCounts discard;
};
void initPileTreasureCounts(struct pileTreasureCounts *C)
{
	initTreasureCounts(&(C->deck));
	initTreasureCounts(&(C->hand));
	initTreasureCounts(&(C->discard));
}



void testOracle(struct gameState* post);
int totalTreasure(struct pileTreasureCounts *counts);
void numTreasure(struct gameState *G, struct pileTreasureCounts *counts);
void treasureReport(struct pileTreasureCounts *counts);



int main()
{
	int i, j, t;
	struct gameState G;	

	SelectStream(2);
	PutSeed(10);

	/*
	 * Intialize and run NUM_TESTS random tests on adventurerEffect().
	 */
	printf("-------RUNNING ADVENTURER CARD RANDOM TEST-------\n\n");
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


		/* run test and oracle */
		testOracle(&G);
	}

	printf("------ADVENTURER TEST COMPLETE.------\n");

	return 0;
}



/*
 * testOracle()
 *
 * 	@desc
 *
 * 		Checks to see that the adventurerEffect() method behaves according
 * 		to how the adventurer card is supposed to work.
 */
void testOracle(struct gameState* post)
{
	bool isError = false;
	struct gameState pre;
	struct pileTreasureCounts countsPre, countsPost;
	int totalsPre, totalsPost;
	int p = post->whoseTurn;
	int i;

	/* make a copy of the game state to compare. */
	memcpy(&pre, post, sizeof pre);

	/*
	 * gather pre data for oracle.
	 */

	/* check how many gold, copper, silver are on the deck. */
	numTreasure(&pre, &countsPre);
	totalsPre = totalTreasure(&countsPre);

	/* prevent segmentation fault. */
	if (totalsPre == 0) { return; }




	/* run the target function. */
	cardEffect(adventurer,0,0,0, post, 0, 0);


	/*
	 * gather post data for oracle
	 */
	numTreasure(post, &countsPost);
	totalTreasure(&countsPost);
	totalsPost = totalTreasure(&countsPost);


#ifdef NOISY
	printf("pre gameState treasure info:\n");
	printf("deckCount: %d\n", pre.deckCount[pre.whoseTurn]);
	printf("handCount: %d\n", pre.handCount[pre.whoseTurn]);
	printf("discardCount: %d\n", pre.discardCount[pre.whoseTurn]);

	treasureReport(&countsPre);
	printf("total for player: %d\n", totalsPre);

	printf("post gameState treasure info:\n");
	treasureReport(&countsPost);
	printf("total for player: %d\n", totalsPost);
#endif

	int treasureAvailable = pileSum(countsPre, deck) +
		pileSum(countsPre, discard);

	int maxTreasure = (treasureAvailable > 1) ? 2 : treasureAvailable;	

	/*
	 * case in which the deck has the max available treasure.
	 */
	if (pileSum(countsPre, deck) >= maxTreasure) {
		/* 
		 * difference in treasure counts in the deck from pre to post
		 * should be negative of the difference in the hand from pre to post.
		 */
		int cDiffDeck = diffTreasure(deck, copper);
		int sDiffDeck = diffTreasure(deck, silver);
		int gDiffDeck = diffTreasure(deck, gold);

		int cDiffHand = diffTreasure(hand, copper);
		int sDiffHand = diffTreasure(hand, silver);
		int gDiffHand = diffTreasure(hand, gold);

		if ( 
				cDiffHand + cDiffDeck || 
				sDiffHand + sDiffDeck || 
				gDiffHand + gDiffDeck
		   ) {
			BUG
				printf("treasure card difference mismatch\nDiffernces:  Deck vs Hand\n");
			printf("copper diff: %d,%d\n", cDiffDeck, cDiffHand);
			printf("silver diff: %d,%d\n", sDiffDeck, sDiffHand);
			printf("gold diff: %d,%d\n", gDiffDeck, gDiffHand);
		}

		/* discard treasure should be unchanged. */
		int cDiffDiscard = diffTreasure(discard, copper);
		int sDiffDiscard = diffTreasure(discard, silver);
		int gDiffDiscard = diffTreasure(discard, gold);

		if ( cDiffDiscard || sDiffDiscard || gDiffDiscard ) {
			BUG
				printf("discard pile treasure cards were altered.\n");
			printf("copper  discard diff: %d\n", cDiffDiscard);
			printf("silver  discard diff: %d\n", sDiffDiscard);
			printf("gold  discard diff: %d\n", gDiffDiscard);
		}

		if (maxTreasure == 2) {
			/* 
			 * discardCount should be increased by the difference of deckCount
			 * minus the two treasure cards.
			 */
			if (
					pre.deckCount[p] - post->deckCount[p] - maxTreasure !=
					post->discardCount[p] - pre.discardCount[p]
			   ) {
				BUG
					printf("Incorrect discard count.\n");
				printf("pre deck count: %d\n", pre.deckCount[p]);
				printf("post deck count: %d\n", post->deckCount[p]);
				printf("pre discard count: %d\n", pre.discardCount[p]);
				printf("post discard count: %d\n", post->discardCount[p]);
			}
		}
	} else {
		/*
		 * the deck doesn't have all of maxTreasure, so shuffling will
		 * definitely by done.  check that the correct number of cards
		 * were dran from the deck/discard piles.  Check sum each treasure
		 * type and check sum the whole game state to make sure other
		 * player's weren't effected...
		 */

		/* get number of each treasure total in deck and discard piles */
		int cDeckDiscardPre = countsPre.deck.copper + countsPre.discard.copper;
		int sDeckDiscardPre = countsPre.deck.silver + countsPre.discard.silver;
		int gDeckDiscardPre = countsPre.deck.gold + countsPre.discard.gold;

		int cDeckDiscardPost = countsPost.deck.copper + countsPost.discard.copper;
		int sDeckDiscardPost = countsPost.deck.silver + countsPost.discard.silver;
		int gDeckDiscardPost = countsPost.deck.gold + countsPost.discard.gold;

		/* get difference of treasure in hand. */
		int cDiffHand = diffTreasure(hand, copper);
		int sDiffHand = diffTreasure(hand, silver);
		int gDiffHand = diffTreasure(hand, gold);

		/* check sum the overall treasure: */
		int pilesDiffTotal = 
			pileSum(countsPre, deck) + 	pileSum(countsPre, discard)
			-	pileSum(countsPost, deck) -	pileSum(countsPost, discard);

		int handDiffTotal = pileSum(countsPost, hand) - pileSum(countsPre, hand);

		if (pilesDiffTotal -  maxTreasure || handDiffTotal - maxTreasure) {
			BUG
				printf("wrong number of treasure cards drawn.\n");
		}

		/* check sum on each treasure type: */
		if (
				cDiffHand + cDeckDiscardPre - cDeckDiscardPost ||
				sDiffHand + sDeckDiscardPre - sDeckDiscardPost ||
				gDiffHand + gDeckDiscardPre - gDeckDiscardPost
		   ) {
			BUG
				printf("Bug in post shuffle piles check sum.\n");
		}
	}
	/*
	 * Game State check sum:
	 *
	 * 	the other player's should be uneffected.  
	 * 	Copy the deck and discard  values over to pre, and compare the 
	 * 	game states.
	 */
	memcpy(pre.deck[p], post->deck[p], sizeof(post->deck[p]));	
	memcpy(pre.discard[p], post->discard[p], sizeof(post->discard[p]));	

	pre.deckCount[p] = post->deckCount[p];
	pre.discardCount[p] = post->discardCount[p];


	/*
	 * 	additionally, the hand should be un-altered, other than adding
	 * 	the two new cards to the end.
	 */
	pre.handCount[p] = post->handCount[p];

	for (i=1; i<=maxTreasure; i++) {
		pre.hand[p][pre.handCount[p] -i] = post->hand[p][post->handCount[p]-i];
	}

	/*
	 * check the hands seperately, since shuffling effects the data outside
	 * the range of [0..handCount]...
	 */
	if (memcmp(pre.hand[p], post->hand[p], (post->handCount[p]) * sizeof(int)) != 0) {
		BUG
			printf("hand memory misatch.\n");
	}

	/* 	Zero out the hands, and then perform the checksum. */
	memset(pre.hand[p], 0, MAX_HAND * sizeof(int));
	memset(post->hand[p], 0, MAX_HAND * sizeof(int));

	if (memcmp(&pre, post, sizeof(struct gameState)) != 0) {
		BUG
			printf("checksum error.\n");
	}

	if (isError == true) {
		printf("==========COUNTS REPORT===========\n");
		printf("maxTreasure = %d\n", maxTreasure);
		printf("deck pile count sum: %d\n", pileSum(countsPre, deck));
		printf("pre counts:\n");
		treasureReport(&countsPre);
		printf("post counts:\n");
		treasureReport(&countsPost);
		printf("=============END ERRORS==================\n\n\n\n");
	}
}


/*
 * numTreasure()
 *
 * 	@desc
 * 		
 * 		counts how many <type> treasure cards are in <pile>.
 *
 * 	@params
 *
 * 		struct gameState			*G
 * 		struct pileTreasureCounts	**counts
 *
 * 	@returns
 * 		
 * 		count of how many <type> cards.
 */
void numTreasure(struct gameState *G, struct pileTreasureCounts *counts)
{
	int i;
	initPileTreasureCounts(counts);

	countTreasure(deck, 	copper);
	countTreasure(hand, 	copper);
	countTreasure(discard, 	copper);
	countTreasure(deck, 	silver);
	countTreasure(hand, 	silver);
	countTreasure(discard, 	silver);
	countTreasure(deck, 	gold);
	countTreasure(hand, 	gold);
	countTreasure(discard, 	gold);
}

int totalTreasure(struct pileTreasureCounts *counts)
{
	return 
		counts->deck.copper + counts->deck.silver + counts->deck.gold +
		counts->hand.copper + counts->hand.silver + counts->hand.gold +
		counts->discard.copper + counts->discard.silver + counts->discard.gold;
}


/*
 * treasureReport()
 *
 * 	@prints treasure diff pre vs post state
 */
void treasureReport(struct pileTreasureCounts *counts)
{
	treasurePrinter(deck,		"deck")
		treasurePrinter(hand,		"hand")
		treasurePrinter(discard,	"discard")
}
