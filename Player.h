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
    int size;
    rotation rot;
    int x, y;
} ship_def;

typedef struct
{
    int x, y;
} pos2d;

class Player
{
private:
    bool *_responded = nullptr;
    void (*_do_game_step)() = nullptr;
    void (*_do_game_reset)(int x, int y) = nullptr;

protected:
    int size_x;
    int size_y;

public:
    void set_responding(bool *respond) { this->_responded = respond; }
    void set_game_step(void (*callback)()) { this->_do_game_step = callback; }
    void set_game_reset(void (*callback)(int x, int y)) { this->_do_game_reset = callback; }
    virtual void need_reset(int x, int y)
    {
        size_x = x;
        size_y = y;
        responded();
    }
    virtual void on_win() = 0;
    virtual void on_loose() = 0;
    virtual void start_place_ships(ship_def ships[10]) = 0;
    virtual void on_step(const bf_tile my[20][20], const bf_tile enemy[20][20], pos2d *result) = 0;

protected:
    void responded()
    {
        if (_responded)
        {
            *_responded = true;
        }
    }
    void do_game_step()
    {
        if (_do_game_step)
        {
            _do_game_step();
        }
    }
    void do_game_reset(int x, int y)
    {
        if (_do_game_reset)
        {
            _do_game_reset(x, y);
        }
    }
};
