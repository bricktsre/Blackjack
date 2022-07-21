#include <stddef.h>
#include <stdio.h>
#include <stdint.h>

#include "hand.h"
#include "card.h"

Hand* ceate_hand() {
	Hand *h = calloc(1, sizeof(Hand));
	clean_hand(h);
	
	return h;
}

void destroy_hand(Hand *h) {
	if (h == NULL) {
		printf("Trying to free NULL Hand pointer\n")
		return;
	}

	free(h);
}


void clean_hand(Hand *h) {
	for (size_t i = 0; i < h->hand_size; i++)
		h->hand[i] = NULL;
}

void add_card(Hand *h, Card *card) {
	h->status = 0;
	
	hand->hand_size++;
	count += card->value;
	
	if (card->value == 11)
		h->aces++;

	if (count == 21) {
		h->status &= 0x02;
		return;
	}

	if (h->hand_size == 2 && h->hand[0].value == h->hand[1].value)
		h->status &= 0x04;


}

uint8_t evaluate_hand(Card **hand, size_t hand_size) {
	uint8_t status = 0;
	int 	count = 0;
	int 	aces = 0;
	size_t 	i;

	for (i = 0; i < hand_size; i++) {
		if ( hand[i] == NULL)
			break;

		int card_value = hand[i]->value;
		
		if (card_value == 11) 
			aces++;
		
		count += card_value;
	}

	if (count == 21)
		return (status & 0x02);
	else if (i == 2 && hand[0]->value == hand[1]->value)
		status & 0x04;

	if (count > 21) {
		if (aces == 0)
			return (status & 0x01);
		else {
			for (;aces > 0; aces--) {
				count -= 10;
				if (count == 21)
					return (status & 0x02);
				else if (count < 21)
					break;
			}
			if (count > 21)
				return (status & 0x01);
		}
	}

	status &= 0x08; 
	       		
	return status;
}

void print_hand(Hand *h) {
	if (h->hand_size == 0)
		return;

	for (size_t i = 0; i < h->hand_size; i++) {
		if (h->hand[i] != NULL)
			print_card(*(h->hand[i]));
		else
			break;
	}
	printf("\n");
}
