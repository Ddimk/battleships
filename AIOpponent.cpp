#include "AIOpponent.h"
#include <iostream>
#include <vector>
#include <cstdlib>
using namespace std;


void AIOpponent::on_win()
{

}

void AIOpponent::on_loose()
{

}

void AIOpponent::start_place_ships(ship_def ships[10])
{
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

}

pos2d AIOpponent::on_step(const bf_tile my[10][10], const bf_tile enemy[10][10])
{
	pos2d res;

	vector<pos2d> pass_3;
	vector<pos2d> pass_1;
	vector<pos2d> pass_0;

	for (int x = 0; x < 10; x++)
	{
		for (int y = 0; y < 10; y++)
		{
			if (enemy[x][y] == HITTED)
			{
				bool has_nearby_hit = false;
				enum rotation rot;
				vector<pos2d> shoot_near;
				if (x < 9)
				{
					if (enemy[x + 1][y] == HITTED)
					{
						has_nearby_hit = true;
						rot = HORIZONTAL;
					}
					else if (enemy[x + 1][y] == UNKNOWN)
					{
						res.x = x + 1;
						res.y = y;
						shoot_near.push_back(res);
					}
				}
				if (y > 0)
				{
					if (enemy[x][y - 1] == HITTED)
					{
						has_nearby_hit = true;
						rot = VERTICAL;
					}
					else if (enemy[x][y - 1] == UNKNOWN)
					{
						res.x = x;
						res.y = y - 1;
						shoot_near.push_back(res);
					}
				}
				if (x > 0)
				{
					if (enemy[x - 1][y] == HITTED)
					{
						has_nearby_hit = true;
						rot = HORIZONTAL;
					}
					else if (enemy[x - 1][y] == UNKNOWN)
					{
						res.x = x - 1;
						res.y = y;
						shoot_near.push_back(res);
					}
				}
				if (y < 9)
				{
					if (enemy[x][y + 1] == HITTED)
					{
						has_nearby_hit = true;
						rot = VERTICAL;
					}
					else if (enemy[x][y + 1] == UNKNOWN)
					{
						res.x = x;
						res.y = y + 1;
						shoot_near.push_back(res);
					}
				}
				if (has_nearby_hit)
				{
					for (int i = 1; i < 4; i++)
					{
						if (rot == HORIZONTAL)
						{
							if (x + i < 10 && enemy[x + i][y] == UNKNOWN)
							{
								res.x = x + i;
								res.y = y;
								return res;
							}
							if (x - i > 0 && enemy[x - i][y] == UNKNOWN)
							{
								res.x = x - i;
								res.y = y;
								return res;
							}
						}
						else
						{
							if (y + i < 10 && enemy[x][y + i] == UNKNOWN)
							{
								res.x = x;
								res.y = y + i;
								return res;
							}
							if (y - i > 0 && enemy[x][y - i] == UNKNOWN)
							{
								res.x = x;
								res.y = y - i;
								return res;
							}
						}
					}
				}
				else
				{
					return shoot_near[rand() % shoot_near.size()];
				}
			}

			if (enemy[x][y] == UNKNOWN)
			{
				pos2d tmp;
				tmp.x = x;
				tmp.y = y;
				if ((x - y) % 4 == 0)
				{
					pass_3.push_back(tmp);
				}
				else if ((x - y) % 2 == 0)
				{
					pass_1.push_back(tmp);
				}
				else
				{
					pass_0.push_back(tmp);
				}
			}
		}
	}

	if (pass_3.size())
	{
		return pass_3[rand() % pass_3.size()];
	}
	else if (pass_1.size())
	{
		return pass_1[rand() % pass_1.size()];
	}
	else
	{
		return pass_0[rand() % pass_0.size()];
	}
}