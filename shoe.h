#ifndef _SHOE_H_
#define _SHOE_H_

#include <stdint.h>

#include "card.h"

typedef struct {
	uint32_t  next_card;
	uint32_t  curr_size;
	uint32_t  orig_size;
	Card  cards[];
}Shoe;

Shoe* create_shoe(uint32_t num_decks);

void destroy_shoe(Shoe *s);

/**
 * Shuffle cards of provided shoe in place
 */
void shuffle_shoe(Shoe *s);

Card* get_next_card(Shoe *s);

#endif /** _SHOE_H_ **/
