#ifndef MOVE_H
#define MOVE_H

struct Move{
	char player_token;
	bool placing = true;
	int i_from = -1;
	int j_from = -1;
	int i_to;
	int j_to;

	Move(char token, int i, int j){
		this->player_token = token;
		this->i_to = i;
		this->j_to = j;
	}
};

#endif