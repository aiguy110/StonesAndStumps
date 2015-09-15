#ifndef COMPUTER_PLAYER_H
#define COMPUTER_PLAYER_H

#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include "player.h"
#include "move.h"
#include "situation.h"

class ComputerPlayer : public Player{
public:
	std::vector<Situation> past_memory;
	std::vector<Situation> recent_memory;

	ComputerPlayer(char token, std::string mem_filename);
	Move GetMove(Board &board);
	void NotifyGameResult(GameResult result);
	void NotifyMove(Board &board, Move move);
	void WriteMemory(std::ostream &out);
	void ReadMemory(std::istream &in);
private:
	std::string mem_filename;

	void FinalizeMemory(bool won);
	void MergeMemory();
	Move FindIntermediateMove(Board board_i, Board board_f);
	Move GetRandomMove(Board &board);
};

#endif
