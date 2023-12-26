#include "defs.h"


/* 
  purpose: handles all client side code
in: ip address of server
*/

void viewEscape(char *ip){
    int connectedClient;
    setupClientSocket(&connectedClient, ip);

	char buff[MAX_BUFF];
    while(1){

	if(strcmp(buff, "quit") == 0)
	    break;
	rcvData(connectedClient, buff);

	if(strcmp(buff, "quit") == 0){
	    break;
	}
	printf("%s\n", buff);
    }

	rcvData(connectedClient, buff);
	printf("%s\n", buff);
	rcvData(connectedClient, buff);
	printf("%s\n", buff);
}
