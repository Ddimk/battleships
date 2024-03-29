#include "Arbitre.h"
#include <cstdlib>
#include <ctime>
#include <utility>
using namespace std;

Arbitre::Arbitre(Player *a, Player *b)
{
    players[0] = a;
    players[1] = b;
    srand(time(nullptr));
    players[0]->set_responding(&is_player_responded[0]);
    players[1]->set_responding(&is_player_responded[1]);
    // Do not ask players for reset as we call reset_game explicitely.
    state = GAME_ENDED;
    is_player_responded[0] = false;
    is_player_responded[1] = false;
}
bool Arbitre::is_correct_placement(const ship_def test_ships[10])
{
    bool tmp[20][20] = { 0 };

    for (int i = 0; i < 10; i++)
    {
        if (test_ships[i].x < 0 || test_ships[i].y < 0 ||
            (test_ships[i].rot == VERTICAL &&
             (test_ships[i].x > size_x - 1 || test_ships[i].y + test_ships[i].size > size_y)) ||
            (test_ships[i].rot == HORIZONTAL &&
             (test_ships[i].y > size_y - 1 || test_ships[i].x + test_ships[i].size > size_x)))
        {
            return false;
        }

        for (int l = -1; l < test_ships[i].size + 1; l++)
        {
            for (int w = -1; w < 2; w++)
            {
                int x, y;
                if (test_ships[i].rot == HORIZONTAL)
                {
                    x = test_ships[i].x + l;
                    y = test_ships[i].y + w;
                }
                else
                {
                    x = test_ships[i].x + w;
                    y = test_ships[i].y + l;
                }
                if (x < 0 || x > size_x - 1 || y < 0 || y > size_y - 1)
                {
                    continue;
                }
                if (tmp[x][y])
                {
                    return false;
                }
            }
        }
        for (int l = 0; l < test_ships[i].size; l++)
        {
            if (test_ships[i].rot == HORIZONTAL)
            {
                tmp[test_ships[i].x + l][test_ships[i].y] = true;
            }
            else
            {
                tmp[test_ships[i].x][test_ships[i].y + l] = true;
            }
        }
    }

    return true;
}
void Arbitre::do_actions()
{
    switch (state)
    {
        // Simply state with no user responces.
    case NEED_INIT: {
        current_player = rand() & 1;

        for (int i = 0; i < size_x; i++)
        {
            for (int j = 0; j < size_y; j++)
            {
                my[0][i][j] = UNKNOWN;
                my[1][i][j] = UNKNOWN;
                enemy[0][i][j] = UNKNOWN;
                enemy[1][i][j] = UNKNOWN;
            }
        }

        state = PLACING;
        is_player_responded[0] = false;
        is_player_responded[1] = false;

        // Start placing but not wait for completion (do it in next state).
        players[current_player]->start_place_ships(ships[current_player]);

        break;
    }
        // State which must be completed once per player.
    case PLACING: {
        if (is_player_responded[current_player])
        {
            if (is_correct_placement(ships[current_player]))
            {
                // This player placed ships. Done with him.
                current_player = 1 - current_player;
                if (is_player_responded[current_player])
                {
                    // Second player already correctly placed ships. We can start game.
                    /// TODO: Change when add more ships.
                    hp[0] = 20;
                    hp[1] = 20;

                    prepare_bf();
                    state = GAME_STEP;

                    is_player_responded[0] = false;
                    is_player_responded[1] = false;

                    // Request first player move.
                    players[current_player]->on_step(my[current_player], enemy[current_player],
                                                     &step);
                }
                else
                {
                    players[current_player]->start_place_ships(ships[current_player]);
                }
            }
            else
            {
                // Incorrect placement, require resubmitting.
                is_player_responded[current_player] = false;
                players[current_player]->start_place_ships(ships[current_player]);
            }
        }
        break;
    }
        // Player do single step.
    case GAME_STEP: {
        if (is_player_responded[current_player])
        {
            int side = current_player;
            int alt_side = 1 - side;

            if (step.x < 0 || step.x > size_x - 1 || step.y < 0 || step.y > size_y - 1)
            {
                // Out of field; immediately re-request player step.
                is_player_responded[current_player] = false;
                players[current_player]->on_step(my[current_player], enemy[current_player], &step);
                break; // end of GAME_STEP
            }

            switch (my[alt_side][step.x][step.y])
            {
                // Unknown state (shot on already marked cell).
            default: {
                break;
            }
                // Player miss. Change sides.
            case UNKNOWN: {
                my[alt_side][step.x][step.y] = MISS;
                enemy[side][step.x][step.y] = MISS;
                swap(side, alt_side);
                break;
            }
                // Hit enemy ship.
            case INTACT:
                my[alt_side][step.x][step.y] = HITTED;
                enemy[side][step.x][step.y] = HITTED;

                bool is_fully_drown = true;
                // Check in every side if ship drowned.
                for (int i = 1; i < 4 && step.y - i >= 0; i++)
                {
                    if (my[alt_side][step.x][step.y - i] != HITTED)
                    {
                        if (my[alt_side][step.x][step.y - i] == INTACT)
                        {
                            is_fully_drown = false;
                        }
                        break;
                    }
                }
                for (int i = 1; i < 4 && step.y + i < size_y; i++)
                {
                    if (my[alt_side][step.x][step.y + i] != HITTED)
                    {
                        if (my[alt_side][step.x][step.y + i] == INTACT)
                        {
                            is_fully_drown = false;
                        }
                        break;
                    }
                }
                for (int i = 1; i < 4 && step.x - i >= 0; i++)
                {
                    if (my[alt_side][step.x - i][step.y] != HITTED)
                    {
                        if (my[alt_side][step.x - i][step.y] == INTACT)
                        {
                            is_fully_drown = false;
                        }
                        break;
                    }
                }
                for (int i = 1; i < 4 && step.x + i < size_x; i++)
                {
                    if (my[alt_side][step.x + i][step.y] != HITTED)
                    {
                        if (my[alt_side][step.x + i][step.y] == INTACT)
                        {
                            is_fully_drown = false;
                        }
                        break;
                    }
                }
                // Mark ship and area near it in case if it fully drown.
                if (is_fully_drown)
                {
                    enemy[side][step.x][step.y] = DROWNED;
                    my[alt_side][step.x][step.y] = DROWNED;
                    for (int i = 1; i < 5 && step.y - i >= 0; i++)
                    {
                        if (my[alt_side][step.x][step.y - i] == HITTED)
                        {
                            enemy[side][step.x][step.y - i] = DROWNED;
                            my[alt_side][step.x][step.y - i] = DROWNED;
                        }
                        else
                        {
                            if (my[alt_side][step.x][step.y - i] == UNKNOWN)
                            {
                                enemy[side][step.x][step.y - i] = USELESS;
                            }
                            break;
                        }
                    }
                    for (int i = 1; i < 5 && step.y + i < size_y; i++)
                    {
                        if (my[alt_side][step.x][step.y + i] == HITTED)
                        {
                            enemy[side][step.x][step.y + i] = DROWNED;
                            my[alt_side][step.x][step.y + i] = DROWNED;
                        }
                        else
                        {
                            if (my[alt_side][step.x][step.y + i] == UNKNOWN)
                            {
                                enemy[side][step.x][step.y + i] = USELESS;
                            }
                            break;
                        }
                    }
                    for (int i = 1; i < 5 && step.x - i >= 0; i++)
                    {
                        if (my[alt_side][step.x - i][step.y] == HITTED)
                        {
                            enemy[side][step.x - i][step.y] = DROWNED;
                            my[alt_side][step.x - i][step.y] = DROWNED;
                        }
                        else
                        {
                            if (my[alt_side][step.x - i][step.y] == UNKNOWN)
                            {
                                enemy[side][step.x - i][step.y] = USELESS;
                            }
                            break;
                        }
                    }
                    for (int i = 1; i < 5 && step.x + i < size_x; i++)
                    {
                        if (my[alt_side][step.x + i][step.y] == HITTED)
                        {
                            enemy[side][step.x + i][step.y] = DROWNED;
                            my[alt_side][step.x + i][step.y] = DROWNED;
                        }
                        else
                        {
                            if (my[alt_side][step.x + i][step.y] == UNKNOWN)
                            {
                                enemy[side][step.x + i][step.y] = USELESS;
                            }
                            break;
                        }
                    }
                }
                if (step.x > 0 && step.y > 0 && my[alt_side][step.x - 1][step.y - 1] == UNKNOWN)
                {
                    enemy[side][step.x - 1][step.y - 1] = USELESS;
                }
                if (step.x < size_x - 1 && step.y > 0 &&
                    my[alt_side][step.x + 1][step.y - 1] == UNKNOWN)
                {
                    enemy[side][step.x + 1][step.y - 1] = USELESS;
                }
                if (step.x < size_x - 1 && step.y < size_y - 1 &&
                    my[alt_side][step.x + 1][step.y + 1] == UNKNOWN)
                {
                    enemy[side][step.x + 1][step.y + 1] = USELESS;
                }
                if (step.x > 0 && step.y < size_y - 1 &&
                    my[alt_side][step.x - 1][step.y + 1] == UNKNOWN)
                {
                    enemy[side][step.x - 1][step.y + 1] = USELESS;
                }
                hp[alt_side] -= 1;
                break;
            }

            if (hp[alt_side] == 0)
            {
                // End of match.
                is_player_responded[0] = false;
                is_player_responded[1] = false;

                players[0]->need_reset(size_x, size_y);
                players[1]->need_reset(size_x, size_y);

                state = GAME_ENDED;
                players[side]->on_win();
                players[alt_side]->on_loose();
            }
            else
            {
                current_player = side;
                is_player_responded[current_player] = false;
                players[current_player]->on_step(my[current_player], enemy[current_player], &step);
            }
            break;
        }
    }
        // Do nothing; wait for game resart.
    case GAME_ENDED: {
        if (is_player_responded[0] && is_player_responded[1])
        {
            // Both of players want a new game.
            state = NEED_INIT;
        }
        break;
    }
    }
}

void Arbitre::prepare_bf()
{
    for (int p = 0; p < 2; p++)
    {
        for (int i = 0; i < 10; i++)
        {
            for (int l = 0; l < ships[p][i].size; l++)
            {
                if (ships[p][i].rot == HORIZONTAL)
                {
                    my[p][ships[p][i].x + l][ships[p][i].y] = INTACT;
                }
                else
                {
                    my[p][ships[p][i].x][ships[p][i].y + l] = INTACT;
                }
            }
        }
    }
}
