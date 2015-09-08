#ifndef BOARD_H
#define BOARD_H

#include "move.h"

enum MoveResult{
	INVALID_MOVE,
	COMPLETE
};

class Board{
public:
	char grid[9];

	MoveResult DoMove(Move move);
private:
	bool ValidMove(Move move);
};

#endif