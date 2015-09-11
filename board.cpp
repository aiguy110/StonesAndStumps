#include <iostream>
#include "board.h"

using namespace std;

MoveResult Board::DoMove(Move move){
	// First make sure this is a valid move
	if (!this->ValidMove(move))
		return INVALID_MOVE;

	// Adjust the board to reflect the move
	this->grid[3 * move.i_to + move.j_to] = move.player_token;

	return COMPLETED;
}

bool Board::ValidMove(Move move){
	// You can't place a blank
	if (move.player_token == ' ')
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

Board::Board(){
	for (int i = 0; i < 9; i++){
		this->grid[i] = ' ';
	}
}

void Board::Display(ostream &out){
	char rowLabels[] = { '1', '2', '3' };
	char columnLabels[] = { 'a', 'b', 'c' };
	for (int y = 0; y < 8; y++){
		for (int x = 0; x < 12; x++){
			// Work out in advance the row and column of the board this "pixel" is in
			int i = (y - 2) / 2;
			int j = (x - 2) / 4;

			// Decide what char to put here
			if (y == 0 && (x == 2 || x == 6 || x == 10)){ // Column Labels
				out << columnLabels[j];
			}
			else if (x == 0 && (y == 2 || y == 4 || y == 6)){ // Row Labels
				out << rowLabels[i];
			}
			else if ((y == 3 || y == 5) && (x == 4 || x == 8)){ // Cross Bars
				out << '+';
			}
			else if ((x == 4 || x == 8) && y != 0){ // Vertical Bars
				out << '|';
			}
			else if ((y == 3 || y == 5) && x != 0){ // Horizontal Bars
				out << '-';
			}
			else if ((y == 2 || y == 4 || y == 6) && (x == 2 || x == 6 || x == 10)){ // Player Marks
				out << this->grid[3 * i + j];
			}
			else{ // Whitespace
				out << ' ';
			}
		}

		// Newline
		out << '\n';
	}
}