#pragma once

enum bf_tile
{
    UNKNOWN,
    HITTED,
    DROWNED,
    MISS,
    USELESS,
    INTACT
};

enum rotation
{
    HORIZONTAL,
    VERTICAL
};

typedef struct
{
    const int size;
    rotation rot;
    int x, y;
} ship_def;

typedef struct
{
    int x, y;
} pos2d;

class Player
{
public:
    virtual void on_win() = 0;
    virtual void on_loose() = 0;
    virtual void start_place_ships(ship_def ships[10]) = 0;
    virtual pos2d on_step(const bf_tile my[10][10], const bf_tile enemy[10][10]) = 0;
};