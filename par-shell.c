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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "commandlinereader.h"
#include "list.h"
#include <sys/wait.h>
/*
Main Program
*/


int main(int argc, char* argv[]){

  char **argVector;
  int PID, num_filhos=0, STATUS=0, i;

  list_t* list = lst_new();

  argVector = (char**) malloc(7*sizeof(char*));
  
  while(1){
    readLineArguments(argVector, 7);
  

    if(strcmp(argVector[0], "exit")==0){

      lst_print(list);

      for(i=0; i<=num_filhos; i++){

        wait(&STATUS);
        printf("%d\n",WEXITSTATUS(STATUS));
        num_filhos--;
      }

      printf("numero de filhos no fim:%d\n",num_filhos);

      exit(0);

      /*exit routine wait exit*/
      
    }

    else {
      
      PID=fork();
      
      if(PID<0){

        perror("Erro no fork()\n");

        exit(1);

      }

      if(PID==0){
        /* filho*/
        /*printf("Passei pelo insert new processes com o PID:%d\n",getpid());

        printf("Pai:%d\n",getppid());*/
         
        printf("num de filhos:%d\n",num_filhos+1);
        
        if(execv(argVector[0],argVector)<0){

        	perror("Executavel nao encontrado!\t");

        	exit(-1);
       
        }
      }
      else if(wait(&STATUS) != PID){ /*nao esta a chegar aqui*/

        printf("um sinal interrompeu o wait\n");
      }        
      

      else{
        /* pai*/
        wait(&STATUS);
        
      }
      /*pathname routine fork exec*/
      wait(&STATUS);
      insert_new_process(list,PID,STATUS);
      num_filhos++;
    }  
  }
}










