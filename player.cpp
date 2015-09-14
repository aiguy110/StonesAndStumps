#include <iostream>
#include <string>
#include "player.h"
#include "move.h"
#include "board.h"

using namespace std;

// Base Player
Player::Player(char token){
	this->my_token = token;
}

Move Player::GetMove(Board &board){
	Move move;
	return move;
}



// Human Player
HumanPlayer::HumanPlayer(char token, istream &in, ostream &out) : Player(token){
	this->in = &in;
	this->out = &out;
}

Move HumanPlayer::GetMove(Board &board) {
	// If they already have 3 pieces out, find out where they'd like to move from
	int n_from = -1;
	if (board.CountToken(this->my_token) == 3){
		*(this->out) << this->my_token << "<-";
		n_from = this->TakeInput();
	}

	// Find out where they'd like to move to
	*(this->out) << this->my_token << "->";
	int n_to = this->TakeInput();

	// Attempt the move
	int i_to = n_to / 3;
	int j_to = n_to % 3;
	Move move;
	move.player_token = this->my_token;
	move.i_to = i_to;
	move.j_to = j_to;
	if (n_from != -1){
		move.placing = false;
		move.i_from = n_from / 3;
		move.j_from = n_from % 3;
	}
	bool valid = board.ValidMove(move);
	if (valid)
		return move;
	else{
		*(this->out) << "That is not a valid move. Try again." << endl;
		return this->GetMove(board);
	}
}

int HumanPlayer::TakeInput(){
	// Valid inputs
	string validInputs[9] = { "a1", "a2", "a3", "b1", "b2", "b3", "c1", "c2", "c3" };

	// Read input
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

	// If the input was not recognized, try again. 
	if (n == -1) {
		*(this->out) << "That input was not recognized. Try again.\n>";
		return this->TakeInput();
	}

	return n;
}