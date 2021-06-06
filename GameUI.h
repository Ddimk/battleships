#pragma once
#include "Player.h"

#define Uses_TKeys
#define Uses_TApplication
#define Uses_TEvent
#define Uses_TRect
#define Uses_TDialog
#define Uses_TParamText
#define Uses_TStaticText
#define Uses_TButton
#define Uses_TMenuBar
#define Uses_TSubMenu
#define Uses_TMenuItem
#define Uses_TStatusLine
#define Uses_TStatusItem
#define Uses_TStatusDef
#define Uses_TDeskTop
#define Uses_TWindow
#include <tvision/tv.h>
#include <iostream>

class GameWindow : public TWindow
{
private:
    TStaticText *labels[41];
    TParamText *my_field[20][20];
    TParamText *other_field[20][20];

public:
    GameWindow();
    void close() { hide(); };
    void print_field(const bf_tile my[10][10], const bf_tile enemy[10][10]);
};

class GameUI : public Player, public TApplication
{
private:
    bool is_step;
    bool is_place;
    GameWindow *display_battlefield;
    ship_def *my_ships;
    int current_ship;
    pos2d *step;

    static TMenuBar *initMenuBar(TRect r);
    void _on_step(int x, int y);
    void _on_place(int x, int y);

public:
    GameUI();
    virtual void handleEvent(TEvent &event);
    void idle() { do_game_step(); };
    void on_win();
    void on_loose();
    void start_place_ships(ship_def ships[10]);
    void on_step(const bf_tile my[10][10], const bf_tile enemy[10][10], pos2d *result);
    void need_reset();
};
