/** @file
 *  Interface of the game engine.
 *  @author Michal Kuzba
 */

#ifndef ENGINE_H
#define ENGINE_H

/**
 * Initializes a game. 
 * Needed before a first INIT.
 */
void startGame();

/**
 * Ends the game.
 * It frees the memory of the board. Needed after finishing game. 
 * @param[in] correct - whether the game has finished correctly.
 */
void endGame(int correct);

/**
 * Initializes a game with size of a board, number of rounds and positions of kings.
 * @param[in] boardSize Size of the board (from range [9, 2^31 - 1])
 * @param[in] turnNumber Number of the turns of the game (from range [1, 2^31 - 1])
 * @param[in] player Number of the player (1 or 2).
 * @param[in] x1 Column number of the first player king.
 * @param[in] y1 Row number of the first player king.
 * @param[in] x2 Column number of the second player king.
 * @param[in] y2 Row number of the second player king.
 * @return 0 - if the command was correct, 1 - if not.
 */
int init(int boardSize, int turnNumber, int player, int x1, int y1, int x2, int y2);

/**
 * Makes a move.
 * @param[in] x1 Column number before a move.
 * @param[in] y1 Row number before a move.
 * @param[in] x2 Column number after a move.
 * @param[in] y2 Row number before a move.
 * @return 0 - if the command was correct, 1 - if not.
 */
int move(int x1, int y1, int x2, int y2);

/**
 * Produces a new knight.
 * @param[in] x1 Column number of the peasant.
 * @param[in] y1 Row number of the peasant.
 * @param[in] x2 Column number of the new knight.
 * @param[in] y2 Row number of the new knight.
 * @return 0 - if the command was correct, 1 - if not.
 */
int produceKnight(int x1, int y1, int x2, int y2);

/**
 * Produces a new peasant.
 * @param[in] x1 Column number of the peasant.
 * @param[in] y1 Row number of the peasant.
 * @param[in] x2 Column number of the new peasant.
 * @param[in] y2 Row number of the new peasant.
 * @return 0 - if the command was correct, 1 - if not.
 */
int producePeasant(int x1, int y1, int x2, int y2);

/**
 * Ends the turn of the player.
 */
void endTurn();

/**
 * @return Is the game finished.
 */
int koniecGry();

/**
 * @return actual player number
 */
int actualPlayer;

/**
 * @return the number of the winner or 0 in case of draw
 */
int gameWinner;

/**
 * Prints (into stdout) top-left corner of the board of size m x m where m = min(n, 10).
 */
void printTopleft();

#endif /* ENGINE_H */
