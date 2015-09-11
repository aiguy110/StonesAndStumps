#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include "move.h"

enum MoveResult{
	INVALID_MOVE,
	COMPLETED
};

class Board{
public:
	char grid[9];

	Board();
	MoveResult DoMove(Move move);
	void Display(std::ostream &out);
	bool ValidMove(Move move);
	int CountToken(char token);
	bool TokenWins(char token);
private:
	static const size_t win_groups[8][3]; 
};

#endif