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

  list_t* list = lst_new(); /*cria uma lista onde é guardado o pid e o status dos processos*/

  argVector = (char**) malloc(NARGUMENTOS*sizeof(char*));
  
  while(1){
    if(readLineArguments(argVector, NARGUMENTOS)>0){ /*verifica se o utilizador escreveu algo */
      
      if(strcmp(argVector[0], "exit")==0){
        
        lst_print(list);
       
        for(i=0; i<num_filhos; i++){
         
          wait(&status);/*aguarda que os processos filhos terminem*/ 

        }
        
        lst_destroy(list);/*apaga todos os eelementos da lista*/
        free(argVector[0]);
        free(argVector);
        exit(EXIT_SUCCESS);/*termina o processo pai*/

      }

      else {
        
        PID=fork();/*guarda na variavel pid o resultado da funçao fork*/
        
        if(PID<0){  /*caso ocorra erro na criacao do processo filho*/
          perror("");
          exit(EXIT_SUCCESS);

        }

        else if(PID==0){/*processo filho*/
          
          if(execv(argVector[0],argVector)<0){ /*verifica se ocorreu um erro a correr o executavel*/

          	perror("");
            free(argVector[0]);
            free(argVector);
            exit(EXIT_FAILURE);
         
          }
        }
        else if(wait(&status) != PID){ 

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










