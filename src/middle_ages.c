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

#define GAME_FINISHED_CORRECTLY { endGame(1); free(command); return returnedResult; }
#define GAME_FINISHED_INCORRECTLY {	endGame(0);	free(command); return 42; }

int thisPlayer = 0;
int returnedResult = 1;

int main() {

    startGame();
      
	while(1) {
    
		Request command = getInput();
				
		// End of the input.	
		if (command == NULL) {
			if(koniecGry()) {
				if (gameWinner == 0) returnedResult = 1; // draw
				else if (gameWinner == thisPlayer) returnedResult = 0;
				else returnedResult = 2;
				GAME_FINISHED_CORRECTLY
			}
			else 
				GAME_FINISHED_INCORRECTLY
		}		
		
		if (!czyPoprawneWejscie) 
			GAME_FINISHED_INCORRECTLY
		
		
		// First command needs to be init.	
		if (thisPlayer == 0 && command->instruction != INIT) 
			GAME_FINISHED_INCORRECTLY
		
			
		switch(command->instruction) {
			case INIT : {
				
				if (init(command->boardSize, command->turnNumber, command->player,
				command->x1, command->y1, command->x2, command->y2)) 
					GAME_FINISHED_INCORRECTLY
				else 
					thisPlayer = command->player;
					
			}
			break;
				
			case END_TURN :
				endTurn();
				break;
				
			case MOVE : 
				if (move(command->x1, command->y1, command->x2, command->y2)) 
					GAME_FINISHED_INCORRECTLY
					
			break;
				
			case PRODUCE_KNIGHT :
				if (produceKnight(command->x1, command->y1, command->x2, command->y2)) 
					GAME_FINISHED_INCORRECTLY
					
			break;
				
			case PRODUCE_PEASANT :
				if (producePeasant(command->x1, command->y1, command->x2, command->y2)) 
					GAME_FINISHED_INCORRECTLY
			break;
			
			// Wrong command.
			case WRONG : 
			GAME_FINISHED_INCORRECTLY
			break;
		}
		
		// Move of this AI programme	
		if (!koniecGry() && thisPlayer == actualPlayer) {
			printf("END_TURN\n");
			fflush(stdout);
			endTurn();
		}
		
		// Game over with a result.
		if (koniecGry()) {
			if (gameWinner == 0) returnedResult = 1; // draw
			else if (gameWinner == thisPlayer) returnedResult = 0;
			else returnedResult = 2;
			GAME_FINISHED_CORRECTLY
		}
			
		
		
		free(command);
		
	}
	
}
