#pragma once
#include "Player.h"

#define Uses_TKeys
#define Uses_TApplication
#define Uses_TEvent
#define Uses_TRect
#define Uses_TDialog
#define Uses_TParamText
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
    TParamText *my_field[21];

public:
    GameWindow();
    void close() { hide(); };
    void print_field(const bf_tile my[10][10], const bf_tile enemy[10][10]);
};

class GameUI : public Player, public TApplication
{
private:
    bool is_need_respond;
    GameWindow *display_battlefield;

    static TMenuBar *initMenuBar(TRect r);
    void _on_step();
    void _on_place();

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
