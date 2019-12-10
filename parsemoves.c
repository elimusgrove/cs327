/*
 * parsemoves.c - Helper functions to rank available set of moves looking a given number of moves ahead.
 * 
 * @author Eli Musgrove
 */

#include "parsemoves.h"
#include <stdlib.h>
#include <stdio.h>

/*
 * Return string representation of the board position.
 */
void intAscii(int index, char* str) {
	int file = index / 10;
	int rank = index % 10;

	str[0] = (char) file + 97;
	str[1] = 8 - rank + 48;
	str[2] = 0;
}

/*  
 * Return the score for the given team on the given gameboard.
 */
int scoreBoard(int redTurn, int** gameboard) {

	/* Initializations */
	unsigned redPawns = 0;
	unsigned redKings = 0;
	unsigned blackPawns = 0;
	unsigned blackKings = 0;

	/* Loop over the gameboard */
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			switch (gameboard[i][j]) {
				case 66:
					blackKings++;
					break;
				case 98:
					blackPawns++;
					break;
				case 82:
					redKings++;
					break;
				case 114:
					redPawns++;
					break;
			}
		}
	}

	/* Determine if moves are available */

	/* Allocate possible moves array */
	int redMoves = 0;
	int blackMoves = 0;
	int ** moves = (int**) malloc(50 * sizeof(int*));
	for (int i = 0; i < 100; i++) {
		moves[i] = malloc(2 * sizeof(int));
	}

	/* Get moves */
	if (redTurn) {
		possibleMoves(&moves, &redMoves, redTurn, gameboard);
		possibleMoves(&moves, &blackMoves, !redTurn, gameboard);
	}
	else {
		possibleMoves(&moves, &redMoves, !redTurn, gameboard);
		possibleMoves(&moves, &blackMoves, redTurn, gameboard);
	}

	/* Free allocated memory */
	for (int i = 0; i < 50; i++) {
		free(moves[i]);
	}
	free(moves);

	/* If no moves available, return */
	if (redMoves <= 0) {
		return redTurn ? -99 : 99;
	}
	if (blackMoves <= 0) {
		return redTurn ? 99 : -99;
	}

	/* No red or black kings, set multiplier to 1 */
	if (!redKings) {
		redKings = 1;
	}
	if (!blackKings) {
		blackKings = 1;
	}

	/* Calculate the score */
	return redTurn ? ((redPawns + 2) * redKings) - ((blackPawns + 2) * blackKings) : ((blackPawns + 2) * blackKings) - ((redPawns + 2) * redKings);
}


/*
 * Modify input array of possible moves represented as integers.
 */
void possibleMoves(int*** moves, int* numMoves, int redTurn, int** gameboard) {
	
	/* Initializations */
	int piece = 0;
	int cornerPiece = 0;

	/* Iterate over given gameboard */
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			piece = gameboard[i][j];

			/* Empty sqare */
			if (piece == 34 || piece == 46) {
				continue;
			}

			/* King */
			if (piece == 82 || piece == 66) {

				/* Up left diagonal */
				if (i > 0 && j > 0) {
					cornerPiece = gameboard[i-1][j-1];

					/* Empty square */
					if (cornerPiece == 46 && ((piece == 82 && redTurn) || (piece == 66 && !redTurn))) {
						(*moves)[*numMoves][0] = (j * 10) + i;
						(*moves)[*numMoves][1] = ((j-1) * 10) + (i-1);
						(*numMoves)++;
					}

					/* Capturable black piece */
					else if ((((cornerPiece == 98 || cornerPiece == 66) && redTurn && (piece == 82 || piece == 114)) || ((cornerPiece == 82 || cornerPiece == 114) && !redTurn && (piece == 66 || piece == 98))) && i >= 2 && j >= 2 && gameboard[i-2][j-2] == 46) {
						(*moves)[*numMoves][0] = (j * 10) + i;
						(*moves)[*numMoves][1] = ((j-2) * 10) + (i-2);
						(*numMoves)++;
					}
				}

				/* Up right diagonal */
				if (i > 0 && j < 7) {
					cornerPiece = gameboard[i-1][j+1];

					/* Empty square */
					if (cornerPiece == 46 && ((piece == 82 && redTurn) || (piece == 66 && !redTurn))) {
						(*moves)[*numMoves][0] = (j * 10) + i;
						(*moves)[*numMoves][1] = ((j+1) * 10) + (i-1);
						(*numMoves)++;
					}

					/* Capturable black piece */
					else if ((((cornerPiece == 98 || cornerPiece == 66) && redTurn && (piece == 82 || piece == 114)) || ((cornerPiece == 82 || cornerPiece == 114) && !redTurn && (piece == 66 || piece == 98))) && i >= 2 && j <= 5 && gameboard[i-2][j+2] == 46) {
						(*moves)[*numMoves][0] = (j * 10) + i;
						(*moves)[*numMoves][1] = ((j+2) * 10) + (i-2);
						(*numMoves)++;
					}
				}

				/* Down left diagonal */
				if (i < 7 && j > 0) {
					cornerPiece = gameboard[i+1][j-1];

					/* Empty square */
					if (cornerPiece == 46 && ((piece == 82 && redTurn) || (piece == 66 && !redTurn))) {
						(*moves)[*numMoves][0] = (j * 10) + i;
						(*moves)[*numMoves][1] = ((j-1) * 10) + (i+1);
						(*numMoves)++;
					}

					/* Capturable black piece */
					else if ((((cornerPiece == 98 || cornerPiece == 66) && redTurn && (piece == 82 || piece == 114)) || ((cornerPiece == 82 || cornerPiece == 114) && !redTurn && (piece == 66 || piece == 98))) && i <= 5 && j >= 2 && gameboard[i+2][j-2] == 46) {
						(*moves)[*numMoves][0] = (j * 10) + i;
						(*moves)[*numMoves][1] = ((j-2) * 10) + (i+2);
						(*numMoves)++;
					}
				}

				/* Down right diagonal */
				if (i < 7 && j < 7) {
					cornerPiece = gameboard[i+1][j+1];

					/* Empty square */
					if (cornerPiece == 46 && ((piece == 82 && redTurn) || (piece == 66 && !redTurn))) {
						(*moves)[*numMoves][0] = (j * 10) + i;
						(*moves)[*numMoves][1] = ((j+1) * 10) + (i+1);
						(*numMoves)++;
					}

					/* Capturable black piece */
					else if ((((cornerPiece == 98 || cornerPiece == 66) && redTurn && (piece == 82 || piece == 114)) || ((cornerPiece == 82 || cornerPiece == 114) && !redTurn && (piece == 66 || piece == 98))) && i <= 5 && j <= 5 && gameboard[i+2][j+2] == 46) {
						(*moves)[*numMoves][0] = (j * 10) + i;
						(*moves)[*numMoves][1] = ((j+2) * 10) + (i+2);
						(*numMoves)++;
					}
				}
			}

			/* Pawn */
			else if (piece == 114 || piece == 98) {

				/* Red left diagonal */
				if (i > 0 && j > 0 && piece == 114 && redTurn) {
					cornerPiece = gameboard[i-1][j-1];

					/* Empty square */
					if (cornerPiece == 46) {
						(*moves)[*numMoves][0] = (j * 10) + i;
						(*moves)[*numMoves][1] = ((j-1) * 10) + (i-1);
						(*numMoves)++;
					} 
	
					/* Capturable black piece */
					else if ((cornerPiece == 98 || cornerPiece == 66) && i >= 2 && j >= 2 && gameboard[i-2][j-2] == 46) {
						(*moves)[*numMoves][0] = (j * 10) + i;
						(*moves)[*numMoves][1] = ((j-2) * 10) + (i-2);
						(*numMoves)++;
					}
				}

				/* Red right diagonal */
				if (i > 0 && j < 7 && piece == 114 && redTurn) {
					cornerPiece = gameboard[i-1][j+1];

					/* Empty square */
					if (cornerPiece == 46) {
						(*moves)[*numMoves][0] = (j * 10) + i;
						(*moves)[*numMoves][1] = ((j+1) * 10) + (i-1);
						(*numMoves)++;
					}

					/* Capturable black piece */
					else if ((cornerPiece == 98 || cornerPiece == 66) && i >= 2 && j <= 5 && gameboard[i-2][j+2] == 46) {
						(*moves)[*numMoves][0] = (j * 10) + i;
						(*moves)[*numMoves][1] = ((j+2) * 10) + (i-2);
						(*numMoves)++;
					}
				}

				/* Black left diagonal */
				if (i < 7 && j > 0 && piece == 98 && !redTurn) {
					cornerPiece = gameboard[i+1][j-1];

					/* Empty square */
					if (cornerPiece == 46) {
						(*moves)[*numMoves][0] = (j * 10) + i;
						(*moves)[*numMoves][1] = ((j-1) * 10) + (i+1);
						(*numMoves)++;
					} 
	
					/* Capturable black piece */
					else if ((cornerPiece == 114 || cornerPiece == 82) && i <= 5 && j >= 2 && gameboard[i+2][j-2] == 46) {
						(*moves)[*numMoves][0] = (j * 10) + i;
						(*moves)[*numMoves][1] = ((j-2) * 10) + (i+2);
						(*numMoves)++;
					}
				}

				/* Black right diagonal */
				if (i < 7 && j < 7 && piece == 98 && !redTurn) {
					cornerPiece = gameboard[i+1][j+1];

					/* Empty square */
					if (cornerPiece == 46) {
						(*moves)[*numMoves][0] = (j * 10) + i;
						(*moves)[*numMoves][1] = ((j+1) * 10) + (i+1);
						(*numMoves)++;
					}

					/* Capturable black piece */
					else if ((cornerPiece == 114 || cornerPiece == 82) && i <= 5 && j <= 5 && gameboard[i+2][j+2] == 46) {
						(*moves)[*numMoves][0] = (j * 10) + i;
						(*moves)[*numMoves][1] = ((j+2) * 10) + (i+2);
						(*numMoves)++;
					}
				}
			}

			/* Other piece */
			else {
				continue;
			}
		}
	}
}


/*
 * Update the given gameboard with the given move.
 */
void updateGameboard(int** gameboard, int** newBoard, int** move) {

	/* Copy gameboard */
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			newBoard[i][j] = gameboard[i][j];
		}
	}

	/* Get the array indices from move */
	int sourceX = (*move)[0] / 10;
	int sourceY = (*move)[0] % 10;
	int destX = (*move)[1] / 10;
	int destY = (*move)[1] % 10;

	/* Piece being moved */
	int piece = newBoard[sourceY][sourceX];

	/* Removed captured piece */
	if (abs(destX - sourceX) == 2) {
		newBoard[((destY - sourceY) / 2) + sourceY][((destX - sourceX) / 2) + sourceX] = 46;
	}

	/* Finish update */
	newBoard[destY][destX] = piece;
	newBoard[sourceY][sourceX] = 46;
}


/*
 * Run recursive calls to calculate the best possible move based on the given depth.
 */
int scoreMovesRec(int redTurn, int redMoving, int** gameboard, int originalDepth, int depth, int verbose) {

	/* Print 4 spaces for every depth */
	if (verbose) {
		for (int i = 0; i < abs(originalDepth - depth); i++) {
			printf("%s", "    ");
		}
	}

	/* Assignments */
	int score;
	int numResponseMoves = 0;

	/* Memory allocation */
	char* str1 = malloc(3 * sizeof(char));
	char* str2 = malloc(3 * sizeof(char));
	if (!str1 || !str2) {
		fprintf(stderr, "%s\n", "Malloc fail!");
		exit(1);
	}
	int** responseMoves = (int**) malloc(50 * sizeof(int*));
	if (!responseMoves) {
		fprintf(stderr, "%s\n", "Malloc fail!");
		exit(1);
	}
	for (int i = 0; i < 50; i++) {
		responseMoves[i] = malloc(2 * sizeof(int));
		if (!responseMoves[i]) {
			fprintf(stderr, "%s\n", "Malloc fail!");
			exit(1);
		}
	}
	int** newBoard = (int**) malloc(8 * sizeof(int*));
	if(!newBoard) {
		fprintf(stderr, "%s\n", "Malloc fail!");
		exit(1);
	}
	for (int i = 0; i < 8; i++) {
		newBoard[i] = (int*) malloc(8 * sizeof(int));
		if (!newBoard[i]) {
			fprintf(stderr, "%s\n", "Malloc fail!");
			exit(1);
		}
	}

	/* Get all possible response moves */
	possibleMoves(&responseMoves, &numResponseMoves, redMoving, gameboard);

	/* Loop over moves to find the optimal score */
	int maxScore = -100;
	int moveScore;
	for (int i = 0; i < numResponseMoves; i++) {
	
		/* Get score for every possible board */
		updateGameboard(gameboard, newBoard, &responseMoves[i]);
		moveScore = scoreBoard(redMoving, newBoard);

		/* Find optimal move */
		if (moveScore > maxScore) {
			maxScore = moveScore;
		}
	}

	/* Opponent moving, pick optimal move */
	if (redMoving != redTurn) {

		/* Loop over each move, only recurse on the optimal moves */
		for (int i = 0; i < numResponseMoves; i++) {

			/* Get the move as a string */
			intAscii(responseMoves[i][0], str1);
			intAscii(responseMoves[i][1], str2);

			/* Get the gameboard as a result of the move */
			updateGameboard(gameboard, newBoard, &responseMoves[i]);

			/* Verbose user messages */
			if (verbose && depth > 0) {
				printf("%s%s%s%s%s%s\n", "? ", str1, "->", str2, " for ", redMoving ? "red:" : "black:");
			}

			/* Get the score for the board */
			score = scoreBoard(redMoving, newBoard);

			/* Opponent didn't win the round */
			if (score != 99 && score != -99) {
				/* If depth is 0 */
				if (depth == 0) {
					if (verbose) {
						if (i != 0) {
							for (int j = 0; j < abs(originalDepth - depth); j++) {
								printf("%s", "    ");
							}
						}
						printf("%s%s%s%s%s%s%d\n", ". ", str1, "->", str2, " for ", redMoving ? "red: score " : "black: score ", score);
					}
				}
				else {
					if (score == maxScore) {
						/* Recursive call on the optimal move(s) */
						score = scoreMovesRec(redTurn, !redMoving, newBoard, originalDepth, depth - 1, verbose);
					}
				}

				if (verbose && depth != 0) {
					if (i != 0) {
						for (int j = 0; j < abs(originalDepth - depth); j++) {
							printf("%s", "    ");
						}
					}
					printf("%s%s%s%s%s%s%d\n", ". ", str1, "->", str2, " for ", redMoving ? "red: score " : "black: score ", score);
				}
			}
		}
	}

	/* Main player moving */
	else {

		/* Execute every move */
		for (int i = 0; i < numResponseMoves; i++) {
			updateGameboard(gameboard, newBoard, &responseMoves[i]);

			intAscii(responseMoves[i][0], str1);
			intAscii(responseMoves[i][1], str2);

			/* Print move */
			if (depth > 0 && verbose) {
				printf("%s%s%s%s%s%s\n", "? ", str1, "->", str2, " for ", redMoving ? "red:" : "black:");
			}

			/* Recurse if depth > 0 */
			if (depth > 0) {
				score = scoreMovesRec(redTurn, !redMoving, newBoard, originalDepth, depth - 1, verbose);
			}
			else {
				score = scoreBoard(redMoving, newBoard);
			}

			/* Print verbose messages */
			if (verbose) {
				if (i != 0) {
					for (int j = 0; j < abs(originalDepth - depth); j++) {
						printf("%s", "    ");
					}
				}
				printf("%s%s%s%s%s%s%d\n", ". ", str1, "->", str2, " for ", redMoving ? "red: score " : "black: score", score);
			}
			/* Back at the first time calling the function, print the score */
			else if (!verbose && depth == originalDepth) {
				printf("%s%s%s%s%s%d\n", str1, "->", str2, " for ", redMoving ? "red: score " : "black: score ", score);
			}
		}
	}

	/* Free allocated memory */
	free(str1);
	free(str2);
	for (int i = 0; i < 50; i++) {
		free(responseMoves[i]);
	}
	free(responseMoves);
	for (int i = 0; i < 8; i++) {
		free(newBoard[i]);
	}
	free(newBoard);

	/* If opponent is moving, return score relative to main player */
	if (redMoving != redTurn) {
		return -1 * maxScore;
	}

	return score;
}
