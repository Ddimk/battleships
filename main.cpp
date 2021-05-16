#include "Arbitre.h"
#include "GameUI.h"
#include "AIOpponent.h"

Arbitre *game;

// Will be called by players.
void do_game_step()
{
    game->do_actions();
}

int main(void)
{
    GameUI a;
    AIOpponent b;
    game = new Arbitre(&a, &b);
    a.set_game_step(do_game_step);
    game->reset_game();

    a.run();

    return 0;
}
