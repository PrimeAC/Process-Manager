#define NARGUMENTOS 7
#define MAXPAR 4
#define LOGFILE "log.txt"
#define EXIT_COMMAND "exit"
#define PREMISSOES 0666
#define DIM 20



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "commandlinereader.h"
#include "list.h"
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

/*PAR-SHELL-TERMINAL */
/*
Main thread
*/
int main(int argc, char* argv[]){
	
	char **argVector, *myfifo="par-shell-in";
	argVector = (char**) malloc(NARGUMENTOS*sizeof(char*));
	int fserv;
	//char myfifo[DIM]="par-shell-in";

	unlink(myfifo);

	if (mkfifo (myfifo, 0777) < 0) {
		perror("Error creating FIFO");
		exit(EXIT_FAILURE);
	}
	
	if ((fserv = open(myfifo,O_WRONLY))<0) {
		perror("Error associating FIFO in par-shell-terminal");
		exit(-1);
	}

	if(readLineArguments(argVector, NARGUMENTOS)>0){ /*verifica se o utilizador escreveu algo */

		write(fserv,argVector,DIM);
	}
	close(fserv);
	unlink(myfifo);
	exit(EXIT_SUCCESS);
}