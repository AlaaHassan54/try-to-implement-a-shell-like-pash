#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#define buf_count 1000


int f=0;

void execute_command(char *line) {
    int argc = 0;
    char *token;
    char *saveptr;

    char *line_copy = strdup(line);  
    token = strtok_r(line_copy, " ", &saveptr);
    while (token != NULL) {
        argc++;
        token = strtok_r(NULL, " ", &saveptr);
    }
    free(line_copy);

     char **argv = (char **)malloc((argc + 1) * sizeof(char *));
    if (!argv) {
        perror("malloc");
        return;
    }

    argc = 0;
    token = strtok_r(line, " ", &saveptr);
    while (token != NULL) {
        argv[argc++] = strdup(token);  
        token = strtok_r(NULL, " ", &saveptr);
    }
    argv[argc] = NULL;  

 
    execvp(argv[0], argv);

    f=1;
    
    printf("IamNotACommand_abcdefxyz: command not found\n");
    //perror("execvp");

    
    for (int i = 0; i < argc; i++) {
        free(argv[i]);
    }
    free(argv);
}

void
echo ()
{
  char *p = strtok (NULL, " ");

  int i = 0;

  while (p != NULL)
    {
      if (i)
	printf (" ");
      printf ("%s", p);
      p = strtok (NULL, " ");
      ++i;
    }

  printf ("\n");

}

void
pwd ()
{

  char path[buf_count];
  printf ("%s\n", getcwd (path, buf_count));

}

void
cd ()
{
  char *path = strtok (NULL, "\0");
  if (chdir (path) < 0)
    {
      printf ("cd: /invalid_directory: No such file or directory\n");
      exit (-1);
    }

}

int
main (int argv , char ** argc)
{
 

  
  ssize_t linelen;
  
    char *buf = NULL;
    size_t bufsize = 0;
  while ((linelen = getline(&buf, &bufsize, stdin)) != -1)
    {
	    

       if (linelen > 0 && buf[linelen - 1] == '\n') {
            buf[--linelen] = '\0';
        }

        if (linelen == 0) continue;
        
         char *buf_copy = strdup(buf);

      char *token = strtok (buf, " ");

      if (token == NULL)
	continue;

      if (strcmp (token, "echo") == 0)
	echo ();


      else if (strcmp (token, "pwd") == 0)
	pwd ();

      else if (strcmp (token, "cd") == 0)
	cd ();


      else if (strcmp (token, "exit") == 0)
        {
    

	  printf ("Good Bye\n");
	  free(buf);
      free(buf_copy);
	  return f;

	}
      else
	{
           size_t pid = fork();

	   if(pid>0)
	   {
	    int status;
	    wait(&status);
        f = WIFEXITED(status) ? WEXITSTATUS(status) : 1;	    
	   }
	   else if(pid==0)
	   {
	    
        execute_command(buf_copy);
        free(buf_copy);

             	    
	   }
	   else
	   {
	   
	    printf("failed to fork\n");
	    
	     exit(-1);
	   }

	}



    }

return f;
}

