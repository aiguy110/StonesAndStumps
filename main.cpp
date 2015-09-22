#include <iostream>
#include <fstream>
#include <string>
#include "game.h"
#include "player.h"
#include "computer_player.h"

using namespace std;

int main(){
	// Run lots of games
	for (int n = 0; n < 3000; n++){
		// Setup game
		//ComputerPlayer p1('O', "bot_memory.mem");
		HumanPlayer p1('O', cin, cout);
		ComputerPlayer p2('X', "bot_memory.mem");

		Game game(p1, p2, cout);

		// Run game
		GameResult result = UNRESOLVED;
		while (true){
			// Draw
			game.ClearScreen();
			cout << "Game#: " << n+1 << endl;
			game.board.Display(cout);

			// Play turn
			result = game.PlayTurn();
			if (result != UNRESOLVED)
				break;
		}


		// Notify Players of the result; Swap results for player two since each player thinks of themself as P1.
		p1.NotifyGameResult(result);
		if (result == P1_WIN)
			p2.NotifyGameResult(P2_WIN);
		else if (result == P2_WIN)
			p2.NotifyGameResult(P1_WIN);
		else
			p2.NotifyGameResult(CATS_GAME);
	}

	// Wait for input before ending program
	cout << endl << "Enter a character to exit..." << endl;
	char temp;
	cin >> temp;

	return 0;
}
