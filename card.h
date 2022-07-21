#ifndef _CARD_H_
#define _CARD_H_

#include "stdint.h"

typedef struct {
	uint8_t  value;
	char     name[10];
}Card;

void print_card(Card c);

#endif /** _CARD_H_ **/
