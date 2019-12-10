/*
 * Header file that defines functions used during input file verification.
 *
 * @author Eli Musgrove
 */

#ifndef PROCESSFILE_H
#define PROCESSFILE_H

/*
 * Skip over commented lines and other ignored characters to the next meaningful section.
 */
void skipForward();

/*
 * Reads a keyword and directs processing to appropriate function defined below.
 */
int processKeyword(int movesToProcess);

/*
 * Processes the rules section, takes rule input to global config array.
 *
 * @return int - 0 if errors in section, 1 if section is formatted properly
 */
int processRules();

/*
 * Processes the turn section, takes turn inpupt into global config array.
 *
 * @return int - 0 if errors in section, 1 if section is formatted properly
 */
int processTurn();

/*
 * Processes the board section, takes piece counts to global config array.
 *
 * @return int - 0 if errors in section, 1 if section is formatted properly
 */
int processBoard();

/*
 * Processes the moves section.
 *
 * @return int - 0 if errors in section, 1 if section is formatted properly
 */
int processMoves(int numToProcess);

/*
 * Uses the other helper functions to process the file into multiple arrays.
 *
 * @param printMessage - boolean - 1 to print success message, 0 to not print a success message
 * @return - boolean - 1 on success, 0 on error in file scan
 */
int processFile(int printMessage, int movesToProcess);
#endif
