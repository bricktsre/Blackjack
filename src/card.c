#include <stdio.h>

#include "card.h"

void print_card(Card c) {
	if (c.value == 11)
		printf("%s ", c.name);
	else
		printf("%d ", c.value);
}
