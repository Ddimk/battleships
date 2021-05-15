#pragma once
#include "Player.h"
class GameConsole : public Player
{
public:
    void on_win();
    void on_loose();
    void start_place_ships(ship_def ships[10]);
    void on_step(const bf_tile my[10][10], const bf_tile enemy[10][10], pos2d *result);
};
