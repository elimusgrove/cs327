/*
 * updategameboard.c - Updates the global gameboard array using global arrays moves and gameboard.
 */

#include <stdio.h>
#include <stdlib.h>
#include "updategameboard.h"

/* Gameboard represented as int array */
extern int gameboard[8][8];

/* Moves as processed from file by processfile.c */
extern int* moves;

/* Array of initial configuration for the game */
extern int initConfig[8];

/* If any, the move that caused an error 
 *		Indices:
 * 			0: the move number
 *			1-4: ranks and files of move
 */
int illegalMove[5];

/* 
 * Updates the global gameboard array given the global moves array.
 *
 * @param numToProcess - the number of moves to execute on the gameboard
 * @return int - 0 if there are errors in the move and file combination, 1 otherwise 
 */
int updateGameboard(int numToProcess) {

	/* Promote any pieces that should be kings in initial board after reaching other side */
	int unpromoted = 0;
	for (int i = 0; i < 8; i++) {
		/* Expecting red king */
		if (gameboard[0][i] == 114) {
			gameboard[0][i] = 82;
			unpromoted = 1;
		}
	}

	for (int i = 0; i < 8; i++) {
		/* Expecting black king */
		if (gameboard[7][i] == 98) {
			gameboard[7][i] = 66;
			unpromoted = 1;
		}
	}

	/* Warn user */
	if (unpromoted) {
		fprintf(stderr, "%s\n", "There was an unpromoted piece that should have been a king, the piece is updated to be a king.");
	}

	/* Initialize vars */
	int sourceFile = -1;
	int sourceRank = -1;
	int targetFile = -1;
	int targetRank = -1;

	int sourceSquare;
	int targetSquare;

	int move = 1;

	int redTurn = initConfig[3] == 114 ? 1 : 0;

	/* Execute moves on gameboard */
	numToProcess = (numToProcess == -1) ? initConfig[0] : numToProcess;
	for (int i = 0; i < numToProcess * 4 + 1; i++) {
		
		/* We have a full move set, process it */
		if (sourceFile != -1 && sourceRank != -1 && targetFile != -1 && targetRank != -1) {
			sourceSquare = gameboard[sourceRank][sourceFile];
			targetSquare = gameboard[targetRank][targetFile];

			/* No piece on sourceSquare, 
				or existing piece in targetSquare 
				or difference in rank or file greater than 2 or equal to 0 
				or team is attempting to move opponent piece */
			if (sourceSquare == 34 || sourceSquare == 46 
				|| targetSquare != 46
				|| abs(targetFile - sourceFile) > 2 || abs(targetRank - sourceRank) > 2
				|| abs(targetFile - sourceFile) == 0 || abs(targetRank - sourceRank) == 0
				|| (redTurn && (sourceSquare == 98 || sourceSquare == 66))
				|| (!redTurn && (sourceSquare == 114 || sourceSquare == 82))
				|| (redTurn && sourceSquare == 114 && targetRank > sourceRank)
				|| (!redTurn && sourceSquare == 98 && sourceRank > targetRank)) {
					illegalMove[0] = move;
					illegalMove[1] = sourceFile;
					illegalMove[2] = sourceRank;
					illegalMove[3] = targetFile;
					illegalMove[4] = targetRank;
					return 0;
			}

			/* If skipping, must jump opponent piece */
			if (abs(targetFile - sourceFile) == 2 && abs(targetRank - sourceRank) == 2) {
				int midRank = (sourceRank + targetRank) / 2;
				int midFile = (sourceFile + targetFile) / 2;
				int midSquare = gameboard[midRank][midFile];

				if ((redTurn && midSquare != 98 && midSquare != 66)
					|| (!redTurn && midSquare != 114 && midSquare != 82)) {
					illegalMove[0] = move;
					illegalMove[1] = sourceFile;
					illegalMove[2] = sourceRank;
					illegalMove[3] = targetFile;
					illegalMove[4] = targetRank;
					return 0;
				}

				/* Delete opponent piece */
				gameboard[midRank][midFile] = 46;
			}

			/* Alter source and target squares */
			gameboard[targetRank][targetFile] = gameboard[sourceRank][sourceFile];
			gameboard[sourceRank][sourceFile] = 46;

			/* Promote pieces as necessary */
			if (redTurn && targetRank == 0) {
				gameboard[targetRank][targetFile] = 82;
			}
			if (!redTurn && targetRank == 7) {
				gameboard[targetRank][targetFile] = 66;
			}

			/* Reset move trackers */
			sourceFile = sourceRank = targetFile = targetRank = -1;

			/* Iterate needed counters */
			i--;
			move++;
			redTurn = !redTurn;

			continue;
		}

		/* We have received a file */			
		if (!(i % 2)) {
			/* Convert file to array index, set vars */

			/* We have a source file set, set target file */
			if (sourceFile != -1) {
				targetFile = moves[i] - 97;
			}

			/* Need to set source file */
			else {
				sourceFile = moves[i] - 97;
			}
		}
		/* We have received a rank */
		else {
			/* Convert file to array index, set vars */

			/* We have a source rank set, set target rank */
			if (sourceRank != -1) {
				targetRank = 8 - (moves[i] - 48);
			}
			/* Need to set source rank */
			else {
				sourceRank = 8 - (moves[i] - 48);
			}
		}
	}

	initConfig[3] = (redTurn) ? 114 : 98;
	return 1;
}

/*
 * Outputs the current state of the gameboard in the exchange format.
 */
void exchange(char* fileName, int numProcessed) {
	FILE* fp = fopen(fileName, "w");

	/* Print comments */
	fprintf(fp, "%s", "#\n# Automatically generated exchange file format.\n# \n\n");

	/* Print rules section */
	fprintf(fp, "%s\n", "RULES:");

	if (!initConfig[1]) {
		fprintf(fp, "%s\n", "no capture");
	} 
	else {
		fprintf(fp, "%s\n", "capture");
	}

	if (!initConfig[2]) {
		fprintf(fp, "%s\n", "single jumps");
	}
	else {
		fprintf(fp, "%s\n", "multiple jumps");
	}

	/* Print turn section */
	fprintf(fp, "\n%s%s\n\n", "TURN:\n", (initConfig[3] == 98) ? "black" : "red");

	/* Print board section */
	fprintf(fp, "%s\n", "BOARD:\n-----------------");
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			fprintf(fp, "%s%c", "|", (char) gameboard[i][j]);

			if (j == 7) {
				fprintf(fp, "%s\n", "|");
			}
		}
	}
	fprintf(fp, "%s\n\n", "-----------------");

	/* Print remaining moves */
	fprintf(fp, "%s\n", "MOVES:");
	if (numProcessed != -1 || illegalMove[0]) {
		int startingMove;

		/* Had an error, start there */
		if (illegalMove[0] && illegalMove[0] <= numProcessed) {
			startingMove = illegalMove[0] - 1;
		}
		/* No errors, not processing certain moves */
		else {
			startingMove = numProcessed;
		}

		printf("%s%d\n", "startingMove: ", startingMove);
		
		/* Print remaining moves */
		for (int i = startingMove * 4; i < initConfig[0] * 4; i++) {
			fprintf(fp, "%c%c%s%c%c\n", (char) moves[i], (char) moves[i+1], "->", (char) moves[i+2], (char) moves[i+3]);
			i += 3;
		}
	}

	fclose(fp);
}

/*
 * Outputs the current state of the gameboard in the human format.
 */
void human(char* fileName, int numProcessed) {
	int index;
	FILE* fp = fopen(fileName, "w");

	/* Print file header */
	fprintf(fp, "%s", "    a   b   c   d   e   f   g   h    \n  +---+---+---+---+---+---+---+---+  \n");

	for (int i = 0; i < 8; i++) {
		index = abs(8 - i);
		fprintf(fp, "%d%s", index, " | ");

		for (int j = 0; j < 8; j++) {
			fprintf(fp, "%c%s", (char) gameboard[i][j], " | ");
		}

		fprintf(fp, "%d\n%s\n", index, "  +---+---+---+---+---+---+---+---+  ");
	}

	/* Print file footer */
	fprintf(fp, "%s\n\n", "    a   b   c   d   e   f   g   h    ");

	/* Print remaining moves */
	if (numProcessed != -1 || illegalMove[0]) {
		int startingMove;

		/* Had an error, start there */
		if (illegalMove[0] && illegalMove[0] <= numProcessed) {
			startingMove = illegalMove[0] - 1;
		}
		/* No errors, not processing certain moves */
		else {
			startingMove = numProcessed;
		}
		
		/* Print remaining moves */
		for (int i = startingMove * 4; i < initConfig[0] * 4; i++) {
			fprintf(fp, "%c%c%s%c%c\n", (char) moves[i], (char) moves[i+1], "->", (char) moves[i+2], (char) moves[i+3]);
			i += 3;
		}
	}
	fclose(fp);
}
