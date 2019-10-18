#define _POSIX_SOURCE		// for ERROR: " implicit declaration of function ‘kill’ "

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
#include "md5.h"

int isOk = -1;
int mdLength = -1;

void
checkMD5 (int s)
{

  // printf ("signal function's \n");

  if (mdLength == 32)
    {				// 0 is success
      isOk = 0;
    }
  else
    {
      isOk = 1;
    }
}

int main ()
{
  int pipe_PS[2];		// pipe parent to child move the original string
  int pipe_SP[2];		// pipe child to parent move the MD5 string to parent
  char buffer[BUFSIZ];		// receive the final string after MD5
  signal (SIGINT, checkMD5);	// change the handler function for sigint 

  if (pipe (pipe_PS) == 0 && pipe (pipe_SP) == 0)
    {				// success
      pid_t pid = fork ();
      if (pid < 0)
	{
	  perror ("Child not created");
	  exit (EXIT_FAILURE);
	}
      else if (pid) // Parent
	{

	  // printf ("Parent-");
	  char data[20];
	  printf ("plain text: ");
	  scanf ("%s", data);

	  if (strlen (data) > 20)
	    {
	      // printf ("The string is too long \n");
	      exit (EXIT_FAILURE);
	    }
	  else // the string is OK
	    {
	      int count_wParent = write (pipe_PS[1], data, strlen (data));
	      // printf ("Parent sent the first msg: %s \n", data);
	      int count_rParent = read (pipe_SP[0], buffer, BUFSIZ);	// waiting the child to write
	      mdLength = strlen (buffer);	// update the size of buffer received
	      // printf ("Parent received the second msg %s \n", buffer);
	      pause ();		// send the process to sleep untill get signal
	      // printf ("Parent is after paused \n");

	      if (isOk == 0) // the string is OK
		{		

		  //cout<< buffer<<ednl; 
		  // printf ("The answer is: %s \n", buffer);
		  kill (pid, SIGQUIT);	// kill the child
		}
	      else
		{
		  // printf ("Md5 failed (length is not 32)");
		}
	    }
	}
      else // child
	{
	  char MD_BUF[BUFSIZ];
	  int count_rChild = read (pipe_PS[0], MD_BUF, BUFSIZ);
	  // printf ("Child received the first msg %s \n", MD_BUF);
	  const char *answer = (md5 (MD_BUF)).c_str ();	// using md5 on the string
	  printf ("encrypted by process %d : %s \n",getpid(),answer);
	  mdLength = strlen (answer);	// update the size of MD_BUF (strlen(answer)
	  int count_wChild = write (pipe_SP[1], answer, mdLength);
	  // printf ("Child sent the second msg %s \n", answer);
	  sleep (1);		// make sure that parent got the msg
	  pid_t parent_PID = getppid ();	// get the parent id in order to send signal
	  kill (parent_PID, SIGINT);	// send signal that use our funciton checkMD5
	  // printf ("Child sent signal to his parent \n");
	  //  pause(); // wait for parent kill me 
	}
    }
}
