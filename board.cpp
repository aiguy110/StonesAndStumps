#include "board.h"

MoveResult Board::DoMove(Move move){
	// First make sure this is a valid move
	if (!this->ValidMove(move))
		return INVALID_MOVE;

	// Adjust the board to reflect the move
	this->grid[3 * move.i_to + move.j_to] = move.player_token;
}

bool Board::ValidMove(Move move){
	// You can't place a blank
	if (move.player_token == 0)
		return false;

	// You must place your token on the board
	if (move.i_to > 2 || move.j_to > 2)
		return false;

	// You cannot place your token on another token
	if (this->grid[3*move.i_to + move.j_to] != 0)
		return false;

	// Looks lagit
	return true;
}