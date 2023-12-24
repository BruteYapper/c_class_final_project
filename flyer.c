#include "defs.h"


/* 
  Purpose: take a given hero and apply the damage from a given flyer
       in-out:  Hero type that was hit by flyer
       in: flyer type that hit hero
*/
void incurDamage(HeroType *hero, FlyerType *flyer){
    hero->health -= flyer->strength;
    if(hero->health <= 0){
	hero->health = 0;
	hero->partInfo.avatar = '+';
	hero->dead = C_TRUE;
    }
}
/* 
  Purpose: to spawn a given flyer and add it into the flyer array
       in-out:  the escape type
       in: the avatar of the flyer
    in: the flyer's damage
    in: the col to spawn the flyer in 
    in: the row to spawn the flyer in 
*/
void spawnFlyer(EscapeType *game, char avatar, int strength, int col, int row){
    FlyerType *f;
    f = malloc(sizeof(FlyerType));
    f->strength = strength;

    f->partInfo.pos.col = col;
    f->partInfo.pos.row = row;
    f->partInfo.avatar = avatar;

    addFlyer(&game->flyers, f);
}
/* 
  Purpose: add a flyer to a flyer array
       in-out:  flyer array to be added to
       in: the flyer to get added to the array
*/
void addFlyer(FlyerArrayType *arr, FlyerType *newFlyer){
    if(arr->size == MAX_ARR)
	return;
    arr->elements[arr->size] = newFlyer;
    arr->size++;
}

/* 
  Purpose: determine the direction of the nearest hero
	in: EscapeType with heros
       in: flyer looking for hero
	out: direction of closest hero
*/
void computeHeroDir(EscapeType *game, FlyerType *flyer, int *dir){
    int direction = game->heroes.elements[0]->partInfo.pos.col - flyer->partInfo.pos.col;
    for (int i = 1;i < game->heroes.size; i++) {
	if(abs(direction) > abs(game->heroes.elements[i]->partInfo.pos.col - flyer->partInfo.pos.col)){
	    direction = game->heroes.elements[i]->partInfo.pos.col - flyer->partInfo.pos.col;
	}
    }
    if(0 < direction)
	*dir = DIR_RIGHT;
    else if(direction < 0)
	*dir = DIR_LEFT;
    else
	*dir = DIR_SAME;


}
/* 
  Purpose: checks if flyer has reached the ground
    in: a flyer
*/
int flyerIsDone(FlyerType *flyer){
    if(flyer->partInfo.pos.row >= MAX_ROW){
	return C_TRUE;
    }
    return C_FALSE;
}
/* 
  Purpose: checks if flyer has hit a hero
    in: a flyer's position
    in: main escape type 
*/
HeroType* checkForCollision(PositionType *flyerPos, EscapeType *game){
    for (int i = 0;i < game->heroes.size;i++) {
	if(game->heroes.elements[i]->partInfo.pos.col == flyerPos->col && 
		game->heroes.elements[i]->partInfo.pos.row == flyerPos->row ){
		return game->heroes.elements[i];
	}
    }
	return NULL;

} 
/* 
  Purpose: moves a flyer according to the type of flyer they are
    in-out: a flyer to move
    in: used to check for collisions
*/
void moveFlyer(FlyerType *flyer, EscapeType *game){
    int newCol, newRow;
    if(flyerIsDone(flyer))
	return;

    if(flyer->partInfo.avatar == '@'){
	int multiplyer, direction;
	if(randomInt(2))
	    multiplyer = 1;
	else
	    multiplyer = -1;

	direction = 0;

	computeHeroDir(game, flyer, &direction);
   

	newCol = flyer->partInfo.pos.col + direction*randomInt(4);
	newRow = flyer->partInfo.pos.row + multiplyer*(randomInt(3)+1);


    }else{

	switch(randomInt(3)){
	    case 0:
	newCol = flyer->partInfo.pos.col - 1;
		break;
	    case 1:
	newCol = flyer->partInfo.pos.col;
		break;
	    case 2:
	newCol = flyer->partInfo.pos.col + 1;
		break;
	}

	newRow = flyer->partInfo.pos.row + 1;
    

    }


	setPos(&flyer->partInfo.pos, newRow, newCol);
	HeroType *rip;

	rip = checkForCollision(&flyer->partInfo.pos, game);

	if(rip != NULL){
	    incurDamage(rip, flyer);
	}
}
