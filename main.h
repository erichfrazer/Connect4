#pragma once
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>

#define HUMAN 1
#define OPPONENT 2
#define GRIDWIDTH 8
#define GRIDHEIGHT 6
#define LOOKAHEAD_MOVES 8

char TheBoard[GRIDHEIGHT][GRIDWIDTH] = { 0 };
int PiecesPlaced = 0;
int Round = 1;

void PrintGrid(char Board[][GRIDWIDTH]);
int GetUserPlayColumn(char Board[][GRIDWIDTH]);
void DropPiece(char Board[][GRIDWIDTH], int Column, int Player);
int GetOpponentPlayColumn(char Board[][GRIDWIDTH]);
bool Any4InARow(char Board[][GRIDWIDTH], int Player, bool MarkIfWin);
int HowManyRowsFilled(char Board[][GRIDWIDTH], int Column);
bool IsTakeable(char Board[][GRIDWIDTH], int x, int y);
int GetOtherPlayer(int WhichPlayer);

// smart routines
bool TryRecursiveMove(
	char Board[GRIDHEIGHT][GRIDWIDTH], 
	int WhichPlayer, 
	int* PathTaken,
	int* ShortestWinMoves, char* LookaheadWinMoves,
	int* LongestLossMoves, char* LookaheadLossMoves,
	int CurrentDepth, int* MovesSearched);
bool CanOpponentMakeConnect4Here(char Board[][GRIDWIDTH], int x, int y, int* MovesNeeded);
