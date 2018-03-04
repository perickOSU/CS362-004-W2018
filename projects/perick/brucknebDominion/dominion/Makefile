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

unittest1:
	gcc -fprofile-arcs -ftest-coverage -Wall -std=c99 dominion.c unittest1.c rngs.c -o unittest1 -lm -g

unittest2:
	gcc -fprofile-arcs -ftest-coverage -Wall -std=c99 dominion.c unittest2.c rngs.c -o unittest2 -lm -g

unittest3:
	gcc -fprofile-arcs -ftest-coverage -Wall -std=c99 dominion.c unittest3.c rngs.c -o unittest3 -lm -g

unittest4:
	gcc -fprofile-arcs -ftest-coverage -Wall -std=c99 dominion.c unittest4.c rngs.c -o unittest4 -lm -g

cardtest1:
	gcc -fprofile-arcs -ftest-coverage -Wall -std=c99 dominion.c cardtest1.c rngs.c -o cardtest1 -lm -g

cardtest2:
	gcc -fprofile-arcs -ftest-coverage -Wall -std=c99 dominion.c cardtest2.c rngs.c -o cardtest2 -lm -g

cardtest3:
	gcc -fprofile-arcs -ftest-coverage -Wall -std=c99 dominion.c cardtest3.c rngs.c -o cardtest3 -lm -g

cardtest4:
	gcc -fprofile-arcs -ftest-coverage -Wall -std=c99 dominion.c cardtest4.c rngs.c -o cardtest4 -lm -g

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out

randomtest: randomtestdriver.h randomtestdriver.c randomtestadventurer.h randomtestadventurer.c randomtestcard1.h randomtestcard1.c randomtestcard2.h randomtestcard2.c dominion.o rngs.o 
	gcc -o test04 randomtestdriver.c randomtestadventurer.c randomtestcard1.c randomtestcard2.c -g dominion.o rngs.o $(CFLAGS)
	./test04 adventurer > randomtestadventurer.out
	gcov -f -b dominion.c >> randomtestadventurer.out
	
	./test04 smithy > randomtestcard1.out
	gcov -f -b dominion.c >> randomtestcard1.out
	
	./test04 council_room > randomtestcard2.out
	gcov -f -b dominion.c >> randomtestcard2.out

assignment3: unittest1.c unittest2.c unittest3.c unittest4.c cardtest1.c cardtest2.c cardtest3.c cardtest4.c rngs.c dominion.o
	gcc unittest1.c dominion.o rngs.o -lm --coverage -o unittest1
	gcc unittest2.c dominion.o rngs.o -lm --coverage -o unittest2
	gcc unittest3.c dominion.o rngs.o -lm --coverage -o unittest3
	gcc unittest4.c dominion.o rngs.o -lm --coverage -o unittest4
	gcc cardtest1.c dominion.o rngs.o -lm --coverage -o cardtest1
	gcc cardtest2.c dominion.o rngs.o -lm --coverage -o cardtest2
	gcc cardtest3.c dominion.o rngs.o -lm --coverage -o cardtest3
	gcc cardtest4.c dominion.o rngs.o -lm --coverage -o cardtest4

	./unittest1 >> unittestresults.out
	./unittest2 >> unittestresults.out
	./unittest3 >> unittestresults.out
	./unittest4 >> unittestresults.out

	./cardtest1 >> unittestresults.out
	./cardtest2 >> unittestresults.out
	./cardtest3 >> unittestresults.out
	./cardtest4 >> unittestresults.out

	gcov dominion.c >> unittestresults.out
	cat dominion.c.gcov >> unittestresults.out

	player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player testDrawCard testBuyCard badTestDrawCard

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe
