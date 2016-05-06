#include <stdio.h>
#include <stdlib.h>

typedef struct Unit* UnitsList;

enum unitType {
	PEASANT, KING, KNIGHT
};

struct Unit {
	enum unitType type; // typ jednostki
	int priority; // 1 - peasant, 2 - king, 3 - knight, wedlug mocy w walce
	int lastMove; // w ktorej kolejce ruszal sie po raz ostatni
	int player; // numer gracza
	int x, y; // wspolrzedne
	UnitsList next;
	UnitsList previous;
};

// globalne Zmienne
static int actualTurnNumber; // numer aktualnej tury
static int gameTurnNumber; // liczba tur w calej grze
static UnitsList globalUnitsList; // lista jednostek
static int actualPlayer; // gracz aktualnie wykonujacy ruch
static char topleft[11][11]; // lewy gorny rog
static int globalBoardSize; // rozmiar planszy
static int gameOver; // czygraskonczona
static int kingOne, kingTwo; // czy zyje krol1/krol2
static int liczbaInitow; // ile initow juz bylo
static int initialisedPlayer = -1; // numer gracza, ktory juz byl zainicjalizowany

/**
 * czy koniec gry
 */
int koniecGry() {
	return gameOver;
}

static int min(int a, int b) {
	if(a < b) return a;
	return b;
}

/**
 * czy punkt wewnatrz planszy 
 */
static int insideBoard(int x, int y, int boardSize) {
	return x >= 1 && y >= 1 && x <= boardSize && y <= boardSize;
}

/**
 * zwraca znak jednostki 
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
 * czy lista jest pusta
 */
static int isEmptyList(UnitsList uniList) {
	return uniList == NULL;
}

/**
 * tworzymy i zwracamy nowa jednostke, jesli sie nie da zwracamy NULL
 * zakladamy ze wspolrzedne sa poprawne
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
	newUnit->lastMove = 0; // moze sie ruszyc od razu po utworzeniu
	newUnit->player = actualPlayer;
	newUnit->next = NULL;
	newUnit->previous = NULL;
	
	return newUnit;
}

/**
 * dodajemy jednostke na liste
 * dodajemy tez na lewy gorny rog jesli mozna
 * zakladamy z gory ze wspolrzedne poprawne
 * zwracamy ta jednostke
 */
static UnitsList addUnit(int x, int y, enum unitType type) {
	UnitsList newUnit = makeUnit(x, y, type);
		
	if(isEmptyList(globalUnitsList)) globalUnitsList = newUnit;
	else {
		newUnit->next = globalUnitsList;
		globalUnitsList->previous = newUnit;
		globalUnitsList = newUnit;
	}
	
	// dodajemy do topleft jesli sie zmiesci
	if (x <= 10 && y <= 10)
		topleft[x][y] = mark(type, actualPlayer);
	
	return newUnit;
}

/**
 * zwracamy wskaznik do jednostki na podanych wspolrzednych
 * zwracamy NULL jesli takiej nie ma
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
 * usuwamy jednostke z listy
 * jesli usuwamy krola to konczymy gre
 * usuwamy tez jesli trzeba z topleft
 */
static void removeUnit(UnitsList uniList) {
		
	if(uniList->previous != NULL) uniList->previous->next = uniList->next;
	if(uniList->next != NULL) uniList->next->previous = uniList->previous;
	
	if(uniList == globalUnitsList) globalUnitsList = uniList->next;
			
	// usuwamy z topleft jesli sie w nim miesci
	if (uniList->x <= 10 && uniList->y <= 10)
		topleft[uniList->x][uniList->y] = '.';
	
	// jesli usuwamy krola to koniec gry
	if(uniList->type == KING) {
		if(uniList->player == 1) kingOne = 0;
		else kingTwo = 0;
		
		gameOver = 1;
	}
	
	free(uniList);
}

/**
 * rekurencyjne usuwanie listy
 */
static void removeList(UnitsList uniList) {
	if(isEmptyList(uniList)) return;
	
	removeList(uniList->next);
	free(uniList);
}

/**
 * czy punkty sasiaduja ze soba
 */
static int isNeighbour(int x1, int y1, int x2, int y2) {
	return abs(x1 - x2) <= 1 && abs(y1 - y2) <= 1;
}

/**
 * inicjalizacja gry
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
 * czyszczenie pamieci, wypisanie wyniku gry
 * czy zakonczylo sie poprawnie czy nie
 */
void endGame(int correct) { 
	removeList(globalUnitsList);
	if (correct) {
		if (kingOne == kingTwo) fprintf(stderr, "DRAW\n");
		else if (kingOne > kingTwo) fprintf(stderr, "PLAYER 1 won\n");
		else fprintf(stderr, "PLAYER 2 won\n");
	}
	else {
		fprintf(stderr, "input error");
	}
}

/**
 * wypisuje lewy gorny rog tablicy (rozmiaru 10 X 10 lub mniej jesli mniejszy)
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
 * Rozstawiamy jednostki na poczatkowych pozycjach.
 */
static void firstDistribution (int x, int y) {
		addUnit(x, y, KING) ;
		addUnit(x + 1, y, PEASANT);
		addUnit(x + 2, y, KNIGHT);
		addUnit(x + 3, y, KNIGHT);
}

/**
 * inicjalizujemy graczy
 * w przypadku bledu zwracamy 1
 */
int init(int boardSize, int turnNumber, int player, int x1, int y1, int x2, int y2) {
	
	// sprawdzamy ktory init z kolei
	if (liczbaInitow > 1) return 1;
	
	// sprawdzamy czy rozny od poprzedniego
	if (player == initialisedPlayer) return 1;
	 
	// oddalenie krolow co najmniej 8 w metryce maksimum
	if (abs(x1 - x2) < 8 && abs(y1 - y2) < 8) return 1;
	
	// rozmiar planszy
	if (boardSize <= 8) return 1;
	
	// numer gracza
	if (player != 1 && player != 2) return 1;
	
	// liczba tur
	if (turnNumber < 1) return 1;

	// czy mozna stworzyc poczatkowe ustawienie
	if(!insideBoard(x1, y1, boardSize)) return 1;
	if(!insideBoard(x1 + 3, y1, boardSize)) return 1;
	if(!insideBoard(x2, y2, boardSize)) return 1;
	if(!insideBoard(x2 + 3, y2, boardSize)) return 1;
	
	if (liczbaInitow == 0) {
		globalBoardSize = boardSize;
		gameTurnNumber = turnNumber;
	
		actualPlayer = 1;	
		firstDistribution(x1, y1);
		
		actualPlayer = 2;
		firstDistribution(x2, y2);
	}
	
	if (liczbaInitow == 1) {
		if (globalBoardSize != boardSize) return 1;
		if (gameTurnNumber != turnNumber) return 1;
		
		// czy zgadza sie z poprzednim initem
		UnitsList king1 = findUnit(x1, y1);
		if(king1 == NULL || king1->type != KING || king1->player != 1) return 1;
		
		UnitsList king2 = findUnit(x2, y2);
		if(king2 == NULL || king2->type != KING || king2->player != 2) return 1;
		
		actualPlayer = 1; // zaczynamy rozgrywke od gracza pierwszego
	}
	
	liczbaInitow++;
	initialisedPlayer = player;
	
	return 0;
}

/**
 * walka miedzy jednostkami
 * w zalozeniu sa na tym samym polu i istnieja
 * zwraca 0 - remis, 1 - wygral pierwszy, 2 - wygral drugi
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
	 
	// obie jednostki gina 
	removeUnit(unitFirst);
	removeUnit(unitSecond);
	return 0;
}

/**
 * sprawdzamy czy chlop moze produkowac
 * produkuje jednostke i zwraca 1 jesli nie moze i zero jesli tak 
 */
static int produceUnit(int x1, int y1, int x2, int y2, enum unitType type) {
	// tworzymy tylko na sasiednim
	if(!isNeighbour(x1, y1, x2, y2)) return 1;
	
	// czy wewnatrz planszy
	if(!insideBoard(x1, y1, globalBoardSize)) return 1;
	if(!insideBoard(x2, y2, globalBoardSize)) return 1;
	
	// nie ma jednostki lub nie jest chlopem na (x1, y1)
	UnitsList peasant = findUnit(x1, y1);
	if (peasant == NULL || peasant->type != PEASANT) return 1;
	
	// czy stal ostatnie dwie kolejki
	if (actualTurnNumber - peasant->lastMove < 3) return 1;
	
	UnitsList secondUnit = findUnit(x2, y2);
	
	// czy nie ma innej jednostki na (x2, y2)
	if(secondUnit != NULL) return 1;
	
	addUnit(x2, y2, type);
		
	// aktualizujemy ostatni ruch chlopa
	peasant->lastMove = actualTurnNumber;

	return 0;	
}

/**
 * przenosi jednostke i zwraca 0
 * jesli nie moze zwraca 1
 */
int move(int x1, int y1, int x2, int y2) {
	// przesuwamy tylko na sasiednie
	if(!isNeighbour(x1, y1, x2, y2)) return 1;
	
	// czy wewnatrz planszy
	if(!insideBoard(x1, y1, globalBoardSize)) return 1;
	if(!insideBoard(x2, y2, globalBoardSize)) return 1;

	UnitsList unitFirst = findUnit(x1, y1);

	// brak jednostki na polu pierwszym
	if (unitFirst == NULL) return 1;
	
	// czy jednostka sie nie ruszala
	if (unitFirst->lastMove == actualTurnNumber) return 1;
	
	// czy przesuwamy wlasna jednostke
	if (unitFirst->player != actualPlayer) return 1;
	
	UnitsList unitSecond = findUnit(x2, y2);	
	
	// przesuwamy na wlasna jednostke
	if(unitSecond != NULL && unitSecond->player == actualPlayer) return 1;

	// przesuwamy na wroga jednostke lub na wolne pole
	if (x1 <= 10 && y1 <= 10) {
		topleft[unitFirst->x][unitFirst->y] = '.';
	}
	unitFirst->x = x2;
	unitFirst->y = y2;
	unitFirst->lastMove = actualTurnNumber;
	
	// na pewno na polu jest jednostka pierwsza
	int winner = 1;
	
	// jesli jest wroga jednostka to zaczynamy walke
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
 * produkujemy rycerza
 */
int produceKnight(int x1, int y1, int x2, int y2) {
	return produceUnit(x1, y1, x2, y2, KNIGHT);
}

/**
 * produkujemy chlopa
 */
int producePeasant(int x1, int y1, int x2, int y2) {
	return produceUnit(x1, y1, x2, y2, PEASANT);
}

/**
 * koniec rundy - zmieniamy gracza
 * zwiekszamy licznik rundy i sprawdzamy czy nie ma konca gry
 */
int endTurn() {
	if (actualPlayer == 1) actualPlayer = 2;
	else {
		actualTurnNumber++;
		actualPlayer = 1;
	}
	
	if (actualTurnNumber > gameTurnNumber) gameOver = 1; 
	return 0;
}
