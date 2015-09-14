#include "computer_player.h"

Move ComputerPlayer::GetMove(Board &board){
	return this->GetRandomMove(board);
}

Move ComputerPlayer::GetRandomMove(Board &board){
	Move move = {};
	move.player_token = this->my_token;

	// Should we be placing or moving?
	move.placing = board.CountToken(this->my_token) < 3;

	// Try lots of stuff
	while (true){
		if (!move.placing){
			move.i_from = rand() % 3;
			move.j_from = rand() % 3;
		}
		move.i_to = rand() % 3;
		move.j_to = rand() % 3;

		// If it's valid, roll with it
		if (board.ValidMove(move))
			return move;
	}
}