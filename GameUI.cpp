#include "GameUI.h"

GameWindow::GameWindow()
    : TWindow(TProgram::deskTop->getExtent(), "Battlefield", 0)
    , TWindowInit(&GameWindow::initFrame)
{
    options |= ofTileable;
    TRect r = getExtent();
    labels[0] = new TStaticText(
        TRect(r.a.x + 1, r.a.y + 1, r.a.x + 88, r.a.y + 2),
        "   A B C D E F G H I J K L M N O P Q R S T     A B C D E F G H I J K L M N O P Q R S T");

    for (int i = 0; i < 20; i++)
    {
        char buf[3];
        snprintf(buf, 3, "%2d", i + 1);
        labels[i + 1] = new TStaticText(TRect(r.a.x + 1, r.a.y + 2 + i, r.a.x + 3, r.a.y + 3 + i),
                                        buf);
        labels[i + 21] = new TStaticText(TRect(r.a.x + 45, r.a.y + 2 + i, r.a.x + 48,
                                               r.a.y + 3 + i),
                                         buf);
    }
    for (int i = 0; i < 41; i++)
    {
        insert(labels[i]);
    }

    for (int y = 0; y < 20; y++)
    {
        for (int x = 0; x < 20; x++)
        {
            my_field[x][y] = new TParamText(
                TRect(r.a.x + 4 + x * 2, r.a.y + 2 + y, r.a.x + 6 + x * 2, r.a.y + 3 + y));
            insert(my_field[x][y]);

            other_field[x][y] = new TParamText(
                TRect(r.a.x + 48 + x * 2, r.a.y + 2 + y, r.a.x + 50 + x * 2, r.a.y + 3 + y));
            insert(other_field[x][y]);
        }
    }
}

void GameWindow::print_field(const bf_tile my[10][10], const bf_tile enemy[10][10])
{
    static const char water[] = { 0xDB, 0xDB, 0 };
    static const char crushing[] = { '<', '>', 0 };
    static const char drowned[] = { 0x11, 0x10, 0 };
    static const char missed[] = { 0xB0, 0xB0, 0 };
    static const char useless[] = { 0xB1, 0xB1, 0 };
    static const char ship[] = { '[', ']', 0 };

    for (int y = 0; y < 10; y++)
    {
        for (int x = 0; x < 10; x++)
        {
            switch (my[x][y])
            {
            case UNKNOWN:
                my_field[x][y]->setText(water);
                break;
            case HITTED:
                my_field[x][y]->setText(crushing);
                break;
            case DROWNED:
                my_field[x][y]->setText(drowned);
                break;
            case MISS:
                my_field[x][y]->setText(missed);
                break;
            case INTACT:
                my_field[x][y]->setText("%s", ship);
                break;
            }

            switch (enemy[x][y])
            {
            case UNKNOWN:
                other_field[x][y]->setText(water);
                break;
            case HITTED:
                other_field[x][y]->setText(crushing);
                break;
            case DROWNED:
                other_field[x][y]->setText(drowned);
                break;
            case MISS:
                other_field[x][y]->setText(missed);
                break;
            case USELESS:
                other_field[x][y]->setText(useless);
                break;
            }
        }
    }
}

GameUI::GameUI()
    : TProgInit(&GameUI::initStatusLine, &GameUI::initMenuBar, &GameUI::initDeskTop)
    , is_place(false)
    , is_step(false)
    , current_ship(0)
{
    TRect r = getExtent();
    r.a.y += 1;
    r.b.y -= 1;
    display_battlefield = new GameWindow();
    deskTop->insert(display_battlefield);
    display_battlefield->hide();
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
    else if ((event.what == evMouseDown) && (event.mouse.buttons == mbLeftButton))
    {
        if ((event.mouse.where.x >= 4) && (event.mouse.where.x < 45) &&
            (event.mouse.where.y >= 3) && (event.mouse.where.y < 23) && is_place)
        {
            int x = (event.mouse.where.x - 4) / 2;
            int y = event.mouse.where.y - 3;
            // printf("NEED PLACE\n");
            _on_place(x, y);
        }
        if ((event.mouse.where.x >= 48) && (event.mouse.where.x < 89) &&
            (event.mouse.where.y >= 3) && (event.mouse.where.y < 23) && is_step)
        {
            int x = (event.mouse.where.x - 48) / 2;
            int y = event.mouse.where.y - 3;
            // printf("NEED STEP\n");
            _on_step(x, y);
        }
    }
}

void GameUI::_on_step(int x, int y)
{
    step->x = x;
    step->y = y;
    is_step = false;
    responded();
}

void GameUI::_on_place(int x, int y)
{
    my_ships[0].rot = HORIZONTAL;
    my_ships[0].x = 6;
    my_ships[0].y = 0;
    my_ships[1].rot = VERTICAL;
    my_ships[1].x = 3;
    my_ships[1].y = 0;
    my_ships[2].rot = VERTICAL;
    my_ships[2].x = 1;
    my_ships[2].y = 4;
    my_ships[3].rot = HORIZONTAL;
    my_ships[3].x = 0;
    my_ships[3].y = 0;
    my_ships[4].rot = VERTICAL;
    my_ships[4].x = 9;
    my_ships[4].y = 2;
    my_ships[5].rot = VERTICAL;
    my_ships[5].x = 8;
    my_ships[5].y = 8;
    my_ships[6].rot = HORIZONTAL;
    my_ships[6].x = 0;
    my_ships[6].y = 9;
    my_ships[7].rot = HORIZONTAL;
    my_ships[7].x = 3;
    my_ships[7].y = 8;
    my_ships[8].rot = HORIZONTAL;
    my_ships[8].x = 5;
    my_ships[8].y = 8;
    my_ships[9].rot = HORIZONTAL;
    my_ships[9].x = 6;
    my_ships[9].y = 5;

    is_place = false;
    responded();
}

void GameUI::on_win()
{
}

void GameUI::on_loose()
{
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
    current_ship = 0;
    /*for (int i = 0; i < 10; i++)
    {
        my_ships[i] = ships[i];
    }*/
    my_ships = ships;
    is_place = true;
}

void GameUI::on_step(const bf_tile my[10][10], const bf_tile enemy[10][10], pos2d *result)
{
    display_battlefield->print_field(my, enemy);
    step = result;
    is_step = true;
}

void GameUI::need_reset()
{
    responded();
}