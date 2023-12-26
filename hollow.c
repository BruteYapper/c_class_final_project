#include "defs.h"


/* 
  purpose: initlize the hollow
in: gives all data needed to fill hollow
out: 2d hollow array
*/
void initHollow(EscapeType *game, char hollow[][MAX_COL]){
    for (int i = 0;i < MAX_ROW; i++) {
	for(int j = 0; j < MAX_COL; j++){
	    hollow[i][j] = ' ';
	}
    }

    for(int i = 0; i < game->heroes.size; i++){
	hollow[game->heroes.elements[i]->partInfo.pos.row][game->heroes.elements[i]->partInfo.pos.col] = game->heroes.elements[i]->partInfo.avatar;
    }

    for (int i = 0;i < game->flyers.size; i++) {
	if(flyerIsDone(game->flyers.elements[i]))
	    continue;
	hollow[game->flyers.elements[i]->partInfo.pos.row][game->flyers.elements[i]->partInfo.pos.col]
	    = game->flyers.elements[i]->partInfo.avatar;
    }
}


/* 
  purpose: take the 2d hollow array and flatten it into a 1d arary
in: gives all required game data
out: 1d array of characters that is the hollow
*/
void serializeHollow(EscapeType *game, char *stringHollow){
    char hollow[MAX_ROW][MAX_COL];
    char NameAndHealth[MAX_STR*2];
    initHollow(game, hollow);

    int offset = 0;
    stringHollow[offset++] = '+';
    while(offset < MAX_COL+1)
	stringHollow[offset++] = '-';
    stringHollow[offset++] = '+';
    stringHollow[offset++] = '\n';

    for (int i = 0;i<MAX_ROW;i++) {
	if(offset == 944)
	    stringHollow[offset++] = '=';
	else
	    stringHollow[offset++] = '|';
	for(int j = 0; j < MAX_COL; j++){
	    stringHollow[offset++] = hollow[i][j];
	}
	printf("%d\n", offset);
	if(offset == 817){
	    sprintf(NameAndHealth, "|%-13s: %3d",
		    game->heroes.elements[0]->name,
		    game->heroes.elements[0]->health);
	    strcat(stringHollow, NameAndHealth);
	    offset += strlen(NameAndHealth);

	}else if(offset == 898){
	    sprintf(NameAndHealth, "|%-13s: %3d",
		    game->heroes.elements[1]->name,
		    game->heroes.elements[1]->health);
	    strcat(stringHollow, NameAndHealth);
	    offset += strlen(NameAndHealth);

	}else if(offset == 10042){
	    stringHollow[offset++] = '=';
	    
	    
	}
	else
	    stringHollow[offset++] = '|';

	stringHollow[offset++] = '\n';
    }
    stringHollow[offset++] = '+';
    for (int i = 0;i < MAX_COL; i++) {
	stringHollow[offset+i] = '-';
    }
    offset+=MAX_COL;
    stringHollow[offset++] = '+';
    stringHollow[offset] = '\n';

    
/*
    for (int i = 0; i < MAX_ROW; i++) {
	stringHollow[i*MAX_ROW+offset] = '|';
	printf("%d\n", (i*MAX_ROW+offset));
	offset++;
	for(int j = 0; j < MAX_COL; j++){
	    stringHollow[i*MAX_ROW+j+offset] = hollow[i][j];
	    printf("%d\n", (i*MAX_ROW+j+offset));
	}
	stringHollow[i*MAX_ROW+MAX_COL+offset] = '\n';
	printf("%d\n", (i*MAX_ROW+MAX_COL+offset));
	offset++;

    }
*/
	/*
	stringHollow[i*MAX_ROW] = '|';
	stringHollow[i*MAX_ROW+1] = '\n';
	for (int j = 2; j < MAX_COL; j++) {
	    stringHollow[i*MAX_ROW+j+offset] = hollow[i][j-2];
	}
    }*/
}

/* 
  purpose: prints and sends hollow to screen and client
in: all required simulation data
*/
void outputHollow(EscapeType *game){
    char *hollowString;
    hollowString = calloc(sizeof(char), MAX_BUFF);
    //hollowString = malloc(sizeof(char)*MAX_BUFF);
    serializeHollow(game, hollowString);
    system("clear");
    printf("%s\n", hollowString);
    
    sendData(game->viewSocket, hollowString);

    free(hollowString);
}

void *outputWrapper(void *game){
    extern sem_t mutex;
    sem_wait(&mutex);
    do {
	sem_post(&mutex); // the reason for the post and wait to be like this is because game needs to be locked while escapeIsOver runs
	usleep(OUTPUT_INTERVAL);
	sem_wait(&mutex);
	outputHollow(game);

    } while (!escapeIsOver(game));
    sem_post(&mutex);
    return(0);
}



/* 
  purpose: sets the position of a position type while checking for boundries
in: new row
in: new col
*/
void setPos(PositionType *pos, int row, int col){
    pos->col = col;
    pos->row = row;

    if(pos->col < 0)
	pos->col = 0;
    else if(MAX_COL-1 < pos->col)
	pos->col = MAX_COL-1;

    if(pos->row < 0)
	pos->row = 0;
    else if(MAX_ROW < pos->row)
	pos->row = MAX_ROW;

}


/* 
  purpose: prints out game result and sends to client
in: all data required for simulation
*/
void handleEscapeResult(EscapeType *game){
    outputHollow(game);
    char result[MAX_STR*4];
    HeroArrayType *temp;
    temp = &game->heroes;
    sprintf(result, "%s is %s \n %s is %s \n", temp->elements[0]->name, (temp->elements[0]->dead)? "DEAD": "ALIVE", temp->elements[1]->name, (temp->elements[1]->dead)? "DEAD": "ALIVE");
    sendData(game->viewSocket, result);
    printf("%s", result);
}
