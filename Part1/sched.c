/* CS3790 Assignment 1 Part 1

Ruben Loewen 5010926

Simple scheduling prototype, starts 3 child processes 
that print out an integer every time they execute

*/


#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

int current = 0;
enum status { on, off};
typedef enum status status;


unsigned  i = 0;
status flag = on;
int pid[3] = {0};                           // create array of 3 children

struct sigaction newhandler, oldhandler;
sigset_t sig;

void next( int code)
{
      kill(pid[current],SIGSTOP);		    // stop current process 
      current++;				               // increment to next process
      if (current == 3) current = 0;  // reset the current process index 
 
     kill(pid[current],SIGCONT);		// stop the next process
}

void main()
  {
    

    
    
    pid[0] = fork();            // create child  1
    if ( pid[0] == 0 )
        {
          while (1) putc('1',stderr);
        }
    else {
	pid[1] = fork(); 	           // create child  2
	if (pid[1] == 0)
	{
	  while(1) putc('2',stderr);
	}else
	{
		pid[2] = fork();	         // create child  3
		if (pid[2] == 0){
		  while(1) putc('3',stderr);
		}
		else{
 	sigemptyset(&sig);  /* clear the signal mask */
        newhandler.sa_handler = next;
        newhandler.sa_mask = sig;
        newhandler.sa_flags = 0;

        if ( sigaction(SIGALRM,&newhandler,&oldhandler) == -1 )
          {
            printf("1 - can't install signal handler \n");
            exit(-1);
          }
        while (1) {
          alarm(3);
          pause();

		}
	}
        	
        }
     }
}