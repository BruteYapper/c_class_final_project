#include "defs.h"



int main(int argc, char *argv[]){
    if(argc > 1){
	viewEscape(argv[1]);
    }else{
	runEscape();
    }
    return(0);
}


int randomInt(int max){


  return(rand() % max);
}

