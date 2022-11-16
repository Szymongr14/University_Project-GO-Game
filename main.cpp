#include<stdio.h>
#include"conio2.h"
#include <cstring>
#define BOARD_SIZE 13
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
char pieces [] = { '-','X','O','#'};



const int BOARD_RANGE = BOARD_SIZE + 2; // 2 is size of margin

int* block[100];
int liberties = 0;
int index = 0;


bool isMarked(int* tab[], int *element) {
	for (int i = 0; i < 1000; i++) {
		if (tab[i] == element) {
			return true;
		}
	}
	return false;
}


struct Player {
	int score;
}white, black;

int group_value(int* tab[]) {
	int i = 0;
	while (true) {
		if (block[i] == 0) break;
		i++;
	}
	return i;
}

void remove_captured() {
	int i = 0;
	while(true){
		if(block[i]==0) break;
		*block[i++] = EMPTY;
	}
}

void count(int board[][BOARD_RANGE], int x, int y, int color) {
	//init piece
	int piece = board[y][x];
	if (piece == OFFBOARD) return;
	
	
	if (piece == color && !isMarked(block, &board[y][x])) {
			block[index++] = &(board[y][x]);
			count(board, x + 1, y,color);
			count(board, x - 1, y,color);
			count(board, x, y+1,color);
			count(board, x, y-1,color);
	}
	else if (piece == EMPTY) {
		liberties++;
	}
	
}

void check_capturing(int board[][BOARD_RANGE],int color) {
	for (int i = 0; i < BOARD_RANGE; i++){
		for (int j = 0; j < BOARD_RANGE; j++){
			if (board[i][j] == color) {
				count(board, j, i, color);
				if (liberties == 0) { 
					remove_captured();
					 if(color==WHITE_STONE)black.score += group_value(block);
					 if (color == BLACK_STONE)white.score += group_value(block);
				}
				//restoring values after checking
				memset(block, 0, sizeof(block));
				liberties = 0;
				index = 0;
				
			}
		}
	}
	//restoring values after checking
	memset(block, 0, sizeof(block));
	liberties = 0;
	index = 0;
}

void fill_field(int field[][BOARD_RANGE]) {
	
	for (int i = 0; i < BOARD_RANGE; i++){
		for (int j = 0; j < BOARD_RANGE; j++){
			if (i == 0 || j == 0 || i == BOARD_RANGE - 1 || j == BOARD_RANGE - 1) field[j][i] = OFFBOARD;
			else field[j][i] = EMPTY;
		}
	}
}

bool isLegal(int board[][BOARD_RANGE], int x, int y, int startx, int starty) {
	//check if coordinate is empty and has at least one liberty
	if (board[(y - starty) + 1][((x - startx) / 2) + 1] == EMPTY && ((board[(y - starty) + 2][((x - startx) / 2) + 1] == EMPTY || board[(y - starty)][((x - startx) / 2) + 1] == EMPTY || board[(y - starty) + 1][((x - startx) / 2) + 2] == EMPTY || board[(y - starty) + 1][((x - startx) / 2)] == EMPTY))) return true;
	return false;
}

void print_board(int x, int y, int field[][BOARD_RANGE]) {
	int starty = y;
	int startx = x;
	
	for (int i = 0; i < BOARD_RANGE; i++){
		x = startx; // rows always from the start
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
	gotoxy(startx, starty++);
	sprintf(txt, "current (x,y) : (%d,%d)", x, y);
	cputs(txt);
	gotoxy(startx, starty+=3);
	gotoxy(startx, starty++);
	textcolor(3);
	cputs("  GAME STATS");
	textcolor(WHITE);
	gotoxy(startx, starty++);
	if(turnBlack) cputs("turn : BLACK");
	if(!turnBlack)cputs("turn : WHITE");
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

		//cursor moving only in board area
		if(zn == 0) {
			zero = 1;		
			zn = getch();		
			if (zn == UP_ARROW && y != starty) y--;
			else if (zn == DOWN_ARROW && y != starty + (BOARD_SIZE - 1)) y++;
			else if (zn == LEFT_ARROW && x != startx) x -= 2;
			else if (zn == RIGHT_ARROW && x != startx + (BOARD_SIZE * 2 - 2)) x += 2;
		}
		// putting stone
		if (zn == 'i') {
			if (turnBlack && isLegal(board,x,y,startx,starty)) {
				board[(y - starty)+1][((x - startx) / 2)+1] = BLACK_STONE;
				check_capturing(board, WHITE_STONE);
				turnBlack = false;
			}
			else if (!turnBlack&& isLegal(board, x, y, startx, starty)) {
				board[(y - starty) + 1][((x - startx) / 2) + 1] = WHITE_STONE;
				check_capturing(board,BLACK_STONE);
				turnBlack = true;
			}
		}

		//new game
		if (zn == 'n') {
			fill_field(board);
			x = startx;
			y = starty;
			white.score = 0;
			black.score = 0;
		}
	} while (zn != 'q');

	_setcursortype(_NORMALCURSOR);
					
	return 0;
	}
