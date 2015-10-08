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
/*
Main Program
*/


int main(int argc, char* argv[]){

  char **argVector;
  int PID, num_filhos=0, status=0, i;

  list_t* list = lst_new();

  argVector = (char**) malloc(NARGUMENTOS*sizeof(char*));
  
  while(1){
    if(readLineArguments(argVector, NARGUMENTOS)>0){
    
      if(strcmp(argVector[0], "exit")==0){

        lst_print(list);

        for(i=0; i<=num_filhos+1; i++){

          wait(&status);
          num_filhos--;

        }
        lst_destroy(list);
        free(argVector[0]);
        free(argVector);
        exit(0);

        /*exit routine wait exit*/
        
      }

      else {
        
        PID=fork();
        if(PID<0){

          perror("");

          exit(EXIT_SUCCESS);

        }

        if(PID==0){
          /* filho*/
          
          if(execv(argVector[0],argVector)<0){

          	perror("");
            free(argVector[0]);
            free(argVector);

          	exit(EXIT_FAILURE);
         
          }
        }
        else if(wait(&status) != PID){ /*nao esta a chegar aqui*/

          perror("um sinal interrompeu o wait\t");
        }        
        

        else{
          /* pai*/
          wait(&status);
          
        }
        /*pathname routine fork exec*/
        if(WIFEXITED(status)==1){ 
          insert_new_process(list,PID,WEXITSTATUS(status));
          num_filhos++;
        }
        free(argVector[0]);
      }  
    }
    else{
      printf("Por favor insira um argumento válido!\n");
    }
  }
  

}










