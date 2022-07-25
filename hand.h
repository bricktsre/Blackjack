#ifndef _HAND_H_
#define _HAND_H_

#include <stddef.h>
#include <stdint.h>

#include "card.h"

#define MAX_HAND_SIZE 15

/**
 * Hand Status byte
 * busted	bit 0
 * blackjack	bit 1
 * splittable	bit 2
 * double down	bit 3
 * stand	bit 4
 */

typedef struct {
	Card*	hand[MAX_HAND_SIZE];
	size_t 	hand_size;	
	uint8_t status;
	size_t 	count;
	size_t 	aces;
	size_t 	wager;
}Hand;


Hand *create_hand();

void destroy_hand(Hand *h);

void clean_hand(Hand *h);

void add_card(Hand *h, Card *card);

void print_hand(Hand *h);

void print_dealer_hand(Hand *h);

#endif /** _HAND_H_ **/
