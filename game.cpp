#include <iostream>
#include <string>
#include "game.h"
#include "move.h"

using namespace std;

Game::Game(Player &p1, Player &p2, ostream &out){
	this->current_player = 0;
	this->playerPtrs[0] = &p1;
	this->playerPtrs[1] = &p2;
	this->out = &out;
}

GameResult Game::PlayTurn(){
	// Update the screen
	this->ClearScreen();
	this->board.Display(*(this->out));

	// Get the move
	Move move = this->playerPtrs[this->current_player]->GetMove(this->board);
	
	// Notify players of the move
	this->playerPtrs[0]->NotifyMove(board, move);
	this->playerPtrs[1]->NotifyMove(board, move);

	// Do move
	this->board.DoMove(move);
	this->current_player = 1 - this->current_player;

	// See if the game is over
	GameResult result = this->CheckResult();
	switch (result)
	{
	case P1_WIN:
		*(this->out) << "Game Over: \"" << this->playerPtrs[0]->my_token << "\"s Win!";
		break;
	case P2_WIN:
		*(this->out) << "Game Over: \"" << this->playerPtrs[1]->my_token << "\"s Win!";
		break;
	case CATS_GAME:
		*(this->out) << "Game Over: Cat's Game.";
		break;
	default:
		break;
	}
	
	return result;
}

GameResult Game::CheckResult(){
	// Get player tokens
	char p1_token = this->playerPtrs[0]->my_token;
	char p2_token = this->playerPtrs[1]->my_token;

	// Check Wins
	if (this->board.TokenWins(p1_token))
		return P1_WIN;
	if (this->board.TokenWins(p2_token))
		return P2_WIN;

	// Check Cats
	int pieceCount = this->board.CountToken(p1_token) + this->board.CountToken(p2_token);
	if (pieceCount == 9)
		return CATS_GAME;

	return UNRESOLVED;
}

void Game::ClearScreen(){
	*(this->out) << string(50, '\n') << endl;
}

