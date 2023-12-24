#include "defs.h"

/* 
  Purpose: initlized the hero
out: double pointer to hero struct 
int: hero avatar
in: col to spawn hero
in: display name of hero
*/
void initHero(HeroType **hero, char avatar, int col, char *name){
    *hero = malloc(sizeof(HeroType));

    strcpy((*hero)->name, name);
    (*hero)->partInfo.avatar = avatar;

    (*hero)->partInfo.pos.col = col;
    (*hero)->partInfo.pos.row = MAX_ROW-1;

    (*hero)->dead = C_FALSE;
   
    (*hero)->health = MAX_HEALTH;
}

/* 
  Purpose: adds hero to hero array struct
out: array to add hero to 
in: hero to be added
*/
void addHero(HeroArrayType *arr, HeroType *hero){
    if(arr->size == MAX_ARR){
	return;
    }

    arr->elements[arr->size] = hero;
    arr->size++;
}





/* 
  purpose: checks if the hero is safe
in: hero to check
*/
int heroIsSafe(HeroType *hero){
    if(hero->partInfo.pos.col >= MAX_COL-1){
	return C_TRUE;
    }
    return C_FALSE;
}



/* 
  purpose: moves hero according to movement pattern
in: un used parameter 
*/
void moveHero(HeroType *hero, EscapeType *game){
    if(hero->dead || heroIsSafe(hero))
	return;
    
    int typeOfMove = randomInt(100)+1;
    int moveAmount;
    if(hero->partInfo.avatar == 'H'){
	if(typeOfMove <= 20)
	    moveAmount = 0;
	else if(21 <= typeOfMove && typeOfMove <=30)
	    moveAmount = 5;
	else if(31 <= typeOfMove && typeOfMove <=40)
	    moveAmount = -4;
	else if(41 <= typeOfMove && typeOfMove <=80)
	    moveAmount = 3;
	else
	    moveAmount = -2;



    }else{
	if(typeOfMove <= 50)
	    moveAmount = 2;
	else if(51 <= typeOfMove && typeOfMove <=80)
	    moveAmount = -1;
	else
	    moveAmount = 1;

    }

    setPos(&hero->partInfo.pos, MAX_ROW-1, hero->partInfo.pos.col+moveAmount);
}
