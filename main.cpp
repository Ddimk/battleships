#include "Arbitre.h"
#include "GameConsole.h"

int main(void)
{
	GameConsole a, b;
	Arbitre game(&a, &b);
	game.reset_game();

	return 0;
}