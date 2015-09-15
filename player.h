#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include "game_result.h"
#include "board.h"
#include "move.h"

class Player{
public:
	char my_token;

	Player(char token);
	virtual Move GetMove(Board &board);
	virtual void NotifyGameResult(GameResult result);
	virtual void NotifyMove(Board &board, Move move);
};

class HumanPlayer : public Player{
public:
	HumanPlayer(char token, std::istream &in, std::ostream &out);
	Move GetMove(Board &board) override;
	void NotifyGameResult(GameResult result);
	void NotifyMove(Board &board, Move move);
private:
	std::istream *in;
	std::ostream *out;

	int TakeInput();
};

#endif