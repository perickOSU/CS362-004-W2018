#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "randomtestdriver.h"
#include "rngs.h"
#include "randomtestadventurer.h"
#include "randomtestcard1.h"
#include "randomtestcard2.h"

const char* str_adventurer = "adventurer";
const char* str_smithy = "smithy";
const char* str_council_room = "council_room";

int main(int argc, char *argv[]) {
	int i;
	/* pre and post gameStates */
	struct validations v;
	struct gameState gs_pre;
	struct gameState gs_post;
	long randomSeed = 123456789; // same as DEFAULT
	
	if (argc != 2) {
		printf("USAGE: %s mode\nWhere mode = ( adventurer | smithy | council_room )\n", argv[0]);
		exit(1);
	}
	
	//set up random number generator
	SelectStream(1);
	PutSeed((long)randomSeed);
	
	
	if (strcmp(str_adventurer, argv[1]) == 0) {
		// clear v
		memset(&v, '\0', sizeof(struct validations));
		printf("============================================================\n");
		printf("==========      BEGINNING OF RANDOM TESTING       ==========\n");

		// run test through TEST_RUNS for adventurer
		printf("============================================================\n");
		printf("Testing adventurer card. Running %d random tests\n", TEST_RUNS);
		for (i = 0; i < TEST_RUNS; i++) {
			// setup random gameState (minor updates in testCard function)
			setupGameState(&gs_pre);
			// call test function for post
			testCard_adventurer(&gs_pre, &gs_post);
			// call validations function to compare gs_pre and gs_post
			validate_adventurer(&gs_pre, &gs_post, &v);
		}
		outputTestResults(&v);
	}
	else if (strcmp(str_smithy, argv[1]) == 0) {
		// clear v
		memset(&v, '\0', sizeof(struct validations));
		
		// run test through TEST_RUNS for smithy
		printf("============================================================\n");
		printf("Testing smithy card. Running %d random tests\n", TEST_RUNS);
		for (i = 0; i < TEST_RUNS; i++) {
			// setup random gameState (minor updates in testCard function)
			setupGameState(&gs_pre);
			// call test function for post
			testCard_smithy(&gs_pre, &gs_post);
			// call validations function to compare gs_pre and gs_post
			validate_smithy(&gs_pre, &gs_post, &v);
		}
		// output results
		outputTestResults(&v);
	}
	else if (strcmp(str_council_room, argv[1]) == 0) {
		// clear v
		memset(&v, '\0', sizeof(struct validations));
		
		// run test through TEST_RUNS for council_room
		printf("============================================================\n");
		printf("Testing council_room card. Running %d random tests\n", TEST_RUNS);
		for (i = 0; i < TEST_RUNS; i++) {
			// setup random gameState (minor updates in testCard function)
			setupGameState(&gs_pre);
			// call test function for post
			testCard_council_room(&gs_pre, &gs_post);
			// call validations function to compare gs_pre and gs_post
			validate_council_room(&gs_pre, &gs_post, &v);
		}
		// output results
		outputTestResults(&v);
	}
	else {
		printf("USAGE: %s mode\nWhere mode = ( adventurer | smithy | council_room )\n", argv[0]);
		exit(1);
	}
	printf("============================================================\n");
	printf("==========            END OF TESTING              ==========\n");
	printf("============================================================\n");
	return 0;
}

void setupGameState(struct gameState *gs) {
	// random number of players
	int playerCount = rand_int(2, MAX_PLAYERS);
	
	// random next player
	int curPlayer = rand_int(0, playerCount - 1); // player is 0-indexed
	
	// randomize the gameState (per lecture)
	int i, j;
	for (i = 0; i < sizeof(struct gameState); i++) {
		((char*)gs)[i] = rand_int(0, 255);
	}
	
	// now set (correct) the necessary fields in gameState
	gs->numPlayers = playerCount;
	gs->whoseTurn = curPlayer;
	gs->numActions = 1;
	gs->numBuys = 1;
	gs->playedCardCount = 0;
	
	// since some cards require draw by other players, setup correctly (randomly)
	// for all players
	for (j = 0; j < playerCount; j++) {
		gs->deckCount[j] = rand_int(TEST_MIN_DECK_COUNT, MAX_DECK);
		gs->discardCount[j] = rand_int(0, MAX_DECK - gs->deckCount[j]);
		gs->handCount[j] = 	rand_int(0, MAX_DECK - gs->deckCount[j] - gs->discardCount[j]);
		
		// randomize i's deck, hand, discardCount
		for (i = 0; i < gs->deckCount[j]; i++)
			gs->deck[j][i] = rand_int(curse, treasure_map);
		for (i = 0; i < gs->handCount[j]; i++)
			gs->hand[j][i] = rand_int(curse, treasure_map);
		for (i = 0; i < gs->discardCount[j]; i++)
			gs->discard[j][i] = rand_int(curse, treasure_map);
	}
	return;
}

void outputTestResults(struct validations *v) {
	int errCount = 0;
	errCount += v->errcount_playerDeckCount; // specific to played card
	errCount += v->errcount_playerHandCount; // specific to played card
	errCount += v->errcount_playerDiscardCount; // specific to played card
	errCount += v->errcount_playedCards;
	errCount += v->errcount_opponentDeckCount; // should not be changed for adventurer, smithy; changes for council_room
	errCount += v->errcount_opponentHandCount; // should not be changed
	errCount += v->errcount_opponentDiscardCount; // should not be changed 
	errCount += v->errcount_whoseTurn; // post should be next player relative to pre
	errCount += v->errcount_otherGamestateElements; // 
	errCount += v->errcount_adventurer_treasure;
	errCount += v->errcount_adventurer_treasureindiscard;
	errCount += v->errcount_adventurer_deckhanddiscard;
	
	if (errCount) {
		printf("Errors:\n");
		if (v->errcount_playerDeckCount)
			printf("Errors in player deck count: %d\n", v->errcount_playerDeckCount);
		if (v->errcount_playerHandCount)
			printf("Errors in player hand count: %d\n", v->errcount_playerHandCount);
		if (v->errcount_playerDiscardCount)
			printf("Errors in player discard count: %d\n", v->errcount_playerDiscardCount);
		if (v->errcount_playedCards)
			printf("Errors in played cards: %d\n", v->errcount_playedCards);
		if (v->errcount_numBuys)
			printf("Errors in numBuys: %d\n", v->errcount_numBuys);
		if (v->errcount_opponentDeckCount)
			printf("Errors in opponents' deck count: %d\n", v->errcount_opponentDeckCount);
		if (v->errcount_opponentHandCount)
			printf("Errors in opponents' hand count: %d\n", v->errcount_opponentHandCount);
		if (v->errcount_opponentDiscardCount)
			printf("Errors in opponents' discard count: %d\n", v->errcount_opponentDiscardCount);
		if (v->errcount_whoseTurn)
			printf("Errors in passing turn to next player: %d\n", v->errcount_whoseTurn);
		if (v->errcount_otherGamestateElements)
			printf("Errors in other gameState elements: %d\n", v->errcount_otherGamestateElements);
		if (v->errcount_adventurer_treasure)
			printf("Errors in adventurer treasure: %d\n", v->errcount_adventurer_treasure);
		if (v->errcount_adventurer_treasureindiscard)
			printf("Errors in adventurer treasure - treasure discarded: %d\n", v->errcount_adventurer_treasureindiscard);
		if (v->errcount_adventurer_deckhanddiscard)
			printf("Errors in managing cards from deck to discard (discard = drawn - 2 + 1): %d\n", v->errcount_adventurer_deckhanddiscard);
	} else {
		printf("No errors!\n");
	}
}

int rand_int(int min, int max) {
	return (floor(Random() * (max - min + 1)) + min);
}

void dumpGameState(struct gameState *gs) {
	int i;
	
	printf("============================================================\n");
	printf("==========          DUMPING GAMESTATE             ==========\n");
	printf("numPlayers: %d\n", gs->numPlayers);
	printf("supplyCount:\n");
	for (i = 0; i < treasure_map + 1; i++)
		printf("  supplyCount for %d: %d\n", i, gs->supplyCount[i]);
	printf("embargoTokens:\n");
	for (i = 0; i < treasure_map + 1; i++)
		printf("  embargoTokens for %d: %d\n", i, gs->embargoTokens[i]);
	printf("outpostPlayed: %d\n", gs->outpostPlayed);
	printf("outpostTurn: %d\n", gs->outpostTurn);
	printf("whoseTurn: %d\n", gs->whoseTurn);
	printf("phase: %d\n", gs->phase);
	printf("numActions: %d\n", gs->numActions);
	printf("coins: %d\n", gs->coins);
	printf("numBuys: %d\n", gs->numBuys);
	printf("handCount: ");
	for (i = 0; i < gs->numPlayers; i++)
		printf("  player %d: %d;", i, gs->handCount[i]);
	printf("\n");
	//hand[MAX_PLAYERS][MAX_HAND];
	printf("deckCount: ");
	for (i = 0; i < gs->numPlayers; i++)
		printf("  player %d: %d;", i, gs->deckCount[i]);
	printf("\n");
	//deck[MAX_PLAYERS][MAX_DECK];
	printf("discardCount: ");
	for (i = 0; i < gs->numPlayers; i++)
		printf("  player %d: %d ", i, gs->discardCount[i]);
	printf("\n");
	//discard[MAX_PLAYERS][MAX_DECK];
	printf("playedCardCount: %d\n", gs->playedCardCount);
	//playedCards[MAX_DECK];
	printf("==========          END OF GAMESTATE              ==========\n");
	printf("============================================================\n");
}
