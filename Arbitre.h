#pragma once
#include "Player.h"

class Arbitre
{
private:
    Player *players[2];
    bf_tile my[2][10][10];
    bf_tile enemy[2][10][10];
    ship_def ships[2][10] = { { { 4, HORIZONTAL, 0, 0 },
                                { 3, HORIZONTAL, 0, 0 },
                                { 3, HORIZONTAL, 0, 0 },
                                { 2, HORIZONTAL, 0, 0 },
                                { 2, HORIZONTAL, 0, 0 },
                                { 2, HORIZONTAL, 0, 0 },
                                { 1, HORIZONTAL, 0, 0 },
                                { 1, HORIZONTAL, 0, 0 },
                                { 1, HORIZONTAL, 0, 0 },
                                { 1, HORIZONTAL, 0, 0 } },
                              { { 4, HORIZONTAL, 0, 0 },
                                { 3, HORIZONTAL, 0, 0 },
                                { 3, HORIZONTAL, 0, 0 },
                                { 2, HORIZONTAL, 0, 0 },
                                { 2, HORIZONTAL, 0, 0 },
                                { 2, HORIZONTAL, 0, 0 },
                                { 1, HORIZONTAL, 0, 0 },
                                { 1, HORIZONTAL, 0, 0 },
                                { 1, HORIZONTAL, 0, 0 },
                                { 1, HORIZONTAL, 0, 0 } } };
    int hp[2];
    bool end;
    void prepare_bf();

public:
    Arbitre(Player *a, Player *b);
    static bool is_correct_placement(const ship_def ships[10]);
    void reset_game();
};
