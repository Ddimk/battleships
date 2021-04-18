#include "Arbitre.h"
#include "GameConsole.h"
#include "AIOpponent.h"

int main(void)
{
	GameConsole a;
	AIOpponent b;
	Arbitre game(&a, &b);
	game.reset_game();

	return 0;
}