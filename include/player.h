#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "hand.h"
#include "card.h"

typedef struct {
	Hand *hands[4];
	size_t numHands;
	size_t initialWager;
	float money;
}Player;

void add_hand(Player *player, size_t wager, Card *cardOne, Card *cardTwo);

#endif /** _PLAYER_H_ **/
