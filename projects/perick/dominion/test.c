#include <stdio.h>

#include "dominion.h"

#define mac(x) \
{ \
	for (i=0; i<tok ## x; i++) { \
		printf("%d", i); \
	} \
}

int main()
{
	int i;
	int tokOne = 3;
	int tokTwo = 6;
	mac(One)
	mac(Two)

	return 0;
}
