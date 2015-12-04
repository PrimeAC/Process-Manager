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
/*
Main thread
*/
int main(int argc, char* argv[]){

	char *my_string, *message, pipeout[20];
	my_string = (char*) malloc(NARGUMENTOS*sizeof(char));
	message = (char*) malloc(DIM*sizeof(char));
	//pipeout = (char*) malloc(DIM*sizeof(char));
	int fserv, fclient;
	size_t nbytes = DIM;
	sprintf(pipeout, "par-shell-%d", getpid());
	printf("%s\n",pipeout );

	if(argv[1] == NULL) {
		printf("%s\n","Too few arguments!");
		exit(EXIT_FAILURE);
	}

	if ((fserv = open(argv[1],O_WRONLY))<0) {
		perror("Error associating FIFO in par-shell-terminal");
		exit(EXIT_FAILURE);
	}
	printf("antes do while\n");

	while (1) {
	
		fgets(my_string, nbytes, stdin);/*verifica se o utilizador escreveu algo */

		if (strcmp(my_string,"stats\n")==0) {
			printf("entrei no stats\n");
			//strtok(my_string, "\n");
			//strcat(my_string, " ");
			//strcat(my_string,pipeout);
			//strcat(my_string, "\n");
			printf("%s\n",my_string );

			if((sprintf(my_string, "stats %s\n", pipeout)) <0){
				perror("Error at sprintf");
				exit(EXIT_FAILURE);
			}
			unlink(pipeout);

			if (mkfifo (pipeout, PREMISSOES) < 0) {
				perror("Error creating FIFO");
				exit(EXIT_FAILURE);
			}
			
			if( write(fserv,my_string,strlen(my_string)) < 0){
				perror("Error writing stream");
				exit(EXIT_FAILURE);
			}

			printf("%s\n",my_string );
			printf("pipeout:%s\n",pipeout );
			if((fclient = open(pipeout, O_RDONLY)) <0 ){
				perror("Error associating FIFO in par-shell");
				exit(EXIT_FAILURE);
			}

			printf("escrevi para a par-shell\n");
			printf("abri e vou ler\n");
			printf("%s-1\n",message );

			if( read(fclient,message,200) < 0){
				perror("Error writing stream");
				exit(EXIT_FAILURE);
			}
			printf("message:%s\n",message);
			unlink(pipeout);
		}
		if (strcmp(my_string,"exit\n")==0) {
			unlink(pipeout);
			break;
		}
		if (strcmp(my_string,"exit-global\n")==0) {
			printf("vou escrever no pipe\n");
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