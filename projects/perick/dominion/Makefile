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

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out




player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

unittestresults.out: unittest1 unittest2
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
	./unittest1 > /dev/null
	./unittest2 > /dev/null
	printf "\n\n========== FULL DOMINION COVERAGE REPORT ==========\n\n" >> unittestresults.out
	printf "\n\ndominion.c Overall Summary:\n\n" >> unittestresults.out
	gcov dominion.c -abcu >> unittestresults.out
	printf "\n\ndominion.c Functional Summary:\n\n" >> unittestresults.out
	gcov dominion.c -abcuf >> unittestresults.out

all: playdom player testDrawCard testBuyCard badTestDrawCard testUpdateCoins

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe
