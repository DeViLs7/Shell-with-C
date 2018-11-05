#include<stdio.h> 
#include<string.h> 
#include<iostream>
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<sys/wait.h> 
#include<readline/readline.h> 
#include<readline/history.h> 
#define NOP 100 //define maximum number of piping
#define NOA 100 //define maximum number of operations
using namespace std;
void printPrompt()
{
     printf("Devils#\n");
}
void clearScreen()
{
     printf("\033[H\033[J");
}
void initScreen()
{
     clearScreen();
     int i ;
     for (i = 0; i < 20; ++i)
	 printf("\n");
     printf("\t\t\t\t\tHello \n\t\t\t\t\tDevils came");
     for (i = 0; i < 20; ++i)
	 printf("\n");
     sleep(2);
     clearScreen();
}
int getInput(char* line)
{
    char * tmp;
    tmp = readline("Devils#");
    if (strlen(tmp) != 0)
    {
       add_history(tmp);
       strcpy(line, tmp);
       return 1;
    }
    return 0;
}
void sepLine(char* cmd, char** arg)
{
     int i;
     char** tmp;
     cout << "Good\n";
     for (i = 0; !(i != 0 && arg[i - 1] == NULL); ++i)
     {
        cout << "Dlne\n";
        arg[i] = strsep(&cmd, " ");
        cout <<"It's " << arg[i] << "\n"; 
     }
      printf("\n");
}

int checkPipe(char* line, char** cmd, char*** arg, int* pipes)
{
    int i;
    cout << "hi\n";
    for (i = 0; !(i != 0 && cmd[i - 1] == NULL); ++i)
    {
        cmd[i] = strsep(&line, "|");
        cout << cmd[i] << "\n";
    }
    *(pipes) = i - 2;
    for (i = 0; i <= *(pipes); ++i)
    {
        int j;
       /* for (j = 0; !(j !=0 && arg[i][j - 1] == NULL); ++j)
        {
            arg[i][j] = strsep(&cmd[i], " ");
            printf("arg %d j= %s\n", j,  arg[i][j]);
        }*/
        sepLine(cmd[i], arg[i]);
       // printf("%s\n", arg[i][0]);
    }
    return *(pipes) != 0;
}
void execPipe(char*** arg, int pipes)
{
     int p[2];
     int std_in = 0;
     pid_t pid;
     while (*arg != NULL)
     {
           pipe(p);
           if ((pid = fork()) == -1)
           {
              printf("Error\n");
              exit(1);
           }
           else if (pid == 0)
           {
              dup2(std_in, 0);
              if (*(arg + 1) != NULL)
                 dup2(p[1], 1);
              close(p[0]);
              execvp((*arg)[0], *arg);
              exit(1);
           }
           else
           {
              wait(NULL);
              close(p[1]);
              std_in = p[0];
              arg++;              
           }
     }
}
void pipeline(char ***cmd)
{
	int fd[2];
	pid_t pid;
	int fdd = 0;				/* Backup */

	while (*cmd != NULL) {
		pipe(fd);				/* Sharing bidiflow */
		if ((pid = fork()) == -1) {
			perror("fork");
			exit(1);
		}
		else if (pid == 0) {
			dup2(fdd, 0);
			if (*(cmd + 1) != NULL) {
				dup2(fd[1], 1);
			}
			close(fd[0]);
			execvp((*cmd)[0], *cmd);
			exit(1);
		}
		else {
			wait(NULL); 		/* Collect childs */
			close(fd[1]);
			fdd = fd[0];
			cmd++;
		}
	}
}

void execNormal(char** arg)
{ 
     pid_t pid = fork();
     if (pid < 0)
        printf("Can't build child\n");
     else if (pid == 0) 
     {
        execvp(arg[0], arg);
        exit(0);
     }
     else
        wait(NULL);
}

int main()
{
    char *line, *cmd[NOP], **arg[NOA];
    initScreen();
    line = (char*) malloc(1000 * sizeof(char));
    cmd[NOP] = (char *) malloc(1000 * sizeof(char));
    arg[NOA] = (char **) malloc(1000000 * sizeof(char));
    int flag;
    int pipes = 0;
    while (1)
    {
         printf("Hello\n");
         pipes = 0;
         if (getInput(line))
	 {
	    if (checkPipe(line, cmd, arg, &pipes))
            {
	      // execPipe(arg, pipes);
               /*char *ls[] = {"ls", NULL};
               char *grep[] = {"grep", "pipe", NULL};
               char *wc[] = {"wc", NULL};
               char ** arg1[] = {ls, grep, wc, NULL};*/
              // pipeline(arg1);
            }
            else
            {
               cout << "done\n";
	       execNormal(arg[0]);
            }
         }
    }
    return 0;
}






