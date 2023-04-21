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

	while (true)
	{
		PrintGrid(TheBoard);
		int x = GetUserPlayColumn(TheBoard);
		DropPiece(TheBoard, x, HUMAN);
		PlayerType ptWon = Any4InARow(TheBoard, true, NULL, NULL, NULL, NULL);
		if (ptWon != EMPTY)
		{
			PrintGrid(TheBoard);
			printf("Player won!\n");
			break;
		}

		PrintGrid(TheBoard);

		x = GetOpponentPlayColumn(TheBoard);
		DropPiece(TheBoard, x, OPPONENT);
		ptWon = Any4InARow(TheBoard, true, NULL, NULL, NULL, NULL);
		if (ptWon != EMPTY)
		{
			PrintGrid(TheBoard);
			printf("Opponent won!\n");
			break;
		}

	}

	return 0;
}

const char* PieceString[5][5] = {
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

void DropPiece(PlayerType Board[][GRIDWIDTH], int Column, PlayerType Player)
{
	int r = HowManyRowsFilled(Board, Column);
	Board[r][Column] = Player;
}

PlayerType Any4InARow(PlayerType Board[][GRIDWIDTH], bool MarkIfWin, int* xStart, int* yStart, int* xDir, int* yDir)
{
	// from each spot, only look up and right
	// since we're going to look up, make sure the loop bounds are correct

	bool Found4 = false;
	int XWinStartLoc, YWinStartLoc, XWinDirection, YWinDirection;

	PlayerType StartingValue;

	for (int y = 0; y <= GRIDHEIGHT - 4; y++)
	{
		for (int x = 0; x <= GRIDWIDTH - 4; x++)
		{
			StartingValue = (PlayerType)Board[y][x];
			if (StartingValue == EMPTY)
			{
				continue;
			}

			// go look in 3 directions from start

			// look up

			int FoundLen = 1;

			for ( int len = 1 ; len <= 3; len++)
			{
				PlayerType v2 = Board[y + len][x];
				if (v2 != StartingValue)
				{
					break;
				}
				FoundLen = len + 1;
			}

			if (FoundLen == 4)
			{
				XWinStartLoc = x;
				YWinStartLoc = y;
				XWinDirection = 0;
				YWinDirection = 1;
				Found4 = true;
				break; // out of X
			}

			// look diagonal up/right

			FoundLen = 1;

			for (int len = 1; len <= 3; len++)
			{
				PlayerType v2 = Board[y + len][x + len];
				if (v2 != StartingValue)
				{
					break;
				}
				FoundLen = len + 1;
			}

			if (FoundLen == 4)
			{
				XWinStartLoc = x;
				YWinStartLoc = y;
				XWinDirection = 1;
				YWinDirection = 1;
				Found4 = true;
				break; // out of X
			}

			// look right

			FoundLen = 1;

			for (int len = 1; len <= 3; len++)
			{
				PlayerType v2 = Board[y][x + len];
				if (v2 != StartingValue)
				{
					break;
				}
				FoundLen = len + 1;
			}

			if (FoundLen == 4)
			{
				XWinStartLoc = x;
				YWinStartLoc = y;
				XWinDirection = 1;
				YWinDirection = 0;
				Found4 = true;
				break; // out of X
			}
		} // for X

		if (Found4)
		{
			break;
		}
	} // for y

	if (!Found4)
	{
		return EMPTY; // no 4
	}

	// mark a win as being different piece types if told to

	if (xStart != NULL)
		*xStart = XWinStartLoc;
	if (yStart != NULL)
		*yStart = YWinStartLoc;
	if (xDir != NULL)
		*xDir = XWinDirection;
	if (yDir != NULL)
		*yDir = YWinDirection;

	if (MarkIfWin)
	{
		int len = 4;
		while (len--)
		{
			if (StartingValue == HUMAN)
			{
				Board[YWinStartLoc][XWinStartLoc] = HUMAN_WON;
			}
			else if (StartingValue == OPPONENT)
			{
				Board[YWinStartLoc][XWinStartLoc] = OPPONENT_WON;
			}
			YWinStartLoc += YWinDirection;
			XWinStartLoc += XWinDirection;
		}
	}

	return StartingValue;
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
	return 4;
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

	if (CurrentDepth == LOOKAHEAD_MOVES) // both players move 4 times
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

		PlayerType WhoWon = Any4InARow(BoardCopy, false, NULL, NULL, NULL, NULL);

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

	printf("Searched %d moves\n\n", MovesSearched);

	// which column has the best win/loss statistics? This is EASY
	// if you want to randomize the game a bit, you could pick the 2nd best guess once in a while?

	float bestwinratio = 0;
	int bestcol = -1;

	for (int c = 0; c < GRIDWIDTH; c++)
	{
		float winratio = ColStats[c].TotalWins / (float)ColStats[c].TotalLosses;
		if (winratio > bestwinratio)
		{
			bestwinratio = winratio;
			bestcol = c;
		}
	}
	return bestcol;
}

