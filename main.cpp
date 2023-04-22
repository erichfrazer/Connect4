#include "main.h"
#include <time.h>


int main(int argc, const char* argv[])
{
	srand(time(nullptr));

	for (int y = 0; y < GRIDHEIGHT; y++)
	{
		for (int x = 0; x < GRIDWIDTH; x++)
		{
			TheBoard[y][x] = EMPTY;
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
		PrintGrid(TheBoard);
		
		int x = GetUserPlayColumn(TheBoard);
		int row = DropPiece(TheBoard, x, HUMAN);
		PlayerType ptWon = Any4InARowAtLoc(TheBoard, HUMAN, true, x, row);
		if (ptWon != EMPTY)
		{
			PrintGrid(TheBoard);
			printf("Player won!\n");
			break;
		}

		PrintGrid(TheBoard);

		x = GetOpponentPlayColumn(TheBoard);
		row = DropPiece(TheBoard, x, OPPONENT);
		ptWon = Any4InARowAtLoc(TheBoard, OPPONENT, true, x, row);
		if (ptWon != EMPTY)
		{
			PrintGrid(TheBoard);

			DisplayYouLose();

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
		"   ___   ",
		" //   \\\\ ",
		"||  h  ||",
		" \\\\   // ",
		"   ---   "
	},
	{
		"   ___   ",
		" // o \\\\ ",
		"|| OOO ||",
		" \\\\ o // ",
		"   ---   "
	},
	{
		"   ___   ",
		" //WIN\\\\ ",
		"||  h  ||",
		" \\\\WIN// ",
		"   ---   "
	},
	{
		"   ___   ",
		" //WIN\\\\ ",
		"|| OOO ||",
		" \\\\WIN// ",
		"   ---   "
	},
	{
		"   ___   ",
		" //???\\\\ ",
		"|| ??? ||",
		" \\\\???// ",
		"   ---   "
	}
};

void PrintGrid(PlayerType Board[][GRIDWIDTH])
{
	printf("\n");
	printf("==========================================================================================================\n");
	printf("== ROUND %3d  ============================================================================================\n", Round);
	Round++;
	printf("==========================================================================================================\n");
	for (int y = GRIDHEIGHT - 1; y >= 0; y--)
	{
		for (int y2 = 0; y2 < 5; y2++)
		{
			printf("|| ");
			for (int x = 0; x < GRIDWIDTH; x++)
			{
				int v = Board[y][x];
				printf("%s || ", PieceString[v][y2]);
			}

			printf("\n");
		}
		printf("==========================================================================================================\n");
	}
	for (int x = 0; x < GRIDWIDTH; x++)
	{
		printf("       %d     ", x + 1);
	}
	printf("\n");
}

int GetUserPlayColumn(PlayerType Board[][GRIDWIDTH])
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

int DropPiece(PlayerType Board[][GRIDWIDTH], int Column, PlayerType Player)
{
	int r = HowManyRowsFilled(Board, Column);
	Board[r][Column] = Player;
	return r;
}

PlayerType Any4InARowAtLoc(
	PlayerType Board[][GRIDWIDTH], 
	PlayerType WhichPlayer,
	bool MarkIfWin,
	int xStart, int yStart)
{
	// from each spot, look in each of 4 directions, both ways, and see how long the line can go
	int dirx[4] = { 1, 1, 1, 0 }; // right-down, right, right-up, and up
	int diry[4] = { -1, 0, 1, 1 };

	for (int dir = 0; dir < 4; dir++)
	{
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
			if (Board[y][x] != WhichPlayer)
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
			if (Board[y][x] != WhichPlayer)
			{
				// when we find a non-match, back up
				reverse_len--;
				break;
			}
		}
		int TotalLen = forward_len + reverse_len + 1; // 1 = current player @ start pos xLoc, yLoc
		if (TotalLen >= 4)
		{
			if (MarkIfWin)
			{
				for (int i = 0; i < 4; i++)
				{
					int x = xStart + dirx[dir] * (-reverse_len + i);
					int y = yStart + diry[dir] * (-reverse_len + i);
					int WhichPiece = Board[y][x];
					if (WhichPiece == HUMAN)
						Board[y][x] = HUMAN_WON;
					else if (WhichPiece == OPPONENT)
						Board[y][x] = OPPONENT_WON;
					else
						Board[y][x] = WRONG_PIECE;
				}
			}
			return WhichPlayer;
		}
	}

	return EMPTY;
}

int HowManyRowsFilled(PlayerType Board[][GRIDWIDTH], int Column)
{
	for (int r = 0; r < GRIDHEIGHT; r++)
	{
		if (Board[r][Column] == EMPTY)
		{
			return r;
		}
	}
	return GRIDHEIGHT;
}

bool IsTakeable(PlayerType Board[][GRIDWIDTH], int x, int y)
{
	if (Board[y][x] != EMPTY) return false;
	// the space is empty
	if (y == 0) return true;
	if (Board[y - 1][x] == EMPTY) return false;
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
	PlayerType Board[GRIDHEIGHT][GRIDWIDTH], 
	PlayerType WhichPlayer,
	int Column,
	ColStats colStats[GRIDWIDTH],
	int CurrentDepth,
	int* MovesSearched)
{
	*MovesSearched = *MovesSearched + 1;

	// if we've gone recursive too many times, we're done. Set this as high as you feel like it

	if (CurrentDepth == LookaheadMoves) // both players move 4 times
	{
		return; // no win found for anybody
	}

	// make a copy of the board

	PlayerType BoardCopy[GRIDHEIGHT][GRIDWIDTH];
	memcpy(BoardCopy, Board, GRIDHEIGHT * GRIDWIDTH * sizeof(PlayerType));

	PlayerType OtherPlayer = GetOtherPlayer(WhichPlayer);

	for (int x = 0; x < GRIDWIDTH; x++)
	{
		// don't try already filled columns

		int rowsFilled = HowManyRowsFilled(BoardCopy, x);
		if (rowsFilled == GRIDHEIGHT)
		{
			continue;
		}

		// temporarily set this column with the piece of the current player

		BoardCopy[rowsFilled][x] = WhichPlayer;

		// see if this player won

		PlayerType WhoWon = Any4InARowAtLoc(BoardCopy, WhichPlayer, false, x, rowsFilled);

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
			TryRecursiveColumn(
				BoardCopy,
				OtherPlayer,
				x,
				colStatsLocal,
				CurrentDepth + 1, MovesSearched);
		}

		// add the recursive win/loss stats to the current column.
		// if we're the 0th try, put it into the parent column, we're about to return

		if (CurrentDepth != 0)
		{
			colStats[Column].TotalLosses += colStatsLocal[x].TotalLosses;
			colStats[Column].TotalWins += colStatsLocal[x].TotalWins;
		}
		else
		{
			colStats[x] = colStatsLocal[x];
		}

		// remember to unset the piece we temporarily set AFTER doing all the recursive calls for this column

		BoardCopy[rowsFilled][x] = EMPTY;
	}

	return;
}

int GetOpponentPlayColumn(PlayerType Board[][GRIDWIDTH])
{
	ColStats ColStats[GRIDWIDTH] = { 0 };

	int MovesSearched = 0;

	TryRecursiveColumn(
		Board,
		OPPONENT,
		0,
		ColStats,
		0,
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

	printf("Searched %d moves, computer picks spot: %d\n\n", MovesSearched, bestcol);

	return bestcol;
}

