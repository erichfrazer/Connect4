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

void SmallPrintGrid(PlayerType Board[][GRIDWIDTH])
{
	for (int y = GRIDHEIGHT - 1; y >= 0; y--)
	{
		for (int x = 0; x < GRIDWIDTH; x++)
		{
			int v = Board[y][x];
			if (v == HUMAN) printf("H");
			if (v == OPPONENT) printf("O");
			if (v == HUMAN_WON) printf("X");
			if (v == OPPONENT_WON) printf("X");
			if (v == EMPTY) printf(".");
		}

		printf("\n");
	}
	printf("\n");
}

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

bool CanOpponentMakeConnect4Here(PlayerType Board[][GRIDWIDTH], int x, int y, int* MovesNeeded)
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
		PlayerType v2 = Board[y + len][x];
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
		PlayerType v2 = Board[y + len][x + len];
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
		PlayerType v2 = Board[y][x + len];
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

PlayerType GetOtherPlayer(PlayerType WhichPlayer)
{
	if (WhichPlayer == HUMAN) return OPPONENT;
	return HUMAN;
}

// entirely deterministic. No AI
int BlockingMoveToPreventHumanWin(PlayerType Board[][GRIDWIDTH], int PathToHumanWin[LOOKAHEAD_MOVES], int MovesLen)
{
	int xStart = -1;
	int yStart = -1;
	int xDir = -1;
	int yDir = -1;
	{
		PlayerType BoardCopy[GRIDHEIGHT][GRIDWIDTH];
		memcpy(BoardCopy, Board, GRIDHEIGHT * GRIDWIDTH * sizeof(PlayerType));

		for (int i = 0; i < MovesLen; i++)
		{
			PlayerType pt = i % 2 == 0 ? OPPONENT : HUMAN;
			int col = PathToHumanWin[i];
			int rowsFilled = HowManyRowsFilled(BoardCopy, col);
			BoardCopy[rowsFilled][col] = pt;
		}

		PlayerType win = Any4InARow(BoardCopy, false, &xStart, &yStart, &xDir, &yDir);
		assert(win != EMPTY);
	}

	// how many human pieces are already set? choose a location closest to an already placed piece for the block
	// X... = XO..
	// X.X. = XOX.
	// X..X = XO.X
	// XX.. = XXO.
	// ..XX = .OXX
	// X.XX = XOXX
	// XXX. = XXXO (and in danger of an immediate win on the other side?)
	// and so on

	int xs = xStart;
	int ys = yStart;
	int humanCount = 0;
	int HumanLocX[4];
	int HumanLocY[4];
	int LineLocX[4];
	int LineLocY[4];
	bool HasHuman[4] = { 0 };
	int LeastDeltaBetweenHumanMoves = 4;
	int LastHumanLoc = -1;
	for (int i = 0; i < 4; i++)
	{
		PlayerType pt = Board[ys][xs];
		if (pt == HUMAN)
		{
			if (LastHumanLoc == -1)
			{
				LastHumanLoc = i;
			}
			int delta = i - LastHumanLoc;
			if (delta != 0)
			{
				if (delta < LeastDeltaBetweenHumanMoves)
				{
					LeastDeltaBetweenHumanMoves = delta;
				}
			}
			HumanLocX[humanCount] = xs;
			HumanLocY[humanCount] = ys;
			HasHuman[i] = Board[ys][xs];
			if (HasHuman[i])
			{
				humanCount++;
			}
		}
		LineLocX[i] = xs;
		LineLocY[i] = ys;
		xs += xDir;
		ys += yDir;
	}

	printf("Find block, human win starts at %d, %d, goes dir %d, %d, and has %d pieces placed so far.\n",
		xStart + 1, yStart + 1,
		xDir, yDir,
		humanCount);

	int UseSpot = -1;

	// if humancount = 1, place it next to the human
	// if humancount = 2, place it between in the middle of the two unless you can't
	// if humancount = 3, then place it in the empty spot
	// if humancount = 4, why are we running this?
	assert(humanCount != 4);
	switch (humanCount)
	{
	case 0:
		assert(false);
		break;
	case 4:
		assert(false);
		break;
	case 1:
		if (HasHuman[0])
		{
			assert(IsTakeable(Board, LineLocX[1], LineLocY[1]));
			printf("There are 1 human pieces at %d, so picking col %d\n", LineLocX[0] + 1, LineLocX[1] + 1);
			UseSpot = 1;
			break;
		}
		else if (HasHuman[3])
		{
			assert(IsTakeable(Board, LineLocX[2], LineLocY[2]));
			printf("There are 1 human pieces at %d, so picking col %d\n", LineLocX[2] + 1, LineLocX[2] + 1);
			UseSpot = 2;
			break;
		}
		else if( HasHuman[1])
		{
			// choose spot 0 or 2
			if (IsTakeable(Board, LineLocX[0], LineLocY[0]))
			{
				printf("There are 1 human pieces at %d, so picking col %d\n", LineLocX[1] + 1, LineLocX[0] + 1);
				UseSpot = 0;
			}
			else if (IsTakeable(Board, LineLocX[2], LineLocY[2]))
			{
				printf("There are 1 human pieces at %d, so picking col %d\n", LineLocX[1] + 1, LineLocX[2] + 1);
				UseSpot = 2;
			}
			else
			{
				assert(false);
			}
			break;
		}
		else
		{
			// at spot 2
			// choose spot 1 or 3
			if (IsTakeable(Board, LineLocX[1], LineLocY[1]))
			{
				printf("There are 1 human pieces at %d, so picking col %d\n", LineLocX[2] + 1, LineLocX[1] + 1);
				UseSpot = 1;
			}
			else if (IsTakeable(Board, LineLocX[3], LineLocY[3]))
			{
				printf("There are 1 human pieces at %d, so picking col %d\n", LineLocX[2] + 1, LineLocX[3] + 1);
				UseSpot = 3;
			}
			else
			{
				assert(false);
			}
			break;
		}
		break;
	case 2:
		// X..X
		// XX..
		// X.X.
		// .X.X
		// ..XX
		// .XX.
	{
		if (LeastDeltaBetweenHumanMoves == 1)
		{
			// the two X's are next to each other
			// XX..
			// .XX.
			// ..XX
			if (HasHuman[0])
			{
				assert(IsTakeable(Board, LineLocX[2], LineLocY[2]));
				printf("There are 2 human pieces, picking col %d\n", LineLocX[0] + 1);
				UseSpot = 2;
			}
			else if (HasHuman[3])
			{
				assert(IsTakeable(Board, LineLocX[1], LineLocY[1]));
				printf("There are 2 human pieces, picking col %d\n", LineLocX[3] + 1);
				UseSpot = 2;
			}
			else
			{
				if (IsTakeable(Board, LineLocX[0], LineLocY[0]))
				{
					printf("There are 2 human pieces, picking col %d\n", LineLocX[0] + 1);
					UseSpot = 0;
				}
				else if (IsTakeable(Board, LineLocX[3], LineLocY[3]))
				{
					printf("There are 2 human pieces, picking col %d\n", LineLocX[3] + 1);
					UseSpot = 3;
				}
				else
				{
					assert(false);
				}
			}
		}
		else if (LeastDeltaBetweenHumanMoves == 2)
		{
			// X.X.
			// .X.X
			// in the middle
			if (HasHuman[0])
			{
				assert(IsTakeable(Board, LineLocX[1], LineLocY[1]));
				printf("There are 2 human pieces sep by 1 space, picking col %d\n", LineLocX[1] + 1);
				UseSpot = 1;
			}
			else
			{
				assert(IsTakeable(Board, LineLocX[2], LineLocY[2]));
				printf("There are 2 human pieces sep by 1 space, picking col %d\n", LineLocX[2] + 1);
				UseSpot = 2;
			}
		}
		else
		{
			// X..X
			if (IsTakeable(Board, LineLocX[1], LineLocY[1]))
			{
				printf("There are 2 human pieces sep by 2 spaces, picking col %d\n", LineLocX[1] + 1);
				UseSpot = 1;
			}
			else if (IsTakeable(Board, LineLocX[2], LineLocY[2]))
			{
				printf("There are 2 human pieces sep by 2 spaces, picking col %d\n", LineLocX[2] + 1);
				UseSpot = 2;
			}
			else
			{
				assert(false);
			}
		}
		break;
	} // case 2
	case 3:
	{
		// XXX.
		// XX.X
		// X.XX
		// .XXX
		// take the one with the hole in it
		int NotTaken = -1;
		for (int i = 0; i < 4; i++)
		{
			if (!HasHuman[i])
			{
				NotTaken = i;
				break;
			}
		}
		if (IsTakeable(Board, LineLocX[NotTaken], LineLocY[NotTaken]))
		{
			printf("There are 3 human pieces and one hole at col %d\n", LineLocX[UseSpot] + 1);
			UseSpot = NotTaken;
		}
		else
		{
			assert(false);
		}
		break;
	} // case 3
	} // switch

	int colPick = LineLocX[UseSpot];
	printf("picking column %d\n", colPick + 1);
	return colPick;
}

// only the two following functions have any brains in them...

// recursive function. Can be called to make a play for USER or OPPONENT. It's point is to find:
// 1: shortest path to a win for OPPONENT without USER winning first

void TryRecursiveColumn(
	PlayerType Board[GRIDHEIGHT][GRIDWIDTH], 
	PlayerType WhichPlayer,
	int Column,
	ColStats colStats[GRIDWIDTH],
	int CurrentDepth,
	int* MovesSearched)
{
	*MovesSearched = *MovesSearched + 1;

	// if we've gone recursive too many times, we're done.

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
		int rowsFilled = HowManyRowsFilled(BoardCopy, x);
		if (rowsFilled == GRIDHEIGHT)
		{
			continue;
		}

		// for each try that is open to try, set it, test it recursively, then afterwards, erase it
		// before we try the next column

		BoardCopy[rowsFilled][x] = WhichPlayer;

		// mark that we went this way. Keep a breadcrumb trail of where we went.

		assert(CurrentDepth <= LOOKAHEAD_MOVES);

		PlayerType WhoWon = Any4InARow(BoardCopy, false, NULL, NULL, NULL, NULL);

		int CurrentDepthMoves = CurrentDepth + 1;

		ColStats colStatsLocal[GRIDWIDTH] = { 0 };

		if (WhoWon == OPPONENT)
		{
			colStatsLocal[x].TotalWins++;
		}
		else if (WhoWon == HUMAN)
		{
			colStatsLocal[x].TotalLosses++;
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

		// add our local totals to the parent's totals

		if (CurrentDepth != 0)
		{
			colStats[Column].TotalLosses += colStatsLocal[x].TotalLosses;
			colStats[Column].TotalWins += colStatsLocal[x].TotalWins;
		}
		else
		{
			colStats[x] = colStatsLocal[x];
		}

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

	// which column has the best stats?
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

