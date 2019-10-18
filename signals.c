#define _POSIX_SOURCE           // for ERROR: " implicit declaration of function ג€˜killג€™ "

#include <sys/wait.h> // wait
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/fcntl.h>
#include <stdbool.h>

int pidNum[6] = { 0 }; // array of pid numbers for the parent

void sigCathcher(int s) // signal handler for the signal SIGINT
{
      if(getpid()!=pidNum[0]){ // while it is no the parent
      printf("PID %d caught one \n", getpid());
      kill(getpid()-1,SIGINT); // send signal to sibling
   }else{
       kill(getpid(),SIGHUP); // send signal to parent in order to tell that all children finish
   }
}

void emptyFuncforKill(){}

int main()
{
    
   signal(SIGINT, sigCathcher); // change the handler function for sigint
   signal(SIGHUP,emptyFuncforKill);
   pidNum[0] = getpid();
   int a;
   for (int i = 1; i < 6; i++)  // loop will run 5 times (n=6) 
   {
      if (getpid() == pidNum[0]) { // parent

         a = fork(); // a will get the pid, if 0 it is the child process else its the parent that get the child's pid
         if (a == 0) // child
         {
            printf("PID %d ready \n",getpid());
            pause(); // waiting for signal from sibling
            exit(2);
         } else {    // we are in parent process so a will be the child pid
            pidNum[i] = a;

            if (getpid() == pidNum[0] && i == 5) { // parent
               sleep(2); // wait for creation of all children  
               kill(pidNum[5], SIGINT); // send signal to the last child in order to start the chain massage
               pause(); // wait for all children to finish in order to collect them 

            // collect the childern (kill alll the zombies)
            for(int i=1;i<6;i++){
                
                a= waitpid(pidNum[i],NULL,0);
              printf("Process %d is dead \n",a);
            }
           }
         }
      }
   }
}
