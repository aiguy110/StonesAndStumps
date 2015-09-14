#ifndef COMPUTER_PLAYER_H
#define COMPUTER_PLAYER_H

#include <vector>
#include "player.h"
#include "move.h"
#include "situation.h"

class ComputerPlayer : public Player{
public:
	std::vector<Situation> past_memorys;
	std::vector<Situation> recent_memory;

	ComputerPlayer(char token) : Player(token){}
	Move GetMove(Board &board);
private:
	Move GetRandomMove(Board &board);
};

#endif
