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

typedef struct _ColStats
{
	int TotalWins;
	int TotalLosses;
} ColStats;

#define GRIDWIDTH 8
#define GRIDHEIGHT 6
int LookaheadMoves = 8;

PlayerType TheBoard[GRIDHEIGHT][GRIDWIDTH];
int Round = 1;

void PrintGrid(PlayerType Board[][GRIDWIDTH]);
int GetUserPlayColumn(PlayerType Board[][GRIDWIDTH]);
int DropPiece(PlayerType Board[][GRIDWIDTH], int Column, PlayerType Player); // returns row
int GetOpponentPlayColumn(PlayerType Board[][GRIDWIDTH]);
PlayerType Any4InARowAtLoc(
	PlayerType Board[][GRIDWIDTH],
	PlayerType WhichPlayer,
	bool MarkIfWin,
	int x, int y); // starting loc
int HowManyRowsFilled(PlayerType Board[][GRIDWIDTH], int Column);
bool IsTakeable(PlayerType Board[][GRIDWIDTH], int x, int y);
PlayerType GetOtherPlayer(PlayerType WhichPlayer);
void DisplayYouLose();

// only smart routine
void TryRecursiveColumn(
	PlayerType Board[GRIDHEIGHT][GRIDWIDTH], 
	PlayerType WhichPlayer,
	int Column,
	ColStats ColStats[GRIDWIDTH],
	int CurrentDepth, 
	int* MovesSearched);
