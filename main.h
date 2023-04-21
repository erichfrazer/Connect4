#pragma once

#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>
#include <assert.h>

typedef enum
{
	EMPTY = 0,
	HUMAN,
	OPPONENT,
	HUMAN_WON,
	OPPONENT_WON
} PlayerType;

typedef struct _ColStats
{
	int TotalWins;
	int TotalLosses;
} ColStats;

#define GRIDWIDTH 8
#define GRIDHEIGHT 6
#define LOOKAHEAD_MOVES 8

PlayerType TheBoard[GRIDHEIGHT][GRIDWIDTH];
int Round = 1;

void PrintGrid(PlayerType Board[][GRIDWIDTH]);
int GetUserPlayColumn(PlayerType Board[][GRIDWIDTH]);
void DropPiece(PlayerType Board[][GRIDWIDTH], int Column, PlayerType Player);
int GetOpponentPlayColumn(PlayerType Board[][GRIDWIDTH]);
PlayerType Any4InARow(PlayerType Board[][GRIDWIDTH], bool MarkIfWin, int* xStart, int* yStart, int* xDir, int* yDir);
int HowManyRowsFilled(PlayerType Board[][GRIDWIDTH], int Column);
bool IsTakeable(PlayerType Board[][GRIDWIDTH], int x, int y);
PlayerType GetOtherPlayer(PlayerType WhichPlayer);

// only smart routine
void TryRecursiveColumn(
	PlayerType Board[GRIDHEIGHT][GRIDWIDTH], 
	PlayerType WhichPlayer,
	int Column,
	ColStats ColStats[GRIDWIDTH],
	int CurrentDepth, 
	int* MovesSearched);
