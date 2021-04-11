#include "Arbitre.h"
#include <cstdlib>
#include <ctime>
#include <utility>
#include <iostream>
using namespace std;

Arbitre::Arbitre(Player* a, Player* b)
{
	players[0] = a;
	players[1] = b;
	srand(time(nullptr));
	end = true;
}
void Arbitre::reset_game()
{
	if (rand() & 1) {
		swap(players[0], players[1]);
	}

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			my[0][i][j] = UNKNOWN;
			my[1][i][j] = UNKNOWN;
			enemy[0][i][j] = UNKNOWN;
			enemy[1][i][j] = UNKNOWN;
		}
	}

	players[0]->start_place_ships(ships[0]);
	players[1]->start_place_ships(ships[1]);

	hp[0] = 20;
	hp[1] = 20;

	prepare_bf();

	end = false;
	pos2d step;
	int side = 0;
	int alt_side = 1;
	while (!end)
	{
		cout << endl << "STEP OF PLAYER " << side << endl << endl;
		step = players[side]->on_step(my[side], enemy[side]);
		switch (my[alt_side][step.x][step.y]) {
		case UNKNOWN:
			my[alt_side][step.x][step.y] = MISS;
			enemy[side][step.x][step.y] = MISS;
			swap(side, alt_side);
			break;
		case INTACT:
			my[alt_side][step.x][step.y] = HITTED;
			enemy[side][step.x][step.y] = HITTED;
			// TODO: check if all ship drowned
			if (step.x > 0 && step.y > 0 && my[alt_side][step.x - 1][step.y - 1] == UNKNOWN)
			{
				enemy[side][step.x - 1][step.y - 1] = USELESS;
			}
			if (step.x < 9 && step.y > 0 && my[alt_side][step.x + 1][step.y - 1] == UNKNOWN)
			{
				enemy[side][step.x + 1][step.y - 1] = USELESS;
			}
			if (step.x < 9 && step.y < 9 && my[alt_side][step.x + 1][step.y + 1] == UNKNOWN)
			{
				enemy[side][step.x + 1][step.y + 1] = USELESS;
			}
			if (step.x > 0 && step.y < 9 && my[alt_side][step.x - 1][step.y + 1] == UNKNOWN)
			{
				enemy[side][step.x - 1][step.y + 1] = USELESS;
			}
			hp[alt_side] -= 1;
			break;
		}

		end = hp[alt_side] == 0;
	}

	players[side]->on_win();
	players[alt_side]->on_loose();
}

void Arbitre::prepare_bf()
{
	for (int p = 0; p < 2; p++)
	{
		for (int i = 0; i < 10; i++)
		{
			for (int l = 0; l < ships[p][i].size; l++)
			{
				if (ships[p][i].rot == HORIZONTAL)
				{
					my[p][ships[p][i].x + l][ships[p][i].y] = INTACT;
				}
				else
				{
					my[p][ships[p][i].x][ships[p][i].y + l] = INTACT;
				}
			}
		}
	}
}