#ifndef MOVE_H
#define MOVE_H

struct Move{
	char player_token = ' ';
	bool placing = true;
	int i_from = -1;
	int j_from = -1;
	int i_to = -1;
	int j_to = -1;

	bool Match(Move &move){
		if (move.player_token == player_token &&
			move.placing == placing &&
			move.i_to == i_to &&
			move.j_to == j_to &&
			move.i_from == i_from &&
			move.j_from == j_from)
			return true;
		return false;
	}
};

#endif