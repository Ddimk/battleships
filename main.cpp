#include "Arbitre.h"
#include "GameConsole.h"
//#include "GameUI.h"
#include "AIOpponent.h"

#define Uses_TApplication
#define Uses_TStaticText
#define Uses_TButton

//#include <tvision/tv.h>


int main(void)
{
    GameConsole a;
    AIOpponent b;
    Arbitre game(&a, &b);
    game.reset_game();

    while (true)
    {
        game.do_actions();
    }

    //THelloApp helloWorld;
    //helloWorld.run();

    return 0;
}
