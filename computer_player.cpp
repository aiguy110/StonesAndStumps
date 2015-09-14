#include "computer_player.h"

using namespace std;

ComputerPlayer::ComputerPlayer(char token) :Player(token){
	srand(time(NULL));
}

void ComputerPlayer::NotifyGameResult(GameResult result){
	switch (result)
	{
	case P1_WIN:
		this->FinalizeMemory(true);
		return;
	case P2_WIN:
		this->FinalizeMemory(false);
		return;
	default:
		return;
	}
}

void ComputerPlayer::FinalizeMemory(bool won){
	// Loop through the situations I've seen in this game
	int sit_count = this->recent_memory.size();
	for (int n = 0; n < sit_count; n++){
		Situation &sit = this->recent_memory[n];
		
		// Who was moving and did they eventually win?
		char mover_token = sit.moves[0].player_token;
		bool mover_won;
		if (won)
			mover_won = (mover_token == this->my_token);
		else
			mover_won = (mover_token != this->my_token);

		// Generalize board tokens
		for (int m = 0; m < 9; m++){
			if (sit.board.grid[m] == mover_token)
				sit.board.grid[m] = '1';
			else if (sit.board.grid[m] != ' ')
				sit.board.grid[m] = '2';
		}

		// Generalize move token (actually unnescisary since it will always be '1', but for elligence...)
		sit.moves[0].player_token = '1';

		// Apply Scoring
		if (mover_won)
			sit.scores.push_back(1);
		else
			sit.scores.push_back(-1);
	}
}

Move ComputerPlayer::GetMove(Board &board){
	Move move = this->GetRandomMove(board);
	
	// Update recent_memory
	Board my_leave = Board(this->recent_memory.back().board);
	Move opponent_move = this->FindIntermediateMove(my_leave, board);

	Situation intermediate_situation;
	intermediate_situation.board = my_leave;
	intermediate_situation.moves.push_back(opponent_move);

	Situation current_situation;
	current_situation.board = board;
	current_situation.moves.push_back(move);

	this->recent_memory.push_back(intermediate_situation);
	this->recent_memory.push_back(current_situation);

	// Return our decision
	return move;
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

Move ComputerPlayer::FindIntermediateMove(Board board_i, Board board_f){
	char token;
	bool placing = true;

	// Find the new piece
	int i_to = -1;
	int j_to = -1;
	for (int n = 0; n < 9; n++){
		if (board_i.grid[n] == ' ' && board_f.grid[n] != ' '){
			token = board_f.grid[n];
			i_to = n / 3;
			j_to = n % 3;
		}
	}

	// Find where it came from if applicable
	int i_from = -1;
	int j_from = -1;
	for (int n = 0; n < 9; n++){
		if (board_i.grid[n] == token && board_f.grid[n] == ' '){
			placing = false;
			i_from = n / 3;
			j_from = n % 3;
		}
	}

	// Return a Move object
	Move move;
	move.player_token = token;
	move.placing = placing;
	move.i_to = i_to;
	move.j_to = j_to;
	move.i_from = i_from;
	move.j_from = j_from;
	return move;
}