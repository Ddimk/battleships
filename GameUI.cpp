#include "GameUI.h"

GameWindow::GameWindow()
    : TWindow(TProgram::deskTop->getExtent(), "Battlefield", 0)
    , TWindowInit(&GameWindow::initFrame)
{
    options |= ofTileable;
    TRect r = getExtent();
    my_field[0] = new TParamText(TRect(r.a.x + 1, r.a.y + 1, r.a.x + 130, r.a.y + 2));
    my_field[0]->setText(
        "   A  B  C  D  E  F  G  H  I  J  K  L  M  N  O  P  Q  R  S  T       A  B  C  D  E  F  G  "
        "H  I  J  K  L  M  N  O  P  Q  R  S  T");

    for (int i = 1; i < 21; i++)
    {
        char buf[128];
        snprintf(buf, 128, "%2d                                                               %2d",
                 i, i);
        my_field[i] = new TParamText(TRect(r.a.x + 1, r.a.y + 1 + i, r.a.x + 130, r.a.y + 2 + i));
        my_field[i]->setText(buf);
    }
    for (int i = 0; i < 21; i++)
    {
        insert(my_field[i]);
    }

    // insert(new TButton(TRect(r.a.x + 25, r.a.y + 25, r.a.x + 29, r.a.y + 27), "😂", 100,
    // bfNormal));
}

void GameWindow::print_field(const bf_tile my[10][10], const bf_tile enemy[10][10])
{
    for (int i = 1; i < 11; i++)
    {
        char buf[128];
        snprintf(
            buf, 128,
            "%2d                                                               %2d             "
            "         "
            "                                     ",
            i, i);
        /*for (int x = 0; x < 10; x++)
        {
            buf[3 + x * 3] = '?';
        }*/

        for (int x = 0; x < 10; x++)
        {
            char c;
            switch (my[x][i - 1])
            {
            case UNKNOWN:
                c = '~';
                break;
            case HITTED:
                c = '@';
                break;
            case DROWNED:
                c = 'X';
                break;
            case MISS:
                c = '*';
                break;
            case USELESS:
                c = '-';
                break;
            case INTACT:
                c = '#';
                break;
            }
            buf[3 + x * 3] = c;
        }

        for (int x = 0; x < 10; x++)
        {
            char c;
            switch (enemy[x][i - 1])
            {
            case UNKNOWN:
                c = '~';
                break;
            case HITTED:
                c = '@';
                break;
            case DROWNED:
                c = 'X';
                break;
            case MISS:
                c = '*';
                break;
            case USELESS:
                c = '-';
                break;
                /*case INTACT:
                        c = '#';
                        break;*/
            }
            buf[68 + x * 3] = c;
        }

        my_field[i]->setText(buf);
    }
}

GameUI::GameUI()
    : TProgInit(&GameUI::initStatusLine, &GameUI::initMenuBar, &GameUI::initDeskTop)
{
    TRect r = getExtent();
    r.a.y += 1;
    r.b.y -= 1;
    display_battlefield = new GameWindow();
    deskTop->insert(display_battlefield);
    display_battlefield->hide();
    // my_field[0][0] = new TButton(TRect(r.a.x, r.a.y, r.a.x + 5, r.a.y + 5), "X", 100, bfNormal);
    // my_field[0][0]->mapColor(0xfefefe);
    is_need_respond = true;
}

TMenuBar *GameUI::initMenuBar(TRect r)
{
    r.b.y = r.a.y + 1;

    return new TMenuBar(r, *new TSubMenu("Menu", kbAltH) + *new TMenuItem("New Game", 100, kbAltG) +
                               newLine() +
                               *new TMenuItem("E~x~it", cmQuit, cmQuit, hcNoContext, "Alt-X"));
}

void GameUI::handleEvent(TEvent &event)
{
    TApplication::handleEvent(event);
    if (event.what == evCommand)
    {
        switch (event.message.command)
        {
        case 100:

            display_battlefield->show();

            clearEvent(event);
            break;
        default:
            break;
        }
    }
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
    bf_tile my[10][10];
    bf_tile enemy[10][10];
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            my[i][j] = UNKNOWN;
            enemy[i][j] = UNKNOWN;
        }
    }

    display_battlefield->print_field(my, enemy);
    is_need_respond = true;
}

void GameUI::on_step(const bf_tile my[10][10], const bf_tile enemy[10][10], pos2d *result)
{
    display_battlefield->print_field(my, enemy);
    is_need_respond = true;
}

void GameUI::need_reset()
{
    responded();
}