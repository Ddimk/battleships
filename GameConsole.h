#pragma once
#include "Player.h"
class GameConsole : public Player
{
public:
    void on_win();
    void on_loose();
    void start_place_ships(ship_def ships[10]);
    void on_step(const bf_tile my[20][20], const bf_tile enemy[20][20], pos2d *result);
};
