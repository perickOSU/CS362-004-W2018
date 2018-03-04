/*******************************************************************************
* Student: Brian Bruckner
* Course:  CS_362_400_W2018
*
* File: unittest1.c
* 
* This file tests the function compare
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "dominion.h"

/*prototypes*/
int compare(const void* a, const void* b);

int main () {
	int a, b;
	
	// test boundary / domain conditions
	a = 1;
	b = 1;
	printf("compare returned %d, expecting %d\n", compare(&a, &b), 0);
	printf("compare returned %d, expecting %d\n", compare(&b, &a), 0);
	a = -1;
	b = -1;
	printf("compare returned %d, expecting %d\n", compare(&a, &b), 0);
	printf("compare returned %d, expecting %d\n", compare(&b, &a), 0);
	a = 0;
	b = 0;
	printf("compare returned %d, expecting %d\n", compare(&a, &b), 0);
	printf("compare returned %d, expecting %d\n", compare(&b, &a), 0);
	a = 1;
	b = 2;
	printf("compare returned %d, expecting %d\n", compare(&a, &b), -1);
	printf("compare returned %d, expecting %d\n", compare(&b, &a), 1);
	a = 0;
	b = 1;
	printf("compare returned %d, expecting %d\n", compare(&a, &b), -1);
	printf("compare returned %d, expecting %d\n", compare(&b, &a), 1);
	a = -1;
	b = 0;
	printf("compare returned %d, expecting %d\n", compare(&a, &b), -1);
	printf("compare returned %d, expecting %d\n", compare(&b, &a), 1);
	a = -2;
	b = -1;
	printf("compare returned %d, expecting %d\n", compare(&a, &b), -1);
	printf("compare returned %d, expecting %d\n", compare(&b, &a), 1);
}