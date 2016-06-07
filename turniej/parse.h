 /** @file
     Interface of parser.
     @author Michal Kuzba
 */
#ifndef PARSE_H
#define PARSE_H

/**
 * Type of the instruction.
 */
enum Instruction {
	INIT, /**< Player inits the game. */
	MOVE, /**< Moves the unit to the adjacent field. */
	PRODUCE_KNIGHT, 
	/**< Peasant produces a new knight on the adjacent field. */
	PRODUCE_PEASANT, 
	/**< Peasant produces a new peasant on the adjacent field. */
	END_TURN, /**< Ends the turn of the player. */
	WRONG /**< Wrong command - any other from listed above. */	
};

/**
 * Instruction.
 * Instruction with all parametres.
 */
struct Command {
	enum Instruction instruction; /**< Type of the instruction. */
	int boardSize; /**< Size of the board. */
	int turnNumber; /**< The number of the turns in the whole game. */
	int player; /**< Number of the player - 1 or 2. */
	int x1; /**< Column number - actual position. */
	int y1; /**< Row number - actual position. */
	int x2;	/**< Column number - final position. */
	int y2;	/**< Row number - final position. */
};

/**
 * Pointer to a Command.
 */
typedef struct Command* Request;


/** 
 * Reads a command from the input line.
 * @return command with all parameteres.
 */
Request getInput();

/**
 * Errors in parsing.
 */
int czyPoprawneWejscie;

#endif /* PARSE_H */
