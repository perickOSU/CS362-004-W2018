#include "dominion.h"
#include "dominion_helpers.h"
#include <stdio.h>
#include <string.h>
#include "rngs.h"
#include <math.h>

#define NUM_TESTS	1
#define NUM_CARD_T	26

#define NOISY


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
	struct gameState pre;
	struct pileTreasureCounts countsPre, countsPost;
	int totalsPre, totalsPost;

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
	adventurerEffect(post, post->whoseTurn);


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

	/*
	 * case in which the deck has more than 1 treasure card:
	 */
	if (pileSum(countsPre, deck) > 1) {
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

		printf("copper diff: %d,%d\n", cDiffDeck, cDiffHand);
		printf("silver diff: %d,%d\n", sDiffDeck, sDiffHand);
		printf("gold diff: %d,%d\n", gDiffDeck, gDiffHand);

		/* discard treasure should be unchanged. */

		/* 
		 * discardCount should be increased by the difference of deckCount
		 * minus the two treasure cards.
		 */

		/* 
		 * the other player's should be uneffected.  Copy the piles for and
		 * count values over to pre, and compare the game states.
		 */
	} else {
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
