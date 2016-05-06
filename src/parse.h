 /** @file
    Interface of parser.

 */
#ifndef PARSE_H
#define PARSE_H

enum Instruction {
	INIT, MOVE, PRODUCE_KNIGHT, PRODUCE_PEASANT, END_TURN, WRONG	
};

struct Command {
	enum Instruction instruction;
	int boardSize;
	int turnNumber;
	int player;
	int x1, y1, x2, y2;	
};

typedef struct Command* Request;


/** Reads a command.
  Returns command with data points using 'command' structure.
  */
Request getInput();

int czyPoprawneWejscie;

#endif /* PARSE_H */
