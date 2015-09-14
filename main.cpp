#include <iostream>
#include <fstream>
#include <string>
#include "game.h"
#include "player.h"
#include "computer_player.h"

using namespace std;

int main(){
	// Setup game
	HumanPlayer p1('X', cin, cout);
	ComputerPlayer p2('O', "bot2_mem.mem");

	Game game(p1, p2, cout);

	// Run game
	GameResult result = UNRESOLVED;
	while (true){
		result = game.PlayTurn();
		if (result != UNRESOLVED)
			break;
	}
	
	cout << "Check longterm memory: " << p2.past_memory.back().board.grid;

	// Notify Players of the result; Swap results for player two since each player thinks of themself as P1.
	p1.NotifyGameResult(result);
	if (result == P1_WIN)
		p2.NotifyGameResult(P2_WIN);
	else if (result == P2_WIN)
		p2.NotifyGameResult(P1_WIN);
	else
		p2.NotifyGameResult(CATS_GAME);

	// Wait for input before ending program
	char temp;
	cin >> temp;

	return 0;
}
