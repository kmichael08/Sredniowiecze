/**
 * @file 
 * Main file.
 * @author Michal Kuzba
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse.h"
#include "engine.h"

int initsAmount = 0;

int main() {

    startGame();
      
	while(1) {
    
		Request command = getInput();
		
		// End of the input.	
		if (command == NULL) {
			endGame(koniecGry());	
			free(command);
			return 42;
		}
				
		if (!czyPoprawneWejscie) {
			endGame(0);
			free(command);
			return 42;
		}
		
		// First two commands needs to be inits.	
		if (initsAmount < 2 && command->instruction != INIT) {
			endGame(0);
			free(command);
			return 42;
		}
			
		switch(command->instruction) {
			case INIT : {
				initsAmount++;
				
				if (init(command->boardSize, command->turnNumber, command->player,
				command->x1, command->y1, command->x2, command->y2)) {
					
					endGame(0);
					free(command);
					return 42;
				}
				printTopleft();
			}
			break;
				
			case END_TURN :
				endTurn();
			break;
				
			case MOVE : 
				if (move(command->x1, command->y1, command->x2, command->y2)) {
					endGame(0);
					free(command);
					return 42;
				}
				printTopleft();
			break;
				
			case PRODUCE_KNIGHT :
				if (produceKnight(command->x1, command->y1, command->x2, command->y2)) {
					endGame(0);
					free(command);
					return 42;
				}
				printTopleft();
			break;
				
			case PRODUCE_PEASANT :
				if (producePeasant(command->x1, command->y1, command->x2, command->y2)) {
					endGame(0);
					free(command);
					return 42;
				}
				printTopleft();
			break;
			
			// Wrong command.
			case WRONG : 
			{
				endGame(0);
				free(command);
				return 42;
			}
		}
		
		// Game over with a result.
		if (koniecGry()) {
			endGame(1);
			free(command);
			return 0;
		}
			
		free(command);
		
	}
	
	// End of the game - end of the turns.
    endGame(1);

    return 0;
}
