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
		int move_count = sit.moves.size();
		
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

	// Canonize the situation
	int canonical_id;
	int inv_rot;
	bool inv_t;
	Situation *canonical_situation = working_sit.Canonical(canonical_id, inv_t, inv_rot);
	delete canonical_situation; // We actually only need the ID

	// Try to find the canonical situation in past_memory
	map<int, Situation>::iterator it = past_memory.find(canonical_id);
	if (it != past_memory.end()){ // Found it!
		// Find the best move in this situation
		Situation *equiv_sit = it->second.Transformed(inv_t, inv_rot);
		int best_move_index = -1;
		int best_score = -10000;
		int move_count = equiv_sit->moves.size();
		for (int m = 0; m < move_count; m++){
			if (equiv_sit->scores[m] > best_score){
				best_score = equiv_sit->scores[m];
				best_move_index = m;
			}
		}

		// Decide whether to use the best known move, or to try learning from a new and random one
		if (best_score == 0) best_score += 1; // Prevent division by zero
		if (rand() % (best_score * UNORIGINALNESS_FACTOR) == 0){ // Go random! (Probablity of going random is inversly proportional to the best score)
			delete equiv_sit;
			return GetRandomMove(board);
		}
		else{ // Tried and true.
			Move final_move(equiv_sit->moves[best_move_index]);
			delete equiv_sit;
			final_move.player_token = my_token;
			return final_move;
		}
	}
	else{ // Couldn't find it
		delete canonical_situation;
		return GetRandomMove(board);
	}
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
	// Loop through the situations of this game
	int recent_mem_count = recent_memory.size();
	for (int r = 0; r < recent_mem_count; r++){
		// Canonize this situation
		int canonical_id;
		int waste_i;
		bool waste_b;
		Situation *canonical_sit = recent_memory[r].Canonical(canonical_id, waste_b, waste_i);
		Situation rm = recent_memory[r];
		
		// Try to find a matching situation in past_memory
		map<int, Situation>::iterator it;
		it = past_memory.find(canonical_id);
		if (it != past_memory.end()){ // past_memory contains and equivilant situation
			Situation &past_sit = it->second;
			
			// Is this a known move?
			int move_count = past_sit.moves.size();
			bool match_found = false;
			for (int m = 0; m < move_count; m++){
				if (past_sit.moves[m].Match(canonical_sit->moves[0])){
					// Matching move found. Add new score to past score
					past_sit.scores[m] += canonical_sit->scores[0];
					match_found = true;
					break;
				}
			}

			// If no match was found, add this move to past situation
			if (!match_found){
				past_sit.moves.push_back(canonical_sit->moves[0]);
				past_sit.scores.push_back(canonical_sit->scores[0]);
			}
		}
		else{ // This is a new situation, not in past_memory. Add it.
			past_memory[canonical_id] = *canonical_sit;
		}
		delete canonical_sit;
	}
}

void ComputerPlayer::WriteMemory(ostream &out){
	int sit_count = this->past_memory.size();
	out << sit_count << endl;
	for (map<int, Situation>::iterator it = past_memory.begin(); it != past_memory.end(); it++){
		it->second.Write(out);
	}
}

void ComputerPlayer::ReadMemory(istream &in){
	int sit_count;
	in >> sit_count;
	for (int n = 0; n < sit_count; n++){
		Situation sit;
		sit.Read(in);
		int id = sit.UniqueID('1');
		past_memory[id] = sit;
	}
}