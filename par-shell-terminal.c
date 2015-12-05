#define NARGUMENTOS 7
#define MAXPAR 4
#define LOGFILE "log.txt"
#define EXIT_COMMAND "exit"
#define PERMISSOES 0777
#define DIM 50



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

void pipeR(int fd, char *buffer, int count ) { /*le um numero de caracteres (count) do conteudo do pipe (fd)*/
	if( read(fd,buffer,count) < 0){
		perror("Error read stream");
		exit(EXIT_FAILURE);
	}
}

void pipeW(int fd, char *buffer, size_t count ) {	/*escreve um numero de caracteres (count) do conteudo do pipe (fd)*/
	if( write(fd,buffer,count) < 0){
		perror("Error writing stream");
		exit(EXIT_FAILURE);
	}
}

/*
Main thread
*/
int main(int argc, char* argv[]){

	char my_string[DIM], message[DIM], pipeout[DIM], terminal[DIM];
	size_t nbytes = DIM;

	if (sprintf(pipeout, "/tmp/par-shell-%d", getpid()) <0) {
		perror("Error at sprintf");
		exit(EXIT_FAILURE);
	}


	if(argv[1] == NULL) {
		printf("%s\n","Too few arguments!");
		exit(EXIT_FAILURE);
	}

	if ((fserv = open(argv[1],O_WRONLY))<0) {
		perror("Error associating FIFO in par-shell-terminal");
		exit(EXIT_FAILURE);
	}

	if (sprintf(terminal, "pid %d\n", getpid()) <0 ) {
		perror("Error at sprintf");
		exit(EXIT_FAILURE);
	}

	pipeW(fserv, terminal, strlen(terminal)); /*envia a mensagem a par-shell com o seu pid, para o inserir na lista*/

	while (1) {
	
		if (fgets(my_string, nbytes, stdin) == NULL) { /*verifica se o utilizador escreveu algo*/
			perror("Error reading from stdin");
			exit(EXIT_FAILURE);
		}

		if (strcmp(my_string,"stats\n")==0) {

			if((sprintf(my_string, "stats %s\n", pipeout)) <0){
				perror("Error at sprintf");
				exit(EXIT_FAILURE);
			}

			unlink(pipeout);

			if (mkfifo (pipeout, PERMISSOES) < 0) { /*cria um pipe com o nome par-shell-"numero do pid"*/
				perror("Error creating FIFO");
				exit(EXIT_FAILURE);
			}

			pipeW(fserv,my_string,strlen(my_string));/*envia o pipe pelo qual vao ser enviadas informacoes pela par-shell relativas ao stats*/

			if((fclient = open(pipeout, O_RDONLY)) <0 ){
				perror("Error associating FIFO in par-shell");
				exit(EXIT_FAILURE);
			}

			pipeR(fclient,message,200);/*le do pipe, as informacoes enviadas pela par-shell sobre o stats*/
			
			printf("%s\n",message);

			continue;
		}
		if (strcmp(my_string,"exit\n")==0) { 
			char msg2[DIM];
			sprintf(msg2, "remove %d\n", getpid());
			
			pipeW(fserv, msg2, strlen(msg2)); 	/*envia o comando que elimina o terminal*/
			
			unlink(pipeout);
			break;
		}
		if (strcmp(my_string,"exit-global\n")==0) { 

			pipeW(fserv,my_string,strlen(my_string));	/*comando para encerrar ordeiramente todos os terminais e a par-shell*/
			
			unlink(pipeout);
			continue;
		}
		else {
			
			pipeW(fserv,my_string,strlen(my_string));
		}
	}
	close(fserv);
	close(fclient);
	exit(EXIT_SUCCESS);	
}