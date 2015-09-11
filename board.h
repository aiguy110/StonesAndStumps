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
};

#endif