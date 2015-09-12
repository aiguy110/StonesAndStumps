#include <iostream>
#include <string>
#include "game.h"

using namespace std;

int main(){
	HumanPlayer p1('X', cin, cout);
	ComputerPlayer p2('O');

	Game game(p1, p2, cout);
	while (game.PlayTurn() == UNRESOLVED)
		continue;
	
	char temp;
	cin >> temp;

	return 0;
}
