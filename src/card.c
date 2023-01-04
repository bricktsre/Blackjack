#include <stdio.h>
#include <string.h>

#include "card.h"

void print_card(Card c) {
	if (c.value == 11 || strcmp(c.name, "J") == 0 || strcmp(c.name, "Q") == 0 || strcmp(c.name, "K") == 0)
		printf("%s ", c.name);
	else
		printf("%d ", c.value);
}
