#include<stdio.h>
#include"conio2.h"
#define BOARD_SIZE 13
#define DEAFULT_DISPLAY true
#define BOARD_BORDER_COLOR 13
#define EMPTY 0
#define BLACK_STONE 1
#define WHITE_STONE 2
#define MARKER 4
#define LIBERTY 3

char pieces [] = { '-','X','O','+','b' };

//int* block[];
//int* liberties[];

bool isInArray(int* tab[], int* element, int size) {
	for (int i = 0; i < size; i++) {
		if (tab[i] == element) {
			return true;
		}
	}
	return false;
}


struct Player {
	int score;
};


//void count(char board[][board_size], int x, int y, int startx, int starty, bool blackturn) {
//	//init piece
//	char piece = board[y - starty][(x - startx) / 2];
//	
//	if (blackturn) {
//		if(piece==black_stone && isinarray(block,&(board[y - starty][(x - startx) / 2]),board_size*board_size)
//	}
//}


void fill_field(int field[][BOARD_SIZE]) {
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			field[j][i] = EMPTY;
		}
	}
}

bool isLegal(int board[][BOARD_SIZE], int x, int y, int startx, int starty) {

	//check if coordinate is empty
	if (board[y - starty][(x - startx) / 2] == EMPTY)
	{	
		
		//check if coordinate is edge
		if (y - starty == 0 || y - starty == BOARD_SIZE - 1 || (x - startx) / 2 == 0 || (x - startx) / 2 == BOARD_SIZE - 1) {
			//left top corner
			if (y - starty == 0 && (x - startx) / 2 == 0) {
				if (board[y - starty + 1][(x - startx) / 2] == EMPTY || board[y - starty][(x + 2 - startx) / 2] == EMPTY) return true;
			}
			//left bottom corner
			if (y - starty == BOARD_SIZE - 1 && (x - startx) / 2 == 0) {
				if (board[y - starty - 1][(x - startx) / 2] == EMPTY || board[y - starty][(x + 2 - startx) / 2] == EMPTY) return true;
			}
			//right top corner
			if (y - starty == 0 && (x - startx) / 2 == BOARD_SIZE - 1) {
				if (board[y - starty + 1][(x - startx) / 2] == EMPTY || board[y - starty][(x - 2 - startx) / 2] == EMPTY) return true;
			}
			//left top corner
			if (y - starty == BOARD_SIZE - 1 && (x - startx) / 2 == BOARD_SIZE - 1) {
				if (board[y - starty + 1][(x - startx) / 2] == EMPTY || board[y - starty][(x - 2 - startx) / 2] == EMPTY) return true;
			}
			//top edge
			else if (y - starty == 0) {
				if (board[y - starty + 1][(x - startx) / 2] == EMPTY || board[y - starty][(x - 2 - startx) / 2] == EMPTY || board[y - starty][((x + 2 - startx) / 2)] == EMPTY) return true;
			}
			//bottom edge
			else if (y - starty == BOARD_SIZE - 1) {
				if (board[y - starty - 1][(x - startx) / 2] == EMPTY || board[y - starty][(x - 2 - startx) / 2] == EMPTY || board[y - starty][((x + 2 - startx) / 2)] == EMPTY) return true;
			}
			//right edge
			else if ((x - startx) / 2 == BOARD_SIZE - 1) {
				if (board[y - starty - 1][(x - startx) / 2] == EMPTY || board[y - starty][(x - 2 - startx) / 2] == EMPTY || board[y - starty + 1][(x - startx) / 2] == EMPTY) return true;
			}
			//left edge
			else if ((x - startx) / 2 == 0) {
				if (board[y - starty - 1][(x - startx) / 2] == EMPTY || board[y - starty][(x + 2 - startx) / 2] == EMPTY || board[y - starty + 1][(x - startx) / 2] == EMPTY) return true;
			}
		}
		//minimum one liberty if is not edge
		else if ((board[y - starty + 1][(x - startx) / 2] == EMPTY || board[y - starty - 1][(x - startx) / 2] == EMPTY || board[y - starty][((x + 2 - startx) / 2)] == EMPTY || board[y - starty][((x - 2 - startx) / 2)] == EMPTY)) return true;
	}  
	return false;
}

void print_board(int x, int y, int field[][BOARD_SIZE]) {
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
		x = startx; // x is always ittering from starting value
		
		//right border
		gotoxy(x-1, y);
		textcolor(BOARD_BORDER_COLOR);
		putch('#');
		textcolor(WHITE);
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			gotoxy(x, y);
			putch(pieces[field[i][j]]);

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

void print_legend(int startx, int starty, int board[][BOARD_SIZE], int x, int y, int startx_board, int starty_board,bool turnBlack, int WhitePoints, int BlackPoints) {
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
	int zn = 0, zero = 0, startx, starty, x, y, legend_x, legend_y;
	int board[BOARD_SIZE][BOARD_SIZE];
	char txt[32];
	bool turnBlack = true;
	struct Player white, black;
	white.score = 3;
	black.score = 5;
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
		
		
		print_legend(legend_x, legend_y, board, x, y, startx, starty,turnBlack,white.score,black.score);
		
		
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
				board[y - starty][(x - startx) / 2] = BLACK_STONE;
				turnBlack = false;
			}
			else if(!turnBlack && isLegal(board, x, y, startx, starty)) {
				board[y - starty][(x - startx) / 2] = WHITE_STONE;
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
