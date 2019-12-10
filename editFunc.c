/*
 * editFunc.c - Helper functions for the edit executable.
 *
 * @author Eli Musgrove
 */

#include "termbox/src/termbox.h"
#include "editFunc.h"
#include <stdlib.h>
#include <stdio.h>

/*
 * Track index of backup array. 
 */
int numBackup = 0;

/* 
 * Backup array to store removed pieces.
 */
int backup[150];

void updateBoard(int*** gameboard, int a, int b, int c, int d, int reverse) {
	/* Convert move to int index */
	a -= 97;
	b = 8 - (b - 48);
	c -= 97;
	d  = 8 - (d - 48);

	(*gameboard)[d][c] = (*gameboard)[b][a];
	(*gameboard)[b][a] = 46;

	/* Jumping */
	if (abs(c - a) == 2 && abs(d - b) == 2) {
		if (reverse) {
			(*gameboard)[(d + b) / 2][(c + a) / 2] = backup[--numBackup];
		}
		else {
			backup[numBackup++] = (*gameboard)[(d + b) / 2][(c + a) / 2];
			(*gameboard)[(d + b) / 2][(c + a) / 2] = 46;
		}
	}
}

/*
 * Dislays 3 squares in the given location.
 */
void displaySquare(int val, int row, int col, int white) {
	if (val == -1) {
		tb_change_cell(col, row, ' ', TB_DEFAULT, white ? TB_WHITE : TB_RED);
		tb_change_cell(col+1, row, ' ', TB_DEFAULT, white ? TB_WHITE : TB_RED);
		tb_change_cell(col+2, row, ' ', TB_DEFAULT, white ? TB_WHITE : TB_RED);
	}
	else {
		tb_change_cell(col, row, ' ', TB_DEFAULT, white ? TB_WHITE : TB_RED);
		switch (val) {
			case 98:
				tb_change_cell(col+1, row, 'o', TB_BLACK, TB_WHITE);
				break;
			case 114:
				tb_change_cell(col+1, row, 'o', TB_RED, TB_WHITE);
				break;
			case 66:
				tb_change_cell(col+1, row, '@', TB_BLACK, TB_WHITE);
				break;
			case 82:
				tb_change_cell(col+1, row, '@', TB_RED, TB_WHITE);
				break;
			default:
				tb_change_cell(col+1, row, ' ', TB_BLACK, white ? TB_WHITE : TB_RED);
				break;
		}
		tb_change_cell(col+2, row, ' ', TB_DEFAULT, white ? TB_WHITE : TB_RED);
	}
}

/*
 * Display a single row of the gameboard.
 */
void displayRow(int **gameboard, int row, int col, int startWhite, int boardRow) {
	/* Display blank row */
	for (int i = 0; i < 8; i++) {
		displaySquare(-1, row, col+(3*i), startWhite);
		startWhite = !startWhite;
	}

	/* Display piece row */
	for (int i = 0; i < 8; i++) {
		displaySquare(gameboard[boardRow][i], row+1, col+(3*i), startWhite);
		startWhite = !startWhite;
	}

	/* Display blank row */
	for (int i = 0; i < 8; i++) {
		displaySquare(-1, row+2, col+(3*i), startWhite);
		startWhite = !startWhite;
	}
}

/*
 * Display the given gameboard.
 */
void displayBoard(int** gameboard, int row, int col) {
	int startWhite = 0;
	for (int i = 0; i < 8; i++) {
		displayRow(gameboard, row+(3*i), col, startWhite, i);
		startWhite = !startWhite;
	}
}

/*
 * Display the moves in the file.
 */
void displayMoves(unsigned int* moves, int startingMove, int numMoves, int illegalMove) {
	int i = startingMove * 4;
 	int start = (tb_width() / 2) - 6;
 	char a, b, c, d = 0;
 	int row = 6;
 	int first = 1;
 	while(i < numMoves * 5) {
 		if (!a) {
 			a = (char) moves[i++];
 			continue;
 		}
 		else if (!b) {
 			b = (char) moves[i++];
 			continue;
 		}
 		else if (!c) {
 			c = (char) moves[i++];
 			continue;
 		}
 		else if (!d) {
 			d = (char) moves[i++];
 			continue;
 		}

 		/* Complete move set collected */
 		if (first) {
 			if (illegalMove) {
 				/* Display invalid move */
 				if (i == illegalMove * 4) {
 					tb_change_cell(0, row, a, TB_RED, TB_DEFAULT);
 					tb_change_cell(1, row, b, TB_RED, TB_DEFAULT);
 					tb_change_cell(2, row, '-', TB_RED, TB_DEFAULT);
 					tb_change_cell(3, row, '>', TB_RED, TB_DEFAULT);
 					tb_change_cell(4, row, c, TB_RED, TB_DEFAULT);
 					tb_change_cell(5, row, d, TB_RED, TB_DEFAULT);
 				}
 				/* Display unknown validity moves */
 				else if (i > illegalMove * 4) {
 					tb_change_cell(0, row, a, TB_WHITE, TB_DEFAULT);
 					tb_change_cell(1, row, b, TB_WHITE, TB_DEFAULT);
 					tb_change_cell(2, row, '-', TB_WHITE, TB_DEFAULT);
 					tb_change_cell(3, row, '>', TB_WHITE, TB_DEFAULT);
 					tb_change_cell(4, row, c, TB_WHITE, TB_DEFAULT);
 					tb_change_cell(5, row, d, TB_WHITE, TB_DEFAULT);
 				}
 				/* Display valid moves */
 				else {
 					tb_change_cell(0, row, a, TB_GREEN, TB_DEFAULT);
 					tb_change_cell(1, row, b, TB_GREEN, TB_DEFAULT);
 					tb_change_cell(2, row, '-', TB_GREEN, TB_DEFAULT);
 					tb_change_cell(3, row, '>', TB_GREEN, TB_DEFAULT);
 					tb_change_cell(4, row, c, TB_GREEN, TB_DEFAULT);
 					tb_change_cell(5, row, d, TB_GREEN, TB_DEFAULT);
 				}
 			}
 			/* Display valid moves */
 			else {
 				tb_change_cell(0, row, a, TB_GREEN, TB_DEFAULT);
 				tb_change_cell(1, row, b, TB_GREEN, TB_DEFAULT);
 				tb_change_cell(2, row, '-', TB_GREEN, TB_DEFAULT);
 				tb_change_cell(3, row, '>', TB_GREEN, TB_DEFAULT);
 				tb_change_cell(4, row, c, TB_GREEN, TB_DEFAULT);
 				tb_change_cell(5, row, d, TB_GREEN, TB_DEFAULT);
 			}
 		}
 		else {
 			/* Moves contain an invalid */
 			if (illegalMove != -1) {
 				/* Display invalid move */
 				if (i == illegalMove * 4) {
 					tb_change_cell(start, row, a, TB_RED, TB_DEFAULT);
 					tb_change_cell(start + 1, row, b, TB_RED, TB_DEFAULT);
 					tb_change_cell(start + 2, row, '-', TB_RED, TB_DEFAULT);
 					tb_change_cell(start + 3, row, '>', TB_RED, TB_DEFAULT);
 					tb_change_cell(start + 4, row, c, TB_RED, TB_DEFAULT);
 					tb_change_cell(start + 5, row, d, TB_RED, TB_DEFAULT);
 				}
 				/* Display unknown validity moves */
 				else if (i > illegalMove * 4) {
 					tb_change_cell(start, row, a, TB_WHITE, TB_DEFAULT);
 					tb_change_cell(start + 1, row, b, TB_WHITE, TB_DEFAULT);
 					tb_change_cell(start + 2, row, '-', TB_WHITE, TB_DEFAULT);
 					tb_change_cell(start + 3, row, '>', TB_WHITE, TB_DEFAULT);
 					tb_change_cell(start + 4, row, c, TB_WHITE, TB_DEFAULT);
 					tb_change_cell(start + 5, row, d, TB_WHITE, TB_DEFAULT);
 				}
 				/* Display valid moves */
 				else {
 					tb_change_cell(start, row, a, TB_GREEN, TB_DEFAULT);
 					tb_change_cell(start + 1, row, b, TB_GREEN, TB_DEFAULT);
 					tb_change_cell(start + 2, row, '-', TB_GREEN, TB_DEFAULT);
 					tb_change_cell(start + 3, row, '>', TB_GREEN, TB_DEFAULT);
 					tb_change_cell(start + 4, row, c, TB_GREEN, TB_DEFAULT);
 					tb_change_cell(start + 5, row, d, TB_GREEN, TB_DEFAULT);
 				}
 			}
 			/* Valid moves */
 			else {
 				tb_change_cell(start, row, a, TB_GREEN, TB_DEFAULT);
 				tb_change_cell(start + 1, row, b, TB_GREEN, TB_DEFAULT);
 				tb_change_cell(start + 2, row, '-', TB_GREEN, TB_DEFAULT);
 				tb_change_cell(start + 3, row, '>', TB_GREEN, TB_DEFAULT);
 				tb_change_cell(start + 4, row, c, TB_GREEN, TB_DEFAULT);
 				tb_change_cell(start + 5, row, d, TB_GREEN, TB_DEFAULT);
 			}
 			row++;
 		}
 		a = b = c = d = 0;
 		first = !first;
 	}
}
