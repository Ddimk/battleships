#include "GameUI.h"

GameUI::GameUI()
    : TProgInit(&GameUI::initStatusLine, &GameUI::initMenuBar, &GameUI::initDeskTop)
{
    is_need_respond = true;
}

void GameUI::on_win()
{
    is_need_respond = true;
}

void GameUI::on_loose()
{
    is_need_respond = true;
}

void GameUI::start_place_ships(ship_def ships[10])
{
    is_need_respond = true;
}

void GameUI::on_step(const bf_tile my[10][10], const bf_tile enemy[10][10], pos2d *result)
{
    is_need_respond = true;
}

void GameUI::need_reset()
{
    responded();
}