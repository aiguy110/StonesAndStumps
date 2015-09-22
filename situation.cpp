#include "situation.h"

using namespace std;

void Situation::Write(ostream &out){
	// Write board
	for (int n = 0; n < 9; n++){
		if (this->board.grid[n] == ' ')
			out << '0';
		else
			out << this->board.grid[n];
	}

	// Write the number of move/score pairs to follow
	int move_count = this->moves.size();
	out << ' ' << move_count << ' ';

	// Write move/score pairs
	for (int n = 0; n < move_count; n++){
		// Move
		out << this->moves[n].player_token << ' ';
		out << this->moves[n].placing << ' ';
		out << this->moves[n].i_to << ' ';
		out << this->moves[n].j_to << ' ';
		out << this->moves[n].i_from << ' ';
		out << this->moves[n].j_from << ' ';

		// Score
		out << this->scores[n] << ' ';
	}

	out << endl;
}

void Situation::Read(istream &in){
	// Read Board
	for (int n = 0; n < 9; n++){
		char c;
		in >> c;
	if (c == '0')
			this->board.grid[n] = ' ';
		else
			this->board.grid[n] = c;
	}

	// Read the number of move/score pairs to follow
	int move_count;
	in >> move_count;
	this->moves.resize(move_count);
	this->scores.resize(move_count);

	// Read move/score pairs
	for (int n = 0; n < move_count; n++){
		// Move
		in >> this->moves[n].player_token;
		in >> this->moves[n].placing;
		in >> this->moves[n].i_to;
		in >> this->moves[n].j_to;
		in >> this->moves[n].i_from;
		in >> this->moves[n].j_from;

		// Score
		in >> this->scores[n];
	}
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

Situation *Situation::Transformed(bool transpose, int ccQuarterTurns){
	Situation *new_situation = new Situation();

	// Transform the Board
	for (int n = 0; n < 9; n++){
		new_situation->board.grid[this->TransformedIndex(n, transpose, ccQuarterTurns)] = this->board.grid[n];
	}

	// Transform moves
	new_situation->moves.resize(this->moves.size());
	new_situation->scores.resize(this->moves.size());
	int move_count = this->moves.size();
	for (int n = 0; n < move_count; n++){
		// Copy player_token
		new_situation->moves[n].player_token = this->moves[n].player_token;

		// Transfrom *_to coords
		int n_to = 3 * this->moves[n].i_to + this->moves[n].j_to;
		int t_n_to = this->TransformedIndex(n_to, transpose, ccQuarterTurns);
		new_situation->moves[n].i_to = t_n_to / 3;
		new_situation->moves[n].j_to = t_n_to % 3;

		// If needed, transform *_from coords
		if (!this->moves[n].placing){
			int n_from = 3 * this->moves[n].i_from + this->moves[n].j_from;
			int t_n_from = this->TransformedIndex(n_from, transpose, ccQuarterTurns);
			new_situation->moves[n].i_from = t_n_from / 3;
			new_situation->moves[n].j_from = t_n_from % 3;
		}

		// Copy scores
		new_situation->scores[n] = this->scores[n];
	}

	return new_situation;
}

Situation *Situation::Canonical(){
	bool waste_b;
	int waste_i;
	return Canonical( waste_i, waste_b, waste_i);
}

Situation *Situation::Canonical( int &canonical_id, bool &inv_transpose, int &inv_ccQuarterTurns){
	Situation *canonical_sit = NULL;
	canonical_id = -1;
	for (int n = 0; n < 8; n++){
		Situation *sit = this->Transformed(n % 2, n / 2);
		int id = sit->UniqueID('1');
		if (id > canonical_id){
			canonical_id = id;
			canonical_sit = sit;
			inv_transpose = n % 2;
			inv_ccQuarterTurns = 4 - (n / 2);
		}
	}
	return canonical_sit;
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

int Situation::UniqueID(char current_token){
	int digs[6] = { 0, 0, 0, 0, 0, 0 };
	int current_i = 0;
	int other_i = 3;
	for (int n = 0; n < 9; n++){
		if (board.grid[n] == current_token)
			digs[current_i++] = n;
		else if (board.grid[n] != ' ')
			digs[other_i++] = n;
	}
	int num = 0;
	for (int d = 0; d < 6; d++){
		num += digs[d] * (int)pow(10, 5-d);
	}
	return num;
}