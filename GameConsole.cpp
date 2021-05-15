#include "GameConsole.h"
#include <iostream>
using namespace std;

void GameConsole::on_win()
{
    cout << "You win!" << endl;
}
void GameConsole::on_loose()
{
    cout << "You loose!" << endl;
}
void GameConsole::start_place_ships(ship_def ships[10])
{
    /*for (int i = 0; i < 10; i++) {
            cout << "Input for ship with length " << ships[i].size << endl;
            cout << "Input x: ";
            cin >> ships[i].x;
            cout << "Input y: ";
            cin >> ships[i].y;
            cout << "Input h or v: ";
            char tmp;
            cin >> tmp;
            if (tmp == 'h')
            {
                    ships[i].rot = HORIZONTAL;
            }
            else
            {
                    ships[i].rot = VERTICAL;
            }
    }*/

    ships[0].rot = HORIZONTAL;
    ships[0].x = 6;
    ships[0].y = 0;
    ships[1].rot = VERTICAL;
    ships[1].x = 3;
    ships[1].y = 0;
    ships[2].rot = VERTICAL;
    ships[2].x = 1;
    ships[2].y = 4;
    ships[3].rot = HORIZONTAL;
    ships[3].x = 0;
    ships[3].y = 0;
    ships[4].rot = VERTICAL;
    ships[4].x = 9;
    ships[4].y = 2;
    ships[5].rot = VERTICAL;
    ships[5].x = 8;
    ships[5].y = 8;
    ships[6].rot = HORIZONTAL;
    ships[6].x = 0;
    ships[6].y = 9;
    ships[7].rot = HORIZONTAL;
    ships[7].x = 3;
    ships[7].y = 8;
    ships[8].rot = HORIZONTAL;
    ships[8].x = 5;
    ships[8].y = 8;
    ships[9].rot = HORIZONTAL;
    ships[9].x = 6;
    ships[9].y = 5;

    cout << "place ships" << endl;
    responded();
}

void GameConsole::on_step(const bf_tile my[10][10], const bf_tile enemy[10][10], pos2d *result)
{
    cout << "Make shot" << endl;
    cout << "  ABCDEFGHIJ   ABCDEFGHIJ" << endl;
    for (int y = 0; y < 10; y++)
    {
        if (y < 9)
            cout << " ";
        cout << y + 1;
        for (int x = 0; x < 10; x++)
        {
            char c;
            switch (my[x][y])
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
            cout << c;
        }

        if (y < 9)
            cout << " ";
        cout << " " << y + 1;
        for (int x = 0; x < 10; x++)
        {
            char c;
            switch (enemy[x][y])
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
            cout << c;
        }
        cout << endl;
    }

    char x;
    int y;

    cin >> x >> y;

    result->x = x - 'a';
    result->y = y - 1;
    responded();
    return;
}
