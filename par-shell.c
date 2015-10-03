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
#include <unistd.h>
#include <time.h>
#include "commandlinereader.h"

/*
Main Program
*/


int main(int argc, char* argv[]){
  char **argVector;
  int PID, status, vectorSize;

  argVector = (char**) malloc(7*sizeof(char*));
  
  while(1){
    readLineArguments(argVector, 7);
    printf("%s\n",argVector[0] );

    if(strcmp(argVector[0], "exit")==0){
      printf("exit\n");

      /*exit routine wait exit*/
      
    }

    else {
      
      PID=fork();
      
      if(PID<0){
        printf("ERRO\n");
        exit(1);

      }

      if(PID==0){
        printf("%d\n",getpid());
        printf("Filho com PID %d\n",PID);
        if(execl(argVector[0],argVector[1], argVector[2], argVector[3], argVector[4], argVector[5], argVector[6], NULL)<0){
        	printf("O exec falhou\n");
        	exit(1);
       
        }
      }

      else{
        printf("Pai com PID %d\n",PID);
        printf("%d\n",getpid());
      }
      /*pathname routine fork exec*/
    }

    
    
  }
}

  /*PID=fork();

  if (PID<0){
    printf("ERRO\n");
  }

  if (PID==0){
    printf("%d\n",getpid());
    execl("/bin/SO/SO-ex1-enunciado-v2", "fibonacci.c", 0);
    printf("Filho com PID %d\n",PID);
    exit(0);
  }
  else{

    printf("Pai com PID %d\n",PID);
    PID=wait(&status);
    printf("%d\n",getpid());
  }

printf("END OF PROGRAM\n");
return 0;

}*/









