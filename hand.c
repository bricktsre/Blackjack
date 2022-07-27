#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "hand.h"
#include "card.h"

Hand* create_hand() {
	Hand *h = calloc(1, sizeof(Hand));

	if (!h) {
		puts("Error using calloc for hand allocation");
		return NULL;
	}

	h->hand_size = 0;
	h->count = 0;
	h->aces = 0;
	h->status = 0;
	h->wager = 0;
		
	for (size_t i = 0; i < MAX_HAND_SIZE; i++)
		h->hand[i] = NULL;

	return h;
}

void destroy_hand(Hand *h) {
	if (h == NULL) {
		printf("Trying to free NULL Hand pointer\n");
		return;
	}

	free(h);
}


void clean_hand(Hand *h) {
	for (size_t i = 0; i < h->hand_size; i++)
		h->hand[i] = NULL;
	
	h->hand_size = 0;
	h->count = 0;
	h->aces = 0;
	h->status = 0;
	h->wager = 0;
}

void add_card(Hand *h, Card *card) {
	h->status = 0;
	
	h->hand[h->hand_size] = card;
	h->hand_size++;
	h->count += card->value;
	//printf("card value: %d\n", card->value);

	if (card->value == 11)
		h->aces++;

	if (h->count == 21) {
		h->status |= 0x02;
		return;
	}

	if (h->hand_size == 2 && h->hand[0]->value == h->hand[1]->value)
		h->status |= 0x04;

	if (h->count > 21) {
		while (h->aces > 0) {
			h->aces--;
			h->count -= 10;
			if (h->count == 21) {
				h->status |= 0x02;
				return;
			} else if (h->count < 21)
				break;
		}
		
		if (h->count > 21) {
			h->status |= 0x01;
			return;
		}
	}
	
	if (h->hand_size == 2)
		h->status |= 0x28;
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
	puts("");
}

void print_dealer_hand(Hand *h) {
	if (h->hand_size == 0)
		return;

	for (size_t i = 0; i < h->hand_size; i++) {
		if (i == 1)
			continue;
		if (h->hand[i] != NULL)
			print_card(*(h->hand[i]));
		else 
			break;
	}
	puts("? ");
}
