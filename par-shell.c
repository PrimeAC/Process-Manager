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
#include <pthread.h>

char **argVector;
int PID, TID, num_filhos=0, status=0, i, flag=0;
pthread_t tid[1];
pthread_mutex_t mutex;
time_t starttime, endtime;
list_t* list;


void *tarefaMonitora(){  


  while (1){
    if (num_filhos==0 && flag==0) {/*nao tem filhos e nao quer sair*/
      
      sleep(1);
    }
    if (num_filhos==0 && flag==1){/*nao tem filhos mas foi acionado o exit*/
      pthread_exit(0);
    }


    else{
      for(;num_filhos>0;num_filhos--){/*nenhum dos casos acima mencionados*/
         
          PID=wait(&status);/*aguarda que os processos filhos terminem*/ 

          if(PID<0){/*verifica se houve erro no processo*/
            continue;
          }

          else if(WIFEXITED(status)){ /*verifica se o processo terminou corretamente*/
            printf("PID a terminar:%d\n",PID );
            update_terminated_process(list,PID,WEXITSTATUS(status),time(NULL)); /* UPDATE DO STATUS E DO ENDTIME*/
          
          }
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
  pthread_mutex_init(&mutex,NULL);

  TID = pthread_create(&tid[1] ,0,tarefaMonitora,0);

  if (TID!=0){
    perror("");
  }

  while(1){


    if(readLineArguments(argVector, NARGUMENTOS)>0){ /*verifica se o utilizador escreveu algo */
      
      if(strcmp(argVector[0], "exit")==0){

        flag=1;
        
        pthread_join(tid[1],NULL);
        printf("num_filhos:%d\n",num_filhos );
       
        lst_print(list);/*imprime a lista dos processos filho*/
        lst_destroy(list);/*apaga todos os elementos da lista*/
        free(argVector[0]);
        free(argVector);
        pthread_mutex_destroy(&mutex);
        exit(EXIT_SUCCESS);/*termina o processo pai*/

      }

      else {
        
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
        /*lista do lab 1 - mudar o nome*/
        printf("num_filhos:%d\n",num_filhos );
        num_filhos++;
        printf("PID a inserir:%d\n",PID );
        insert_new_process(list, PID,0, time(NULL));
        printf("num_filhos:%d\n",num_filhos );
        
        //free(argVector[0]);
        
    }
    else{
      printf("Por favor insira um argumento válido!\n");
    }
  }
  

}










