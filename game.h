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
	Player *playerPtrs[2];
	size_t current_player;

	Game(Player &p1, Player &p2, std::ostream &out);
	GameResult PlayTurn();
	GameResult CheckResult();
private:
	void ClearScreen();
	std::ostream *out;
};

#endif