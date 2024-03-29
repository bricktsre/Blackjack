#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "shoe.h"
#include "card.h"
#include "common.h"

Shoe* create_shoe(uint32_t num_decks) {
	Shoe* s = calloc(1, sizeof(Shoe) + (num_decks * 52 * sizeof(Card)));
	if (!s) {
		printf("Error on calloc for Shoe\n");
		return NULL;
	}
	
	s->next_card = 0;	
	s->orig_size = num_decks * 52;
	s->curr_size = num_decks * 52;

	for (size_t i = 0; i <= s->orig_size - 1; i++) {
		int j = (i % 13) + 1;
		
		if (j == 1) {
			s->cards[i].value = 11;
			sprintf(s->cards[i].name, "%s", "A");
		} else if (j <= 10) {
			s->cards[i].value = j;
			sprintf(s->cards[i].name, "%d", j);
		} else {
			s->cards[i].value = 10;
			if (j == 11)
				sprintf(s->cards[i].name, "%s", "J");
			else if (j == 12)
				sprintf(s->cards[i].name, "%s", "Q");
			else
				sprintf(s->cards[i].name, "%s", "K");
		}
		int k = ((i * 4) / 52) % 4; 
		
		if (k == 0)
			sprintf(s->cards[i].suit, "%s", "diamonds");
		else if (k == 1)
			sprintf(s->cards[i].suit, "%s", "clubs");
		else if (k == 2)
			sprintf(s->cards[i].suit, "%s", "hearts");
		else	
			sprintf(s->cards[i].suit, "%s", "spades");
	}

	return s;
}

void destroy_shoe(Shoe *s) {
	if (s == NULL)
		printf("Trying to free a NULL Pointer\n");

	free(s);
}

void shuffle_shoe(Shoe *s) {
	for (uint32_t i = s->orig_size - 1; i > 0; i--) {
		uint32_t j = get_random_number(i+1);
		Card temp = s->cards[i];
		s->cards[i] = s->cards[j];
		s->cards[j] = temp;
	}
	s->next_card = 0;
	s->curr_size = s->orig_size;
}

Card* get_next_card(Shoe *s) {
	if (s == NULL)
		return NULL;
	if (s->curr_size == 0)
		return NULL;
	
	s->curr_size--;
	return &(s->cards[s->next_card++]);
}

float get_penetration(Shoe *s) {
	if (s->orig_size == 0)
		return -1;

	return s->curr_size / (float)s->orig_size;
}
