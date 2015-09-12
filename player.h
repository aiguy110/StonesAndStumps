#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include "board.h"
#include "move.h"

class Player{
public:
	char my_token;

	virtual Move GetMove(Board &board);
};

class HumanPlayer : public Player{
public:
	HumanPlayer(char token, std::istream &in, std::ostream &out);
	Move GetMove(Board &board) override;
private:
	std::istream *in;
	std::ostream *out;

	int TakeInput();
};

#endif