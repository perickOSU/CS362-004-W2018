#ifndef RANDOMTESTDRIVER_H
#define RANDOMTESTDRIVER_H
/*
#includes
------------------------------------------------------------*/
#include "dominion.h"

/*
#defines
------------------------------------------------------------*/
#define TEST_RUNS 100
#define TEST_MIN_DECK_COUNT 3
#define TEST_MIN_DECK_COUNT 3
#define TEST_MIN_HAND_COUNT 0
#define TEST_MIN_DISCARD_COUNT 0



/*
structs
------------------------------------------------------------*/
struct validations {
	int errcount_playerDeckCount; // specific to played card
	int errcount_playerHandCount; // specific to played card
	int errcount_playerDiscardCount; // specific to played card
	int errcount_playedCards;
	int errcount_numBuys;
	int errcount_opponentDeckCount; // should not be changed for adventurer, smithy; changes for council_room
	int errcount_opponentHandCount; // should not be changed
	int errcount_opponentDiscardCount; // should not be changed 
	int errcount_whoseTurn; // post should be next player relative to pre
	int errcount_otherGamestateElements; // 
	int errcount_adventurer_treasure;
	int errcount_adventurer_treasureindiscard;
	int errcount_adventurer_deckhanddiscard;
};

/*
function prototypes
------------------------------------------------------------*/
void setupGameState(struct gameState *gs_pre);
void outputTestResults(struct validations *v);
int rand_int(int min, int max);
void testCard_adventurer(struct gameState *gs_pre, struct gameState *gs_post);
void validate_adventurer(struct gameState *gs_pre, struct gameState *gs_post, struct validations *v);
int rand_int(int min, int max);
void dumpGameState(struct gameState *gs);
#endif