#include "GameUI.h"

GameWindow::GameWindow()
    : TWindow(TProgram::deskTop->getExtent(), "Battlefield", 0)
    , TWindowInit(&GameWindow::initFrame)
{
    dragMode = 0;
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

void GameWindow::print_field(const bf_tile my[20][20], const bf_tile enemy[20][20], int size_x,
                             int size_y)
{
    static const char water[] = { (char)0xDB, (char)0xDB, 0 };
    static const char crushing[] = { '<', '>', 0 };
    static const char drowned[] = { 0x11, 0x10, 0 };
    static const char missed[] = { 0xB0, 0xB0, 0 };
    static const char useless[] = { 0xB1, 0xB1, 0 };
    static const char ship[] = { '[', ']', 0 };

    for (int y = 0; y < size_y; y++)
    {
        for (int x = 0; x < size_x; x++)
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
                               *new TMenuItem("Exit", cmQuit, cmQuit, hcNoContext, "Alt-X"));
}

void GameUI::handleEvent(TEvent &event)
{
    TApplication::handleEvent(event);
    if (event.what == evCommand)
    {
        switch (event.message.command)
        {
        case 100:
        {
            char data[2][3] = { "10", "10" };
            TDialog *d = new TDialog(TRect(25, 5, 55, 15), "New game");

            d->insert(new TInputLine(TRect(16, 2, 23, 3), 3));
            d->insert(new TInputLine(TRect(16, 4, 23, 5), 3));
            d->insert(new TStaticText(TRect(7, 2, 14, 3), "X size:"));
            d->insert(new TStaticText(TRect(7, 4, 14, 5), "Y size:"));
            d->insert(new TButton(TRect(5, 7, 25, 9), "Start new game", cmCancel, bfNormal));

            d->setData(data);
            deskTop->execView(d);
            d->getData(data);
            destroy(d);

            int x = max(10, min(20, atoi(data[0])));
            int y = max(10, min(20, atoi(data[1])));

            do_game_reset(x, y);
            display_battlefield->show();

            clearEvent(event);
            break;
        }
        default:
            break;
        }
    }
    else if (event.what == evMouseDown)
    {
        TPoint pos = display_battlefield->makeLocal(event.mouse.where);
        if ((pos.x >= 4) && (pos.x < 45) && (pos.y >= 2) && (pos.y < 22) && is_place)
        {
            int x = (pos.x - 4) / 2;
            int y = pos.y - 2;
            if (is_place)
            {
                _on_place(x, y, (event.mouse.buttons != mbLeftButton));
            }
        }
        if ((pos.x >= 48) && (pos.x < 89) && (pos.y >= 2) && (pos.y < 22) && is_step)
        {
            int x = (pos.x - 48) / 2;
            int y = pos.y - 2;
            if (is_step)
            {
                _on_step(x, y);
            }
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

void GameUI::_on_place(int pos_x, int pos_y, bool horizontal)
{
    if (current_ship == 0)
    {
        for (int i = 0; i < size_x; i++)
            for (int j = 0; j < size_y; j++)
                bf[i][j] = true;
    }

    if ((pos_x < 0) || (pos_x > size_x - 1) || (pos_y < 0) || (pos_y > size_y - 1))
    {
        return;
    }

    if ((horizontal && (pos_x + my_ships[current_ship].size > size_x)) ||
        ((!horizontal) && (pos_y + my_ships[current_ship].size > size_y)))
    {
        return;
    }

    bool can_add = true;

    my_ships[current_ship].x = pos_x;
    my_ships[current_ship].y = pos_y;
    if (horizontal)
    {
        my_ships[current_ship].rot = HORIZONTAL;
        for (int x = pos_x; x < pos_x + my_ships[current_ship].size; x++)
        {
            can_add = can_add && bf[x][pos_y];
        }
    }
    else
    {
        my_ships[current_ship].rot = VERTICAL;
        for (int y = pos_y; y < pos_y + my_ships[current_ship].size; y++)
        {
            can_add = can_add && bf[pos_x][y];
        }
    }
    if (!can_add)
    {
        return;
    }

    if (horizontal)
    {
        for (int x = max(0, pos_x - 1); x < min(size_x, pos_x + my_ships[current_ship].size + 1);
             x++)
        {
            for (int y = max(0, pos_y - 1); y < min(size_y, pos_y + 2); y++)
            {
                bf[x][y] = false;
            }
        }
    }
    else
    {
        for (int x = max(0, pos_x - 1); x < min(size_x, pos_x + 2); x++)
        {
            for (int y = max(0, pos_y - 1);
                 y < min(size_y, pos_y + my_ships[current_ship].size + 1); y++)
            {
                bf[x][y] = false;
            }
        }
    }

    if (horizontal)
    {
        for (int x = pos_x; x < pos_x + my_ships[current_ship].size; x++)
        {
            tmy[x][pos_y] = INTACT;
        }
    }
    else
    {
        for (int y = pos_y; y < pos_y + my_ships[current_ship].size; y++)
        {
            tmy[pos_x][y] = INTACT;
        }
    }

    display_battlefield->print_field(tmy, tenemy, size_x, size_y);

    if (current_ship == 9)
    {
        is_place = false;
        responded();
    }
    else
    {
        current_ship++;
    }
}

void GameUI::on_win()
{
    TDialog *d = new TDialog(TRect(25, 5, 55, 15), "You win!");

    d->insert(new TStaticText(TRect(10, 2, 25, 3), "You win!"));
    d->insert(new TButton(TRect(5, 5, 25, 9), "Start new game", cmCancel, bfNormal));

    deskTop->execView(d);
    destroy(d);
}

void GameUI::on_loose()
{
    TDialog *d = new TDialog(TRect(25, 5, 55, 15), "You lose.");

    d->insert(new TStaticText(TRect(10, 2, 25, 3), "You lose."));
    d->insert(new TButton(TRect(5, 5, 25, 9), "Start new game", cmCancel, bfNormal));

    deskTop->execView(d);
    destroy(d);
}

void GameUI::start_place_ships(ship_def ships[10])
{
    for (int i = 0; i < size_x; i++)
    {
        for (int j = 0; j < size_y; j++)
        {
            tmy[i][j] = UNKNOWN;
            tenemy[i][j] = UNKNOWN;
        }
    }

    display_battlefield->print_field(tmy, tenemy, size_x, size_y);
    current_ship = 0;
    my_ships = ships;
    is_place = true;
}

void GameUI::on_step(const bf_tile my[20][20], const bf_tile enemy[20][20], pos2d *result)
{
    display_battlefield->print_field(my, enemy, size_x, size_y);
    step = result;
    is_step = true;
}

void GameUI::need_reset()
{
    responded();
}