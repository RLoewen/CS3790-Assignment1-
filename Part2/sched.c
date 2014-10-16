/*
CS3790 Assignment 1 Part 2

Ruben Loewen 5010926

This program allows the user to enter the name of a program in the run/ directory 
of this folder, if multiple programs are executed the scheduler allows each a 3 second 
operating time before switching to the next program process
*/




#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

enum status { on, off};
typedef enum status status;

enum boolean { false, true };
typedef enum boolean boolean;

unsigned  short i = 0;
int pid[3] = {0};		// create an array for three processes
int currentProc = 0;		// init index for pid array, tracks current pid
status flag = on;

struct sigaction newhandler, oldhandler;
sigset_t sig;

void setblock( int fd ,boolean block)
{
   static int blockf, nonblockf;
   static boolean first = true;

   int flags;

   if (first) {
      first = false;
      if (( flags = fcntl(fd,F_GETFL,0)) == -1)
      {
         fprintf(stderr," fcntl - error \n");
         exit(1);
      }
     blockf = flags & ~ O_NDELAY;
     nonblockf = flags | O_NDELAY;
  }
  if ( fcntl(fd,F_SETFL, block ? blockf : nonblockf) == -1 )
      {
         fprintf(stderr," fcntl2 - error \n");
         exit(1);
      }
}
void next( int code)
{
  
	
  	if ( 0 == pid[currentProc]) return;	        // if in the child return to parent process
  	
	kill(pid[currentProc],SIGSTOP);		          // stop current process
	currentProc++;				                     // increment current process pointer
     	if (currentProc == 3) currentProc =0;		// wraps the array around to the 0th process if the index is 3
	kill(pid[currentProc],SIGCONT);		       // start the next process
	
}

void main(int argc, char *argv[])
   {
    char mess[80];
    int fd;
    int numch;
    char parameter[32] = "run/";
    int parameterLen = 4;   
     { 

        fd = open("/dev/tty",O_RDWR);
        setblock(fd,false);

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
          switch  ( numch = read(fd,mess,80))  {
          case -1 :
          case 0 :
              break;
          default: 
	      fprintf(stderr," %d   <%s>\n",numch,mess);
              mess[numch-1] = '\0';
    	      pid[currentProc] = fork();
              for( i=0; i< numch;i++) parameter[parameterLen+i] = mess[i]; // combine the users arguement with the path of exec
	      fprintf(stderr," %d   <%s>   <%s>\n",numch,mess,parameter);
              
    	      if ( pid[currentProc] == 0 )			//if in the child process execute the program indicated by user input
        		{		
			execl(parameter, mess, NULL);
          		}
          fprintf(stderr," in parent\n");
        }
     }
  }
}
