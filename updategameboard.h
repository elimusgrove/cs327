/*
 * Header file that defines functions used in updating the global gameboard array while processing the given number of moves.
 * 
 * @author Eli Musgrove
 */

#ifndef UPDATEGAMEBOARD_H
#define UPDATEGAMEBOARD_H

/* 
 * Updates the gameboard using the global gameboard and moves arrays. If there is an invalid move, it sets that in the errorMove global array.
 *
 * @param numToProcess - the number of moves to execute on the gameboard, -1 for all moves
 * @return int - 1 for successful moves, 0 for an invalid move.
 */
int updateGameboard(int numToProcess);

/*
 * Creates the exchange file format based on the current state of the gameboard and moves arrays.
 * 
 * @param fileName - name of file to be created
 * @param numProcessed - number of moves already processed
 */
void exchange(char* fileName, int numProcessed);

/* 
 * Creates the human file format output file based on the current state of the gameboard and moves arrays.
 * 
 * @param fileName - name of file to be created
 * @param numProcessed - number of moves already processed
 */
void human(char* fileName, int numProcessed);

#endif
