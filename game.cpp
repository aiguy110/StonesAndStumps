#include <iostream>
#include "game.h"
#include "player.h"
#include "move.h"

using namespace std;

Game::Game(Player &p1, Player &p2, ostream &out){
	this->current_player = 0;
	this->players[0] = p1;
	this->players[1] = p2;
	this->out = &out;
}

GameResult Game::PlayTurn(){
	// Make the move
	Move move = this->players[this->current_player].GetMove(this->board);
	this->board.DoMove(move);
	this->current_player = 1 - this->current_player;

	// Update the screen
	this->ClearScreen();
	this->board.Display(*(this->out));

	return UNRESOLVED;
}

void Game::ClearScreen(){
	*(this->out) << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" << endl;
}

