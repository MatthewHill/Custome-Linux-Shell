#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <signal.h>
#define MAX_LINE 80 /* The maximum length command */
int commandCount = 0;
int concurrency = 0;
int i;
char input[MAX_LINE];
char hold[MAX_LINE];
char commandHistory[10][80];
int should_run =1; /* flag to determine when to exit program */
int redirect(char *argv1[], char argv2[]) /*redirect function */
{
	close(0);
	fopen(argv2,"r");
	if(fork() == 0)
	{
			execvp(argv1[0],argv1);
	}
	return 0;
}
int piper(char *argv1[],char *argv2[]) /*pipe function */
{
	pid_t pid1, pid2;
	int pipefd[2];
	pipe(pipefd);
	pid1 = fork();
	if(pid1 ==0) /* Child */
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		execvp(argv1[0],argv1);
		return 1;
	}
	if(pid2 == 0)
	{
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[1]);
		execvp(argv2[0],argv2);
		return 1;
	}
	close(pipefd[0]);
   close(pipefd[1]);
	return 0;
}
int execute(char *args[])
	{
				pid_t pid;
				if((pid = fork()) < 0) /* Fork to child process */
				{
					printf("failed forking child process");
				}
				else if (pid == 0)
				{
						printf("Running(Pid: %d)",getppid());
						int j =0;
						while(args[j])
							{
							printf(" %s",args[j]);
							j++;
							}
							printf("\n");
						if(concurrency ==1)
						{
							//char tempArray[MAX_LINE];
							//strncpy(tempArray,args[0],strlen(args[0])-1);
							//strncpy(args[0],tempArray,strlen(args[0]));
							//printf("Command conncurrent %s",args[0]);
							//j = 1;
							//while(args[j])
							//{
							//printf(" %s",args[j]);
							//j++;
							//}
							//printf("\n");
							args[j-1] = NULL;
						//	printf("Args[1] = %s\n",args[1]);
						}
						if ( execvp(args[0], args) < 0 ) /* Execute command */
						{
							printf("failed to execute child ");
							printf("command %s\n",args[0]);
							should_run = 0;
							kill(-1,0);
						}
						else
						{
						commandCount++; /* Increment the command counter when a command is completed successfully */
						strcpy(commandHistory[commandCount], args[0]);
						printf("Child complete");
						should_run = 0;
						kill(0,SIGKILL);
						}
				}
				else if (pid != 0) /*Parent process */
				{
					if (concurrency == 0)
					{
						   int returnStatus;    
							waitpid(pid, &returnStatus, 0);  // Parent process waits here for child to terminate.

							if (returnStatus == 0)  // Verify child process terminated without error.  
							{
							   printf("The child process  %d terminated normally.\n",pid);    
							}

							if (returnStatus == 1)      
							{
							   printf("The child process terminated with an error!.\n");    
							}
					}
					else
					{
						printf("Running background Process\n");
					}
				} 
				return 0;
	}
int main(void)
{
	
	char *args[MAX_LINE/2 + 2]; /* command line arguments */
	int  histInt[10];	/* Maximum commands in history */
	 i = 0;
	while(should_run =1) 
	{
			printf("Hill_osh>");
			fflush(stdout);
			if(fgets(input,MAX_LINE,stdin) == NULL)
			{
				printf("Error retreiving command");
			}
	if(input[0] != '\n') /*Check for no entry */
	{
			i = 0;
			
		char hold[MAX_LINE];
		strncpy(hold, input,MAX_LINE);
		//printf("print hold: %s", hold);

			concurrency = 0;
			args[i] = strtok(input, " \n");
			if(args[i] == NULL)
			{
			printf("Tokenizing Error");
			}
			while (args[i] != NULL)
			{
				i++;
				args[i] = strtok(NULL, " \n");
			}
			//strcpy(args, args[0]);
			//printf("Test");
			//printf(args[i-1]);
			//printf("Value of concurrencty %d\n",concurrency);
			//char* pPosition;
			//printf("Args[i-1] = %s\n",args[i-1]);
			//printf("Args : %s\n",args[0]);
			//if (pPosition = strchr(args[i-1], '&'))  /* Checks to see if child should be run concurrently */
		for(int i=0; args[i] != NULL; i++)
			{
			if(strcmp(args[i],"<") == 0)
				{
					printf("Redirect called\n");
					char arg1[MAX_LINE];
					for(int j=0;j<i;j++)
						{
						strcpy(arg1, args[j]);
						printf("copy");
						}
					printf("Args[0] : ");
					printf(args[0]);
					printf("Arg1[0] : ");
					printf(arg1);
					char *arg2[MAX_LINE];
				}
			}
		for(int i=0; args[i] != NULL; i++)
			{
			if(strcmp(args[i],"|") == 0)
				{
					printf("Pipe called\n");
					char arg1[MAX_LINE];
					for(int j=0;j<i;j++)
						{
						strcpy(arg1, args[j]);
						printf("copy");
						}
					printf("Args[0] : ");
					printf(args[0]);
					printf("Arg1[0] : ");
					printf(arg1);
					char *arg2[MAX_LINE];
				}
			}
		if(strcmp(args[i-1],"&")==0)
			{
				concurrency = 1;
				printf("Command is to be run  concurrently\n");
			}
			//printf("Value of concurrencty %d\n",concurrency);
		if (strcmp(args[0], "exit") == 0) /* Checks to see if the program should exit */
			{
				printf("Program exiting\n");
				should_run = 0;
				kill(0,SIGKILL);
				break;
			}
		else if(strcmp(args[0], "history") == 0) /* Checks to see if history is called */
			{
				if(commandCount == 0)
				{
					printf("No commands in history\n");
				}
				else
				{
					int tempHistory = commandCount;
					while(tempHistory > (commandCount - 10))
					{
						if(tempHistory > 0)
						{
						printf("%d " , tempHistory); 
						printf(commandHistory[tempHistory%10]);
						}
						tempHistory--;
					}
				}
			}
			else if(hold[0] == '!' && hold[1] != '!') /* Checks to see if history is called for nth diget, but not !! */
			{
			args[0] +=1;
			int value = atoi(args[0]);
			//printf("%d\n",value);
			char temp[MAX_LINE];
			strcpy(temp, commandHistory[value%10]);
			i=0;
			args[i] = strtok(temp, " \n");
			while (args[i] !=NULL)
			{
					i++;
					args[i] = strtok(NULL, " \n");
			}
			if(strcmp(args[i-1],"&")==0)
				{
					concurrency = 1;
					printf("Command is to be run  concurrently\n");
				}
			if(execute(args) < 0)
				{
					printf("Execution error");
				}	
			//printf("history n\n");	
			}
			else if(strcmp(args[0], "!!") == 0) /* Checks to see if history is called */
			{
				printf("history last\n");
				char temp[MAX_LINE];
				strcpy(temp, commandHistory[commandCount%10]);
				i=0;
				args[i] = strtok(temp, " \n");
				while (args[i] !=NULL)
				{
						i++;
						args[i] = strtok(NULL, " \n");
				}
				if(strcmp(args[i-1],"&")==0)
				{
					concurrency = 1;
					printf("Command is to be run  concurrently\n");
				}
				if(execute(args) < 0)
					{
						printf("Execution error");
					}
						
			}
			else /* Executes regularly if no extra parameters are chosen */
			{
				if(execute(args) < 0)
				{
					printf("Execution error");
				}
				commandCount++; /* Increment the command counter when a command is completed successfully */
		strcpy(commandHistory[commandCount%10], hold);
			}
	}
	}	
	return 0;
}