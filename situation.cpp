#include "situation.h"

using namespace std;

void Situation::Write(ostream &out){
	// Write board
	for (int n = 1; n < 9; n++){
		out << this->board.grid[n];
	}

	// Write the number of move/score pairs to follow
	int move_count = this->moves.size();
	out << move_count;

	// Write move/score pairs
	for (int n = 0; n < move_count; n++){
		// Move
		cout << this->moves[n].player_token;
		cout << this->moves[n].placing;
		cout << this->moves[n].i_to;
		cout << this->moves[n].j_to;
		cout << this->moves[n].i_from;
		cout << this->moves[n].j_from;

		// Score
		cout << this->scores[n];
	}

	// Flush
	cout << endl;
}

bool Situation::Match(Situation situation){
	bool match = true;
	for (int n = 0; n < 9; n++){
		if (situation.board.grid[n] != this->board.grid[n]){
			match = false;
			break;
		}
	}

	return match;
}

Situation Situation::Transformed(bool transpose, int ccQuarterTurns){
	Situation new_situation;

	// Transform the Board
	for (int n = 0; n < 9; n++){
		new_situation.board.grid[this->TransformedIndex(n, transpose, ccQuarterTurns)] = this->board.grid[n];
	}

	// Transform moves
	new_situation.moves.resize(this->moves.size());
	new_situation.scores.resize(this->moves.size());
	int move_count = this->moves.size();
	for (int n = 0; n < move_count; n++){
		// Transfrom *_to coords
		int n_to = 3 * this->moves[n].i_to + this->moves[n].j_to;
		int t_n_to = this->TransformedIndex(n_to, transpose, ccQuarterTurns);
		new_situation.moves[n].i_to = t_n_to / 3;
		new_situation.moves[n].j_to = t_n_to % 3;

		// If needed, transform *_from coords
		if (!this->moves[n].placing){
			int n_from = 3 * this->moves[n].i_from + this->moves[n].j_from;
			int t_n_from = this->TransformedIndex(n_from, transpose, ccQuarterTurns);
			new_situation.moves[n].i_from = t_n_from / 3;
			new_situation.moves[n].j_from = t_n_from % 3;
		}

		// Copy scores
		new_situation.scores[n] = this->scores[n];
	}

	return new_situation;
}

int Situation::TransformedIndex(int index, bool transpose, int ccQuarterTurns){
	int i = index / 3;
	int j = index % 3;

	int t_i;
	int t_j;
	if (transpose){
		t_i = j;
		t_j = i;
	}
	else{
		t_i = i;
		t_j = j;
	}

	int rt_i;
	int rt_j;
	switch (ccQuarterTurns % 4){
	case 0:
		rt_i = t_i;
		rt_j = t_j;
		break;
	case 1:
		rt_i = t_j;
		rt_j = 2 - t_i;
		break;
	case 2:
		rt_i = 2 - t_i;
		rt_j = 2 - t_j;
		break;
	case 3:
		rt_i = 2 - t_j;
		rt_j = t_i;
		break;
	}

	return 3 * rt_i + rt_j;
}