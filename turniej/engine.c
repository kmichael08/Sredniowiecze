/**
 * @file engine.c
 * Engine of the game.
 * Implements the board and units.
 * Implements the moves of the game.
 * @author Michal Kuzba
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
};

// global variables
static int actualTurnNumber; 
static int gameTurnNumber; // Turn number in the whole game
int actualPlayer;
static int globalBoardSize; 
static int gameOver;
static int kingOne, kingTwo; // is king1/king2 alive
static int liczbaInitow; // how many inits were so far.
int gameWinner = 0; //default 0 - draw, otherwise number of the winner
UnitsList gameBoard[21][21]; // plansza do turnieju
int kingx1;
int kingy1;
int kingx2;
int kingy2;
/**
 * is the game over
 */
int koniecGry() {
	return gameOver;
}

/**
 * is the point inside the board 
 */
static int insideBoard(int x, int y, int boardSize) {
	return x >= 1 && y >= 1 && x <= boardSize && y <= boardSize;
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
	else if(type == KING) 
	{ 
		newUnit->priority = 2;
		// poczatkowa pozycja krola
		if (actualPlayer == 1) {
			kingx1 = x;
			kingy1 = y;
		}
		else {
			kingx2 = x;
			kingy2 = y;
		}
	}
	else newUnit->priority = 3;
	
	newUnit->x = x;
	newUnit->y = y;
	newUnit->lastMove = 0; // Unit can move from the very beginning.
	newUnit->player = actualPlayer;
	
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
	gameBoard[x][y] = newUnit;
	return newUnit;
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
	
}

/**
 * memory cleaning, printing the game result
 * parameter - was the game finished correctly or not
 */
void endGame(int correct) { 
	if (correct) {
		if (kingOne == kingTwo) gameWinner = 0;
		else if (kingOne > kingTwo) {
			gameWinner = 1;
		 }
		else {
			gameWinner = 2;
		}
	}
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

UnitsList findUnit(int x, int y) {
	return gameBoard[x][y];
}

/**
 * fight between units
 * assumes that they are on the same field 
 * returns : 0 - draw, 1 - first won, 2 - second won
 */
static int walka(UnitsList unitFirst, UnitsList unitSecond) {
	int x = unitFirst->x;
	int y = unitFirst->y;
	
	if (unitFirst->priority > unitSecond->priority)  {
		gameBoard[x][y] = unitFirst;
		return 1;
	}
	
	if(unitFirst->priority < unitSecond->priority) {
		 gameBoard[x][y] = unitSecond;
		 return 2;
	 }
	 
	// both units die 
	gameBoard[x][y] = NULL;
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
	unitFirst->x = x2;
	unitFirst->y = y2;
	gameBoard[x2][y2] = unitFirst;
	gameBoard[x1][y1] = NULL;

	// jesli przesuwamy krola
	if (unitFirst->type == KING) {
		if (actualPlayer == 1) {
			kingx1 = x2;
			kingy1 = y2;
		}
		else {
			kingx2 = x2;
			kingy2 = y2;
		}
		
	}
	unitFirst->lastMove = actualTurnNumber;
	
	// unit first is on the field for sure
	int winner = 1;
	
	// starts the fight if there is an enemy unit
	if (unitSecond != NULL) 
		winner = walka(unitFirst, unitSecond);
			
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

int tryproduceknight(int direction, int i, int j) {
	if (!produceKnight(i, j, i, j + direction)) {
		printf("PRODUCE_KNIGHT %d %d %d %d\n", i, j, i, j + direction);
		fflush(stdout);
	}
				
}

int tryproducepeasant(int direction, int i, int j) {
	if (!producePeasant(i, j, i, j + direction)) {
		printf("PRODUCE_PEASANT %d %d %d %d\n", i, j, i, j + direction);
		fflush(stdout);
		return 1;
	}
	return 0;
}

void movewithwrite(int x1, int y1, int x2, int y2) {
	if(move(x1, y1, x2, y2)) return;
	printf("MOVE %d %d %d %d\n", x1, y1, x2, y2);
	fflush(stdout);
}


int tryproduce(int i, int j) {
	int peasant;
	
	if (actualTurnNumber < 10) peasant = 50;
	else peasant = 10;
	
	int los = rand() % 100;
	
	if (actualPlayer == 1) {
		if (los > peasant)
			tryproduceknight(1, i, j);
		else
			{
			if (tryproducepeasant(1, i, j)) {
					movewithwrite(i, j + 1, i - 1, j + 1);
					movewithwrite(i, j + 1, i + 1, j + 1);				
				}
			
			}
	}
	if (actualPlayer == 2) {
		if (los > peasant)
			tryproduceknight(-1, i, j);
		else
			if (tryproducepeasant(-1, i, j)) {
				movewithwrite(i, j - 1, i - 1, j - 1);
				movewithwrite(i, j - 1, i + 1, j - 1);				
			}	
	}
}


void prodpeaswithwrite(int x1, int y1, int x2, int y2) {
	if(producePeasant(x1, y1, x2, y2)) return;
	printf("PRODUCE_PEASANT %d %d %d %d\n", x1, y1, x2, y2);
	fflush(stdout);

}

void prodrycwithwrite(int x1, int y1, int x2, int y2) {
	if (produceKnight(x1, y1, x2, y2)) return;
	printf("PRODUCE_KNIGHT %d %d %d %d\n", x1, y1, x2, y2);
	fflush(stdout);

}

void zakonczture() {
	endTurn();
	printf("END_TURN\n");
	fflush(stdout);
}

int sgn(int number) {
	if (number > 0) return 1;
	else if (number == 0) return 0;
	return -1;
}

void ruch() {
	srand(time(NULL));
		
	int i, j;
	for (i = 1; i <= globalBoardSize; i++)
		for (j = 1; j <= globalBoardSize; j++) {
			UnitsList unit = gameBoard[i][j];
			
			if (unit == NULL) continue;
			if (unit->player != actualPlayer) continue;
			if (unit->type == PEASANT) {
					tryproduce(i, j);
			}
			
			
			if (unit->type == KNIGHT) {
				if (actualPlayer == 1) {
					int dir1, dir2;
					dir1 = sgn(kingx2 - i);
					dir2 = sgn(kingy2 - j);
					movewithwrite(i, j, i + dir1, j + dir2);
				

					
									
					
				}
				if (actualPlayer == 2) {
					int dir1, dir2;
					dir1 = sgn(kingx1 - i);
					dir2 = sgn(kingy1 - j);
					movewithwrite(i, j, i + dir1, j + dir2);
				
				}		
				
			}
			
		}
			
	zakonczture();
		
			
}
	
