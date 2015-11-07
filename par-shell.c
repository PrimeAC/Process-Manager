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

FILE *fp;
char **argVector, line[60];
int PID, TID, num_filhos=0, status=0, i, flag=0, tempo, iteracao;
pthread_t tid[1];/*cria um vetor com as tarefas a criar*/
pthread_mutex_t mutex, cond_mutex;/*trinco*/
pthread_cond_t semFilhos, numProcessos;
time_t starttime, endtime;
list_t* list;/*lista que guarda os processos filho*/

int obtemTempo(){
	
	rewind(fp);
	while( fgets (line, 60, fp)!=NULL ) {
		continue;	
   	}
   	sscanf(line, "total execution time: %d s", &tempo);
   	return tempo;
}

int obtemIteracao(){

   	int i;
   	char line1[15];
   	rewind(fp);
   	while( fgets (line, 60, fp)!=NULL ) {
		if(line[0]==105){ /*verifica se a string comeca em i*/
			for(i=0;i<15;i++){
				line1[i]=line[i];/*line1 guarda a ultima string começada por i, onde esta a iteraçao*/
			}	
   		}
   	}
   	sscanf(line1, "iteracao %d", &iteracao);
   	return iteracao;
}

void Atualiza(int pid,int exec_time){
	int total_time;
	rewind(fp);
	if (fgetc(fp)==EOF){
		fprintf(fp,"iteracao 0\npid: %d execution time: %d s\ntotal execution time: %d s\n", pid, exec_time,exec_time);
		return;
	}
	total_time=obtemTempo()+exec_time;
	iteracao=obtemIteracao()+1;
	fprintf(fp,"iteracao %d\npid: %d execution time: %d s\ntotal execution time: %d s\n",iteracao, pid, exec_time,total_time );

	
}



void *tarefaMonitora(){ /*Tarefa responsável por monitorizar os tempos de execução de cada processo filho */

	printf("Tarefa monitora inicializada!\n");

	while (1){
    
		pthread_mutex_lock(&mutex);
	    if (num_filhos==0) {/*nao existem processos filhos*/
	      
	     	if(flag==0){/*verifica se foi acionado o comando exit*/
	        	pthread_mutex_unlock(&mutex); 
	        	pthread_mutex_lock(&cond_mutex);
	        	while (num_filhos==0 && flag==0) pthread_cond_wait(&semFilhos, &cond_mutex);
	        	pthread_mutex_unlock(&cond_mutex);
	      	}
	      
	      	else {/*foi acionado o comando exit*/
		        pthread_mutex_unlock(&mutex);
		        printf("Tarefa monitora finalizada!\n");
		        pthread_exit(EXIT_SUCCESS);/*termina a tarefe monitora*/
	      	}
	    }
	    
	    else{/*Existem processos filhos*/
	      	pthread_mutex_unlock(&mutex);
	      	while(num_filhos>0){
	         
	          	PID=wait(&status);/*aguarda que os processos filhos terminem*/ 

	      		pthread_cond_signal(&numProcessos);
	      		

	         	if(PID<0){/*verifica se houve erro fatal no processo*/
	            	continue;
	          	}

	          	else if(WIFEXITED(status)){ /*verifica se o processo terminou corretamente*/
		            pthread_mutex_lock(&mutex);
		            update_terminated_process(list,PID,WEXITSTATUS(status),time(NULL)); /* guarda o status e o tempo final do processo*/
		            pthread_mutex_unlock(&mutex);
	          	}

		        pthread_mutex_lock(&mutex);
		        num_filhos--;
		        pthread_mutex_unlock(&mutex);
	      	}
	    }
	}
}

/*
Main Program
*/
int main(int argc, char* argv[]){
	
	list = lst_new(); /*cria uma lista onde é guardado o PID e o status dos processos*/

	argVector = (char**) malloc(NARGUMENTOS*sizeof(char*));

	pthread_mutex_init(&mutex,NULL);/*inicializa o trinco*/

	pthread_cond_init(&semFilhos,NULL);
	pthread_cond_init(&numProcessos, NULL);
	
	fp=fopen("log.txt", "a+");
	if (fp==NULL){
		perror("log.txt");
		exit(EXIT_FAILURE);
	}
	for(i=0;i<5;i++){
		Atualiza(i,6);
	}
	fclose(fp);

	TID = pthread_create(&tid[0] ,NULL,tarefaMonitora,NULL);/*cria a tarefa monitora*/

	if (TID!=0){/*verifica se houve um erro a criar a tarefa*/
		perror("");
	}

	while(1){


		if(readLineArguments(argVector, NARGUMENTOS)>0){ /*verifica se o utilizador escreveu algo */
		  
			if(strcmp(argVector[0], "exit")==0){
		    

			    pthread_mutex_lock(&mutex);
			    flag=1;/*memoriza o acionamento do comando exit*/
			    pthread_mutex_unlock(&mutex);

			    pthread_cond_signal(&semFilhos);
			    pthread_mutex_unlock(&cond_mutex);
			   
			    pthread_join(tid[0],NULL);/*aguarda que a tarefa monitora termine*/
			    
			    lst_print(list);/*imprime a lista dos processos filho*/
			    lst_destroy(list);/*apaga todos os elementos da lista*/

			    free(argVector[0]);
			    free(argVector);

			    pthread_mutex_destroy(&mutex);/*elimina o mutex e liberta os recursos a ele associados*/
			    pthread_mutex_destroy(&cond_mutex);

			    pthread_cond_destroy(&semFilhos);
			    pthread_cond_destroy(&numProcessos);

			    exit(EXIT_SUCCESS);/*termina o processo pai*/
			}

			else {
		    	 
		    	pthread_mutex_lock(&cond_mutex);
		    	while(num_filhos>=MAXPAR) pthread_cond_wait(&numProcessos,&cond_mutex);
		    	pthread_mutex_unlock(&cond_mutex);
			    PID=fork();/*guarda na variavel PID o resultado da funçao fork*/
			    
			    if(PID<0){  /*caso ocorra erro na criacao do processo filho*/
			    	perror("");
			    }

			    else if(PID==0){/*processo filho*/
			      
					if(execv(argVector[0],argVector)<0){ /*verifica se ocorreu um erro a correr o executavel*/

			      		perror("");
				        free(argVector[0]);
				        free(argVector);
				        exit(EXIT_FAILURE);/*termina o processo filho*/
				    }
				}
	  		}
	  
		  	pthread_mutex_lock(&mutex);
		  	num_filhos++;

		  	insert_new_process(list, PID, time(NULL));/*insere na lista o PID  e o tempo inicial do processo filho*/
		  	pthread_mutex_unlock(&mutex);
		  	
		  	pthread_cond_signal(&semFilhos);
		  	
		  	free(argVector[0]);
	    } 

		else{
	  		printf("Por favor insira um argumento válido!\n");
		}
	}
}










