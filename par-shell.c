/*'########::'########:::'#######::::::::'##:'########:'########::'#######::::::'######:::'#######::
   ##.... ##: ##.... ##:'##.... ##::::::: ##: ##.....::... ##..::'##.... ##::::'##... ##:'##.... ##:
   ##:::: ##: ##:::: ##: ##:::: ##::::::: ##: ##:::::::::: ##:::: ##:::: ##:::: ##:::..:: ##:::: ##:
   ########:: ########:: ##:::: ##::::::: ##: ######:::::: ##:::: ##:::: ##::::. ######:: ##:::: ##:
   ##.....::: ##.. ##::: ##:::: ##:'##::: ##: ##...::::::: ##:::: ##:::: ##:::::..... ##: ##:::: ##:
   ##:::::::: ##::. ##:: ##:::: ##: ##::: ##: ##:::::::::: ##:::: ##:::: ##::::'##::: ##: ##:::: ##:
   ##:::::::: ##:::. ##:. #######::. ######:: ########:::: ##::::. #######:::::. ######::. #######::
   ..:::::::::..:::::..:::.......::::......:::........:::::..::::::.......:::::::......::::.......:::

Afonso Caetano 82539
Bruno Santos 82053
João Correia 81990

Sistemas Operativos
*/
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

/* 
Variaveis Globais 
*/

char **argVector, buffer[50];
FILE *fp;
int PID, TID, num_filhos=0, status=0, flag=0, total_time, iteracao;
pthread_t tid[1];/*cria um vetor com as tarefas a criar*/
pthread_mutex_t mutex, cond_mutex;/*trinco*/
pthread_cond_t semFilhos, numProcessos;
time_t starttime, endtime;
list_t* list;/*lista que guarda os processos filho*/

/*
Funcoes axiliares gerais
*/

void mutex_lock(pthread_mutex_t* mutex) {
  if (pthread_mutex_lock(mutex)) {
    perror("Error locking mutex");
    exit(EXIT_FAILURE);
  }
}

void mutex_unlock(pthread_mutex_t* mutex) {
  if (pthread_mutex_unlock(mutex)) {
    perror("Error unlocking mutex");
    exit(EXIT_FAILURE);
  }
}

void condition_wait(pthread_cond_t* condition, pthread_mutex_t* cond_mutex) {
  if (pthread_cond_wait(condition, cond_mutex)) {
    perror("Error waiting on condition");
    exit(EXIT_FAILURE);
 }
}

void condition_signal(pthread_cond_t* condition) {
  if (pthread_cond_signal(condition)) {
    perror("Error signaling on condition");
    exit(EXIT_FAILURE);
  }
}

/* 
Funcoes auxiliares do ficheiro 
*/
void FileManager(int pid, int exec_time){
	rewind(fp);
	if (fgetc(fp)==EOF){ /*se o ficheiro estiver vazio*/
		fprintf(fp,"iteracao 0\npid: %d execution time: %d s\ntotal execution time: %d s\n", pid, 
			exec_time, exec_time);
		return;
	}
	else {
		iteracao++;
   		fprintf(fp,"iteracao %d\npid: %d execution time: %d s\ntotal execution time: %d s\n",iteracao, pid, 
		exec_time,total_time+exec_time);

	}
	if (fflush(fp)) {
		perror("Error at flushing file");
		exit(EXIT_FAILURE);
	}	
}

void verificaFormato(){
	int i=0, pid_aux, exec_time_aux;
	char line[60];

	rewind(fp);
	if (fgetc(fp)==EOF){ /*se o ficheiro estiver vazio*/
		return;
	}

	rewind(fp);
	while( fgets (line, 60, fp)!=NULL ) { /*verifica se esta na ultima linha*/
		if(i==0){
			if(sscanf(line, "iteracao %d", &iteracao)!=0){
				i++;
				continue;
			}
			else{
				printf("Ficheiro em formato inválido\n");
				exit(EXIT_FAILURE);
			}
		}
		else if(i==1){
			if(sscanf(line, "pid: %d execution time: %d s", &pid_aux, &exec_time_aux)!=0){
				i++;
				continue;
			}
			else{
				printf("Ficheiro em formato inválido\n");
				exit(EXIT_FAILURE);
			}
		}
		else if(i==2){
			if(sscanf(line, "total execution time: %d s", &total_time)!=0){
				i=0;
				continue;
			}
			else{
				printf("Ficheiro em formato inválido\n");
				exit(EXIT_FAILURE);
			}
		}
   	}
   	if(sscanf(line, "total execution time: %d s", &total_time)==0){ /*verifica se a ultima linha do ficheiro e a do total time*/
   		printf("Ficheiro em formato inválido\n");
		exit(EXIT_FAILURE);
	}
}

/* 
Tarefa Monitora 
*/

void *tarefaMonitora(){ /*Tarefa responsável por monitorizar os tempos de execução de cada processo filho */

	printf("Tarefa monitora inicializada!\n");

	fp=fopen(LOGFILE, "a+");
	if (fp==NULL){
		perror("Error opening/creating file");
		exit(EXIT_FAILURE);
	}
	
	verificaFormato();

	while (1){
    
		mutex_lock(&mutex);
	    if (num_filhos==0) {/*nao existem processos filhos*/
	      
	     	if(flag==0){/*verifica se foi acionado o comando exit*/
	        	mutex_unlock(&mutex);
	        	mutex_lock(&cond_mutex);
	        	while (num_filhos==0 && flag==0) condition_wait(&semFilhos, &cond_mutex); /*bloqueia e fica a espera do signal*/
	        	mutex_unlock(&cond_mutex);
	      	}
	      
	      	else {/*foi acionado o comando exit*/
		        mutex_unlock(&mutex);
		        printf("Tarefa monitora finalizada!\n");
		        pthread_exit(EXIT_SUCCESS);/*termina a tarefe monitora*/
	      	}
	    }
	    
	    else{/*Existem processos filhos*/
	      	mutex_unlock(&mutex);
	      	while(num_filhos>0){
	         
	          	PID=wait(&status);/*aguarda que os processos filhos terminem*/ 

	      		

	         	if(PID<0){/*verifica se houve erro fatal no processo*/
	      			perror("Error at child waiting");
	            	continue;
	          	}

	          	else if(WIFEXITED(status)){ /*verifica se o processo terminou corretamente*/
		            mutex_lock(&mutex);
		            update_terminated_process(list,PID,WEXITSTATUS(status),time(NULL)); /* guarda o status e o tempo final do processo*/
		            FileManager(PID,get_execution_time(list,PID)); /*escreve no ficheiro */
		            mutex_unlock(&mutex);
	          	}

		        mutex_lock(&mutex);
		        num_filhos--;
		        mutex_unlock(&mutex);
	      		condition_signal(&numProcessos); /*asinala que terminou um processo */
	      	}
	    }
	}
}

/*
Main thread
*/
int main(int argc, char* argv[]){
	
	list = lst_new(); /*cria uma lista onde é guardado o PID e o status dos processos*/
	
	argVector = (char**) malloc(NARGUMENTOS*sizeof(char*));

	if(pthread_mutex_init(&mutex,NULL)) { /*inicializa o trinco*/
		perror("Error at mutex initialization");
		exit(EXIT_FAILURE);
	}

	if(pthread_cond_init(&semFilhos,NULL)) {
		perror("Error at initialization of condition variable: semFilhos");
		exit(EXIT_FAILURE);
	}

	if(pthread_cond_init(&numProcessos, NULL)) {
		perror("Error at initialization of condition variable: numProcessos");
		exit(EXIT_FAILURE);
	}

	if(pthread_create(&tid[0] ,NULL,tarefaMonitora,NULL) != 0) { /*cria a tarefa monitora*/
		perror("Error at thread creation");
		exit(EXIT_FAILURE);
	}

	while(1){

		int fserv;
		char myfifo[DIM] = "par-shell-in" ;

		/*if(mkfifo(myfifo,PREMISSOES)<0) {
			perror("Error creating FIFO");
			exit(-1);
		}*/
		if( (fserv=open(myfifo,O_RDONLY)) < 0) {
			perror("Error associating FIFO in par-shell");
			exit(EXIT_FAILURE);
		}

		read(fserv,argVector,20);
		close(fserv);
		  
			if(strcmp(argVector[0], EXIT_COMMAND)==0){
		    

			    mutex_lock(&mutex);
			    flag=1;/*memoriza o acionamento do comando exit*/
			    mutex_unlock(&mutex);

			    condition_signal(&semFilhos); /*desbloquia a tarefa monitora podendo assim terminar  */
			    mutex_unlock(&cond_mutex);
			   
			    if (pthread_join(tid[0],NULL)) { /*aguarda que a tarefa monitora termine*/
			    	perror("Error waiting for thread");
			    	exit(EXIT_FAILURE);
			    }
			    
			    lst_print(list);/*imprime a lista dos processos filho*/
			    lst_destroy(list);/*apaga todos os elementos da lista*/

			    if(fclose(fp)) {
			    	perror("Error closing the file");
			    	exit(EXIT_FAILURE);
			    }

			    free(argVector[0]);
			    free(argVector);

			    pthread_mutex_destroy(&mutex);/*elimina o mutex e liberta os recursos a ele associados*/
			    pthread_mutex_destroy(&cond_mutex);

			    if(pthread_cond_destroy(&semFilhos)) {
			    	perror("Error at destruction of condition: semFilhos");
			    	exit(EXIT_FAILURE);
			    }

			    if(pthread_cond_destroy(&numProcessos)) {
			    	perror("Error ate destruction of condition: numProcessos");
			    	exit(EXIT_FAILURE);
			    }

			    exit(EXIT_SUCCESS);/*termina o processo pai*/
			}

			else {
		    	 
		    	mutex_lock(&cond_mutex);
		    	while(num_filhos>=MAXPAR) condition_wait(&numProcessos, &cond_mutex); /*bloqueia quando esta a correr o numero max de processo permitidos*/
		    	mutex_unlock(&cond_mutex);
			    PID=fork();/*guarda na variavel PID o resultado da funçao fork*/


			    if(PID<0){  /*caso ocorra erro na criacao do processo filho*/
			    	perror("Error at child creation");
			    }

			    else if(PID==0){/*processo filho*/
				    close(1);
				    sprintf(buffer, "par-shell-out-%d.txt", getpid());
				
				    open(buffer, O_WRONLY | O_CREAT);
		
					if(execv(argVector[0],argVector)<0){ /*verifica se ocorreu um erro a correr o executavel*/

			      		perror("Error at execution");
				        free(argVector[0]);
				        free(argVector);
				        exit(EXIT_FAILURE);/*termina o processo filho*/
				    }
				}
	  		}
	  
		  	mutex_lock(&mutex);
		  	num_filhos++;

		  	insert_new_process(list, PID, time(NULL));/*insere na lista o PID  e o tempo inicial do processo filho*/
		  	mutex_unlock(&mutex);
		  	
		  	condition_signal(&semFilhos);  /*da signal para a tarefa monitora desbloquear */
		  	
		  	free(argVector[0]);
	     

		//else{
	  	//	printf("Please insert a valid argument!\n");
		//}
	}
}










