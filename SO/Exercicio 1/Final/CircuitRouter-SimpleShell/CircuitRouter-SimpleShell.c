#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include "../lib/commandlinereader.h"

#define SIZE 500
#define INVALIDCMD "Invalid command.\n"

pid_t pid[SIZE];
int max_children = 0;
int child_count = 0;
int vectorSize = SIZE;
int bufferSize = SIZE;
int argumentSize = 0;
char *argVector[SIZE];
char buffer[SIZE];
int status[SIZE];
int i, aux = 0;

int runExec()
{
	pid_t pidn = fork();

	if(pidn == -1) 
	{
		printf("Error creating child proccess: %s\n", strerror(errno));	
		return -1;
	}					
	else if(pidn == 0) 
	{	
		FILE* fp = NULL;
		if( (fp = fopen(argVector[1], "r")) != NULL)
		{
			char* args[3] = {"CircuitRouter-SeqSolver", argVector[1], NULL};
			if( (execv("../CircuitRouter-SeqSolver/CircuitRouter-SeqSolver", (char* const*) args)) == -1)
			{
				 printf("Failed during execv: %s\n", strerror(errno));
				 return -1;
			}
		}
		else
		{
			printf("Input file path is invalid: %s\n", strerror(errno));
		}

		if(fclose(fp) == EOF)
		{
			printf("Failed to close file stream: %s\n", strerror(errno));
		}
	}
	else
	{
		//printf("Fork() -- PID: %d\n", pidn);
		child_count++;	
	}
	return 0;
}

void parseStatus()
{
	if(WIFEXITED(status[aux]))
	{
		status[aux] = WEXITSTATUS(status[aux]);
	}
	else if(WIFSIGNALED(status[aux]))
	{
		status[aux] = WTERMSIG(status[aux]); 
	}
}

int main(int argc, char *argv[]) 
{	
	if(argc > 2)
	{
		printf("Invalid start parameters, no proccess limit set.\n");
	}
	else if(argc == 1)
	{
		printf("No input ... Shell started with no proccess limit.\n");
	}
	else
	{
		max_children = atoi(argv[1]);
		if(max_children == 0)
		{
			printf("Start parameter was zero or invalid ... Shell started with no proccess limit.\n");
		}
		else
		{
			printf("Shell started with proccess limit: %d\n", max_children);
		}
	}

	for(;;)
	{					
		argumentSize = readLineArguments(argVector, vectorSize, buffer, bufferSize);
		if(argumentSize == -1)
		{
			printf("Error reading from command line.\n");
			exit(1);
		}
		else if(argumentSize == 0)
		{
			continue;
		}
		else if(argumentSize > 2)
		{
			printf(INVALIDCMD);
		}
		else
			if(strcmp(argVector[0], "run") == 0 && argumentSize == 2)
			{
				int ret = 0;

				if(child_count < max_children || max_children == 0)
				{
					ret = runExec();
					if(ret == -1)
					{
						continue;
					}
				}	
				else 
				{
					//printf("Proccess limit reached, waiting for a free slot...\n");
					pid[aux] = wait(&status[aux]);
					//printf("OK - CHILD EXITED (PID: %d) - RUNNING NEW PROCCESS...\n", pid[aux]);

					parseStatus();
					//printf("PID: %d --- Status: %d\n", pid[aux], status[aux]);
					child_count--;
					aux++;

					ret = runExec();
					if(ret == -1)
					{
						continue;
					}
				}
			}
			else if(strcmp(argVector[0], "exit") == 0)
			{
				while(child_count != 0)
				{
					pid[aux] = wait(&status[aux]);

					parseStatus();

					child_count--;
					aux++;
				}
				for(i = 0; i < aux; i++)
				{
					if(status[i] == 0)
					{
						printf("CHILD EXITED (%d; RETURN OK)\n", pid[i]);
					}
					else
					{
						printf("CHILD EXITED (%d; RETURN NOK)\n", pid[i]);
					}
					//printf("CHILD EXITED (%d; RETURN %d)\n", pid[i], status[i]);
				}
				printf("END.\n");
				break;
			}
			else
				printf(INVALIDCMD);
	}

	exit(0);
}

