#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>

typedef enum
{
	EMPTY = 0,
	HUMAN,
	OPPONENT,
	HUMAN_WON,
	OPPONENT_WON,
	WRONG_PIECE
} PlayerType;

typedef struct _Grid
{
	PlayerType Player;
	int PlayCount;
} Grid;

typedef struct _PathData
{
	PlayerType Player;
	int Column;
} PathData;

typedef struct _ColStats
{
	int TotalWins;
	int TotalLosses;
} ColStats;

#define GRIDWIDTH 8
#define GRIDHEIGHT 6
int LookaheadMoves = 8;

Grid TheBoard[GRIDHEIGHT][GRIDWIDTH];
ColStats TheColStats[GRIDWIDTH];

int PlayCount = 1;

void PrintGrid(Grid Board[][GRIDWIDTH], PlayerType WhichPlayer);
void PrintMiniGrid(Grid Board[][GRIDWIDTH]);
int GetUserPlayColumn(Grid Board[][GRIDWIDTH]);
int DropPiece(Grid Board[][GRIDWIDTH], int Column, PlayerType Player); // returns row
int GetOpponentPlayColumn(Grid Board[][GRIDWIDTH]);
PlayerType Any4InARowAtLoc(
	Grid Board[][GRIDWIDTH],
	PlayerType WhichPlayer,
	bool MarkIfWin,
	int x, int y); // starting loc
int HowManyRowsFilled(Grid Board[][GRIDWIDTH], int Column);
bool IsTakeable(Grid Board[][GRIDWIDTH], int x, int y);
PlayerType GetOtherPlayer(PlayerType WhichPlayer);
void DisplayYouLose();

// only smart routine
void TryRecursiveColumn(
	Grid Board[GRIDHEIGHT][GRIDWIDTH],
	PlayerType WhichPlayer,
	ColStats ColStats[GRIDWIDTH],
	int CurrentDepth,
	PathData ColumnsTaken[],
	int* MovesSearched);
