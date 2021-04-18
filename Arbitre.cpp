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
bool Arbitre::is_correct_placement(const ship_def test_ships[10])
{
	bool tmp[10][10] = { 0 };

	for (int i = 0; i < 10; i++)
	{
		if (test_ships[i].x < 0 || test_ships[i].y < 0 ||
			(test_ships[i].rot == VERTICAL && (test_ships[i].x > 9 || test_ships[i].y + test_ships[i].size - 1 > 9)) ||
			(test_ships[i].rot == HORIZONTAL && (test_ships[i].y > 9 || test_ships[i].x + test_ships[i].size - 1 > 9)))
		{
			cout << "Ship " << i << " is outside of field" << endl;
			return false;
		}
			
		for (int l = -1; l < test_ships[i].size + 1; l++)
		{
			for (int w = -1; w < 2; w++)
			{
				int x, y;
				if (test_ships[i].rot == HORIZONTAL)
				{
					x = test_ships[i].x + l;
					y = test_ships[i].y + w;
				}
				else
				{
					x = test_ships[i].x + w;
					y = test_ships[i].y + l;
				}
				if (x < 0 || x > 9 || y < 0 || y > 9)
				{
					continue;
				}
				if (tmp[x][y])
				{
					cout << "Ship " << i << " has collision" << endl;
					return false;
				}
			}
		}
		for (int l = 0; l < test_ships[i].size; l++)
		{
			if (test_ships[i].rot == HORIZONTAL)
			{
				tmp[test_ships[i].x + l][test_ships[i].y] = true;
			}
			else
			{
				tmp[test_ships[i].x][test_ships[i].y + l] = true;
			}
		}
	}

	return true;
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
	while (!is_correct_placement(ships[0]))
	{
		cout << "Player 0 places ships incorrect" << endl;
		players[0]->start_place_ships(ships[0]);
	}
	players[1]->start_place_ships(ships[1]);
	while (!is_correct_placement(ships[1]))
	{
		cout << "Player 1 places ships incorrect" << endl;
		players[1]->start_place_ships(ships[1]);
	}

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
		if (step.x < 0 || step.x > 9 || step.y < 0 || step.y > 9)
		{
			cout << "Incorrect coordinates" << endl;
			continue;
		}

		cout << "Player " << side << " make step " << (char)(step.x + 'A') << step.y + 1 << endl;

		switch (my[alt_side][step.x][step.y]) {
		default:
			cout << "Incorrect step" << endl;
			break;
		case UNKNOWN:
			my[alt_side][step.x][step.y] = MISS;
			enemy[side][step.x][step.y] = MISS;
			swap(side, alt_side);
			break;
		case INTACT:
			my[alt_side][step.x][step.y] = HITTED;
			enemy[side][step.x][step.y] = HITTED;
			// TODO: check if all ship drowned
			bool is_fully_drown = true;
			for (int i = 1; i < 4 && step.y - i >= 0; i++)
			{
				if (my[alt_side][step.x][step.y - i] != HITTED)
				{
					if (my[alt_side][step.x][step.y - i] == INTACT)
					{
						is_fully_drown = false;
					}
					break;
				}
			}
			for (int i = 1; i < 4 && step.y + i < 10; i++)
			{
				if (my[alt_side][step.x][step.y + i] != HITTED)
				{
					if (my[alt_side][step.x][step.y + i] == INTACT)
					{
						is_fully_drown = false;
					}
					break;
				}
			}
			for (int i = 1; i < 4 && step.x - i >= 0; i++)
			{
				if (my[alt_side][step.x - i][step.y] != HITTED)
				{
					if (my[alt_side][step.x - i][step.y] == INTACT)
					{
						is_fully_drown = false;
					}
					break;
				}
			}
			for (int i = 1; i < 4 && step.x + i < 10; i++)
			{
				if (my[alt_side][step.x + i][step.y] != HITTED)
				{
					if (my[alt_side][step.x + i][step.y] == INTACT)
					{
						is_fully_drown = false;
					}
					break;
				}
			}
			if (is_fully_drown)
			{
				cout << "Ship destroyed" << endl;
				enemy[side][step.x][step.y] = DROWNED;
				my[alt_side][step.x][step.y] = DROWNED;
				for (int i = 1; i < 5 && step.y - i >= 0; i++)
				{
					if (my[alt_side][step.x][step.y - i] == HITTED)
					{
						enemy[side][step.x][step.y - i] = DROWNED;
						my[alt_side][step.x][step.y - i] = DROWNED;
					}
					else
					{
						if (my[alt_side][step.x][step.y - i] == UNKNOWN)
						{
							enemy[side][step.x][step.y - i] = USELESS;
						}
						break;
					}
				}
				for (int i = 1; i < 5 && step.y + i < 10; i++)
				{
					if (my[alt_side][step.x][step.y + i] == HITTED)
					{
						enemy[side][step.x][step.y + i] = DROWNED;
						my[alt_side][step.x][step.y + i] = DROWNED;
					}
					else
					{
						if (my[alt_side][step.x][step.y + i] == UNKNOWN)
						{
							enemy[side][step.x][step.y + i] = USELESS;
						}
						break;
					}
				}
				for (int i = 1; i < 5 && step.x - i >= 0; i++)
				{
					if (my[alt_side][step.x - i][step.y] == HITTED)
					{
						enemy[side][step.x - i][step.y] = DROWNED;
						my[alt_side][step.x - i][step.y] = DROWNED;
					}
					else
					{
						if (my[alt_side][step.x - i][step.y] == UNKNOWN)
						{
							enemy[side][step.x - i][step.y] = USELESS;
						}
						break;
					}
				}
				for (int i = 1; i < 5 && step.x + i < 10; i++)
				{
					if (my[alt_side][step.x + i][step.y] == HITTED)
					{
						enemy[side][step.x + i][step.y] = DROWNED;
						my[alt_side][step.x + i][step.y] = DROWNED;
					}
					else
					{
						if (my[alt_side][step.x + i][step.y] == UNKNOWN)
						{
							enemy[side][step.x + i][step.y] = USELESS;
						}
						break;
					}
				}
			}
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