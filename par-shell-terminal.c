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
	
	char *my_string;
	my_string = (char*) malloc(NARGUMENTOS*sizeof(char));
	int fserv;
	//size_t nbytes = NARGUMENTOS;

	if(argv[1] == NULL) {
		printf("%s\n","Too few arguments!");
		exit(EXIT_FAILURE);
	}

	if ((fserv = open(argv[1],O_WRONLY))<0) {
		perror("Error associating FIFO in par-shell-terminal");
		exit(EXIT_FAILURE);
	}
	while (1) {
		fgets(my_string, 100, stdin);/*verifica se o utilizador escreveu algo */
		if (strcmp(my_string,"exit-global\n")==0) {
			if( write(fserv,my_string,strlen(my_string)) < 0){
				perror("Error writing stream");
				exit(EXIT_FAILURE);
			}
			break;
		}
		if (strcmp(my_string, "stats\n")==0) {

		}
		else {
			if( write(fserv,my_string,strlen(my_string)) < 0){
				perror("Error writing stream");
				exit(EXIT_FAILURE);
			}
		}
	}
	close(fserv);
	exit(EXIT_SUCCESS);
}