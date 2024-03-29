#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <string.h>

#include "card.h"
#include "shoe.h"
#include "common.h"
#include "hand.h"
#include "player.h"

#define PLAYER_INPUT_SIZE 10

void play_hand(Hand *hand, Player *player, Shoe *shoe, size_t playerNum);

int main(int argc, char *argv[]) {
	size_t playerIter = 0;
	size_t handIter   = 0;
	
	srand(time(0));

	Shoe* shoe = create_shoe(1);
	shuffle_shoe(shoe);

	/*shoe->cards[0].value = 7;
	shoe->cards[1].value = 10;
	shoe->cards[2].value = 6;
	shoe->cards[3].value = 6;
	shoe->cards[4].value = 5;
	shoe->cards[5].value = 3;
	shoe->cards[6].value = 10;
	shoe->cards[7].value = 7;
	shoe->cards[8].value = 5;
	shoe->cards[9].value = 6;
	shoe->cards[10].value = 7;*/
	

	size_t numPlayers = 1;
	size_t maxHands = 4;	
	Player players[numPlayers];
	for (playerIter = 0; playerIter < numPlayers; playerIter++) {
		for(handIter = 0; handIter < maxHands; handIter++) 
			players[playerIter].hands[handIter] = create_hand();
		players[playerIter].numHands = 0;
		players[playerIter].initialWager = 0;
		players[playerIter].money = 100;
	}

	Hand *dealerHand = create_hand();
	
	char playerInput[PLAYER_INPUT_SIZE];
	memset(playerInput, 0, sizeof(playerInput));

	puts("Welcome to Blackjack!\n");	
	while (1) {
		for (playerIter = 0; playerIter < numPlayers; playerIter++) {
			Player* player = &(players[playerIter]);
			if (player->initialWager != 0) {
				if (player->initialWager > player->money)
					puts("Not enough money to rebet");
				else {
					printf("Player %ld's bet: %ld\n", playerIter + 1, player->initialWager);
					continue;
				}	
			}
			printf("Player %ld's money: %.2f\n", playerIter + 1, players[playerIter].money);
			while (1) {
				printf("How much would you like to bet? ");
				if (fgets(playerInput, PLAYER_INPUT_SIZE-1, stdin) != NULL) {
					int pWager = 0;
					if (sscanf(playerInput, "%d", &pWager) != 1) {
						puts("Not a number");
						continue;
					}
					
					if (pWager < 0 || (pWager % 5 != 0))
						puts("Wager must be a multiple of 5");
					else if (pWager > player->money)
						puts("Can't bet more than you have!");
					else {
						player->initialWager = pWager;
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

		for (playerIter = 0; playerIter < numPlayers; playerIter++) {
			players[playerIter].numHands = 0;
			add_hand(&players[playerIter], players[playerIter].initialWager, get_next_card(shoe), get_next_card(shoe));
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
				play_hand(players[playerIter].hands[handIter], &players[playerIter], shoe, playerIter + 1);
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
						printf("dealer: %ld player: %ld\n", dealerHand->count, hand->count);
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
			printf("Player %ld money: %.2f\nRebet and deal or (C)hange bet ", playerIter + 1, players[playerIter].money);
			memset(playerInput, 0, sizeof(playerInput));
			if (fgets(playerInput, PLAYER_INPUT_SIZE-1, stdin) != NULL) {
				char c = playerInput[0];
				if (c == 'C' || c == 'c')
					players[playerIter].initialWager = 0;
			}
		}
	
		if (players[0].money == 0) {
			printf("You are out of money. Game over\n");
			break;
		}

		if (get_penetration(shoe) < 0.2) {
	//int i;
	//for(i=0;i<52;i++)
	//	printf("%s %s\n", shoe->cards[i].name, shoe->cards[i].suit);
			shuffle_shoe(shoe);
		
	//puts("");
	//		for(i=0;i<52;i++)
	//	printf("%s %s\n", shoe->cards[i].name, shoe->cards[i].suit);
		}
	}
	
	for (playerIter = 0; playerIter < numPlayers; playerIter++) {
		for (handIter = 0; handIter < maxHands; handIter++)
			destroy_hand(players[playerIter].hands[handIter]);
	}
	
	destroy_shoe(shoe);
}

void play_hand(Hand *hand, Player *player, Shoe *shoe, size_t playerNum) {
	printf("Player %ld's hand: ", playerNum);
	print_hand(hand);
	size_t reprint = 0;

	while(!(hand->status & 0x13)) {
		if (hand->status == 0)
			printf("(H)it or (S)tand? ");
		else {
			printf("(H)it, (S)tand, ");
			if (hand->status & 0x04)
				printf("(D)ouble down, S(p)lit, or S(u)rrender? ");
			else if (hand->status & 0x28)
				printf("(D)ouble down, or S(u)rrender? ");
		}
		char playerInput[PLAYER_INPUT_SIZE];
		memset(playerInput, 0, sizeof(playerInput));
		if (fgets(playerInput, PLAYER_INPUT_SIZE-1, stdin) != NULL) {
			char c = playerInput[0];
			if (c == 'H' || c == 'h') {
				add_card(hand, get_next_card(shoe));
				reprint = 1;
			} else if (c == 'S' || c == 's') {
				hand->status |= 0x10;
			} else if (c == 'U' || c == 'u') {
				player->money += hand->wager * 0.5;
				clean_hand(hand);
				break;
			} else if ((c == 'D' || c == 'd') && hand->status & 0x08) {
				if (player->money >= hand->wager) {
					player->money -= hand->wager;
					hand->wager *= 2;
					add_card(hand, get_next_card(shoe));
					hand->status |= 0x10;
					reprint = 1;
				}
				else 
					puts("Not enough money to double down");
			} else if ((c == 'P' || c == 'p') && hand->status & 0x04) {
				if (player->money < hand->wager)
					puts("Not enough money to split");
				else if (player->numHands == MAX_HAND_SIZE)
					puts("Already split max times");
				else {	
					Card* tempCard = remove_card(hand);
					add_card(hand, get_next_card(shoe));
					add_hand(player, hand->wager, tempCard, get_next_card(shoe));
					reprint = 1;
				}
			}

			if (reprint) {
				printf("Player %ld's hand: ", playerNum);
				print_hand(hand);
				reprint = 0;
			}
		}
	}
}

uint32_t get_random_number(uint32_t range) {
	uint32_t x;
	do {
		x = rand();
	} while (x >= range * RAND_INV_RANGE (range));
	x /= RAND_INV_RANGE (range);
	return x;
}
