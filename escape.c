#include "defs.h"


/* 
  Purpose:  Runs the server version of the application
*/
void runEscape(){
    
    EscapeType *game;
    game = malloc(sizeof(EscapeType));

    //EscapeType game;

    initEscape(game);

    while(!escapeIsOver(game)){
	outputHollow(game);
	// move heros
	for (int i = 0; i < game->heroes.size; i++) {
	    moveHero(game->heroes.elements[i], game);
	}

	int spawnBird, spawnMonkey;
	spawnBird = randomInt(100)+1;
	spawnMonkey = randomInt(100)+1;
	// spawn more bad guys
	if(spawnBird < 81)
	    spawnFlyer(game, 'v', randomInt(3)+3, randomInt(MAX_COL), randomInt(5));
	if(spawnMonkey < 41)
	    spawnFlyer(game, '@', randomInt(4)+8, randomInt(MAX_COL), randomInt(15));

	

	// move bad guys
	for (int i = 0; i < game->flyers.size; i++) {
	    moveFlyer(game->flyers.elements[i], game);
	}

	usleep(100000);
    }

    
    sendData(game->viewSocket, "quit");
    handleEscapeResult(game);
}



/* 
  Purpose:  
       in:  Main escape type
*/
int escapeIsOver(EscapeType *game){

    for (int i = 0; i < game->heroes.size; i++) {
	if(game->heroes.elements[i]->dead == C_FALSE && heroIsSafe(game->heroes.elements[i]) == C_FALSE){
	    return C_FALSE;
	}
    }
    return C_TRUE;

}
/* 
  Purpose:  to initlize all values in main escape type
       out:  Main escape type
*/

void initEscape(EscapeType *game){
    game->heroes.elements = malloc(sizeof(HeroType*)*MAX_ARR);
    game->heroes.size = 0;

    srand(time(NULL));
    HeroType *t, *h;

    int TPos, HPos;
    TPos = randomInt(5);
    HPos = randomInt(5);
    while(TPos == HPos)
	TPos = randomInt(5);

    initHero(&t, 'T', TPos, "Timmy");
    initHero(&h, 'H', HPos, "The Bunny");

    addHero(&game->heroes, h);
    addHero(&game->heroes, t);

    game->flyers.size = 0;
    
    setupServerSocket(&game->listenSocket);
    acceptConnection(game->listenSocket, &game->viewSocket);


}

/* 
  Purpose:  de-allocate all memory
       in:  Main escape type
*/

void cleanupEscape(EscapeType *game){
    for (int i = 0; i < game->heroes.size; i++) {
	free(game->heroes.elements[i]);
    }
    
    for(int i = 0; i < game->flyers.size; i++){
	free(game->flyers.elements[i]);
    }

    close(game->viewSocket);
    close(game->listenSocket);

    free(game->heroes.elements);
    free(game);
}


