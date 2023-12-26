#include "defs.h"
#include <pthread.h>
#include <semaphore.h>

sem_t mutex;
/* 
  Purpose:  Runs the server version of the application
*/
void runEscape(){


    pthread_t outputThread;
    EscapeType *game;
    game = malloc(sizeof(EscapeType));

    if(sem_init(&mutex, 0, 1) < 0){
	perror("Semaphore initilalization failed");
	exit(-1);
    }
    pthread_create(&outputThread, NULL, outputWrapper, game);
    usleep(OUTPUT_INTERVAL+5); // to make sure the output is run before the while loop is
    //TODO: put a semiphore around most of the while loop
    //TODO: make outputHollow the threaded function

    initEscape(game);

    
    
    while(!escapeIsOver(game)){
	sem_wait(&mutex);
	// move heros
	for (int i = 0; i < game->heroes.size; i++) {
	    moveHero(game->heroes.elements[i], game);
	}

	int spawnBird, spawnMonkey;
	spawnBird = randomInt(100)+1;
	spawnMonkey = randomInt(100)+1;
	// spawn more bad guys
	if(spawnBird <= BIRD_SPAWN_RATE)
	    spawnFlyer(game, 'v', randomInt(3)+3, randomInt(MAX_COL), randomInt(5));
	if(spawnMonkey <= MONKEY_SPAWN_RATE)
	    spawnFlyer(game, '@', randomInt(4)+8, randomInt(MAX_COL), randomInt(15));

	

	// move bad guys
	for (int i = 0; i < game->flyers.size; i++) {
	    moveFlyer(game->flyers.elements[i], game);
	}
	sem_post(&mutex);

	usleep(100000);
    }

    sendData(game->viewSocket, "quit");
    //TODO: join the thread here
    pthread_join(outputThread, NULL);

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


