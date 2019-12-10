/*
 * edit.c - Display a game of checkers and scroll through the moves.
 *
 * @author Eli Musgrove
 */


/* -------- */
/* INCLUDES */
/* -------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "termbox/src/termbox.h"
#include "processfile.h"
#include "editFunc.h"
#include "updategameboard.h"


/* ----------- */
/* GLOBAL VARS */
/* ----------- */
extern int initConfig[8];
extern unsigned int* moves;
extern int gameboard[8][8];
extern int illegalMove[5];


/* ----------- */
/* MAIN METHOD */
/* ----------- */
int main(int argc, char** argv) {


	/* -------- */
	/* FILE I/O */
	/* -------- */

	/* Initializations */
	int TB_CURSOR_X = 0;
	int TB_CURSOR_Y = 6;
	int MOVE_NUM = 0;
	char* filename = 0;
	int set = 0;

	/* Get input filename */
	for (int i = 1; i < argc; i++) {
		/* Filename already set */
		if (set) {
			fprintf(stderr, "%s\n", "Error: Multiple file names given.");
			exit(1);
		}
		set = 1;
		filename = argv[i];
	}

	/* Assign stdin if filename given */
	stdin = 0;
	if (filename) {
		stdin = fopen(filename, "r");

		/* Invalid file? */
		if (!stdin) {
			fprintf(stderr, "%s\n", "Error: The file could not be read.");
			exit(1);
		}
	}
	else {
		fprintf(stderr, "%s\n", "Error: No input file given.");
		exit(1);
	}


	/* --------- */
	/* READ FILE */
	/* --------- */

	/* Process input file without message */
	int success = processFile(0, -1);
	if(!success) {
		exit(1);
	}
	

	/* ------------------ */
	/* INITIALIZE TERMBOX */
	/* ------------------ */
	int init = tb_init();
	if (init < 0) {
		tb_shutdown();
		fprintf(stderr, "%s%d\n", "Error: Termbox initialization failure, code ", init);
		exit(1);
	}

	/* Input/output mode */
	tb_select_input_mode(TB_INPUT_ESC);
	tb_select_output_mode(TB_OUTPUT_NORMAL);

	/* Set cursor position */
	tb_set_cursor(TB_CURSOR_X, TB_CURSOR_Y);

	/* Additional initializations */
	int DISPLAYABLE_MOVES = tb_height() - 6;
	int START_MOVE = 0;


	/* ---------------------------- */
	/* COPY GLOBAL ARRAY TO POINTER */
	/* ---------------------------- */
	int **newBoard = (int **) malloc(8 * sizeof(int *)); 
    for (int i = 0; i < 8; i++) {
         newBoard[i] = (int *) malloc(8 * sizeof(int));
	}
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			newBoard[i][j] = gameboard[i][j];
		}
	}


	/* -------------- */
	/* VALIDATE MOVES */
	/* -------------- */
	updateGameboard(-1);


	/* -------------- */
	/* DISPLAY HEADER */
	/* -------------- */

 	/* Display title */
	char header[] = "Checkers";
	int i = 0;
	int start = (tb_width() / 2) - 4;
 	while(header[i]) {
  		tb_change_cell(start, 0, header[i], TB_RED, TB_DEFAULT);
  		start++;
  		i++;
 	}

 	/* Display filename */
 	i = 0;
 	char fileMsg[] = "Filename: ";
 	while (fileMsg[i]) {
 		tb_change_cell(i, 1, fileMsg[i], TB_WHITE, TB_DEFAULT);
 		i++;
 	}
 	i = 0;
 	start = 10;
 	while(filename[i]) {
 		tb_change_cell(start, 1, filename[i], TB_WHITE, TB_DEFAULT);
 		start++;
 		i++;
 	}

 	/* Display commands */
 	i = 0;
 	char instrMsg[] = "Commands: {up, down: move cursor}, {esc: quit}";
 	while(instrMsg[i]) {
 		tb_change_cell(i, 2, instrMsg[i], TB_WHITE, TB_DEFAULT);
 		i++;
 	}

 	/* Display move count */
 	i = 0;
 	char moveNum[] = "Move: 0";
 	while(moveNum[i]) {
 		tb_change_cell(i, 3, moveNum[i], TB_WHITE, TB_DEFAULT);
 		i++;
 	}

 	/* Display teams */
 	i = 0;
 	char *firstTeam = (initConfig[3] == 114) ? "Red:" : "Black:";
 	char *secondTeam = (initConfig[3] == 114) ? "Black:" : "Red:";
 	while (firstTeam[i]) {
 		tb_change_cell(i, 5, firstTeam[i], TB_WHITE, TB_DEFAULT);
 		i++;
 	}
 	start = (tb_width() / 2) - strlen(secondTeam);
 	i = 0;
 	while (secondTeam[i]) {
 		tb_change_cell(start, 5, secondTeam[i], TB_WHITE, TB_DEFAULT);
 		i++;
 		start++;
 	}

 	/* ------------- */
	/* DISPLAY MOVES */
	/* ------------- */
	displayMoves(moves, 0, initConfig[0], illegalMove[0]);


 	/* ----------------- */
	/* DISPLAY GAMEBOARD */
	/* ----------------- */
 	displayBoard(newBoard, 1, tb_width() - 24);


 	/* ----------------- */
	/* HANDLE KEY EVENTS */
	/* ----------------- */

	/* Sync buffer with terminal */
	tb_present();

	/* Handle key press events */
	while(1) {
  		struct tb_event ev;
  		tb_poll_event(&ev);

  		/* Keys */
  		if (ev.key == TB_KEY_ESC) {
  			/* Free memory */
			for (int i = 0; i < 8; i++) {
				free(newBoard[i]);
			}
			free(newBoard);
   			tb_shutdown();
   			printf("%s %d %s %d %s %d\n", "MOVE_NUM: ", MOVE_NUM, ", initConfig[0]: ", initConfig[0], ", illegalMove[0]: ", illegalMove[0]);
   			return 0;
  		}
  		/* Move up */
  		else if (ev.key == TB_KEY_ARROW_UP && (TB_CURSOR_Y > 6 || TB_CURSOR_X > 0)) {
  			if (!TB_CURSOR_X) {
  				TB_CURSOR_X = tb_width() / 2 - 6;
  				tb_set_cursor(TB_CURSOR_X, --TB_CURSOR_Y);
  			}
  			else {
  				TB_CURSOR_X = 0;
  				tb_set_cursor(TB_CURSOR_X, TB_CURSOR_Y);
  			}

  			MOVE_NUM--;

	  		/* Update board */
	  		updateBoard(&newBoard, moves[MOVE_NUM * 4 + 2], moves[MOVE_NUM * 4 + 3], moves[MOVE_NUM * 4], moves[MOVE_NUM * 4 + 1], 1);
  			displayBoard(newBoard, 1, tb_width() - 24);
  		}
  		/* Move down */
  		else if (ev.key == TB_KEY_ARROW_DOWN && (TB_CURSOR_Y < tb_height() || (TB_CURSOR_X == 0 && TB_CURSOR_Y == tb_height())) && (MOVE_NUM < initConfig[0])) {
  			if ((illegalMove[0] && (MOVE_NUM + 1 < illegalMove[0])) || !illegalMove[0]) {
	  			if (!TB_CURSOR_X) {
	  				TB_CURSOR_X = tb_width() / 2 - 6;
	  				tb_set_cursor(TB_CURSOR_X, TB_CURSOR_Y);
	  			}
	  			else {
	  				TB_CURSOR_X = 0;
	  				tb_set_cursor(TB_CURSOR_X, ++TB_CURSOR_Y);

	  				/* Scroll */
	  				if ((MOVE_NUM == (DISPLAYABLE_MOVES * 2) - 1) && (MOVE_NUM < initConfig[0])) {
	  					START_MOVE += 2;
	  					displayMoves(moves, START_MOVE, initConfig[0], illegalMove[0]);
	  				}
	  			}

	  			/* Update board */
	  			updateBoard(&newBoard, moves[MOVE_NUM * 4], moves[MOVE_NUM * 4 + 1], moves[MOVE_NUM * 4 + 2], moves[MOVE_NUM * 4 + 3], 0);
	  			displayBoard(newBoard, 1, tb_width() - 24);
	  			MOVE_NUM++;
  			}
  		}


 		/* ------------------- */
		/* UPDATE MOVE COUNTER */
		/* ------------------- */
  		for (i = 6; i < 38; i++) {
  			tb_change_cell(i, 3, ' ', TB_WHITE, TB_DEFAULT);
  		}
 		i = 0;
  		int counter = 6;
  		int moveDisplay = MOVE_NUM;
  		int moveNumArr[32];
  		while (moveDisplay >= 0) {
  			moveNumArr[i] = moveDisplay % 10;
  			moveDisplay /= 10;
  			i++;

  			if (moveDisplay == 0) {
  				break;
  			}
  		}
  		for (int j = i - 1; j >= 0; j--) {
  			tb_change_cell(counter++, 3, (char) (moveNumArr[j] + 48), TB_WHITE, TB_DEFAULT);
  		}

  		/* Display changes */
  		tb_present();
 	}
}
