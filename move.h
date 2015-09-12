#ifndef MOVE_H
#define MOVE_H

struct Move{
	char player_token = ' ';
	bool placing = true;
	int i_from = -1;
	int j_from = -1;
	int i_to = -1;
	int j_to = -1;
};

#endif