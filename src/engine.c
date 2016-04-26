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
 * tworzymy nowa jednostke, jesli sie nie da zwracamy NULL
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
	newUnit->lastMove = -3; // moze sie ruszyc od razu po utworzeniu
	newUnit->player = actualPlayer;
	newUnit->next = NULL;
	newUnit->previous = NULL;
	
	return newUnit;
}

/**
 * dodajemy jednostke na liste
 * zwracamy jeden jesli nie mozna
 * dodajemy tez na lewy gorny rog jesli mozna
 * zakladamy z gory ze wspolrzedne poprawne
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
 * zwraca czy pole x1, y1 jest zajete
 */
static int existUnit(int x, int y) {
	return findUnit(x, y) != NULL;
}

/**
 * usuwamy jednostke z listy
 * jesli usuwamy krola to konczymy gre
 * usuwamy tez jesli trzeba z topleft
 */
static void removeUnit(UnitsList uniList) {
	if(uniList->previous != NULL) uniList->previous->next = uniList->next;
	if(uniList->next != NULL) uniList->next->previous = uniList->previous;
	
	int x = uniList->x;
	int y = uniList->y;
		
	// usuwamy z topleft jesli sie w nim miesci
	if (x <= 10 && y <= 10)
		topleft[x][y] = '.';
	
	// jesli usuwamy krola to koniec gry
	if(uniList->type == KING) {
		if(uniList->player == 1) kingOne = 0;
		else kingTwo = 0;
		
		gameOver = 1;
	}
	
	free(uniList);
}

/**
 * usuwamy jednostke z listy, wyszukujac ja po wspolrzednych
 * moze zbedne
 */
static void removeUnitCoordinates(int x, int y) {
	UnitsList uniList = findUnit(x, y);
	
	removeUnit(uniList);
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
	int i, j;
	
	for (i = 1; i <= 10; i++)
		for (j = 1; j <= 10; j++)
			topleft[i][j] = '.';
}

/**
 * czyszczenie pamieci, wypisanie wyniku gry
 */
void endGame() {
	removeList(globalUnitsList);
	if (kingOne == kingTwo) fprintf(stderr, "DRAW\n");
	else if (kingOne > kingTwo) fprintf(stderr, "PLAYER 1 won\n");
	else fprintf(stderr, "PLAYER 2 won\n");
}

/**
 * wypisuje lewy gorny rog tablicy (10 X 10) lub mniej jesli mniejszy
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
 * inicjalizujemy graczy
 * w przypadku bledu zwracamy 1
 */
int init(int boardSize, int turnNumber, int player, int x1, int y1, int x2, int y2) { 
	// oddalenie krolow co najmniej 8 w metryce maksimum
	if( (x1 - x2) < 8 && (y1 - y2) < 8) return 1;
	
	// czy mozna stworzyc poczatkowe ustawienie
	if(!insideBoard(x1, y1, globalBoardSize)) return 1;
	if(!insideBoard(x1 + 3, y1, globalBoardSize)) return 1;
	if(!insideBoard(x1, y2, globalBoardSize)) return 1;
	if(!insideBoard(x1 + 3, y2, globalBoardSize)) return 1;

	
	if (player == 1) {
		actualPlayer = 1;
		globalBoardSize = boardSize;
		gameTurnNumber = turnNumber;
								
		addUnit(x1, y1, KING) ;
		addUnit(x1 + 1, y1, PEASANT);
		addUnit(x1 + 2, y1, KNIGHT);
		addUnit(x1 + 3, y1, KNIGHT);
		
		actualPlayer = 2;
		
		addUnit(x2, y1, KING);
		addUnit(x2 + 1, y2, PEASANT);
		addUnit(x2 + 2, y2, KNIGHT);
		addUnit(x2 + 3, y2, KNIGHT);

	}
	
	if (player == 2) {
		if (globalBoardSize != boardSize) return 1;
		if (gameTurnNumber != turnNumber) return 1;
		
		// czy zgadza sie z poprzednim initem
		UnitsList king1 = findUnit(x1, y1);
		if(king1 == NULL || king1->type != KING || king1->player != 1) return 1;
		
		UnitsList king2 = findUnit(x2, y2);
		if(king2 == NULL || king2->type != KING || king2->player != 2) return 1;
		
	}
	
	printTopleft();	
	return 0;
}

/**
 * walka miedzy jednostkami
 * w zalozeniu sa na tym samym polu i istnieja
 */
static void walka(UnitsList unitFirst, UnitsList unitSecond) {
	if (unitFirst->priority > unitSecond->priority) removeUnit(unitSecond);
		else if(unitFirst->priority < unitSecond->priority) removeUnit(unitFirst);
		else {
			removeUnit(unitFirst);
			removeUnit(unitSecond);
		}
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
	
	// czy nie ma wlasnej jednostki na (x2, y2)
	if(secondUnit != NULL && secondUnit->player == actualPlayer) return 1;
	
	UnitsList producedUnit = addUnit(x2, y2, type);
	
	// jesli na wroga jednostke to walka
	if(secondUnit != NULL) 
		walka(producedUnit, secondUnit);	
	
	// aktualizujemy ostatni ruch chlopa
	peasant->lastMove = actualTurnNumber;

	printTopleft();
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
	
	UnitsList unitSecond = findUnit(x2, y2);
	UnitsList unitFirst = findUnit(x1, y1);
	
	// brak jednostki na polu pierwszym
	if (unitFirst == NULL) return 1;
	
	// czy przesuwamy wlasna jednostke
	if (unitFirst->player != actualPlayer) return 1;
	
	// przesuwamy na wlasna jednostke
	if(unitSecond != NULL && unitSecond->player == actualPlayer) return 1;
	
	// przesuwamy na wroga jednostke lub na wolne pole
	unitFirst->x = x2;
	unitFirst->y = y2;
	unitFirst->lastMove = actualTurnNumber;
	
	// jesli jest wroga jednostka to zaczynamy walke
	if(unitSecond != NULL) 
		walka(unitFirst, unitSecond);
	
	printTopleft();
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

int main() {
	
	// dorobic testy
	// przerzucic implementacje planszy do board.c
	// czy topleft wewnatrz funkcji?
	// czy raczej wyeksportowac, czy przy blednym poleceniu wypisac topleft 
	// mimo wszystko?
	// poprawic jakosc komentarzy
	
	globalBoardSize = 8;
	startGame();
		
	addUnit(2, 3, KING);
	
	addUnit(1, 5, PEASANT);
	printf("%d\n", existUnit(2, 3));
	printf("%d\n", existUnit(1, 4));
	removeUnitCoordinates(2, 3);
	printf("%d\n", existUnit(2, 3));
	
	endGame();
	
	return 0;
}
