#ifndef PLAYER_H
#define PLAYER_H

#include "board.h"
#include "move.h"

class Player{
public:
	char my_token;
	virtual Move GetMove(Board *board);
};

#endif