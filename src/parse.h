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
	INIT, MOVE, PRODUCE_KNIGHT, PRODUCE_PEASANT, END_TURN, WRONG	
};

/**
 * Instruction.
 * Instruction with all parametres.
 */
struct Command {
	enum Instruction instruction;
	int boardSize;
	int turnNumber;
	int player;
	int x1, y1, x2, y2;	
};

/**
 * Pointer to Command.
 */
typedef struct Command* Request;


/** 
 * Reads a command from the input line.
 * @return command.
 */
Request getInput();

/**
 * Errors in parsing.
 */
int czyPoprawneWejscie;

#endif /* PARSE_H */
