#ifndef _HAND_H_
#define _HAND_H_

#include <stddef.h>
#include <stdint.h>

#include "card.h"

/**
 * Hand Status byte
 * busted	bit 0
 * blackjack	bit 1
 * splittable	bit 2
 * double down	bit 3
 */

typedef struct {
	Card*	hand[10]
	size_t 	hand_size;	
	uint8_t status;
	size_t 	count;
	size_t 	aces
	size_t 	wager;
}Hand;


Hand *create_hand();

void destroy_hand(Hand *h);

void clean_hand(Hand *h);

uint8_t evaluate_hand(Hand *h);

void print_hand(Hand *h);

#endif /** _HAND_H_ **/
