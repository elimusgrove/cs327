/*
 * Header file that defines functions used while rating available moves.
 *
 * @author Eli Musgrove
 */

#ifndef PARSEMOVES_H
#define PARSEMOVES_H

/*
 * Returns a string representation of the board position.
 * 
 * @param index - integer index of the board position rank and file
 * @param str - string to be modified
 * @return string representation of the move rank and file
 */
void intAscii(int index, char* str);


/*  
 * Return the score for the given team on the global gameboard array.
 * 
 * @param red - 1 if red is current playing team, 0 otherwise
 * @return int - the score of the given team
 */
int scoreBoard(int red, int** gameboard);


/*
 * List the possible moves for a given player on a given gameboard.
 *
 * @param redTurn - 1 if red is current playing team, 0 otherwise
 * @param gameboard - the current state of the gameboard
 */
void possibleMoves(int*** moves, int* numMoves, int redTurn, int** gameboard);


/*
 * Update the given gameboard with the given move. Move assumed to be valid.
 *
 * @param gameboard - given gameboard
 * @param newBoard - given gameboard to be updated
 * @param move - given move, assumed to be valid 
 */
void updateGameboard(int** gameboard, int** newBoard, int** move);


/*
 * Recursive function to display the available moves to the user in the specified format.
 * 
 * TODO: ADD PARAMETER AND RETURN DESCRIPTIONS
 */
int scoreMovesRec(int redTurn, int redMoving, int** gameboard, int originalDepth, int depth, int verbose);

#endif
