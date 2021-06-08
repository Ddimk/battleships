#include "Arbitre.h"
#include "GameUI.h"
#include "AIOpponent.h"

Arbitre *game;

// Will be called by players.
void do_game_step()
{
    game->do_actions();
}

void do_reset_game(int x, int y)
{
    game->reset_game(x, y);
}

int main(void)
{
    GameUI a;
    AIOpponent b;
    game = new Arbitre(&a, &b);
    a.set_game_step(do_game_step);
    a.set_game_reset(do_reset_game);

    a.run();

    return 0;
}
