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
#define FAILURE -1
#define SUCCESS 0
#define NARGUMENTOS 7

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "commandlinereader.h"
#include "list.h"
#include <sys/wait.h>

void tarefaMonitora(){  

  while (1){
    if (num_filhos==0) 

      sleep(1);
    else
      wait();
      update_terminated_process(list, PID, endtime)

}

/*
Main Program
*/
int main(int argc, char* argv[]){

  char **argVector;
  int PID, num_filhos=0, status=0, i, 
  pthread_t = tid[1]
  time_t 
  list_t* list = lst_new(); /*cria uma lista onde é guardado o PID e o status dos processos*/
  int 

  argVector = (char**) malloc(NARGUMENTOS*sizeof(char*));


  while(1){

    err = pthread_create(tid,0,tarefaMonitora,0)

    if(readLineArguments(argVector, NARGUMENTOS)>0){ /*verifica se o utilizador escreveu algo */
      
      if(strcmp(argVector[0], "exit")==0){
        
        pthread_join(tid[1],NULL)
       
        for(;num_filhos>0;num_filhos--){
         
          PID=wait(&status);/*aguarda que os processos filhos terminem*/ 

          if(PID<0){/*verifica se houve erro no processo*/
            continue;
          }

          if(WIFEXITED(status)){ /*verifica se o processo terminou corretamente*/
            insert_new_process(list,PID,WEXITSTATUS(status)); /*guarda o PID do processo e o status correspondente*/
          
          }
        }

        lst_print(list);/*imprime a lista dos processos filho*/
        lst_destroy(list);/*apaga todos os elementos da lista*/
        free(argVector[0]);
        free(argVector);
        exit(EXIT_SUCCESS);/*termina o processo pai*/

      }

      else {
        
        PID=fork();/*guarda na variavel PID o resultado da funçao fork*/

        insert_new_process(list_t *list, int PID, time_t starttime)
        
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
        insert_new_process(list, PID, tstarttime)/*lista do lab 1 - mudar o nome*/
        num_filhos++;
        
        free(argVector[0]);
      }  
    }
    else{
      printf("Por favor insira um argumento válido!\n");
    }
  }
  

}










