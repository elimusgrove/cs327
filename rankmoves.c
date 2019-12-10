/*
 * rankmoves.c - Rank the available set of moves looking a given number of moves ahead.
 * 
 * @author Eli Musgrove
 */

#include "processfile.h"
#include "parsemoves.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Game configuration array */
extern int initConfig[8];

/* Initial configuration array filled by processfile.c */
extern int gameboard[8][8];

int main(int argc, char *argv[]) {

	/* Initializations */
	short int verbose = 0;
	unsigned depth = 0;
	char* filename;
	int set = 0;

	/* Loop over args */
	for (int i = 1; i < argc; i++) {
		/* If a switch is found, set the pointer to the filename */
		if (!strcmp(argv[i], "-d")) {
			depth = atoi(argv[++i]);
		}
		else if (!strcmp(argv[i], "-v")) {
			verbose = 1;
		}
		else {

			if (set) {
				fprintf(stderr, "%s\n", "Multiple file names given!");
				exit(1);
			}

			set = 1;
			filename = argv[i];
		}
	}

	if (filename != NULL) {
		stdin = fopen(filename, "r");
	}

	/* Process moves into gameboard without success message */
	int success = processFile(0, -1);
	if(!success) {
		exit(1);
	}

	/* If moves given in input file */
	if (initConfig[0] > 0) {
		fprintf(stderr, "%s\n", "You may not provide moves in the input file.");
		exit(1);
	}

	/* Copy the gameboard */
	int** newBoard = (int**) malloc(8 * sizeof(int*));
	if(!newBoard) {
		fprintf(stderr, "%s\n", "Malloc fail!");
		exit(1);
	}
	for (int i = 0; i < 8; i++) {
		newBoard[i] = (int*) malloc(8 * sizeof(int));
		if(!newBoard[i]) {
			fprintf(stderr, "%s\n", "Malloc fail!");
			exit(1);
		}
	}
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			newBoard[i][j] = gameboard[i][j];
		}
	}

	/* Recursively score the gameboard for the current player */
	int numMoves = 0;
	int redTurn = initConfig[3] == 114 ? 1 : 0;

	/* Allocate possible moves array */
	int** moves = (int**) malloc(50 * sizeof(int*));
	if(!moves) {
		fprintf(stderr, "%s\n", "Malloc fail!");
		exit(1);
	}
	for (int i = 0; i < 50; i++) {
		moves[i] = malloc(2 * sizeof(int));
		if(!moves[i]) {
			fprintf(stderr, "%s\n", "Malloc fail!");
			exit(1);
		}
	}

	/* Get moves */
	possibleMoves(&moves, &numMoves, redTurn, newBoard);

	/* No available moves */
	int redScore;
	int blackScore;
	if (redTurn) {
		redScore = scoreBoard(1, newBoard);
		blackScore = scoreBoard(0, newBoard);
	}
	else {
		redScore = scoreBoard(0, newBoard);
		blackScore = scoreBoard(1, newBoard);
	}
	if (abs(redScore) == 99 || abs(blackScore) == 99) {
		if (redTurn) {
			printf("%s%s%s\n", "Player ", redScore == -99 ? "red" : "black", " has no available moves.");
		} else {
			printf("%s%s%s\n", "Player ", redScore == -99 ? "black" : "red", " has no available moves.");
		}

		/* Free allocated memory */
		for (int i = 0; i < 50; i++) {
			free(moves[i]);
		}
		free(moves);
		for (int i = 0; i < 8; i++) {
			free(newBoard[i]);
		}
		free(newBoard);
		
		exit(0);
	}

	/* Initiate recursive calls */
	if (depth < 0) {
		depth = 0;
	}
	scoreMovesRec(redTurn, redTurn, newBoard, depth, depth, verbose);

	/* Free allocated memory */
	for (int i = 0; i < 50; i++) {
		free(moves[i]);
	}
	free(moves);
	for (int i = 0; i < 8; i++) {
		free(newBoard[i]);
	}
	free(newBoard);
}
