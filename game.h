#ifndef GAME_H
#define GAME_H

#include <iostream>
#include "board.h"
#include "player.h"

enum GameResult{
	P1_WIN,
	P2_WIN,
	CATS_GAME,
	UNRESOLVED
};

class Game{
public:
	Board board;
	Player players[2];
	size_t current_player;

	GameResult PlayTurn();
	void Display(std::ostream out);
};

#endif