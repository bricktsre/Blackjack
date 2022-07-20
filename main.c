#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>

#include "card.h"
#include "shoe.h"
#include "common.h"

int main(int argc, char *argv[]) {
	srand(time(0));

	Shoe* s = create_shoe(1);
	destroy_shoe(s);
}

uint32_t get_random_number(uint32_t range) {
	uint32_t x;
	do {
		x = rand();
	} while (x >= range * RAND_INV_RANGE (range));
	x /= RAND_INV_RANGE (range);
	return x;
}
