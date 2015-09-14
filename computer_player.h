#ifndef COMPUTER_PLAYER_H
#define COMPUTER_PLAYER_H

#include <vector>
#include <cstdlib>
#include <ctime>
#include "player.h"
#include "move.h"
#include "situation.h"

class ComputerPlayer : public Player{
public:
	std::vector<Situation> past_memorys;
	std::vector<Situation> recent_memory;

	ComputerPlayer(char token);
	Move GetMove(Board &board);
	void NotifyGameResult(GameResult result);
private:
	void FinalizeMemory(bool won);
	Move FindIntermediateMove(Board board_i, Board board_f);
	Move GetRandomMove(Board &board);
};

#endif
