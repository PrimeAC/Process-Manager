#define NARGUMENTOS 7
#define MAXPAR 4
#define LOGFILE "log.txt"
#define EXIT_COMMAND "exit"
#define PREMISSOES 0777
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
int fserv, fclient,n;

void removepid(){
	char terminal1[20];
	sprintf(terminal1, "remove %d\n", getpid());

	if (write(fserv, terminal1, strlen(terminal1)) < 0) {
		perror("");
	    exit(EXIT_FAILURE);
	}

}
/*
Main thread
*/
int main(int argc, char* argv[]){

	char *my_string, *message, pipeout[20], terminal[20];
	my_string = (char*) malloc(NARGUMENTOS*sizeof(char));
	message = (char*) malloc(DIM*sizeof(char));
	size_t nbytes = DIM;
	sprintf(pipeout, "par-shell-%d", getpid());

	signal (SIGINT, removepid);

	if(argv[1] == NULL) {
		printf("%s\n","Too few arguments!");
		exit(EXIT_FAILURE);
	}

	if ((fserv = open(argv[1],O_WRONLY))<0) {
		perror("Error associating FIFO in par-shell-terminal");
		exit(EXIT_FAILURE);
	}

	sprintf(terminal, "pid %d\n", getpid());

	if (write(fserv, terminal, strlen(terminal)) < 0) {
		perror("");
	    exit(EXIT_FAILURE);
	}

	while (1) {
	
		fgets(my_string, nbytes, stdin);/*verifica se o utilizador escreveu algo */

		if (strcmp(my_string,"stats\n")==0) {

			if((sprintf(my_string, "stats %s\n", pipeout)) <0){
				perror("Error at sprintf");
				exit(EXIT_FAILURE);
			}
			unlink(pipeout);

			if (mkfifo (pipeout, PREMISSOES) < 0) {
				perror("Error creating FIFO");
				exit(EXIT_FAILURE);
			}

			if(write(fserv,my_string,strlen(my_string))< 0) {
				perror("Error writing stream");
				exit(EXIT_FAILURE);
			}

			if((fclient = open(pipeout, O_RDONLY)) <0 ){
				perror("Error associating FIFO in par-shell");
				exit(EXIT_FAILURE);
			}

			if( read(fclient,message,200) < 0){
				perror("Error writing stream");
				exit(EXIT_FAILURE);
			}
			printf("%s\n",message);

			continue;
		}
		if (strcmp(my_string,"exit\n")==0) {
			char msg2[DIM];
			sprintf(msg2, "remove %d\n", getpid());
			
			if (write(fserv, msg2, strlen(msg2)) < 0) {
				perror("Error writing stream");
	    		exit(EXIT_FAILURE);
			}
			
			unlink(pipeout);
			break;
		}
		if (strcmp(my_string,"exit-global\n")==0) {

			if( write(fserv,my_string,strlen(my_string)) < 0){
				perror("Error writing stream");
				exit(EXIT_FAILURE);
			}
			unlink(pipeout);
			break;
		}
		else {
			
			if( write(fserv,my_string,strlen(my_string)) < 0){
				perror("Error writing stream");
				exit(EXIT_FAILURE);
			}
		}
	}
	close(fserv);
	close(fclient);
	exit(EXIT_SUCCESS);
	
}