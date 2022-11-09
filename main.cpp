#include<stdio.h>
#include"conio2.h"
#define BOARD_SIZE 9
#define DEAFULT_DISPLAY true
#define BOARD_BORDER_COLOR 13


void fill_field(char field[][BOARD_SIZE]) {
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			field[j][i] = '*';
		}
	}
}

bool isLegal(char board[][BOARD_SIZE],int x, int y, int startx, int starty) {
	if (board[y - starty][(x - startx) / 2] == '*') return true;
	return false;
}

void print_board(int x, int y, char field[][BOARD_SIZE]) {
	int starty = y;
	int startx = x;
	//top border
	gotoxy(startx -1, starty -1);
	textcolor(BOARD_BORDER_COLOR);
	for (int i = 0; i <= (BOARD_SIZE * 2) + 1; i++) {
		putch('#');
	}
	textcolor(WHITE);
	//printing board
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		x = startx; // y is always ittering from starting value
		
		//right border
		gotoxy(x-1, y);
		textcolor(BOARD_BORDER_COLOR);
		putch('#');
		textcolor(WHITE);
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			gotoxy(x, y);
			putch(field[i][j]);

			x += 2;
		}
		//left border
		gotoxy(x, y);
		textcolor(BOARD_BORDER_COLOR);
		putch('#');
		textcolor(WHITE);
		y++;
	}
	//bottom border
	gotoxy(startx - 1, starty + BOARD_SIZE);
	textcolor(BOARD_BORDER_COLOR);
	for (int i = 0; i <= (BOARD_SIZE * 2)+1; i++) {
		putch('#');
	}
	textcolor(WHITE);
}

void print_legend(int startx, int starty, char board[][BOARD_SIZE], int x, int y, int startx_board, int starty_board,bool turnBlack) {
	char txt[32];
	gotoxy(startx, starty++);
	cputs("     q = exit");
	gotoxy(startx, starty++);
	cputs("arrows = moving");
	gotoxy(startx, starty++);
	cputs("     i = place a stone");
	gotoxy(startx, starty++);
	cputs("     n = new game");
	gotoxy(startx, starty++);
	sprintf(txt, "current  (x,y) : (%d,%d)", x, y);
	cputs(txt);
	gotoxy(startx, starty++);
	textcolor(3);
	if(turnBlack) cputs("  turn : BLACK");
	textcolor(4);
	if(!turnBlack)cputs("  turn : WHITE");
	textcolor(WHITE);

	//sprintf(txt, "White : (%d,%d)", x, y);
	//sprintf(txt, "Black : (%d,%d)", x, y);
	print_board(startx_board, starty_board, board);
}

int main() {
	int zn = 0, zero = 0, startx, starty, x, y, legend_x, legend_y;
	char board[BOARD_SIZE][BOARD_SIZE];
	char txt[32];
	bool turnBlack = true;
	textmode(C80);//console widnow size

#ifndef __cplusplus
	Conio2_Init();
#endif
	settitle("GO GAME");
	fill_field(board);

	if (DEAFULT_DISPLAY) { // legend-right   board-left
		
		startx = 38;
		//centering board based on screen width and height
		starty = 13-BOARD_SIZE/2;
		
		legend_x = 9;
		legend_y = 6;
	}
	else { // legend-left   board-right
		startx = 8;
		starty = 13 - BOARD_SIZE / 2;
		legend_x = 50;
		legend_y = 7;
	}
	x = startx;
	y = starty;

	_setcursortype(_NOCURSOR);
	do {
		
		
		clrscr();
		gotoxy(25, 1);
		cputs("Szymon Groszkowski nr 193141");
		
		
		print_legend(legend_x, legend_y, board, x, y, startx, starty,turnBlack);
		
		
		gotoxy(x, y);
		putch('@');

		
		zero = 0;
		zn = getch();

		//cursor moving only in board area
		if(zn == 0) {
			zero = 1;		
			zn = getch();		
			if (zn == 0x48 && y!= starty) y--;
			else if (zn == 0x50 && y!=starty+(BOARD_SIZE-1)) y++;
			else if(zn == 0x4b && x!=startx) x-=2;
			else if(zn == 0x4d && x != startx+(BOARD_SIZE*2-2)) x+=2;
		}
		// putting stone
		if (zn == 'i') {
			if (turnBlack && isLegal(board,x,y,startx,starty)) {
				board[y - starty][(x - startx) / 2] = 'X';
				turnBlack = false;
			}
			else if(!turnBlack && board[y - starty][(x - startx) / 2] == '*') {
				board[y - starty][(x - startx) / 2] = 'O';
				turnBlack = true;
			}
		}

		//new game
		if (zn == 'n') {
			fill_field(board);
			x = startx;
			y = starty;
			//wyzerowac punkty black and white
		}
	} while (zn != 'q');

	_setcursortype(_NORMALCURSOR);
					
	return 0;
	}
