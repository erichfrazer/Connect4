#include "main.h"
#include <time.h>


int main(int argc, const char* argv[])
{
	srand(time(nullptr));

	while (true)
	{
		PrintGrid(TheBoard);
		int x = GetUserPlayColumn(TheBoard);
		DropPiece(TheBoard, x, HUMAN);
		bool bWon = Any4InARow(TheBoard, HUMAN, true);
		if (bWon)
		{
			PrintGrid(TheBoard);
			printf("Player won!\n");
			break;
		}

		x = GetOpponentPlayColumn(TheBoard);
		DropPiece(TheBoard, x, OPPONENT);
		bWon = Any4InARow(TheBoard, OPPONENT, true);
		if (bWon)
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

void PrintGrid(char Board[][GRIDWIDTH])
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

int GetUserPlayColumn(char Board[][GRIDWIDTH])
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

void DropPiece(char Board[][GRIDWIDTH], int Column, int Player)
{
	int r = HowManyRowsFilled(Board, Column);
	Board[r][Column] = Player;
}

bool Any4InARow(char Board[][GRIDWIDTH], int Player, bool MarkIfWin)
{
	// from each spot, only look up and right
	// since we're going to look up, make sure the loop bounds are correct

	bool Found4 = false;
	int XWinStartLoc, YWinStartLoc, XWinDirection, YWinDirection;

	for (int y = 0; y <= GRIDHEIGHT - 4; y++)
	{
		for (int x = 0; x <= GRIDWIDTH - 4; x++)
		{
			int StartingValue = Board[y][x];
			if (StartingValue == 0) // empty
			{
				continue;
			}

			// go look in 3 directions from start

			// look up

			int FoundLen = 1;

			for ( int len = 1 ; len <= 3; len++)
			{
				int v2 = Board[y + len][x];
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

			FoundLen = 1;

			for (int len = 1; len <= 3; len++)
			{
				int v2 = Board[y + len][x + len];
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
				break; // out of X
			}

			FoundLen = 1;

			for (int len = 1; len <= 3; len++)
			{
				int v2 = Board[y][x + len];
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
				break; // out of X
			}
		} // for X

		if (Found4)
		{
			break;
		}
	} // for y

	if (Found4)
	{
		if (MarkIfWin)
		{
			int len = 4;
			while (len--)
			{
				Board[YWinStartLoc][XWinStartLoc] += 2; // make it look like a win token
				YWinStartLoc += YWinDirection;
				XWinStartLoc += XWinDirection;
			}
		}
	}

	return Found4;
}

int HowManyRowsFilled(char Board[][GRIDWIDTH], int Column)
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

bool IsTakeable(char Board[][GRIDWIDTH], int x, int y)
{
	if (Board[y][x] != 0) return false;
	// the space is empty
	if (y == 0) return true;
	if (Board[y - 1][x] == 0) return false;
	return true;
}

bool CanOpponentMakeConnect4Here(char Board[][GRIDWIDTH], int x, int y, int* MovesNeeded)
{
	// each spot has to be taken by opponent, or choosable.
	*MovesNeeded = -1;
	int BestMovesNeeded = 5;

	int OpponentCount = 0;
	if (Board[y][x] == HUMAN)
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
		if (v2 == HUMAN)
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
		if (v2 == HUMAN)
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
		if (v2 == HUMAN)
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

int GetOtherPlayer(int WhichPlayer)
{
	if (WhichPlayer == HUMAN) return OPPONENT;
	return HUMAN;
}

// only the two following functions have any brains in them...

// recursive function. Can be called to make a play for USER or OPPONENT. It's point is to find:
// 1: shortest path to a win for OPPONENT without USER winning first
// 2: longest path to the USER winning. prolong as much as possible
bool TryRecursiveMove(
	char Board[GRIDHEIGHT][GRIDWIDTH], 
	int WhichPlayer,
	int* PathTaken,
	int* ShortestWinMoves, int* LookaheadWinMoves,
	int* LongestLossMoves, int* LookaheadLossMoves,
	int CurrentDepth, int* MovesSearched)
{
	// if we've gone recursive too many times, we're done.

	if (CurrentDepth == LOOKAHEAD_MOVES) // both players move 4 times
	{
		return false; // not sure who wins, too far ahead
	}

	int OtherPlayer = GetOtherPlayer(WhichPlayer);

	if (Any4InARow(Board, OtherPlayer, false))
	{
		return true;
	}

	// make a copy of the board

	char BoardCopy[GRIDHEIGHT][GRIDWIDTH];
	memcpy(BoardCopy, Board, GRIDHEIGHT * GRIDWIDTH * sizeof(char));

	for (int x = 0; x < GRIDWIDTH; x++)
	{
		int rowsFilled = HowManyRowsFilled(BoardCopy, x);
		if (rowsFilled == GRIDHEIGHT)
		{
			continue;
		}

		// for each try that is open to try, set it, test it recursively, then afterwards, erase it
		// before we try the next column

		BoardCopy[rowsFilled][x] = OtherPlayer;

		// mark that we went this way. Keep a breadcrumb trail of where we went.

		PathTaken[CurrentDepth] = x;

		bool bFoundWin = TryRecursiveMove(
			BoardCopy,
			OtherPlayer,
			PathTaken,
			ShortestWinMoves, LookaheadWinMoves,
			LongestLossMoves, LookaheadLossMoves,
			CurrentDepth + 1, MovesSearched);

		BoardCopy[rowsFilled][x] = 0;

		if (bFoundWin)
		{
			if (OtherPlayer == OPPONENT)
			{
				if (CurrentDepth < *ShortestWinMoves)
				{
					PathTaken[CurrentDepth] = x;
					*ShortestWinMoves = CurrentDepth;
					memcpy(LookaheadWinMoves, PathTaken, LOOKAHEAD_MOVES);
				}
			}
			else
			{
				if (CurrentDepth > *LongestLossMoves)
				{
					PathTaken[CurrentDepth] = x;
					*LongestLossMoves = CurrentDepth;
					memcpy(LookaheadLossMoves, PathTaken, LOOKAHEAD_MOVES);
				}
			}
		}
	}

	return
}

int GetOpponentPlayColumn(char Board[][GRIDWIDTH])
{
	// computer has to pick a column to go in...
	// 1. Pick any that will result in an immediate win.
	// 2. Pick one that will not result in next move for user to be an immediate win
	// 3. Pick one that sets up a future line
	// we can do this with repeated simulations, each time picking a different column for the user to move, and finding which is the best one
	int LookaheadWinMoves[LOOKAHEAD_MOVES];
	int LookaheadLossMoves[LOOKAHEAD_MOVES];
	int PathTaken[LOOKAHEAD_MOVES];

	for (int i = 0; i < LOOKAHEAD_MOVES; i++)
	{
		LookaheadWinMoves[i] = -1;
		LookaheadLossMoves[i] = -1;
		PathTaken[i] = -1;
	}

	int ShortestWinMoves = GRIDWIDTH * GRIDHEIGHT;
	int MovesSearched = 0;
	int LongestLossMoves = 0;
	TryRecursiveMove(
		Board, 
		HUMAN, 
		PathTaken,
		&ShortestWinMoves, LookaheadWinMoves,
		&LongestLossMoves, LookaheadLossMoves,
		-1, 
		&MovesSearched);

	printf("Searched %d moves\n", MovesSearched);

	int BestColShortestWin = -1;
	if (ShortestWinMoves <= LOOKAHEAD_MOVES)
	{
		BestColShortestWin = LookaheadWinMoves[0];
	}

	int BestColLongestLoss = -1;
	if (LongestLossMoves < LOOKAHEAD_MOVES)
	{
		// loss is imminent. If it's more than that, we can pick a spot at random instead, or try strategy?
		BestColLongestLoss = LookaheadLossMoves[0];
	}

	bool bUseShortestWin = false;
	bool bUseLongestLoss = false;

	if (BestColShortestWin != -1 && BestColLongestLoss != -1)
	{
		// pick the move that blocks the loss, over the move
		// that gets us the win.

		if (ShortestWinMoves < LongestLossMoves)
		{
			bUseShortestWin = true;
		}
		else
		{
			bUseLongestLoss = true;
		}
	}
	else
	{
		// if there is no loss blocker, pick the win, if there is one
		//
		if (BestColShortestWin != -1)
		{
			bUseShortestWin = true;
		}
		else if (BestColLongestLoss != -1)
		{
			bUseLongestLoss = true;
		}
	}

	if (bUseShortestWin)
	{
		printf("Opponent picking shortest win in %d moves\n", ShortestWinMoves );
		for (int i = 0; i < ShortestWinMoves; i++)
		{
			if (i % 2 == 0)
				printf("Opponent col: %d\n", LookaheadWinMoves[i]);
			else
				printf("Player col: %d\n", LookaheadWinMoves[i]);
		}
		printf("\n");
		return BestColShortestWin;
	}

	if (bUseLongestLoss)
	{
		printf("Opponent picking longest loss in %d moves\n", LongestLossMoves);
		for (int i = 0; i < LongestLossMoves; i++)
		{
			if (i % 2 == 0)
				printf("Opponent col: %d\n", LookaheadLossMoves[i]);
			else
				printf("Player col: %d\n", LookaheadLossMoves[i]);
		}
		printf("\n");
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

