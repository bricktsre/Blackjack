#include "player.h"
#include "hand.h"
#include "card.h"

void add_hand(Player *player, size_t wager, Card *cardOne, Card *cardTwo) {
	player->money -= wager;
	player->hands[player->numHands]->wager = wager;
	add_card(player->hands[player->numHands], cardOne);
	add_card(player->hands[player->numHands], cardTwo);
	player->numHands++;
}
