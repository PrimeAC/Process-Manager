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
int PID, TID, num_filhos=0, status=0, flag=0, total_time, iteracao;
pthread_t tid[1];/*cria um vetor com as tarefas a criar*/
pthread_mutex_t mutex, cond_mutex;/*trinco*/
pthread_cond_t semFilhos, numProcessos;
time_t starttime, endtime;
list_t* list;/*lista que guarda os processos filho*/


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
}
void *tarefaMonitora(){ /*Tarefa responsável por monitorizar os tempos de execução de cada processo filho */

	printf("Tarefa monitora inicializada!\n");

	while (1){
    
		pthread_mutex_lock(&mutex);
	    if (num_filhos==0) {/*nao existem processos filhos*/
	      
	     	if(flag==0){/*verifica se foi acionado o comando exit*/
	        	pthread_mutex_unlock(&mutex); 
	        	pthread_mutex_lock(&cond_mutex);
	        	while (num_filhos==0 && flag==0) pthread_cond_wait(&semFilhos, &cond_mutex); /*bloqueia e fica a espera do signal*/
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

	      		

	         	if(PID<0){/*verifica se houve erro fatal no processo*/
	            	continue;
	          	}

	          	else if(WIFEXITED(status)){ /*verifica se o processo terminou corretamente*/
		            pthread_mutex_lock(&mutex);
		            update_terminated_process(list,PID,WEXITSTATUS(status),time(NULL)); /* guarda o status e o tempo final do processo*/
		            FileManager(PID,get_execution_time(list,PID)); /*escreve no ficheiro */
		            pthread_mutex_unlock(&mutex);
	          	}

		        pthread_mutex_lock(&mutex);
		        num_filhos--;
		        pthread_mutex_unlock(&mutex);
	      		pthread_cond_signal(&numProcessos); /*asinala que terminou um processo */
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
	
	verificaFormato();

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

			    pthread_cond_signal(&semFilhos); /*desbloquia a tarefa monitora podendo assim terminar  */
			    pthread_mutex_unlock(&cond_mutex);
			   
			    pthread_join(tid[0],NULL);/*aguarda que a tarefa monitora termine*/
			    
			    lst_print(list);/*imprime a lista dos processos filho*/
			    lst_destroy(list);/*apaga todos os elementos da lista*/
			    fflush(fp);
			    fclose(fp);
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
		    	while(num_filhos>=MAXPAR) pthread_cond_wait(&numProcessos,&cond_mutex); /*bloqueia quando esta a correr o numero max de processo permitidos*/
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
		  	
		  	pthread_cond_signal(&semFilhos);  /*da signal para a tarefa monitora desbloquear */
		  	
		  	free(argVector[0]);
	    } 

		else{
	  		printf("Por favor insira um argumento válido!\n");
		}
	}
}










