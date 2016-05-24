/**
 * @file engine.c
 * Engine of the game.
 * Implements the board and units.
 * Implements the moves of the game.
 * @author Michal Kuzba
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct Unit* UnitsList;

enum unitType {
	PEASANT, KING, KNIGHT
};

/**
 * Units on the board.
 */
struct Unit {
	enum unitType type; /**< theType of the unit. */
	int priority; 
	/**< 1 - peasant, 2 - king, 3 - knight, power in the fight */
	int lastMove; /**< Number of the last turn, the unit has moved. */
	int player; /**< Number of the player - 1 or 2 */
	int x; /**< Column number. */
	int y; /**< Row number. */
	UnitsList next; /**< Next unit on the units list. */
	UnitsList previous; /**< Previous unit on the units list. */
};

// global variables
static int actualTurnNumber; 
static int gameTurnNumber; // Turn number in the whole game
static UnitsList globalUnitsList; // structure - list of the all units
int actualPlayer;
static char topleft[11][11]; // top-left corner of the board
static int globalBoardSize; 
static int gameOver;
static int kingOne, kingTwo; // is king1/king2 alive
static int liczbaInitow; // how many inits were so far.
int winner = 0; //default 0 - draw, otherwise number of the winner
/**
 * is the game over
 */
int koniecGry() {
	return gameOver;
}

static int min(int a, int b) {
	if(a < b) return a;
	return b;
}

/**
 * is the point inside the board 
 */
static int insideBoard(int x, int y, int boardSize) {
	return x >= 1 && y >= 1 && x <= boardSize && y <= boardSize;
}

/**
 * returns the mark assigned to the type of the unit 
 */
static char mark(enum unitType unit, int playerNumber) {
	switch(unit) {
		case PEASANT : if (playerNumber == 1) return 'C'; else return 'c';
		break;
		
		case KING : if (playerNumber == 1) return 'K'; else return 'k';
		break;
		
		case KNIGHT : if (playerNumber == 1) return 'R'; else return 'r';
		break;	
	}
	
	return '.';
}

/**
 * is the list empty
 */
static int isEmptyList(UnitsList uniList) {
	return uniList == NULL;
}

/**
 * creates and returning a new unit or returning NULL if it is not possible
 * assumes coordinates are correct
 */
static UnitsList makeUnit(int x, int y, enum unitType type) {
	
	UnitsList newUnit;
	newUnit = malloc(sizeof(struct Unit));
	newUnit->type = type;
	if(type == PEASANT) newUnit->priority = 1;
	else if(type == KING) newUnit->priority = 2;
	else newUnit->priority = 3;
	
	newUnit->x = x;
	newUnit->y = y;
	newUnit->lastMove = 0; // Unit can move from the very beginning.
	newUnit->player = actualPlayer;
	newUnit->next = NULL;
	newUnit->previous = NULL;
	
	return newUnit;
}

/**
 * adds a unit to the list
 * adds to the topleft if applicable
 * assumes coordinates are correct
 * returns the unit
 */
static UnitsList addUnit(int x, int y, enum unitType type) {
	UnitsList newUnit = makeUnit(x, y, type);
		
	if(isEmptyList(globalUnitsList)) globalUnitsList = newUnit;
	else {
		newUnit->next = globalUnitsList;
		globalUnitsList->previous = newUnit;
		globalUnitsList = newUnit;
	}
	
	// adding to the topleft if it should be there.
	if (x <= 10 && y <= 10)
		topleft[x][y] = mark(type, actualPlayer);
	
	return newUnit;
}

/**
 * returns the pointer to the unit on the given coordinates
 * returns NULL if there is no unit there
 */
static UnitsList findUnit(int x, int y) {

	UnitsList uniList = globalUnitsList;
	
	while(!isEmptyList(uniList)) {
		if(uniList->x == x && uniList->y == y) return uniList;
		uniList = uniList->next;
	}
		
	return NULL;
}

/**
 * removes the unit from the list
 * if the king is removed sets the game over is true
 * removes from topleft if applicable
 */
static void removeUnit(UnitsList uniList) {
		
	if(uniList->previous != NULL) uniList->previous->next = uniList->next;
	if(uniList->next != NULL) uniList->next->previous = uniList->previous;
	
	if(uniList == globalUnitsList) globalUnitsList = uniList->next;
			
	// removes from topleft if it is there.
	if (uniList->x <= 10 && uniList->y <= 10)
		topleft[uniList->x][uniList->y] = '.';
	
	// removing the king ends the game.
	if(uniList->type == KING) {
		if(uniList->player == 1) kingOne = 0;
		else kingTwo = 0;
		
		gameOver = 1;
	}
	
	free(uniList);
}

/**
 * recursive list removing
 */
static void removeList(UnitsList uniList) {
	if(isEmptyList(uniList)) return;
	
	removeList(uniList->next);
	free(uniList);
}

/**
 * are the points neighbouring
 */
static int isNeighbour(int x1, int y1, int x2, int y2) {
	return abs(x1 - x2) <= 1 && abs(y1 - y2) <= 1;
}

/**
 * game initialization
 */
void startGame() {
	gameOver = 0;
	kingOne = 1;
	kingTwo = 1;
	actualTurnNumber = 1;
	actualPlayer = 1;
	liczbaInitow = 0;
	
	int i, j;
	for (i = 1; i <= 10; i++)
		for (j = 1; j <= 10; j++)
			topleft[i][j] = '.';
}

/**
 * memory cleaning, printing the game result
 * parameter - was the game finished correctly or not
 */
void endGame(int correct) { 
	removeList(globalUnitsList);
	if (correct) {
		if (kingOne == kingTwo) fprintf(stderr, "draw\n");
		else if (kingOne > kingTwo) {
			 fprintf(stderr, "player 1 won\n");
			 winner = 1;
		 }
		else {
			fprintf(stderr, "player 2 won\n");
			winner = 2;
		}
	}
	else {
		fprintf(stderr, "input error\n");
	}
}

/**
 * prints the topleft corner of the array (10 X 10 or smaller)
 */
void printTopleft() {
	int i, j;
	for (i = 1; i <= min(10, globalBoardSize); i++) {
		for (j = 1; j <= min(10, globalBoardSize); j++)
			printf("%c", topleft[j][i]);
			
		printf("\n");	
	}
	
	printf("\n");
}

/**
 * Placing units on the initial positions
 */
static void firstDistribution (int x, int y) {
		addUnit(x, y, KING) ;
		addUnit(x + 1, y, PEASANT);
		addUnit(x + 2, y, KNIGHT);
		addUnit(x + 3, y, KNIGHT);
}

/**
 * Players initialization
 * returns 1 in case of error
 */
int init(int boardSize, int turnNumber, int player, int x1, int y1, int x2, int y2) {
	
	// checking which init is it
	if (liczbaInitow > 0) return 1;
		 
	// distatnce between kings in the maximum norm at least 8
	if (abs(x1 - x2) < 8 && abs(y1 - y2) < 8) return 1;
	
	// board size > 8
	if (boardSize <= 8) return 1;
	
	// player number 1 or 2
	if (player != 1 && player != 2) return 1;
	
	// turn number positive
	if (turnNumber < 1) return 1;

	// are the initial positions correct
	if(!insideBoard(x1, y1, boardSize)) return 1;
	if(!insideBoard(x1 + 3, y1, boardSize)) return 1;
	if(!insideBoard(x2, y2, boardSize)) return 1;
	if(!insideBoard(x2 + 3, y2, boardSize)) return 1;
	
	
	globalBoardSize = boardSize;
	gameTurnNumber = turnNumber;
	
	actualPlayer = 1;	
	firstDistribution(x1, y1);
		
	actualPlayer = 2;
	firstDistribution(x2, y2);
	
	actualPlayer = 1;
	
	liczbaInitow++;
	
	return 0;
}

/**
 * fight between units
 * assumes that they are on the same field 
 * returns : 0 - draw, 1 - first won, 2 - second won
 */
static int walka(UnitsList unitFirst, UnitsList unitSecond) {
	if (unitFirst->priority > unitSecond->priority)  {
		removeUnit(unitSecond);
		return 1;
	}
	
	if(unitFirst->priority < unitSecond->priority) {
		 removeUnit(unitFirst);
		 return 2;
	 }
	 
	// both units die 
	removeUnit(unitFirst);
	removeUnit(unitSecond);
	return 0;
}

/**
 * checks if the peasant can produce
 * produce the unit and returns 0 if succesful and 1 if not
 * checks if the peasant can produce
 */
static int produceUnit(int x1, int y1, int x2, int y2, enum unitType type) {
	// only on the neighbouring field
	if(!isNeighbour(x1, y1, x2, y2)) return 1;
	
	// is it inside the board
	if(!insideBoard(x1, y1, globalBoardSize)) return 1;
	if(!insideBoard(x2, y2, globalBoardSize)) return 1;
	
	// no unit or it is not a peasant on (x1, y1)
	UnitsList peasant = findUnit(x1, y1);
	if (peasant == NULL || peasant->type != PEASANT) return 1;
	
	// has the unit moved last two turns
	if (actualTurnNumber - peasant->lastMove < 3) return 1;
	
	UnitsList secondUnit = findUnit(x2, y2);
	
	// is there any other unit on (x2, y2)
	if(secondUnit != NULL) return 1;
	
	addUnit(x2, y2, type);
		
	// updates the last move of the peasant
	peasant->lastMove = actualTurnNumber;

	return 0;	
}

/**
 * moves the unit and returns 0
 * returns 1 if not possible
 */
int move(int x1, int y1, int x2, int y2) {
	// moves only to the neighbouring
	if(!isNeighbour(x1, y1, x2, y2)) return 1;
	
	// inside board
	if(!insideBoard(x1, y1, globalBoardSize)) return 1;
	if(!insideBoard(x2, y2, globalBoardSize)) return 1;

	UnitsList unitFirst = findUnit(x1, y1);

	// first unit does not exist
	if (unitFirst == NULL) return 1;
	
	// has the unit moved already
	if (unitFirst->lastMove == actualTurnNumber) return 1;
	
	// does the player move his own unit
	if (unitFirst->player != actualPlayer) return 1;
	
	UnitsList unitSecond = findUnit(x2, y2);	
	
	// moving onto the own unit
	if(unitSecond != NULL && unitSecond->player == actualPlayer) return 1;

	// moving on the enemy unit or the empty field
	if (x1 <= 10 && y1 <= 10) {
		topleft[unitFirst->x][unitFirst->y] = '.';
	}
	unitFirst->x = x2;
	unitFirst->y = y2;
	unitFirst->lastMove = actualTurnNumber;
	
	// unit first is on the field for sure
	int winner = 1;
	
	// starts the fight if there is an enemy unit
	if (unitSecond != NULL) 
		winner = walka(unitFirst, unitSecond);
		
	if (x2 <= 10 && y2 <= 10) {
		if (winner == 0) topleft[x2][y2] = '.';
		else if (winner == 1)
			topleft[x2][y2] = mark(unitFirst->type, unitFirst->player);
		else 
			topleft[x2][y2] = mark(unitSecond->type, unitSecond->player);
	}
	
	return 0;
}

/**
 * produces the new knight
 */
int produceKnight(int x1, int y1, int x2, int y2) {
	return produceUnit(x1, y1, x2, y2, KNIGHT);
}

/**
 * produces the new peasant
 */
int producePeasant(int x1, int y1, int x2, int y2) {
	return produceUnit(x1, y1, x2, y2, PEASANT);
}

/**
 * end of the turn - changing the actual player
 * increases the turn number and checks whether the game is over
 */
void endTurn() {
	if (actualPlayer == 1) actualPlayer = 2;
	else {
		actualTurnNumber++;
		actualPlayer = 1;
	}
	
	if (actualTurnNumber > gameTurnNumber) gameOver = 1; 
}
