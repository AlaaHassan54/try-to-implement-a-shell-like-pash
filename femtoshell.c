#include<stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define BUF_COUNT 5000



int main(int argc, char **argv)
{

    ssize_t linelen;
    char *buf = NULL;
    size_t bufsize = 0;
    int f=0;
     
    while ((linelen = getline(&buf, &bufsize, stdin)) != -1 ){

	if (linelen > 0 && buf[linelen - 1] == '\n') {
            buf[--linelen] = '\0';
        }

	if (linelen == 0) {
            continue;
        }

	char *token = strtok(buf, " ");

	if (token == NULL)
	    continue;

	if (strncmp("echo\0", token, 5) == 0
	    || strncmp("echo ", token, 5) == 0) {
	    int i = 0;
	    while (token) {

		if (i>1)printf(" ");
		
		if(i)
		  printf("%s", token);

		token = strtok(NULL, " ");
		
		++i;

	    }

	    printf("\n");
	    f=0;

	} else if (strncmp("exit\0", token, 5) == 0
		   || strncmp("exit ", token, 5) == 0) {
               
            char *p= strtok(NULL," ");

	    if(p==NULL)
	    {
	        printf("Good Bye\n");
	        free(buf);

            return f;
	    }
          
	    else 
	    {
	    printf("Invalid command\n");
	     f=1;
	    }
 
	} else {

	    printf("Invalid command\n");
          f=1;      
   
	}



     
    }
    
     free(buf);
     return f;
}

