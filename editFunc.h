/*
 * Header file that defines helper functions used for edit executable.
 *
 * @author Eli Musgrove
 */

#ifndef EDITFUNC_H
#define EDITFUNC_H

/*
 * Update the gameboard given a previously confirmed valid move.
 *
 * @param gamebord - board to be updated
 * @param a - source rank
 * @param b - source file
 * @param c - target rank
 * @param d - target file
 * @param reverse - 1 if moving backwards, 0 otherwise
 */
void updateBoard(int*** gameboard, int a, int b, int c, int d, int reverse);

/*
 * Dislays 3 squares in the given location.
 *
 * @param val - the value of the gameboard to display if any
 * @param row - starting row
 * @param col - starting col
 * @param white - background color, white or red
 */
void displaySquare(int val, int row, int col, int white);

/*
 * Display a single row of the gameboard.
 * 
 * @param gameboard - current gameboard
 * @param row - the starting row to display the gameboard row on
 * @param startWhite - true/false if the first square of the row should be white
 * @param boardRow - the 0 index row of the board to display
 */
void displayRow(int** gameboard, int row, int col, int startWhite, int boardRow);

/*
 * Display the given gameboard.
 *
 * @param row - the starting row to display the gameboard on
 * @param col - the starting column to display the gameboard
 */
void displayBoard(int** gameboard, int row, int col);

/*
 * Display the moves in the available vertical space.
 *
 * @param moves - array of moves
 * @param startingMove - 0 base index of the starting move to display
 * @param numMoves - the number of total moves
 * @param illegalMove - 0 based index of the move that is invalid
 */
void displayMoves(unsigned int* moves, int startingMove, int numMoves, int illegalMove);

#endif
