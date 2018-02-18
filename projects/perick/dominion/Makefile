CFLAGS = -Wall -fpic -coverage -lm

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/
testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

badTestDrawCard: badTestDrawCard.c dominion.o rngs.o
	gcc -o badTestDrawCard -g  badTestDrawCard.c dominion.o rngs.o $(CFLAGS)

testBuyCard: testDrawCard.c dominion.o rngs.o
	gcc -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

testAll: dominion.o testSuite.c
	gcc -o testSuite testSuite.c -g  dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)


unittest1: unittest1.c dominion.o rngs.o
	gcc  -o unittest1 -g  unittest1.c dominion.o rngs.o $(CFLAGS)

unittest2: unittest2.c dominion.o rngs.o
	gcc  -o unittest2 -g  unittest2.c dominion.o rngs.o  $(CFLAGS)

unittest3: unittest3.c dominion.o rngs.o
	gcc  -o unittest3 -g  unittest3.c dominion.o rngs.o  $(CFLAGS)

unittest4: unittest4.c dominion.o rngs.o
	gcc  -o unittest4 -g  unittest4.c dominion.o rngs.o  $(CFLAGS)

cardtest1: cardtest1.c dominion.o rngs.o
	gcc  -o cardtest1 -g  cardtest1.c dominion.o rngs.o  $(CFLAGS)

cardtest2: cardtest2.c dominion.o rngs.o
	gcc  -o cardtest2 -g  cardtest2.c dominion.o rngs.o  $(CFLAGS)

cardtest3: cardtest3.c dominion.o rngs.o
	gcc  -o cardtest3 -g  cardtest3.c dominion.o rngs.o  $(CFLAGS)

cardtest4: cardtest4.c dominion.o rngs.o
	gcc  -o cardtest4 -g  cardtest4.c dominion.o rngs.o  $(CFLAGS)

randomtestadventurer: randomtestadventurer.c dominion.o rngs.o
	gcc  -o randomtestadventurer -g  randomtestadventurer.c dominion.o rngs.o  $(CFLAGS)

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out




player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

unittestresults.out: unittest1 unittest2 unittest3 unittest4 cardtest1 cardtest2 cardtest3 cardtest4
	#
	# Do non-card unit tests
	#
	./unittest1 > unittestresults.out
	printf "\n\n=========== UNIT TEST COVERAGE REPOT ============\n\n" >> unittestresults.out
	printf "updateCoints function:\n" >> unittestresults.out
	gcov -abcfu dominion.c | sed -n '/updateCoins/{n;p; n;p; n;p; n;p}' >> unittestresults.out
	#
	rm dominion.gcda
	./unittest2 >> unittestresults.out
	printf "\n\n=========== UNIT TEST COVERAGE REPOT ============\n\n" >> unittestresults.out
	printf "getCost function:\n" >> unittestresults.out
	gcov -abcfu dominion.c | sed -n '/getCost/{n;p; n;p; n;p; n;p}' >> unittestresults.out
	#
	rm dominion.gcda
	./unittest3 >> unittestresults.out
	printf "\n\n=========== UNIT TEST COVERAGE REPOT ============\n\n" >> unittestresults.out
	printf "whoseTurn function:\n" >> unittestresults.out
	gcov -abcfu dominion.c | sed -n '/whoseTurn/{n;p; n;p; n;p; n;p}' >> unittestresults.out
	#
	rm dominion.gcda
	./unittest4 >> unittestresults.out
	printf "\n\n=========== UNIT TEST COVERAGE REPOT ============\n\n" >> unittestresults.out
	printf "numHandCards function:\n" >> unittestresults.out
	gcov -abcfu dominion.c | sed -n '/numHandCards/{n;p; n;p; n;p; n;p}' >> unittestresults.out
	#
	# Do card unit tests
	#
	rm dominion.gcda
	./cardtest1 >> unittestresults.out
	printf "\n\n=========== UNIT TEST COVERAGE REPOT ============\n\n" >> unittestresults.out
	printf "smithy card effect function:\n" >> unittestresults.out
	gcov -abcfu dominion.c | sed -n '/smithyEffect/{n;p; n;p; n;p; n;p}' >> unittestresults.out
	#
	rm dominion.gcda
	./cardtest2 >> unittestresults.out
	printf "\n\n=========== UNIT TEST COVERAGE REPOT ============\n\n" >> unittestresults.out
	printf "adventurer card effect function:\n" >> unittestresults.out
	gcov -abcfu dominion.c | sed -n '/adventurerEffect/{n;p; n;p; n;p; n;p}' >> unittestresults.out
	#
	rm dominion.gcda
	./cardtest3 >> unittestresults.out
	printf "\n\n=========== UNIT TEST COVERAGE REPOT ============\n\n" >> unittestresults.out
	printf "great hall card effect function:\n" >> unittestresults.out
	gcov -abcfu dominion.c | sed -n '/great_hallEffect/{n;p; n;p; n;p; n;p}' >> unittestresults.out
	#
	rm dominion.gcda
	./cardtest4 >> unittestresults.out
	printf "\n\n=========== UNIT TEST COVERAGE REPOT ============\n\n" >> unittestresults.out
	printf "village card effect function:\n" >> unittestresults.out
	gcov -abcfu dominion.c | sed -n '/villageEffect/{n;p; n;p; n;p; n;p}' >> unittestresults.out
	#
	# Do all tests and print overall report
	#
	rm dominion.gcda
	./unittest1 > /dev/null
	./unittest2 > /dev/null
	./unittest3 > /dev/null
	./unittest4 > /dev/null
	./cardtest1 > /dev/null
	./cardtest2 > /dev/null
	./cardtest3 > /dev/null
	./cardtest4 > /dev/null
	printf "\n\n========== FULL DOMINION COVERAGE REPORT ==========\n\n" >> unittestresults.out
	printf "\n\ndominion.c Overall Summary:\n\n" >> unittestresults.out
	gcov dominion.c -abcu >> unittestresults.out
	printf "\n\ndominion.c Function Break-Down Summary:\n\n" >> unittestresults.out
	gcov dominion.c -abcuf >> unittestresults.out

all: playdom player testDrawCard testBuyCard badTestDrawCard testUpdateCoins

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe
