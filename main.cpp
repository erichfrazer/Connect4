#include "main.h"
#include <time.h>


int main(int argc, const char* argv[])
{
	srand(time(nullptr));

	while (true)
	{
		PrintGrid(Board);
		int x = GetUserPlayColumn(Board);
		DropPiece(Board, x, USER);
		int xs, ys, xi, yi;
		bool bWon = Any4InARow(Board, USER, &xs, &ys, &xi, &yi);
		if (bWon)
		{
			PrintGrid(Board);
			printf("Player won!\n");
			break;
		}

		x = GetOpponentPlayColumn(Board);
		DropPiece(Board, x, OPPONENT);
		bWon = Any4InARow(Board, OPPONENT, &xs, &ys, &xi, &yi);
		if (bWon)
		{
			PrintGrid(Board);
			printf("Opponent won!\n");
			break;
		}

	}

	return 0;
}

const char* PieceString[3][5] = {
	{
		"         ",
		"         ",
		"         ",
		"         ",
		"         "
	},
	{
		"   ___   ",
		" // | \\\\ ",
		"|| -+- ||",
		" \\\\ | // ",
		"   ---   "
	},
	{
		"   ___   ",
		" // o \\\\ ",
		"|| OOO ||",
		" \\\\ o // ",
		"   ---   "
	}
};

void PrintGrid(int Board[][GRIDWIDTH])
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

int GetUserPlayColumn(int Board[][GRIDWIDTH])
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

void DropPiece(int Board[][GRIDWIDTH], int Column, int Player)
{
	int r = HowManyRowsFilled(Board, Column);
	Board[r][Column] = Player;
}

bool Any4InARow(int Board[][GRIDWIDTH], int Player, int* xs, int* ys, int* xi, int* yi)
{
	// from each spot, only look up and right
	// since we're going to look up, make sure teh loop bounds are correct
	int StartingValue = -1;
	for (int y = 0; y <= GRIDHEIGHT - 4; y++)
	{
		for (int x = 0; x <= GRIDWIDTH - 4; x++)
		{
			int v = Board[y][x];
			if (StartingValue == -1)
			{
				StartingValue = v;
			}

			if (v == 0) // empty
			{
				continue;
			}

			// go look in 3 directions from start

			// look up

			int FoundLen = 1;

			for ( int len = 1 ; len <= 3; len++)
			{
				int v2 = Board[y + len][x];
				if (v2 != v)
				{
					break;
				}
				FoundLen = len + 1;
			}

			if (FoundLen == 4)
			{
				*xs = x;
				*ys = y;
				*xi = 0;
				*yi = 1;
				return true;
			}

			FoundLen = 1;

			for (int len = 1; len <= 3; len++)
			{
				int v2 = Board[y + len][x + len];
				if (v2 != v)
				{
					break;
				}
				FoundLen = len + 1;
			}

			if (FoundLen == 4)
			{
				*xs = x;
				*ys = y;
				*xi = 1;
				*yi = 1;
				return true;
			}

			FoundLen = 1;

			for (int len = 1; len <= 3; len++)
			{
				int v2 = Board[y][x + len];
				if (v2 != v)
				{
					break;
				}
				FoundLen = len + 1;
			}

			if (FoundLen == 4)
			{
				*xs = x;
				*ys = y;
				*xi = 1;
				*yi = 0;
				return true;
			}
		}
	}

	return false;
}

int HowManyRowsFilled(int Board[][GRIDWIDTH], int Column)
{
	for (int r = 0; r < GRIDHEIGHT; r++)
	{
		if (Board[r][Column] == 0)
		{
			return r;
		}
	}
	return 4;
}

bool IsTakeable(int Board[][GRIDWIDTH], int x, int y)
{
	if (Board[y][x] != 0) return false;
	// the space is empty
	if (y == 0) return true;
	if (Board[y - 1][x] == 0) return false;
	return true;
}

bool CanOpponentMakeConnect4Here(int Board[][GRIDWIDTH], int x, int y, int* MovesNeeded)
{
	// each spot has to be taken by opponent, or choosable.
	*MovesNeeded = -1;
	int BestMovesNeeded = 5;

	int OpponentCount = 0;
	if (Board[y][x] == USER)
	{
		return false;
	}
	if (Board[y][x] == OPPONENT)
	{
		OpponentCount = 1;
	}
	if (!IsTakeable(Board, x, y))
	{
		return false;
	}

	//											  /   ^
	//                                           /    |
	// go look in 3 directions from start --->  /     |

	// look up

	bool bFoundUser = false;
	
	for (int len = 1; len <= 3; len++)
	{
		int v2 = Board[y + len][x];
		if (v2 == USER)
		{
			bFoundUser = true;
			break;
		}
		if (v2 == OPPONENT)
		{
			OpponentCount++;
		}
	}

	if (!bFoundUser)
	{
		int RemainingPlays = 4 - OpponentCount;
		if (RemainingPlays < BestMovesNeeded)
		{
			BestMovesNeeded = RemainingPlays;
		}
	}

	// look diagnonal up

	bFoundUser = false;

	for (int len = 1; len <= 3; len++)
	{
		int v2 = Board[y + len][x + len];
		if (v2 == USER)
		{
			bFoundUser = true;
			break;
		}
		if (v2 == OPPONENT)
		{
			OpponentCount++;
		}
	}

	if (!bFoundUser)
	{
		int RemainingPlays = 4 - OpponentCount;
		if (RemainingPlays < BestMovesNeeded)
		{
			BestMovesNeeded = RemainingPlays;
		}
	}

	// look right

	bFoundUser = false;

	for (int len = 1; len <= 3; len++)
	{
		int v2 = Board[y][x + len];
		if (v2 == USER)
		{
			bFoundUser = true;
			break;
		}
		if (v2 == OPPONENT)
		{
			OpponentCount++;
		}
	}

	if (!bFoundUser)
	{
		int RemainingPlays = 4 - OpponentCount;
		if (RemainingPlays < BestMovesNeeded)
		{
			BestMovesNeeded = RemainingPlays;
		}
	}

	*MovesNeeded = BestMovesNeeded;
	if (BestMovesNeeded > 4)
	{
		return false;
	}

	return true;
}

// only the two following functions have any brains in them...

// recursive function. Can be called to make a play for USER or OPPONENT. It's point is to find:
// 1: shortest path to a win for OPPONENT without USER winning first
//		If there is an immediate win, take it!
//		If there is a win in 2 moves and every other path is a non-result or takes longer, take the shortest path to the win
// 2: shortest path to the USER winning.
void BestMoveForOpponent(int Board[GRIDHEIGHT][GRIDWIDTH], int WhichPlayer, 
	int* ShortestWinMoves, int* BestColForShortestWin,
	int* ShortestLossMoves, int* BestColForShortestLoss,
	int CurrentDepth, int* MovesSearched)
{
	if (CurrentDepth > 4 * 2) // both players move 4 times
	{
		return; // not sure who wins, too far ahead
	}

	// is there an immediate win at this depth?

	for (int x = 0; x < GRIDWIDTH; x++)
	{
		int rowsFilled = HowManyRowsFilled(Board, x);
		if (rowsFilled == GRIDHEIGHT)
		{
			continue;
		}
		*MovesSearched = *MovesSearched + 1;

		Board[rowsFilled][x] = WhichPlayer;
		int xs, ys, xi, yi;
		bool Any4 = Any4InARow(Board, WhichPlayer, &xs, &ys, &xi, &yi);
		Board[rowsFilled][x] = 0; // unset it
		if (Any4)
		{
			if (WhichPlayer == OPPONENT)
			{
				if (*BestColForShortestWin == -1)
				{
					*ShortestWinMoves = CurrentDepth;
					*BestColForShortestWin = x;
				}
			}
			else
			{
				if (*BestColForShortestLoss == -1)
				{
					*ShortestLossMoves = CurrentDepth;
					*BestColForShortestLoss = x;
				}
			}
			return;
		}
	}

	// NONE of them were an immediate win. Now we'll try each one and recursively...

	for (int x = 0; x < GRIDWIDTH; x++)
	{
		int rowsFilled = HowManyRowsFilled(Board, x);
		if (rowsFilled == GRIDHEIGHT)
		{
			continue;
		}

		// Set our piece, then recursively see who wins and when

		int Other;
		if (WhichPlayer == USER)
			Other = OPPONENT;
		else
			Other = USER;

		int BoardCopy[GRIDHEIGHT][GRIDWIDTH];
		memcpy(BoardCopy, Board, GRIDHEIGHT * GRIDWIDTH * sizeof(int));
		BoardCopy[rowsFilled][x] = Other;

		BestMoveForOpponent(BoardCopy, Other, 
			ShortestWinMoves, BestColForShortestWin, 
			ShortestLossMoves, BestColForShortestLoss,
			CurrentDepth + 1, MovesSearched);
	}

	return;
}

int GetOpponentPlayColumn(int Board[][GRIDWIDTH])
{
	// computer has to pick a column to go in...
	// 1. Pick any that will result in an immediate win.
	// 2. Pick one that will not result in next move for user to be an immediate win
	// 3. Pick one that sets up a future line
	// we can do this with repeated simulations, each time picking a different column for the user to move, and finding which is the best one

	int ShortestWinMoves = GRIDWIDTH * GRIDHEIGHT;
	int BestColShortestWin = -1;
	int MovesSearched = 0;
	int ShortestLossMoves = GRIDWIDTH * GRIDHEIGHT;
	int BestColShortestLoss = -1;
	BestMoveForOpponent(Board, OPPONENT, 
		&ShortestWinMoves, &BestColShortestWin, 
		&ShortestLossMoves, &BestColShortestLoss,
		0, &MovesSearched);
	printf("Searched %d moves\n", MovesSearched);
	if (ShortestWinMoves < 9)
	{
		printf("ShortestWinMoves = %d @ col %d\n", ShortestWinMoves, BestColShortestWin);
	}
	if (ShortestLossMoves < 9)
	{
		printf("ShortestLossMoves = %d @ col %d\n", ShortestLossMoves, BestColShortestLoss);
	}

	if (BestColShortestWin != -1 && BestColShortestLoss != -1)
	{
		// pick the move that blocks the loss, over the move
		// that gets us the win.

		if (ShortestWinMoves < ShortestLossMoves)
		{
			printf("ShortestWinMoves is smallest, Opponent is chosing for the win at col %d\n", BestColShortestWin);
			return BestColShortestWin;
		}
		else
		{
			printf("ShortestLossMoves is smallest, Opponent is chosing for the block at col %d\n", BestColShortestLoss);
			return BestColShortestLoss;
		}
	}

	// if there is no loss blocker, pick the win, if there is one
	//
	if (BestColShortestWin != -1)
	{
		printf("There is no loss blocker, so choosing shortest win path @ col %d\n", BestColShortestWin);
		return BestColShortestWin;
	}

	if (BestColShortestLoss != -1)
	{
		printf("There is no winning move in sight, so choosing shortest blocker @ col %d\n", BestColShortestLoss);
		return BestColShortestLoss;
	}

	printf("Choosing a spot at random?\n");

	// winner unclear in the near future
	// use some kind of strategy?
	// find a piece on the board we already played. If there are 3 pieces in any direction from it that are played by OPPONENT or are open,
	// go pick them preferentially.

	int MinPlaysToWin = GRIDWIDTH * GRIDHEIGHT;
	int BestCol = -1;

	for (int i = 0; i < GRIDWIDTH; i++)
	{
		int rowsfilled = HowManyRowsFilled(Board, i);
		if (rowsfilled == GRIDHEIGHT)
			continue;

		int PlaysToWin = 0;
		bool room = CanOpponentMakeConnect4Here(Board, i, rowsfilled, &PlaysToWin);
		if (room)
		{
			if (PlaysToWin < MinPlaysToWin)
			{
				MinPlaysToWin = PlaysToWin;
				BestCol = i;
			}
		}
	}

	if (BestCol != -1)
	{
		return BestCol;
	}

	// return one at random?
	while (true)
	{
		int col = rand() % GRIDWIDTH;
		int rowsfilled = HowManyRowsFilled(Board, col);
		if (rowsfilled != GRIDHEIGHT)
		{
			return col;
		}
	}
}

