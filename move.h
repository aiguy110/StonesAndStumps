#ifndef MOVE_H
#define MOVE_H

struct Move{
	char player_token;
	bool placing;
	int i_from, j_from;
	int i_to, j_to;
};

#endif