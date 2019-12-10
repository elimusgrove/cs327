/*
 * processfile.c - Process an input file from stdin, read input into global config array.
 *
 * @author Eli Musgrove
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "processfile.h"

/* The line number we are scanning, if errors occur we report this line. */
int lineNumber = 1;

/* Array of initial configuration for the game. Index order is important.
 *      Order:
 *          numMoves, capturePieces, multiJump, turn, redPawns, redKings, blackPawns, blackKings
 */
int initConfig[8];

/* Gameboard represented as int array.
 *      Meaning: (ascii codes of the char symbols)
 *          46- empty black square
 *          34- empty red square
 *          98- black pawn
 *          114- red pawn
 *          66- black king
 *          82- red king
 */
int gameboard[8][8];

/* Array to store moves as numbers (1-9) with the board positions converted to ascii
 *      ex. moves[i] = 5351 means the board position (either source or dest.) is e3
 */
unsigned int* moves;


/*
 * Skip over commented lines and other ignored characters to the next meaningful section.
 */
void skipForward() {
    /* Iterate over the file char-by-char */
    int c;
    while (!feof(stdin)) {
        c = fgetc(stdin);

        /* Comment found, read until newline found */
        if (c == '#') {
            while (c != '\n' && !feof(stdin)) {
                c = fgetc(stdin);
            }
        }

        /* Newline found */
        if (c == '\n') {
            lineNumber++;
            continue;
        }

        /* Skip ignored chars */
        if (c != ' ' && c != '|' && c != '+' && c != '-' && c != '\t') {
            ungetc(c, stdin);
            break;
        }
    }
}


/*
 * Reads a keyword and directs processing to appropriate function.
 *
 * @return int - 1 if keyword is valid, 0 if keyword is invalid or an error occurred.
 */
int processKeyword(int movesToProcess) {
    skipForward();

    /* Initializations */
    int c = 0;
    int i = 0;
    char keyword[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    /* While keyword not fully read */
    while (c != ':' && !feof(stdin)) {
        c = fgetc(stdin);

        /* Invalid character found */
        if ((c < 65 || c > 90) && c != ':') {
            fprintf(stderr, "Error: Invalid character '%c' found in section keyword on line %d.\n", c, lineNumber);
            return 0;
        }

        /* Add valid char */
        keyword[i] = (char) c;

        i++;
    }

    /* Process keyword */
    char rules[] = "RULES:";
    char turn[] = "TURN:";
    char moves[] = "MOVES:";
    char board[] = "BOARD:";

    /* Skip any possible newlines or spaces, process appropriate section */
    skipForward();
    if (!strcmp(rules, keyword)) {
        return processRules();
    }
    else if (!strcmp(turn, keyword)) {
        return processTurn();
    }
    else if (!strcmp(moves, keyword)) {
        return processMoves(movesToProcess);
    }
    else if (!strcmp(board, keyword)) {
        return processBoard();
    }
    else {
        fprintf(stderr, "\nError: Invalid section keyword '%s' on line %d.\n", keyword, lineNumber);
        return 0;
    }
}


/*
 * Processes the rules section.
 *
 * @return int - 0 if errors in section, 1 if section is formatted properly
 */
int processRules() {
    skipForward();

    /* Initializations */
    int c = 0;
    int i = 0;

    /* Strings to store rule */
    char* captureRule = (char*) malloc(500 * sizeof(char));
    char* jumpRule = (char*) malloc(500 * sizeof(char));

    /* Process 1st rule */
    while (!feof(stdin)) {
        c = fgetc(stdin);

        if (c == '#' || c == '\n') {
            break;
        }

        if (c == ' ') {
            continue;
        }

        /* Add char to array, iterate counter */
        captureRule[i] = (char) c;
        i++;

        /* Read characters until we have the correct rule or i > 8 and we don't have the rule yet */
        if (!strcmp(captureRule, "capture") || !strcmp(captureRule, "nocapture") || i > 9) {
            break;
        }
    }

    /* Invalid rule */
    // printf("%s\n", );
    if (strcmp(captureRule, "capture") && strcmp(captureRule, "nocapture")) {
        fprintf(stderr, "Error: Invalid rule '%s' on line %d.\n", captureRule, lineNumber);
        return 0;
    }

    /* Process 2nd rule */
    i = 0;
    skipForward();
    while (!feof(stdin)) {
        c = fgetc(stdin);

        if (c == ' ') {
            skipForward();
            continue;
        }

        if (c == '#' || c == '\n') {
            break;
        }

        /* Add char to array, iterate counter */
        jumpRule[i] = (char) c;
        i++;

        /* Read characters until we have the correct rule or i > 14 and we don't have the rule yet */
        if (!strcmp(jumpRule, "multiplejumps") || !strcmp(jumpRule, "singlejumps") || i > 14) {
            break;
        }
    }

    /* Invalid rule */
    if (strcmp(jumpRule, "singlejumps") && strcmp(jumpRule, "multiplejumps")) {
        fprintf(stderr, "Error: Invalid rule '%s' on line %d.\n", jumpRule, lineNumber);
        return 0;
    }

    /* Set global config array */
    initConfig[1] = !strcmp(captureRule, "capture");
    initConfig[2] = !strcmp(jumpRule, "multiplejumps");

    free(captureRule);
    free(jumpRule);

    return 1;
}


/*
 * Processes the turn section.
 *
 * @return int - 0 if errors in section, 1 if section is formatted properly
 */
int processTurn() {
    skipForward();

    /* Initialization */
    int i = 0;
    int c = 0;
    char turn[10];
    int newLine = 0;

    /* Scan line */
    while (!feof(stdin)) {
        c = fgetc(stdin);

        /* We ran into a newline */
        if (c == '\n') {
            newLine = 1;
            break;
        }

        /* Not possible to have valid team */
        if (i > 5 || c == ' ') {
            break;
        }

        /* Iterate counter */
        turn[i] = (char) c;
        i++;
    }

    /* Check if team is valid */
    if (strcmp(turn, "black") && strcmp(turn, "red")) {
        fprintf(stderr, "Error: Invalid team '%s' on line %d.\n", turn, lineNumber);
        return 0;
    }
        /* Valid initial team, add to global config */
    else {
        initConfig[3] = turn[0];
    }

    /* We had a new line */
    if (newLine) {
        lineNumber++;
    }

    /* If we get here, a valid team is received */
    return 1;
}


/*
 * Processes the board section.
 *
 * @return int - 0 if errors in section, 1 if section is formatted properly
 */
int processBoard() {
    skipForward();

    /* Initializations */

    /* Character from file */
    int c = 0;

    /* Game piece counts */
    int redNext = 1;
    int redPawns = 0;
    int blackPawns = 0;
    int redKings = 0;
    int blackKings = 0;

    /* Track board square iteration */
    int i = 0;

    /* Track current row */
    int row = -1;

    /* Process flipped board */
    while (i < 64 && !feof(stdin)) {
        skipForward();
        c = fgetc(stdin);

        /* Iterate row */
        if (i % 8 == 0) {
            row++;
        }

        /* Expecting a red square, didn't get one */
        if (redNext && c != '"') {
            fprintf(stderr, "Error: Red board square expected on line %d.\n", lineNumber);
            return 0;
        }

        /* Expecting a black square, didn't get one */
        if (!redNext && c != 'b' && c != 'B' && c != 'r' && c != 'R' && c != '.') {
            fprintf(stderr, "Error: Black board square expected on line %d.\n", lineNumber);
            return 0;
        }

        /* Process valid board square */
        switch (c) {
            case 'r':
                redPawns++;
                break;
            case 'R':
                redKings++;
                break;
            case 'b':
                blackPawns++;
                break;
            case 'B':
                blackKings++;
                break;
        }

        /* Set the board square */
        gameboard[row][i % 8] = (int) c;

        /* Swap expected board square unless going to new line */
        if ((i+1) % 8 != 0) {
            redNext = !redNext;
        }
        i++;
    }

    /* Set global array values */
    initConfig[4] = redPawns;
    initConfig[5] = redKings;
    initConfig[6] = blackPawns;
    initConfig[7] = blackKings;

    /* If we got here, no errors in board */
    return 1;
}


/*
 * Processes the moves section.
 *
 * @param int num - the number of moves to process, -1 for all moves
 * @return int - 0 for error in moves, 1 for successful processing
 */
int processMoves(int numToProcess) {
    skipForward();

    /* Initializations */
    int c = 0;
    int expectFile = 1;
    int expectRank = 0;
    int numMoves = 0;
    int validBoardPos = 0;

    /* Array to track moves */
    moves = (unsigned int*) malloc(sizeof(unsigned int));

    /* Track the current slot we can add to moves */
    int i = 0;

    /* Malloc fail */
    if (!moves) {
        fprintf(stdout, "%s\n", "Malloc failed!");
        exit(1);
    }

    /* Loop over moves file section */
    while (!feof(stdin)) {
        /* Skip ignored chars, get the next char */
        skipForward();
        c = fgetc(stdin);

        /* Add space for the next board position */
        moves = (unsigned int*) realloc(moves, (numMoves + 1) * 1000); /* TODO: Smarter way to do this besides way over-allocating */

        /* Malloc fail */
        if (!moves) {
            fprintf(stdout, "%s\n", "Realloc failed!");
            exit(1);
        }

        /* End of file, stop here */
        if (c == -1) {
            break;
        }

        /* Arrow found */
        if (c == '>') {
            expectFile = 1;
            expectRank = 0;
            continue;
        }

        /* Expecting file */
        if (expectFile) {

            /* Invalid file received */
            if ((c < 97 || c > 103) && (numMoves < numToProcess || numToProcess == -1)) {
                fprintf(stderr, "%s%d%s%c%s\n", "Expected a valid file on line ", lineNumber, " but received '", c, "'.");
                return 0;
            }

            /* Add the move to moves */
            moves[i] = c;
            i++;
            validBoardPos++;

            /* Finished processing a moves */
            if (validBoardPos == 4) {
                numMoves++;
                validBoardPos = 0;
            }

            expectFile = 0;
            expectRank = 1;
            continue;
        }

        /* Expecting rank */
        if (expectRank) {

            /* Invalid rank received */
            if ((c < 49 || c > 56) && (numMoves < numToProcess || numToProcess == -1)) {
                fprintf(stderr, "%s%d%s%c%s\n", "Expected a valid rank on line ", lineNumber, " but received '", c, "'.");
                return 0;
            }

            /* Add the move to moves */
            moves[i] = c;
            i++;
            validBoardPos++;

            /* Finished processing a moves */
            if (validBoardPos == 4) {
                numMoves++;
                validBoardPos = 0;
            }

            expectFile = 1;
            expectRank = 0;
            continue;
        }
    }

    /* Set the number of valid moves */
    initConfig[0] = numMoves;

    /* If we reach this point, no errors in moves */
    return 1;
}


/*
 * Uses the other helper functions to process the file into multiple arrays.
 *
 * @param printMessage - boolean - 1 to print success message, 0 to not print a success message
 * @return - boolean - 1 on success, 0 on error in file scan
 */
int processFile(int printMessage, int movesToProcess) {
    /* Iterate over the file, processing each section individually */
    int processed = 0;
    for (int i = 0; i < 4; i++) {
        processed = processKeyword(movesToProcess);

        /* We had an error */
        if (!processed) {
            return 0;
        }
    }

    /* Get the starting turn to be displayed */
    char* turn = (initConfig[3] == 'b') ? "black" : "red";

    /* Print success message to stdout */
    if (printMessage) {
        fprintf(stdout, "VALID INPUT\nInitial configuration:\nTurn: %s\nRed: %d kings, %d pawns\nBlack: %d kings, %d pawns\n",
            turn, initConfig[5], initConfig[4], initConfig[7], initConfig[6]);
    }

    return 1;
}
