#include "computer_player.h"

#define UNORIGINALNESS_FACTOR 5

using namespace std;

ComputerPlayer::ComputerPlayer(char token, string mem_filename) :Player(token){
	// Seed rand from system time
	srand(time(NULL));

	// Read in our memory from filename
	ifstream mem_file(mem_filename);
	if (mem_file.good())
		this->ReadMemory(mem_file);
	mem_file.close(); 

	//Store filename for later use
	this->mem_filename = mem_filename;
}

void ComputerPlayer::NotifyGameResult(GameResult result){
	switch (result)
	{
	case P1_WIN:
		this->FinalizeMemory(true);
		break;
	case P2_WIN:
		this->FinalizeMemory(false);
		break;
	default:
		return;
	}
	this->MergeMemory();
	
	// Write memory to file
	ofstream file(this->mem_filename, ofstream::trunc);
	this->WriteMemory(file);
	file.close();
}

void ComputerPlayer::NotifyMove(Board &board, Move move){
	Situation sit;
	Board new_board(board);
	sit.board = new_board;
	sit.moves.push_back(move);

	this->recent_memory.push_back(sit);
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
	// Convert board to Situation
	Board working_board(board);
	for (int n = 0; n < 9; n++){
		if (board.grid[n] == my_token)
			working_board.grid[n] = '1';
		else if (board.grid[n] != ' ')
			working_board.grid[n] = '2';
	}
	Situation working_sit;
	working_sit.board = working_board;

	// Try to find an equivilant situation in past_memory
	int past_mem_cout = past_memory.size();
	for (int p = 0; p < past_mem_cout; p++){
		for (int t = 0; t < 8; t++){
			Situation equiv_sit = past_memory[p].Transformed((bool)(t / 8), t % 4);
			if (equiv_sit.Match(working_sit)){
				// Find the best move in this situation
				int best_move_index = -1;
				int best_score = -100;
				int move_count = equiv_sit.moves.size();
				for (int m = 0; m < move_count; m++){
					if (equiv_sit.scores[m] > best_score){
						best_score = equiv_sit.scores[m];
						best_move_index = m;
					}
				}

				// Decide whether to use the best known move, or to try learning from a new and random one
				if (best_score == 0) best_score += 1; // Prevent division by zero
				if (rand() % (best_score * UNORIGINALNESS_FACTOR) == 0){ // Go random! (Probablity of going random is inversly proportional to the best score)
					return GetRandomMove(board);
				}
				else{ // Tried and true.
					Move final_move(equiv_sit.moves[best_move_index]);
					final_move.player_token = my_token;
					return final_move;
				}
			}
		}
	}

	// Found nothing? Make a random move.
	return GetRandomMove(board);
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


void ComputerPlayer::MergeMemory(){
	int recent_mem_count = recent_memory.size();
	int past_mem_count = past_memory.size();
	for (int r = 0; r < recent_mem_count; r++){
		bool match_found = false;
		for (int p = 0; p < past_mem_count; p++){
			for (int t = 0; t < 8; t++){
				Situation equiv_sit = recent_memory[r].Transformed((bool)(t / 8), t % 4);
				if (equiv_sit.Match(past_memory[p])){
					match_found = true;
					int move_count = past_memory[p].moves.size();
					int matched_move = -1;
					for (int m = 0; m < move_count; m++){
						if (past_memory[p].moves[m].Match(equiv_sit.moves[0])){
							matched_move = m;
							break;
						}
					}
					if (matched_move != -1){
						past_memory[p].scores[matched_move] += equiv_sit.scores[0];
					}
					else{
						past_memory[p].moves.push_back( equiv_sit.moves[0] );
						past_memory[p].scores.push_back ( equiv_sit.scores[0] );
					}
					break;
				}
			}
			if (match_found)
				break;
		}
		if (!match_found)
			past_memory.push_back(recent_memory[r]);
	}
}

void ComputerPlayer::WriteMemory(ostream &out){
	int sit_count = this->past_memory.size();
	out << sit_count << endl;
	for (int n = 0; n < sit_count; n++){
		this->past_memory[n].Write(out);
	}
}

void ComputerPlayer::ReadMemory(istream &in){
	int sit_count;
	in >> sit_count;
	for (int n = 0; n < sit_count; n++){
		Situation sit;
		sit.Read(in);
		this->past_memory.push_back(sit);
	}
}