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
	Hand  *hands[4];
	size_t numHands;
	size_t initialWager;
	float  money;
}Player;

int main(int argc, char *argv[]) {
	size_t playerIter = 0;
	size_t handIter   = 0;
	
	srand(time(0));

	Shoe* shoe = create_shoe(2);
	shuffle_shoe(shoe);

	size_t numPlayers = 1;
	size_t maxHands = 4;	
	Player players[numPlayers];
	for (playerIter = 0; playerIter < numPlayers; playerIter++) {
		for(handIter = 0; handIter < maxHands; handIter++) 
			players[playerIter].hands[handIter] = create_hand();
		players[playerIter].numHands = 1;
		players[playerIter].initialWager = 0;
		players[playerIter].money = 100;
	}

	Hand *dealerHand = create_hand();
	
	char playerInput[PLAYER_INPUT_SIZE];
	memset(playerInput, 0, sizeof(playerInput));

	puts("Welcome to Blackjack!\n");	
	while (get_penetration(shoe) >= 0.2) {
		for (playerIter = 0; playerIter < numPlayers; playerIter++) {
			Player* player = &(players[playerIter]);
			if (player->initialWager != 0) {
				if (player->initialWager > player->money)
					puts("Not enough money to rebet");
				else {
					printf("Player %ld's bet: %ld\n", playerIter + 1, player->initialWager);
					player->hands[0]->wager += player->initialWager;
					players->money -= player->initialWager;
					continue;
				}	
			}
			printf("Player %ld's money: %.2f\nHow much would you like to bet\n", playerIter + 1, players[playerIter].money);
			while (1) {
				if (fgets(playerInput, PLAYER_INPUT_SIZE-1, stdin) != NULL) {
					int pWager = 0;
					if (sscanf(playerInput, "%d", &pWager) != 1) {
						puts("Not a number");
						continue;
					}
					
					if (pWager < 0 || (pWager % 5 != 0))
						puts("Wager must be a multiple of 5");
					else if (pWager > players[playerIter].money)
						puts("Can't bet more than you have!");
					else {
						players[playerIter].hands[0]->wager += pWager;
						players[playerIter].money -= pWager;
						players[playerIter].initialWager = pWager;
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

		for (playerIter = 0; playerIter < numPlayers; playerIter++) {
			players[playerIter].numHands = 1;
			add_card(players[playerIter].hands[0], get_next_card(shoe));
       			add_card(players[playerIter].hands[0], get_next_card(shoe));
			printf("Player %ld's hand: ", playerIter + 1);
			print_hand(players[playerIter].hands[0]);	
			//printf("Status: %2.2x\n",players[playerIter].hands[0]->status);
		}
		
		if (dealerHand->hand[0]->value == 11 || dealerHand->hand[0]->value == 10) {
			// Insurance and Even money
			if (dealerHand->count == 21) {
				for (playerIter = 0; playerIter < numPlayers; playerIter++) {
					if (players[playerIter].hands[0]->count == 21) {
						players[playerIter].money += players[playerIter].hands[0]->count;
						clean_hand(players[playerIter].hands[0]);	
					}
				}
				roundOver = 1;
			}
		}

		// Each player plays till bust or stand
		for (playerIter = 0; playerIter < numPlayers; playerIter++) {
			if (roundOver)
				break;
			for (handIter = 0; handIter < players[playerIter].numHands; handIter++) {
				if (handIter >= 1) {
					printf("Player %ld's hand: ", playerIter + 1);
					print_hand(players[playerIter].hands[handIter]);	
				}

				while (!(players[playerIter].hands[handIter]->status & 0x13)) {
					if (players[playerIter].hands[handIter]->status == 0)
						puts("(H)it or (S)tand?");
					else {
						printf("(H)it, (S)tand, ");
						if (players[playerIter].hands[handIter]->status & 0x04)
							puts("(D)ouble down, S(p)lit, or S(u)rrender?");
						else if (players[playerIter].hands[handIter]->status & 0x28)
							puts("(D)ouble down, or S(u)rrender?");
					}

					memset(playerInput, 0, sizeof(playerInput));
					if (fgets(playerInput, PLAYER_INPUT_SIZE-1, stdin) != NULL) {
						char c = playerInput[0];
						if (c == 'H' || c == 'h') {
							add_card(players[playerIter].hands[handIter], get_next_card(shoe));
							reprint = 1;
						} else if (c == 'S' || c == 's') {
							players[playerIter].hands[handIter]->status |= 0x10;
						} else if (c == 'U' || c == 'u') {
							players[playerIter].money += players[playerIter].hands[handIter]->wager * 0.5;
							clean_hand(players[playerIter].hands[handIter]);
							break;
						} else if ((c == 'D' || c == 'd') && (players[playerIter].hands[handIter]->status & 0x08)) {
							if (players[playerIter].money >= players[playerIter].hands[handIter]->wager) {
								players[playerIter].money -= players[playerIter].hands[handIter]->wager;
								players[playerIter].hands[handIter]->wager *= 2;
								add_card(players[playerIter].hands[handIter], get_next_card(shoe));
								players[playerIter].hands[handIter]->status |= 0x10;
								reprint = 1;
							} else
								puts("Not enough money to double down");
						} else if ((c == 'P' || c == 'p') && (players[playerIter].hands[handIter]->status & 0x04)) {
							if (players[playerIter].money < players[playerIter].hands[handIter]->wager) 
								puts("Not enough money to split");
							else if (players[playerIter].numHands == MAX_HAND_SIZE)
								puts("Already split max times");
							else {	
								players[playerIter].money -= players[playerIter].hands[handIter]->wager;
								players[playerIter].hands[players[playerIter].numHands]->wager = players[playerIter].hands[handIter]->wager;

								add_card(players[playerIter].hands[players[playerIter].numHands], remove_card(players[playerIter].hands[handIter]));
								add_card(players[playerIter].hands[players[playerIter].numHands], get_next_card(shoe));
								add_card(players[playerIter].hands[handIter], get_next_card(shoe));
								
								players[playerIter].numHands++;
								printf("Player %ld's hand: ", playerIter + 1);
								print_hand(players[playerIter].hands[handIter]);	
							} 
						}
					}

					if (reprint) {
						printf("Player %ld's hand: ", playerIter + 1);
						print_hand(players[playerIter].hands[handIter]);	
						reprint = 0;
					}
					//printf("Status: %2.2x\n",players[playerIter].hands[0]->status);
				}
			
				if ((players[playerIter].hands[handIter]->status & 0x02) && players[playerIter].hands[handIter]->hand_size == 2) {
					puts("Blackjack!");
					players[playerIter].money += players[playerIter].hands[handIter]->wager * 2.5;
					clean_hand(players[playerIter].hands[handIter]);
				} else if (players[playerIter].hands[handIter]->status & 0x01) {
					puts("Busted!");
					clean_hand(players[playerIter].hands[handIter]);
				}
				puts("");
			}
		}

		// Dealer plays	
		printf("Dealer's hand: ");
		print_hand(dealerHand);
		
		roundOver = 0;
		for (playerIter = 0; playerIter < numPlayers; playerIter++) {
			for ( handIter = 0; handIter < players[playerIter].numHands; handIter++)
				roundOver |= players[playerIter].hands[handIter]->status;
		}

		while (!(dealerHand->status & 0x03) && dealerHand->count < 17 && roundOver) {
			add_card(dealerHand, get_next_card(shoe));
			printf("Dealer's hand: ");
			print_hand(dealerHand);
			//printf("Status: %2.2x\n",dealerHand->status);
		}
		puts("");

		
		// Evaluate Player winning or losing versus Dealer
		for (playerIter = 0; playerIter < numPlayers; playerIter++) {
			Player* player = &(players[playerIter]);
			for (handIter = 0; handIter < player->numHands; handIter++) {
				Hand *hand = player->hands[handIter];
				if (hand->status != 0) {
					if (player->numHands == 1)
						printf("player: %ld dealer: %ld\n", hand->count, dealerHand->count);
					else
						printf("Player %ld's hand %ld: %ld dealer: %ld\n", playerIter + 1, handIter + 1, hand->count, dealerHand->count);
					
					if (hand->count == dealerHand->count) {
						players->money += hand->wager;
						puts("It's a push");
					} else if (hand->count > dealerHand->count || (dealerHand->status &= 0x01)) {
						players->money += hand->wager * 2;
						printf("Player %ld won %ld\n", playerIter + 1, hand->wager);
					} else
						puts("You lost");
					clean_hand(players[playerIter].hands[handIter]);
					puts("");
				}
			}
		}
		clean_hand(dealerHand);
		
		for (playerIter = 0; playerIter < numPlayers; playerIter++) {
			printf("Player %ld money: %.2f\nRebet and deal or (C)hange bet\n", playerIter + 1, players[playerIter].money);
			memset(playerInput, 0, sizeof(playerInput));
			if (fgets(playerInput, PLAYER_INPUT_SIZE-1, stdin) != NULL) {
				char c = playerInput[0];
				if (c == 'C' || c == 'c')
					players[playerIter].initialWager = 0;
			}
		}
		
	}
	
	for (playerIter = 0; playerIter < numPlayers; playerIter++) {
		for (handIter = 0; handIter < maxHands; handIter++)
			destroy_hand(players[playerIter].hands[handIter]);
	}
	
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
