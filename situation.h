#ifndef SITUATION_H
#define SITUATION_H

#include <vector>
#include <cmath>
#include <iostream>
#include "board.h"
#include "move.h"


class Situation{
public:
	Board board;
	std::vector<Move> moves;
	std::vector<int> scores;

	bool Match(Situation situation);
	Situation *Transformed(bool transpose, int ccQuarterTurns);
	Situation *Canonical();
	Situation *Canonical(int &canonical_id, bool &inv_transpose, int &ccQuarterTurns);
	void Write(std::ostream &out);
	void Read(std::istream &in);
	int UniqueID(char current_token);

private:
	int TransformedIndex(int index, bool transpose, int ccQuarterTurns);
};

#endif