#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>

#include "card.h"
#include "shoe.h"
#include "common.h"
#include "hand.h"

#define MAX_HAND_SIZE 10

int main(int argc, char *argv[]) {
	srand(time(0));

	Shoe* shoe = create_shoe(1);
	shuffle_shoe(shoe);

	Card *playerHand[MAX_HAND_SIZE];
	clean_hand(playerHand, MAX_HAND_SIZE);
	
	playerHand[0] = get_next_card(shoe);
       	playerHand[1] = get_next_card(shoe);
	
	print_hand(playerHand, MAX_HAND_SIZE);	

	uint8_t hand_status = evaluate_hand(playerHand, MAX_HAND_SIZE);
	printf("hand_status: %d\n", hand_status);

	if (hand_status & 0x01)
		printf("Busted!\n");
	else if (hand_status & 0x02)
		printf("Blackjack!\n");

	destroy_shoe(shoe);
}

uint32_t get_random_number(uint32_t range) {
	uint32_t x;
	do {
		x = rand();
	} while (x >= range * RAND_INV_RANGE (range));
	x /= RAND_INV_RANGE (range);
	return x;
}
