/**
 * @file Parser.
 * @author Michal Kuzba
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINE_SIZE 100

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

int czyPoprawneWejscie; // czy polecenie bylo poprawne 

static int isDigit(char c) {
	return (c >= '0' && c <= '9');	
}

static int cyfra(char c) {
	return (c - '0');
}

/**
 * zwraca liczbe z ciagu znakow lub -1 jesli ujemne lub przekroczy int
 */
static int numberOfString(char* str) {
	int wynik = 0;
	
	int i = 0;
	
	while (str[i] != '\0') {
		if (!isDigit(str[i]))
			return -1;
			
		wynik *= 10;
		wynik += cyfra(str[i]);
		
		if (wynik <= 0)
			return -1;
		i++;
	}
	
	return wynik;
}

/**
 * zwraca polecenie ze stringa
 */
static enum Instruction getCommand(char *pol) {
	if(strcmp(pol, "INIT") == 0) return INIT;
	if(strcmp(pol, "MOVE") == 0) return MOVE;
	if(strcmp(pol, "PRODUCE_KNIGHT") == 0) return PRODUCE_KNIGHT;
	if(strcmp(pol, "PRODUCE_PEASANT") == 0) return PRODUCE_PEASANT;
	if(strcmp(pol, "END_TURN") == 0) return END_TURN;
	return WRONG;
}

// indeks nastepnej spacji (znaku konca)
static int nextSpace(char *line, int begin) {
	int iter = begin;
	
	while(line[iter] != ' ' && line[iter] != '\n' && line[iter] != '\0') {
		iter++;
	}
	
	return iter;
}

// do word zapisujemy fragment napisu line w zakresie [begin, end)
static void getWord(char* line, char *word, int begin, int end) {
	
	int i = 0;
	while (i < end - begin)
	{
		word[i] = line[i + begin];
		i++;
	}
	word[i] = '\0';
}

/**
 * zwraca nastepna liczbe z linii od pozycji begin
 * ustawia dalej wartosci begin, end
 */
static int nextNumber(int* begin, int* end, char* line) {
	*begin = *end + 1;
	
	*end = nextSpace(line, *begin);
					
	char *numberString = malloc(sizeof(char) *  (*end - *begin + 2)); 
			
	getWord(line, numberString, *begin, *end);
		
	int result = numberOfString(numberString);
		
	if (result == -1) 
		czyPoprawneWejscie = 0;

	free(numberString);
		
	return result;
}

/**
 * wczytywanie do structa
 */
static Request readLine(char *line) {
	
	czyPoprawneWejscie = 1;
	
	int begin = 0;
	int end = 0;
	
	Request actualCommand = malloc(sizeof(struct Command));
			
	// wyluskujemy polecenie
	end = nextSpace(line, 0);
	
	char *word = malloc(sizeof(char) * (end + 2)); 
	
	getWord(line, word, 0, end);
			
	actualCommand->instruction = getCommand(word);
	
	free(word);
	
	switch(actualCommand->instruction) {
			
		case INIT :
		{	
			// przepisujemy liczbe ze stringa na inta
			actualCommand->boardSize = nextNumber(&begin, &end, line);
			actualCommand->turnNumber = nextNumber(&begin, &end, line);
			actualCommand->player = nextNumber(&begin, &end, line);
			actualCommand->x1 = nextNumber(&begin, &end, line);
			actualCommand->y1 = nextNumber(&begin, &end, line);
			actualCommand->x2 = nextNumber(&begin, &end, line);
			actualCommand->y2 = nextNumber(&begin, &end, line);
			if(line[end] != '\n') czyPoprawneWejscie = 0;
			
		}
		break;
				
		case END_TURN : {
			if(line[end] != '\n') czyPoprawneWejscie = 0;
		}
		break;
		
		case WRONG : {
			czyPoprawneWejscie = 0;
		}
		break;
		
		// polecenia PRODUCE_KNIGHT, PRODUCE_PEASANT, MOVE obslugujemy tak samo
		default : {
			actualCommand->x1 = nextNumber(&begin, &end, line);
			actualCommand->y1 = nextNumber(&begin, &end, line);
			actualCommand->x2 = nextNumber(&begin, &end, line);
			actualCommand->y2 = nextNumber(&begin, &end, line);
			if(line[end] != '\n') czyPoprawneWejscie = 0;
		}
		break;
		
	}	
	return actualCommand;
}

Request getInput() {
	char* input = malloc(sizeof(char) * 102);
	
	char c = 'z'; // dowolna wartosc poczatkowa rozna od znaku konca linii
	int i = 0;
	
	while (c != '\n') {
			c = getchar();
			
			if (c == EOF) {
				free(input);
				return NULL;
			}
			
			// wczytujemy pierwszych 100 znakow do tablicy
			if (i < 100) {
				input[i] = c;
				input[i + 1] = '\0';
			}
		i++;
	}
	
	if (i > 100) {
		free(input);
		return NULL;
	}
		
	Request wynik = readLine(input);
	
	free(input);
	
	return wynik;
}


