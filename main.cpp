#include "main.h"
#include <time.h>


int main(int argc, const char* argv[])
{
	srand(time(nullptr));

	for (int y = 0; y < GRIDHEIGHT; y++)
	{
		for (int x = 0; x < GRIDWIDTH; x++)
		{
			TheBoard[y][x].Player = EMPTY;
			TheBoard[y][x].PlayCount = -1;
		}
	}

	int LookAhead = 0;
	do
	{
		printf("How many moves do you want the computer to look ahead? Pick between 2-10, anything over 8 is nuts:");
		scanf("%d", &LookAhead);
		if (LookAhead < 2 || LookAhead > 10)
		{
			printf("Bad lookahead. try again\n");
		}
	} while (LookAhead < 2 || LookAhead > 10);
	LookaheadMoves = LookAhead;

	while (true)
	{
		PrintGrid(TheBoard, HUMAN);
		
		int x = GetUserPlayColumn(TheBoard);
		int row = DropPiece(TheBoard, x, HUMAN);
		PlayerType ptWon = Any4InARowAtLoc(TheBoard, HUMAN, true, x, row);
		if (ptWon != EMPTY)
		{
			PrintGrid(TheBoard, HUMAN);
			printf("Player won!\n");
			break;
		}

		PrintGrid(TheBoard, OPPONENT);

		x = GetOpponentPlayColumn(TheBoard);

		printf("WINS  ");
		for (int x = 0; x < GRIDWIDTH; x++)
		{
			printf("  %5d      ", TheColStats[x].TotalWins);
		}
		printf("\n");
		printf("LOSSES");
		for (int x = 0; x < GRIDWIDTH; x++)
		{
			printf("  %5d      ", TheColStats[x].TotalLosses);
		}
		printf("\n");

		row = DropPiece(TheBoard, x, OPPONENT);
		ptWon = Any4InARowAtLoc(TheBoard, OPPONENT, true, x, row);
		if (ptWon != EMPTY)
		{
			DisplayYouLose();

			PrintGrid(TheBoard, OPPONENT);

			printf("Opponent won!\n");
			break;
		}

	}

	return 0;
}

const char* PieceString[6][5] = {
	{
		"         ",
		"         ",
		"         ",
		"         ",
		"         "
	},
	{
		"___   ",
		" // y \\\\ ",
		"|| you ||",
		" \\\\ u // ",
		"   ---   "
	},
	{
		"___   ",
		" //   \\\\ ",
		"||     ||",
		" \\\\   // ",
		"   ---   "
	},
	{
		"___   ",
		" //WIN\\\\ ",
		"|| YOU ||",
		" \\\\WIN// ",
		"   ---   "
	},
	{
		"___   ",
		" //WIN\\\\ ",
		"||     ||",
		" \\\\WIN// ",
		"   ---   "
	},
	{
		"___   ",
		" //???\\\\ ",
		"|| ??? ||",
		" \\\\???// ",
		"   ---   "
	}
};

void PrintGrid(Grid Board[][GRIDWIDTH], PlayerType WhichPlayer)
{
	printf("\n");
	printf("     ==========================================================================================================\n");
	printf("     == Play # %3d  ============================================================================================\n", PlayCount);
	printf("     ==========================================================================================================\n");
	for (int y = GRIDHEIGHT - 1; y >= 0; y--)
	{
		for (int y2 = 0; y2 < 5; y2++)
		{
			printf("     ");
			printf("|| ");
			for (int x = 0; x < GRIDWIDTH; x++)
			{
				int v = Board[y][x].Player;
				if (v != EMPTY)
				{
					if (y2 == 0)
					{
						printf("%3d", Board[y][x].PlayCount);
					}
				}
				printf("%s || ", PieceString[v][y2]);
			}

			printf("\n");
		}
		printf("     ==========================================================================================================\n");
	}
	printf("     ");
	for (int x = 0; x < GRIDWIDTH; x++)
	{
		printf("       %d     ", x + 1);
	}
	printf("\n\n");
}

int GetUserPlayColumn(Grid Board[][GRIDWIDTH])
{
	int col;
	while (true)
	{
		printf("Pick a column to drop your piece into, 1-%d: ", GRIDWIDTH);
		scanf_s("%d", &col);
		col--; // 0-based
		
		if (col < 0 || col >= GRIDWIDTH )
		{
			printf("Pick a valid colunn.\n");
			char buf[256];
			gets_s(buf);
			continue;
		}

		int r = HowManyRowsFilled(Board, col);
		if (r == GRIDHEIGHT)
		{
			printf("Pick a column that has an empty space.\n");
			continue;
		}

		return col;
	}
}

int DropPiece(Grid Board[][GRIDWIDTH], int Column, PlayerType Player)
{
	int r = HowManyRowsFilled(Board, Column);
	Board[r][Column].Player = Player;
	Board[r][Column].PlayCount = PlayCount;
	PlayCount++;
	return r;
}

PlayerType Any4InARowAtLoc(
	Grid Board[][GRIDWIDTH], 
	PlayerType WhichPlayer,
	bool MarkIfWin,
	int xStart, int yStart)
{
	// from each spot, look in each of 4 directions, both ways, and see how long the line can go
	int dirx[4] = { 1, 1, 1, 0 }; // right-down, right, right-up, and up
	int diry[4] = { -1, 0, 1, 1 };

	for (int dir = 0; dir < 4; dir++)
	{
		if (dir == 3)
		{
			int stop = 0;
		}
		int forward_len = 1;
		for ( ; ; forward_len++)
		{
			int x = xStart + dirx[dir] * forward_len;
			int y = yStart + diry[dir] * forward_len;
			if (x >= GRIDWIDTH || y < 0 || y >= GRIDHEIGHT)
			{
				// when we find a non-match, back up
				forward_len--;
				break;
			}
			if (Board[y][x].Player != WhichPlayer)
			{
				// when we find a non-match, back up
				forward_len--;
				break;
			}
		}
		int reverse_len = 1;
		for (; ; reverse_len++)
		{
			int x = xStart - dirx[dir] * reverse_len;
			int y = yStart - diry[dir] * reverse_len;
			if (x < 0 || y < 0 || y >= GRIDHEIGHT)
			{
				// when we find a non-match, back up
				reverse_len--;
				break;
			}
			if (Board[y][x].Player != WhichPlayer)
			{
				// when we find a non-match, back up
				reverse_len--;
				break;
			}
		}
		int TotalLen = forward_len + reverse_len + 1; // 1 = current player @ start pos xLoc, yLoc
		if (TotalLen >= 4)
		{
#if false
			switch (dir)
			{
			case 0:
				printf("Found 4 right-down\n");
				break;
			case 1:
				printf("Found 4 horizontal\n");
				break;
			case 2:
				printf("Found 4 right-up\n");
				break;
			case 3:
				printf("Found 4 vertically\n");
				break;

			}
			if (Board[3][0].Player == HUMAN && Board[3][0].PlayCount == 1 &&
				Board[2][0].Player == OPPONENT && Board[2][0].PlayCount == 2 &&
				Board[3][1].Player == HUMAN && Board[3][1].PlayCount == 3 &&
				Board[2][1].Player == OPPONENT && Board[2][1].PlayCount == 4 )
			{
				int stop = 0;
			}

			PrintMiniGrid(Board);
#endif

			if (MarkIfWin)
			{
				for (int i = 0; i < 4; i++)
				{
					int x = xStart + dirx[dir] * (-reverse_len + i);
					int y = yStart + diry[dir] * (-reverse_len + i);
					int WhichPiece = Board[y][x].Player;
					if (WhichPiece == HUMAN)
						Board[y][x].Player = HUMAN_WON;
					else if (WhichPiece == OPPONENT)
						Board[y][x].Player = OPPONENT_WON;
					else
						Board[y][x].Player = WRONG_PIECE;
				}
			}
			return WhichPlayer;
		}
	}

	return EMPTY;
}

void PrintMiniGrid(Grid Board[][GRIDWIDTH])
{
	for (int xx = 0; xx < GRIDWIDTH + 2; xx++)
	{
		printf("----");
	}
	printf("\n");

	for (int yy = GRIDHEIGHT - 1; yy >= 0; yy--)
	{
		printf("|");
		for (int xx = 0; xx < GRIDWIDTH; xx++)
		{
			if (Board[yy][xx].Player == HUMAN)
			{
				printf("%2dH ", Board[yy][xx].PlayCount);
			}
			else if (Board[yy][xx].Player == OPPONENT)
			{
				printf("%2dO ", Board[yy][xx].PlayCount);
			}
			else
			{
				printf("    ");
			}
		}
		printf("|\n");
	}
}

int HowManyRowsFilled(Grid Board[][GRIDWIDTH], int Column)
{
	for (int r = 0; r < GRIDHEIGHT; r++)
	{
		if (Board[r][Column].Player == EMPTY)
		{
			return r;
		}
	}
	return GRIDHEIGHT;
}

bool IsTakeable(Grid Board[][GRIDWIDTH], int x, int y)
{
	if (Board[y][x].Player != EMPTY) return false;
	// the space is empty
	if (y == 0) return true;
	if (Board[y - 1][x].Player == EMPTY) return false;
	return true;
}

PlayerType GetOtherPlayer(PlayerType WhichPlayer)
{
	if (WhichPlayer == HUMAN) return OPPONENT;
	return HUMAN;
}

void DisplayYouLose()
{
	FILE* f = fopen("YouLose.txt", "r");
	char buf[256];
	while (!feof(f))
	{
		fgets(buf, 256, f);
		printf(buf);
	}
	fclose(f);
}


// only the two following functions have any brains in them...
// This function tries EVERYTHING under it, and calculates the win and loss rate for every column
// at the current (depth) layer, and then recursively calls itself for each sub-column. Keeps the win/loss
// stats per column value, which is evaluated by the first caller of this function

void TryRecursiveColumn(
	Grid Board[GRIDHEIGHT][GRIDWIDTH], 
	PlayerType WhichPlayer,
	ColStats colStats[GRIDWIDTH],
	int CurrentDepth,
	PathData ColumnsTaken[],
	int* MovesSearched)
{
	// the first time we're called, it's with a CurrentDepth of 0 and we need to try all OPPONENT moves from here.
	// always add this up

	*MovesSearched = *MovesSearched + 1;

	// if we've gone recursive too many times, we're done. Set this as high as you feel like it

	if (CurrentDepth == LookaheadMoves)
	{
		return;
	}

	PlayerType OtherPlayer = GetOtherPlayer(WhichPlayer);

	for (int x = 0; x < GRIDWIDTH; x++)
	{
		// don't try already filled columns

		int rowsFilled = HowManyRowsFilled(Board, x);
		if (rowsFilled == GRIDHEIGHT)
		{
			continue;
		}

		// temporarily set this column with the piece of the incoming player

		Board[rowsFilled][x].Player = WhichPlayer;
		Board[rowsFilled][x].PlayCount = PlayCount;
		ColumnsTaken[CurrentDepth].Column = x;
		ColumnsTaken[CurrentDepth].Player = WhichPlayer;
		PlayCount++;

		if (CurrentDepth == 5)
			if (ColumnsTaken[0].Column == 2 && ColumnsTaken[0].Player == OPPONENT &&
				ColumnsTaken[1].Column == 3 && ColumnsTaken[1].Player == HUMAN &&
				ColumnsTaken[2].Column == 2 && ColumnsTaken[2].Player == OPPONENT &&
				ColumnsTaken[3].Column == 3 && ColumnsTaken[3].Player == HUMAN &&
				ColumnsTaken[4].Column == 2 && ColumnsTaken[4].Player == OPPONENT &&
				ColumnsTaken[5].Column == 3 && ColumnsTaken[5].Player == HUMAN &&
				true )
			{
				PrintMiniGrid(Board);
				int Stop = 0;
			}

		if( CurrentDepth == 5 )
			if (ColumnsTaken[0].Column == 3 && ColumnsTaken[0].Player == HUMAN &&
				ColumnsTaken[1].Column == 2 && ColumnsTaken[0].Player == OPPONENT &&
				ColumnsTaken[2].Column == 3 && ColumnsTaken[0].Player == HUMAN &&
				ColumnsTaken[3].Column == 2 && ColumnsTaken[0].Player == OPPONENT &&
				ColumnsTaken[4].Column == 3 && ColumnsTaken[0].Player == HUMAN )
			{
				PrintMiniGrid(Board);
				int Stop = 0;
			}

		// see if this player won

		PlayerType WhoWon = Any4InARowAtLoc(Board, WhichPlayer, false, x, rowsFilled);

		ColStats colStatsLocal[GRIDWIDTH] = { 0 };

		if (WhoWon == OPPONENT)
		{
			colStatsLocal[x].TotalWins = 1;
		}
		else if (WhoWon == HUMAN)
		{
			colStatsLocal[x].TotalLosses = 1;
		}
		else
		{
			// try the OTHER player, at every single column, with us having set that temp piece where it is
			TryRecursiveColumn(
				Board,
				OtherPlayer,
				colStatsLocal,
				CurrentDepth + 1,
				ColumnsTaken,
				MovesSearched);
		}

		// we got win/loss stats for all the columns under us. We need to add those to our current column
		
		for (int i = 0; i < GRIDWIDTH; i++)
		{
			colStats[x].TotalLosses += colStatsLocal[i].TotalLosses;
			colStats[x].TotalWins += colStatsLocal[i].TotalWins;
		}

		// remember to unset the piece we temporarily set AFTER doing all the recursive calls for this column

		Board[rowsFilled][x].Player = EMPTY;
		Board[rowsFilled][x].PlayCount = -1;
		ColumnsTaken[CurrentDepth].Player = EMPTY; 
		ColumnsTaken[CurrentDepth].Column = -1;

		PlayCount--;
	}

	return;
}

int GetOpponentPlayColumn(Grid Board[][GRIDWIDTH])
{
	ColStats ColStats[GRIDWIDTH] = { 0 };

	int MovesSearched = 0;
	PathData ColumnsTaken[32];

	TryRecursiveColumn(
		Board,
		OPPONENT,
		ColStats,
		0,
		ColumnsTaken,
		&MovesSearched);

	// which column has the best win/loss statistics? This is EASY
	// if you want to randomize the game a bit, you could pick the 2nd best guess once in a while?

	float bestwinratio = 0;
	int bestcol = -1;

	// don't allow losses to be 0 and then divide by 0. Just add one to each one to make the stats even
	for (int c = 0; c < GRIDWIDTH; c++)
	{
		ColStats[c].TotalLosses++;
		ColStats[c].TotalWins++;
		TheColStats[c] = ColStats[c];
	}

	for (int c = 0; c < GRIDWIDTH; c++)
	{
		int rowsFilled = HowManyRowsFilled(Board, c);
		if (rowsFilled == GRIDHEIGHT) continue;

		float winratio = ColStats[c].TotalWins / (float)ColStats[c].TotalLosses;
		if (winratio > bestwinratio)
		{
			bestwinratio = winratio;
			bestcol = c;
		}
	}

	printf("Searched %d moves, computer picks spot: %d\n", MovesSearched, bestcol + 1);

	return bestcol;
}

