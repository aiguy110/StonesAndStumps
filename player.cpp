#include <iostream>
#include <string>
#include "player.h"
#include "move.h"
#include "board.h"

using namespace std;

// Base Player
Move Player::GetMove(Board &board){
	Move move(' ', -1, -1);
	// TODO: Nothing. This should never be called
	return move;
}

// Human Player
HumanPlayer::HumanPlayer(char token, istream &in, ostream &out){
	this->my_token = token;
	this->in = &in;
	this->out = &out;
}

Move HumanPlayer::GetMove(Board &board) {
	// Valid inputs
	string validInputs[9] = { "a1", "b1", "c1", "a2", "b2", "c2", "a3", "b3", "c3" };
	
	// Prompt the player for a their move
	*(this->out) << this->my_token<< "->";
	string input = "";
	*(this->in) >> input;

	// Try to match the input to a recognized input
	int n = -1;
	for (int i = 0; i < 9; i++){
		if (validInputs[i] == input){
			n = i;
			break;
		}
	}

	if (n == -1){ // If the input was not recognized, try again. 
		*(this->out) << "That input was not recognized. Try again." << endl;
		return this->GetMove(board);
	}
	else{
		// Attempt the move
		int i = n / 3;
		int j = n % 3;
		Move move(this->my_token, i, j);
		bool valid = board.ValidMove(move);
		if (valid)
			return move;
		else{
			*(this->out) << "That is not a valid move. Try again." << endl;
			return this->GetMove(board);
		}
	}
}

