#pragma once
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>

#define USER 1
#define OPPONENT 2
#define GRIDWIDTH 8
#define GRIDHEIGHT 6

int Board[GRIDHEIGHT][GRIDWIDTH] = { 0 };
int PiecesPlaced = 0;
int Round = 1;

void PrintGrid(int Board[][GRIDWIDTH]);
int GetUserPlayColumn(int Board[][GRIDWIDTH]);
void DropPiece(int Board[][GRIDWIDTH], int Column, int Player);
int GetOpponentPlayColumn(int Board[][GRIDWIDTH]);
bool Any4InARow(int Board[][GRIDWIDTH], int Player, int* xs, int* ys, int* xi, int* yi);
int HowManyRowsFilled(int Board[][GRIDWIDTH], int Column);
int FindWinForOpponent(int Board[GRIDHEIGHT][GRIDWIDTH], int WhichPlayer, int* BestCol, int* BestDepth, int Depth);
bool RoomForOpponentWin(int Board[][GRIDWIDTH], int x, int y, int* HowManyPlays);
bool IsTakeable(int Board[][GRIDWIDTH], int x, int y);
