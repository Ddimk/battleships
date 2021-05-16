#pragma once
#include "Player.h"

#define Uses_TApplication
#include <tvision/tv.h>
#include <iostream>

class GameUI : public Player, public TApplication
{
private:
    bool is_need_respond;

    void _on_step();
    void _on_place();
public:
    GameUI();
    void idle() { do_game_step(); };
    void on_win();
    void on_loose();
    void start_place_ships(ship_def ships[10]);
    void on_step(const bf_tile my[10][10], const bf_tile enemy[10][10], pos2d *result);
    void need_reset();
};
