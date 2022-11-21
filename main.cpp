#include<stdio.h>
#include"conio2.h"
#include <cstring>
#define BOARD_SIZE 19
#define DEAFULT_DISPLAY true
#define BOARD_BORDER_COLOR 13
#define UP_ARROW 0x48
#define DOWN_ARROW 0x50
#define LEFT_ARROW 0x4b
#define RIGHT_ARROW 0x4d


// stone encoding
#define EMPTY 0
#define BLACK_STONE 1
#define WHITE_STONE 2
#define OFFBOARD 3
const char pieces [] = { '-','X','O','#'};
const int BOARD_RANGE = BOARD_SIZE + 2; // 2 is size of margin



//global variables to count_librties
int* chain[100];//array storing adresses of stones in chain
int liberties = 0;//value of liberties in chain
int chain_index = 0;//variable which helps ittering in chain array 


bool isInChain(int* tab[], int *element) {
	for (int i = 0; i < 100; i++) {
		if (tab[i] == element) {
			return true;
		}
	}
	return false;
}


struct Player {
	int score;
}white, black;

int chain_value(int* tab[]) {
	int i = 0;
	while (true) {
		if (tab[i] == 0) break;
		i++;
	}
	return i;
}

void remove_captured() {
	int i = 0;
	while(true){
		if(chain[i]==0) break;
		*chain[i++] = EMPTY;
	}
}


//function which scan board after each move to find captured chain of stones
void count_liberties(int board[][BOARD_RANGE], int x, int y, int color) {
	//init current_square
	int current_square = board[y][x];
	if (current_square == OFFBOARD) return;
	
	if (current_square == color && !isInChain(chain, &board[y][x])) {
			chain[chain_index++] = &(board[y][x]);
			count_liberties(board, x + 1, y,color);
			count_liberties(board, x - 1, y,color);
			count_liberties(board, x, y+1,color);
			count_liberties(board, x, y-1,color);
	}
	else if (current_square == EMPTY) {
		liberties++;
	}
	
}

void check_capturing(int board[][BOARD_RANGE],int color) {
	for (int i = 0; i < BOARD_RANGE; i++){
		for (int j = 0; j < BOARD_RANGE; j++){
			if (board[i][j] == color) {
				count_liberties(board, j, i, color);
				if (liberties == 0) { 
					remove_captured();
					if(color==WHITE_STONE)black.score += chain_value(chain);
					if (color == BLACK_STONE)white.score += chain_value(chain);
				}
				//restoring global variables after chain liberties counting
				memset(chain, 0, sizeof(chain));
				liberties = 0;
				chain_index = 0;
				
			}
		}
	}
}

void fill_field(int field[][BOARD_RANGE]) {
	
	for (int i = 0; i < BOARD_RANGE; i++){
		for (int j = 0; j < BOARD_RANGE; j++){
			if (i == 0 || j == 0 || i == BOARD_RANGE - 1 || j == BOARD_RANGE - 1) field[j][i] = OFFBOARD;
			else field[j][i] = EMPTY;
		}
	}
}

bool isLegal(int board[][BOARD_RANGE], int x, int y, int startx, int starty,int color) {
	//check if coordinate is empty and has at least one liberty
	if (board[(y - starty) + 1][((x - startx) / 2) + 1] == EMPTY) {
		if(board[(y - starty) + 2][((x - startx) / 2) + 1] == EMPTY || board[(y - starty)][((x - startx) / 2) + 1] == EMPTY || board[(y - starty) + 1][((x - startx) / 2) + 2] == EMPTY || board[(y - starty) + 1][((x - startx) / 2)] == EMPTY) return true;
		else if (board[(y - starty) + 2][((x - startx) / 2) + 1] == color || board[(y - starty)][((x - startx) / 2) + 1] == color || board[(y - starty) + 1][((x - startx) / 2) + 2] == color || board[(y - starty) + 1][((x - startx) / 2)] == color) return true;
	}

	return false;
}

void print_board(int x, int y, int field[][BOARD_RANGE]) {
	int starty = y;
	int startx = x;
	
	for (int i = 0; i < BOARD_RANGE; i++){
		x = startx;
		for (int j = 0; j < BOARD_RANGE; j++){	
			if (i == 0 || j == 0 || i == BOARD_RANGE - 1 || j == BOARD_RANGE - 1) {
				gotoxy(x, y);
				textcolor(13);
				putch(pieces[field[i][j]]);
				textcolor(WHITE);
				x += 2;
			}
			else {
				gotoxy(x, y);
				putch(pieces[field[i][j]]);
				x += 2;
			}
		}
		y++;
	}
}

void print_legend(int startx, int starty, int board[][BOARD_RANGE], int x, int y, int startx_board, int starty_board,bool turnBlack, int WhitePoints, int BlackPoints) {
	char txt[32];
	char txt1[32];
	gotoxy(startx, starty++);
	textcolor(3);
	cputs("  GAME CONTROLS");
	textcolor(WHITE);
	gotoxy(startx, starty++);
	cputs("     q = exit");
	gotoxy(startx, starty++);
	cputs("arrows = moving");
	gotoxy(startx, starty++);
	cputs("     i = place a stone");
	gotoxy(startx, starty++);
	cputs("     n = new game");
	gotoxy(startx-8, starty++);
	cputs("     b = restart + change board size");
	gotoxy(startx, starty++);
	sprintf(txt, "current (x,y) : (%d,%d)", x, y);
	cputs(txt);
	gotoxy(startx, starty+=3);
	gotoxy(startx, starty++);
	textcolor(3);
	cputs("  GAME STATS");
	textcolor(WHITE);
	gotoxy(startx, starty++);
	if(turnBlack) cputs(" turn : BLACK");
	if(!turnBlack)cputs(" turn : WHITE");
	gotoxy(startx, starty++);
	sprintf(txt1, "White : %d", WhitePoints );
	cputs(txt1);
	gotoxy(startx, starty++);
	sprintf(txt, "Black : %d", BlackPoints);
	cputs(txt);
	print_board(startx_board, starty_board, board);
}

int main() {
	//board_start = beginig board with borders
	//start = beging of game board
	int zn = 0, zero = 0, board_startx, board_starty, x, y, legend_x, legend_y, startx, starty;
	int board[BOARD_RANGE][BOARD_RANGE];
	char txt[32];
	bool turnBlack = true;
	white.score = 0;
	black.score = 0;
	textmode(C80);//console widnow size

#ifndef __cplusplus
	Conio2_Init();
#endif
	settitle("GO GAME");
	fill_field(board);

	if (DEAFULT_DISPLAY) { // legend-right   board-left
		
		board_startx = 38;
		//centering board based on screen width and height
		board_starty = 13-BOARD_RANGE/2;
		
		legend_x = 9;
		legend_y = 6;
		startx = board_startx + 2;
		starty = board_starty + 1;
	}
	else { // legend-left   board-right
		board_startx = 8;
		board_starty = 13 - BOARD_RANGE / 2;
		legend_x = 55;
		legend_y = 7;
		startx = board_startx + 2;
		starty = board_starty + 1;
	}


	x = startx;
	y = starty;

	_setcursortype(_NOCURSOR);
	do {
		
		
		clrscr();
		gotoxy(25, 1);
		cputs("Szymon Groszkowski nr 193141");
		
		
		print_legend(legend_x, legend_y, board, x, y, board_startx, board_starty,turnBlack,white.score,black.score);
		
		
		gotoxy(x, y);
		putch('@');

		
		zero = 0;
		zn = getch();

		switch (zn) {
			case DOWN_ARROW:
				if (y != starty + (BOARD_SIZE - 1)) y++;
				break;
			case UP_ARROW:
				if (zn == UP_ARROW && y != starty) y--;
				break;
			case LEFT_ARROW:
				if (x != startx) x -= 2;
				break;
			case RIGHT_ARROW:
				if (x != startx + (BOARD_SIZE * 2 - 2)) x += 2;
				break;
			case 'i':
				//black stone placement
				if (turnBlack && isLegal(board, x, y, startx, starty, BLACK_STONE)) {
					board[(y - starty) + 1][((x - startx) / 2) + 1] = BLACK_STONE;
					check_capturing(board, WHITE_STONE);
					turnBlack = false;
				}
				//white stone placement
				else if (!turnBlack && isLegal(board, x, y, startx, starty, WHITE_STONE)) {
					board[(y - starty) + 1][((x - startx) / 2) + 1] = WHITE_STONE;
					check_capturing(board, BLACK_STONE);
					turnBlack = true;
				}
			case 'n':
				//new game
				if (zn == 'n') {
					fill_field(board);
					x = startx;
					y = starty;
					white.score = 0;
					black.score = 0;
				}

		}
	} while (zn != 'q');

	_setcursortype(_NORMALCURSOR);
					
	return 0;
	}
