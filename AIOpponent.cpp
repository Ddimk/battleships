#include "AIOpponent.h"
#include <iostream>
#include <vector>
#include <cstdlib>
using namespace std;

void AIOpponent::on_win()
{
}

void AIOpponent::on_loose()
{
}

void AIOpponent::start_place_ships(ship_def ships[10])
{
    bool is_finish;
    do
    {
        is_finish = true;
        bool bf[10][10];
        for (int i = 0; i < 10; i++)
            for (int j = 0; j < 10; j++)
                bf[i][j] = true;
        for (int i = 0; i < 10; i++)
        {
            bool is_placed = false;
            while (!is_placed && is_finish)
            {
                bool can_add = true;
                if (rand() % 2)
                {
                    ships[i].rot = HORIZONTAL;
                    ships[i].x = rand() % (11 - ships[i].size);
                    ships[i].y = rand() % 10;
                    for (int x = ships[i].x; x < ships[i].x + ships[i].size; x++)
                    {
                        can_add = can_add && bf[x][ships[i].y];
                    }
                }
                else
                {
                    ships[i].rot = VERTICAL;
                    ships[i].x = rand() % 10;
                    ships[i].y = rand() % (11 - ships[i].size);
                    for (int y = ships[i].y; y < ships[i].y + ships[i].size; y++)
                    {
                        can_add = can_add && bf[ships[i].x][y];
                    }
                }
                if (can_add)
                {
                    is_placed = true;
                    if (ships[i].rot == HORIZONTAL)
                    {
                        for (int x = max(0, ships[i].x - 1);
                             x < min(10, ships[i].x + ships[i].size + 1); x++)
                        {
                            for (int y = max(0, ships[i].y - 1); y < min(10, ships[i].y + 3); y++)
                            {
                                bf[x][y] = false;
                            }
                        }
                    }
                    else
                    {
                        for (int x = max(0, ships[i].x - 1); x < min(10, ships[i].x + 3); x++)
                        {
                            for (int y = max(0, ships[i].y - 1);
                                 y < min(10, ships[i].y + ships[i].size + 1); y++)
                            {
                                bf[x][y] = false;
                            }
                        }
                    }
                }
                else
                {
                    is_finish = false;
                    for (int x = 0; x < 10; x++)
                    {
                        int count = 0;
                        for (int y = 0; y < 10; y++)
                        {
                            if (bf[x][y])
                            {
                                count++;
                                if (count == ships[i].size)
                                {
                                    is_finish = true;
                                    break;
                                }
                            }
                            else
                            {
                                count = 0;
                            }
                        }
                        if (is_finish)
                        {
                            break;
                        }
                    }
                    if (!is_finish)
                    {
                        for (int y = 0; y < 10; y++)
                        {
                            int count = 0;
                            for (int x = 0; x < 10; x++)
                            {
                                if (bf[x][y])
                                {
                                    count++;
                                    if (count == ships[i].size)
                                    {
                                        is_finish = true;
                                        break;
                                    }
                                }
                                else
                                {
                                    count = 0;
                                }
                            }
                            if (is_finish)
                            {
                                break;
                            }
                        }
                    }
                }
            }
            if (!is_finish)
            {
                break;
            }
        }
    } while (!is_finish);
    responded();
}

void AIOpponent::on_step(const bf_tile my[10][10], const bf_tile enemy[10][10], pos2d* result)
{
    pos2d res;

    vector<pos2d> pass_3;
    vector<pos2d> pass_1;
    vector<pos2d> pass_0;

    for (int x = 0; x < 10; x++)
    {
        for (int y = 0; y < 10; y++)
        {
            if (enemy[x][y] == HITTED)
            {
                bool has_nearby_hit = false;
                enum rotation rot;
                vector<pos2d> shoot_near;
                if (x < 9)
                {
                    if (enemy[x + 1][y] == HITTED)
                    {
                        has_nearby_hit = true;
                        rot = HORIZONTAL;
                    }
                    else if (enemy[x + 1][y] == UNKNOWN)
                    {
                        res.x = x + 1;
                        res.y = y;
                        shoot_near.push_back(res);
                    }
                }
                if (y > 0)
                {
                    if (enemy[x][y - 1] == HITTED)
                    {
                        has_nearby_hit = true;
                        rot = VERTICAL;
                    }
                    else if (enemy[x][y - 1] == UNKNOWN)
                    {
                        res.x = x;
                        res.y = y - 1;
                        shoot_near.push_back(res);
                    }
                }
                if (x > 0)
                {
                    if (enemy[x - 1][y] == HITTED)
                    {
                        has_nearby_hit = true;
                        rot = HORIZONTAL;
                    }
                    else if (enemy[x - 1][y] == UNKNOWN)
                    {
                        res.x = x - 1;
                        res.y = y;
                        shoot_near.push_back(res);
                    }
                }
                if (y < 9)
                {
                    if (enemy[x][y + 1] == HITTED)
                    {
                        has_nearby_hit = true;
                        rot = VERTICAL;
                    }
                    else if (enemy[x][y + 1] == UNKNOWN)
                    {
                        res.x = x;
                        res.y = y + 1;
                        shoot_near.push_back(res);
                    }
                }
                if (has_nearby_hit)
                {
                    for (int i = 1; i < 4; i++)
                    {
                        if (rot == HORIZONTAL)
                        {
                            if (x + i < 10 && enemy[x + i][y] == UNKNOWN)
                            {
                                result->x = x + i;
                                result->y = y;
                                responded();
                                return;
                            }
                            if (x - i > 0 && enemy[x - i][y] == UNKNOWN)
                            {
                                result->x = x - i;
                                result->y = y;
                                responded();
                                return;
                            }
                        }
                        else
                        {
                            if (y + i < 10 && enemy[x][y + i] == UNKNOWN)
                            {
                                result->x = x;
                                result->y = y + i;
                                responded();
                                return;
                            }
                            if (y - i > 0 && enemy[x][y - i] == UNKNOWN)
                            {
                                result->x = x;
                                result->y = y - i;
                                responded();
                                return;
                            }
                        }
                    }
                }
                else
                {
                    *result = shoot_near[rand() % shoot_near.size()];
                    responded();
                    return;
                }
            }

            if (enemy[x][y] == UNKNOWN)
            {
                pos2d tmp;
                tmp.x = x;
                tmp.y = y;
                if ((x - y) % 4 == 0)
                {
                    pass_3.push_back(tmp);
                }
                else if ((x - y) % 2 == 0)
                {
                    pass_1.push_back(tmp);
                }
                else
                {
                    pass_0.push_back(tmp);
                }
            }
        }
    }

    if (pass_3.size())
    {
        *result = pass_3[rand() % pass_3.size()];
        responded();
        return;
    }
    else if (pass_1.size())
    {
        *result = pass_1[rand() % pass_1.size()];
        responded();
        return;
    }
    else
    {
        *result = pass_0[rand() % pass_0.size()];
        responded();
        return;
    }
}
