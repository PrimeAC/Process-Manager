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

/*
Main Program
*/


int main(int argc, char* argv[]){
  char **argVector;
  
  int PID, num_filhos=0,status;

  argVector = (char**) malloc(7*sizeof(char*));
  
  while(1){
    readLineArguments(argVector, 7);
  

    if(strcmp(argVector[0], "exit")==0){
      exit(1);

      /*exit routine wait exit*/
      
    }

    else {
      
      printf("num de abortos falhados:%d\n",num_filhos );
      PID=fork();
      
      if(PID<0){
        perror("ERRO\n");
        exit(1);

      }

      if(PID==0){
        /* filho*/
        printf("pid do aborto falhado:%d\n",getpid());
        printf("pid do pai:%d\n",getppid());
       
        num_filhos++;
        printf("num de filhotes:%d\n",num_filhos);
        if(execv(argVector[0],argVector)<0){
        	printf("O exec falhou\n");
        	exit(1);
       
        }
        else{
          exit(status);
          num_filhos--;
        }
      }

      else{
        /* pai*/
        
        printf("pid do papai:%d\n",getpid());
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









