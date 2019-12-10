/*
 * change.c - Process checkers input gameboard and moves from text file from stdin.
 *
 * @author Eli Musgrove
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "processfile.h"
#include "updategameboard.h"

/* Game configuration array */
extern int initConfig[8];

/* Initial configuration array filled by processfile.c */
extern int gameboard[8][8];

/* Moves as processed from file by processfile.c */
extern unsigned int* moves;

/* If any, the move that caused errors */
extern int illegalMove[5];

int main(int argc, char *argv[]) {
	/* Store the filename of the exchange/human output file, null if no switch given */
	char* exchangeName = 0;
	char* humanName = 0;
	unsigned movesToProcess = -1;
	char* filename;

	/* Loop over args */
	for (int i = 1; i < argc; i++) {
		/* If a switch is found, set the pointer to the filename */
		if (!strcmp(argv[i], "-e")) {
			exchangeName = argv[++i];
		}
		else if (!strcmp(argv[i], "-h")) {
			humanName = argv[++i];
		}
		else if (!strcmp(argv[i], "-m")) {
			movesToProcess = atoi(argv[++i]);
		}
		else {
			filename = argv[i];
		}
	}

	if (filename != NULL) {
		stdin = fopen(filename, "r");
	}

	/* Process moves into gameboard without success message */
	int success = processFile(0, movesToProcess);
	
	/* There is an error in the file format, stop here */
	if (!success) {
		exit(1);
	}

	/* Display and exit if unimplemented features are requested */
	if (initConfig[1]) {
		fprintf(stderr, "%s\n", "The capture rule is not implemented.");
		exit(1);
	}

	if (initConfig[2]) {
		fprintf(stderr, "%s\n", "The multiple jumps rule is not implemented.");
		exit(1);
	}

	if (gameboard[0][0] != 34) {
		fprintf(stderr, "%s\n", "Flipped board gameplay is not implemented.");
		exit(1);
	}

	/* Output based on number of moves */
	if (movesToProcess == -1 || movesToProcess > initConfig[0]) {
		printf("%s%d%s\n%s%d%s\n", "Input file has ", initConfig[0], " moves", "Processing ", initConfig[0], " moves");
		movesToProcess = -1;
	}
	else {
		printf("%s%d%s\n%s%d%s\n", "Input file has ", initConfig[0], " moves", "Processing ", movesToProcess, " moves");
	}

	success = updateGameboard(movesToProcess);

	/* No errors in moves */
	if (success) {
		printf("%s\n", "All moves are legal");
	}
	else {
		/* Error, display the error move */
		printf("%s%d%s%c%d%s%c%d\n", "Move ", illegalMove[0], " is illegal: ", (char) illegalMove[1] + 97, (char) (8 - illegalMove[2]), "->", 
			(char) illegalMove[3] + 97, (char) (8 - illegalMove[4]));
	}

	/* Human file creation */
	if (humanName) {
		if (!success) {
			human(humanName, illegalMove[0]);
		} 
		else {
			human(humanName, movesToProcess);
		}
	}

	/* Exchange file creation */
	if (exchangeName) {
		if (!success) {
			exchange(exchangeName, illegalMove[0]);
		} 
		else {
			exchange(exchangeName, movesToProcess);
		}
	}

	/* Free moves */
	free(moves);
	return 0;
}
