#pragma once
#include "Player.h"

class Arbitre
{
private:
    enum game_state
    {
        NEED_INIT,
        PLACING,
        GAME_STEP,
        GAME_ENDED
    };

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
    void prepare_bf();
    bool is_player_responded[2];
    pos2d step;
    int current_player;
    game_state state;

public:
    Arbitre(Player *a, Player *b);
    static bool is_correct_placement(const ship_def ships[10]);
    void reset_game()
    {
        state = GAME_ENDED;
        is_player_responded[0] = false;
        is_player_responded[1] = false;
        players[0]->need_reset();
        players[1]->need_reset();
        do_actions();
    };
    void do_actions();
};
