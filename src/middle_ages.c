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

#define GAME_FINISHED_CORRECTLY { endGame(1); free(command); return 0; }
#define GAME_FINISHED_INCORRECTLY {	endGame(0);	free(command); return 42; }

int initsAmount = 0;

int main() {

    startGame();
      
	while(1) {
    
		Request command = getInput();
		
		// End of the input.	
		if (command == NULL) {
			if(koniecGry())
				GAME_FINISHED_CORRECTLY
			else 
				GAME_FINISHED_INCORRECTLY
		}		
		
		if (!czyPoprawneWejscie) 
			GAME_FINISHED_INCORRECTLY
		
		// First two commands needs to be inits.	
		if (initsAmount < 2 && command->instruction != INIT) 
			GAME_FINISHED_INCORRECTLY
			
		switch(command->instruction) {
			case INIT : {
				initsAmount++;
				
				if (init(command->boardSize, command->turnNumber, command->player,
				command->x1, command->y1, command->x2, command->y2)) 
					GAME_FINISHED_INCORRECTLY
					
				printTopleft();
			}
			break;
				
			case END_TURN :
				endTurn();
			break;
				
			case MOVE : 
				if (move(command->x1, command->y1, command->x2, command->y2)) 
					GAME_FINISHED_INCORRECTLY
					
				printTopleft();
			break;
				
			case PRODUCE_KNIGHT :
				if (produceKnight(command->x1, command->y1, command->x2, command->y2)) 
					GAME_FINISHED_INCORRECTLY
					
				printTopleft();
			break;
				
			case PRODUCE_PEASANT :
				if (producePeasant(command->x1, command->y1, command->x2, command->y2)) 
					GAME_FINISHED_INCORRECTLY
				printTopleft();
			break;
			
			// Wrong command.
			case WRONG : 
			GAME_FINISHED_INCORRECTLY
			break;
		}
		
		// Game over with a result.
		if (koniecGry()) 
			GAME_FINISHED_CORRECTLY
			
		free(command);
		
	}
	
	// End of the game - end of the turns.
    endGame(1);

    return 0;
}
