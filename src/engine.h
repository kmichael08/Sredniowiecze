/** @file
    Interface of game engine.

 */

#ifndef ENGINE_H
#define ENGINE_H

/**
 * Initializes a game. Needed before first INIT.
 */
void startGame();

/**
 * Frees memory. Needed after finishing game.
 */
void endGame();

/**
 * Initializes a game with size of a board, number of rounds and positions of kings.
 */
int init(int boardSize, int turnNumber, int player, int x1, int y1, int x2, int y2);

/**
 * Makes a move.
 * @param[in] x1 Column number before a move.
 * @param[in] y1 Row number before a move.
 * @param[in] x2 Column number after a move.
 * @param[in] y2 Row number before a move.
 * @return 0.
 */
int move(int x1, int y1, int x2, int y2);

int produceKnight(int x1, int y1, int x2, int y2);

int producePeasant(int x1, int y1, int x2, int y2);

int endTurn();

/**
 * czy koniec gry
 */
int koniecGry();


/**
 * Prints (into stdout) top-left corner of the board of size m x m where m = min(n,10).
 */
void printTopleft();

#endif /* ENGINE_H */
