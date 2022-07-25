#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <string.h>

#include "card.h"
#include "shoe.h"
#include "common.h"
#include "hand.h"

#define PLAYER_INPUT_SIZE 10

typedef struct {
	Hand  *hand;
	float  money;
}Player;

int main(int argc, char *argv[]) {
	size_t iter = 0;
	
	srand(time(0));

	Shoe* shoe = create_shoe(2);
	shuffle_shoe(shoe);

	size_t numPlayers = 1;	
	Player players[numPlayers];
	for (iter = 0; iter < numPlayers; iter++) {
		players[iter].hand = create_hand();
		players[iter].money = 100;
	}

	Hand *dealerHand = create_hand();
	
	char playerInput[PLAYER_INPUT_SIZE];
	memset(playerInput, 0, sizeof(playerInput));

	puts("Welcome to Blackjack!\n");	
	while (get_penetration(shoe) >= 0.2) {
		for (iter = 0; iter < numPlayers; iter++) {
			printf("Your money: %.2f\n", players[iter].money);
			puts("How much would you like to bet?");
			while (1) {
				if (fgets(playerInput, PLAYER_INPUT_SIZE-1, stdin) != NULL) {
					int pWager = 0;
					if (sscanf(playerInput, "%d", &pWager) != 1) {
						puts("Not a number");
						continue;
					}
					
					if (pWager < 0 || (pWager % 5 != 0))
						puts("Wager must be 5, 10, 25, 50, or 100");
					else if (pWager > players[iter].money)
						puts("Can't bet more than you have!");
					else {
						players[iter].hand->wager += pWager;
						players[iter].money -= pWager;
						break;
					}
				}
			}
		}
		puts("");

		add_card(dealerHand, get_next_card(shoe));
		add_card(dealerHand, get_next_card(shoe));
		printf("Dealer's hand: ");
		print_dealer_hand(dealerHand);
		int roundOver = 0;
		int reprint = 0;	

		for (iter = 0; iter < numPlayers; iter++) {
			add_card(players[iter].hand, get_next_card(shoe));
       			add_card(players[iter].hand, get_next_card(shoe));
			printf("Player %ld's hand: ", iter + 1);
			print_hand(players[iter].hand);	
			printf("Status: %2.2x\n",players[iter].hand->status);
		}
		
		if (dealerHand->hand[0]->value == 11) {
			// Insurance and Even money
			if (dealerHand->count == 21) {
				for (iter = 0; iter < numPlayers; iter++) {
					if (players[iter].hand->count == 21) {
						players[iter].money += players[iter].hand->count;
						clean_hand(players[iter].hand);	
					}
				}
				roundOver = 1;
			}
		}

		// Each player plays till bust or stand
		for (iter = 0; iter < numPlayers; iter++) {
			if (roundOver)
				break;

			while (!(players[iter].hand->status & 0x13)) {
				if (players[iter].hand->status == 0)
					puts("(H)it or (S)tand?");
				else {
					printf("(H)it, (S)tand, ");
					if (players[iter].hand->status & 0x08)
						puts("or (D)ouble down?");
				}

				memset(playerInput, 0, sizeof(playerInput));
				if (fgets(playerInput, PLAYER_INPUT_SIZE-1, stdin) != NULL) {
					char c = playerInput[iter];
					if (c == 'H' || c == 'h') {
						add_card(players[iter].hand, get_next_card(shoe));
						reprint = 1;
					} else if (c == 'S' || c == 's') {
						players[iter].hand->status |= 0x10;
					} else if ((c == 'D' || c == 'd') && (players[iter].hand->status & 0x08)) {
						if (players[iter].money >= players[iter].hand->wager) {
							players[iter].money -= players[iter].hand->wager;
							players[iter].hand->wager *= 2;
							add_card(players[iter].hand, get_next_card(shoe));
							players[iter].hand->status |= 0x10;
							reprint = 1;
							
						} else
							puts("Not enough money to double down");

					}
				}
				if (reprint) {
					printf("Player %ld's hand: ", iter + 1);
					print_hand(players[iter].hand);	
					reprint = 0;
				}
				printf("Status: %2.2x\n",players[iter].hand->status);
			}
			
			if ((players[iter].hand->status & 0x02) && players[iter].hand->count == 2) {
				puts("Blackjack!");
				players[iter].money += players[iter].hand->wager * 2.5;
				clean_hand(players[iter].hand);
			} else if (players[iter].hand->status & 0x01) {
				puts("Busted!");
				clean_hand(players[iter].hand);
			}
			puts("");
		}

		// Dealer plays	
		printf("Dealer's hand: ");
		print_hand(dealerHand);
		while (!(dealerHand->status & 0x03) && dealerHand->count < 17) {
			add_card(dealerHand, get_next_card(shoe));
			printf("Dealer's hand: ");
			print_hand(dealerHand);
			printf("Status: %2.2x\n",dealerHand->status);
		}
		puts("");

		
		// Evaluate Player winning or losing versus Dealer
		for (iter = 0; iter < numPlayers; iter++) {
			if (players[iter].hand->status != 0) {
				printf("player: %ld dealer: %ld\n", players[iter].hand->count, dealerHand->count);
				if (players[iter].hand->count == dealerHand->count)
					players[iter].money += players[iter].hand->wager;
				else if (players[iter].hand->count > dealerHand->count || (dealerHand->status &= 0x01))
					players[iter].money += players[iter].hand->wager * 2;
				clean_hand(players[iter].hand);
				puts("");
			}
		}
		clean_hand(dealerHand);
		
	}
	
	for (iter = 0; iter < numPlayers; iter++)
		destroy_hand(players[iter].hand);
	
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
