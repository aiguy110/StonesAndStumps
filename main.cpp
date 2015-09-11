#include <iostream>
#include <string>
#include "game.h"

using namespace std;

int main(){
	HumanPlayer p1('X', cin, cout);
	HumanPlayer p2('O', cin, cout);

	Game game(p1, p2, cout);
	while (true)
	{
		game.PlayTurn();
	}
	
	char temp;
	cin >> temp;

	return 0;
}