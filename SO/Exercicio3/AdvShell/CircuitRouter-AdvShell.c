
/*
// Projeto SO - exercise 1, version 1
// Sistemas Operativos, DEI/IST/ULisboa 2018-19
*/

#include "lib/commandlinereader.h"
#include "lib/vector.h"
#include "CircuitRouter-AdvShell.h"
#include "lib/timer.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>

#define COMMAND_EXIT "exit"
#define COMMAND_RUN "run"
#define RUN_SUCCESS "Circuit Solved\n"
#define INVALID_COMMAND "Command not supported.\n"

#define MAXARGS 3
#define BUFFER_SIZE 100
#define WAITERROR -1


int runningChildren = 0;
vector_t *children;
TIMER_T startTime;
TIMER_T stopTime;
int error_flag;

void checkErrors()
{
    if(error_flag == WAITERROR)
    {
        perror("Unexpected error while waiting for child.");
        exit(EXIT_FAILURE);
    }
    return;
}

void waitForChild(int signo) {
    TIMER_READ(stopTime);
    runningChildren--;

    int status;
    int pid = wait(&status);

    if(pid < 0) 
        error_flag = WAITERROR;
    for(int i = 0; i < vector_getSize(children); i++)
    {
        child_t *child = vector_at(children, i);
        if(child->pid == pid)
        {
            child->status = status;
            child->stop = stopTime;
            break;
        }
    }
    return;
}

void printChildren(vector_t *children) {
    for (int i = 0; i < vector_getSize(children); ++i) {
        child_t *child = vector_at(children, i);
        int status = child->status;
        pid_t pid = child->pid;

        if (pid != -1) {
            const char* ret = "NOK";
            if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                ret = "OK";
            }
            int time = TIMER_DIFF_SECONDS(child->start, child->stop);
            if(time >= 1)
            {
                int exec_time = nearbyintf(time);
                printf("CHILD EXITED: (PID=%d; return %s ; %d s)\n", pid, ret, exec_time);
            }
            else
            {
                char* exec_time = "<1";
                printf("CHILD EXITED: (PID=%d; return %s ; %s s)\n", pid, ret, exec_time);
            }
        }
    }
    puts("END.");
}

void invalidSyntax(char buffer[], char *args[])
{
    char clientPipe[BUFFER_SIZE] = "../CircuitRouter-Client/";
    strcat(clientPipe, args[0]);
    int clientFD;           
    printf("Invalid syntax received from client.\n");

    if((clientFD = open(clientPipe, O_NONBLOCK | O_WRONLY)) == -1 && errno != EINTR)
    {
        printf("Error opening client pipe.\n");
        return;
    }

    int auxwrite;

    strcpy(buffer, INVALID_COMMAND);

    if((auxwrite = write(clientFD, buffer, BUFFER_SIZE)) == 0)
    {
        if(close(clientFD) == -1 && errno != EINTR)
        {
            printf("Error closing client pipe.\n");
            exit(EXIT_FAILURE);
        }
    }
    else if(auxwrite == -1 && errno != EINTR)
    {
        printf("Error writing to client pipe.\n");
        if(close(clientFD) == -1 && errno != EINTR)
        {
            printf("Error closing client pipe.\n");
            exit(EXIT_FAILURE);
        }
        return;
    }
    if(close(clientFD) == -1 && errno != EINTR)
    {
        printf("Error closing client pipe.\n");
        exit(EXIT_FAILURE);
    }
}

int parsePipeCommand(char **argVector, int vectorSize, char *input, int bufferSize)
{
	int numTokens = 0;
	char *s = " \r\n\t";

	int i;

	char *token;

	if (argVector == NULL || input == NULL || vectorSize <= 0 || bufferSize <= 0)
	 return 0;

	/* get the first token */
	token = strtok(input, s);

	/* walk through other tokens */
	while( numTokens < vectorSize-1 && token != NULL ) {
		argVector[numTokens] = token;
		numTokens ++;

		token = strtok(NULL, s);
	}

	for (i = numTokens; i<vectorSize; i++) {
		argVector[i] = NULL;
	}

	return numTokens;
}

int main (int argc, char** argv) {
    int fdFIFO_IN;
    char *fifoPath = "CircuitRouter-AdvShell.pipe";
    char *args[MAXARGS + 1];
    char buffer[BUFFER_SIZE];
    int MAXCHILDREN = -1;
    fd_set fdset;
    int numArgs;
    struct sigaction s;
    sigemptyset(&s.sa_mask);
    s.sa_flags = SA_NOCLDSTOP | SA_RESTART;
    s.sa_handler = waitForChild;
    sigaction(SIGCHLD, &s, NULL);

    if(argv[1] != NULL){
        MAXCHILDREN = atoi(argv[1]);
    }

    children = vector_alloc(MAXCHILDREN); 

    int value;

    if((value = mkfifo(fifoPath, 0777)) == -1 && errno != EEXIST)
    {
            printf("Error creating main pipe.\n");
            exit(EXIT_FAILURE);
    }

    if((fdFIFO_IN = open(fifoPath,O_NONBLOCK | O_RDONLY)) == -1)
    {
        printf("Error connecting to main pipe.\n");
        exit(EXIT_FAILURE);
    }

    printf("Welcome to CircuitRouter-AdvShell\n\n");

    while (1) {
        checkErrors();
        FD_ZERO(&fdset);       
        FD_SET(STDIN_FILENO, &fdset);       
        FD_SET(fdFIFO_IN,&fdset);      
        if((select(fdFIFO_IN+1, &fdset, NULL, NULL, NULL)) == -1 && errno != EINTR)
        {
            printf("Error in select.\n");
            exit(EXIT_FAILURE);
        }

        if(FD_ISSET(STDIN_FILENO, &fdset)){

            numArgs = readLineArguments(args, MAXARGS+1, buffer, BUFFER_SIZE);

            /* EOF (end of file) do stdin ou comando "sair" */
            if (numArgs < 0 || (numArgs > 0 && (strcmp(args[0], COMMAND_EXIT) == 0))) {
                printf("CircuitRouter-AdvShell will exit.\n--\n");

                /* Espera pela terminacao de cada filho */
                while (runningChildren > 0) {
                }

                printChildren(children);
                printf("--\nCircuitRouter-AdvShell ended.\n");
                break;
            }

            else if (numArgs > 0 && strcmp(args[0], COMMAND_RUN) == 0){
                int pid;
                if (numArgs < 2) {
                    printf("%s: invalid syntax. Try again.\n", COMMAND_RUN);
                    continue;
                }
                if (MAXCHILDREN != -1 && runningChildren >= MAXCHILDREN) {

                }
                TIMER_READ(startTime);
                pid = fork();
                if (pid < 0) {
                    perror("Failed to create new process.");
                    exit(EXIT_FAILURE);
                }

                if (pid > 0) {
                    runningChildren++;
                    child_t *child = malloc(sizeof(child_t));

                    if (child == NULL) {
                        perror("Error allocating memory");
                        exit(EXIT_FAILURE);
                    }
                    child->pid = pid;
                    child->start = startTime;
                    vector_pushBack(children, child);

                    printf("%s: background child started with PID %d.\n\n", COMMAND_RUN, pid);
                    continue;
                } else {
                    char seqsolver[] = "../CircuitRouter-SeqSolver/CircuitRouter-SeqSolver";
                    char *newArgs[3] = {seqsolver, args[1], NULL};
                    printf("%s",args[1]);
                    printf("%s",args[2]);
                    execv(seqsolver, newArgs);
                    perror("Error while executing child process"); // Nao deveria chegar aqui
                    exit(EXIT_FAILURE);
                }
            }

            else if (numArgs == 0){
                /* Nenhum argumento; ignora e volta a pedir */
                continue;
            }
            else
                printf("Unknown command. Try again.\n");
        }
        else if(FD_ISSET(fdFIFO_IN,&fdset)){
            printf("Entered Pipe stuff.\n");
            int readaux;

            if((readaux = read(fdFIFO_IN, buffer, BUFFER_SIZE)) == -1 && errno != EINTR){
                printf("Error reading from main pipe.\n");
                exit(EXIT_FAILURE);
            }
            else if(readaux == 0){
                if(close(fdFIFO_IN && errno != EINTR))
                {
                    printf("Error closing main pipe.\n");
                    exit(EXIT_FAILURE);
                }
                if((fdFIFO_IN = open(fifoPath,O_NONBLOCK | O_RDONLY)) == -1 && errno != EINTR)
                {
                    printf("Error reconnecting to main pipe.\n");
                    exit(EXIT_FAILURE);
                }
                continue;
            }

            numArgs = parsePipeCommand(args, 4, buffer, BUFFER_SIZE) - 1;

            if (numArgs > 0 && strcmp(args[1], COMMAND_RUN) == 0){
                int pid;

                if (numArgs < 2) {
                    invalidSyntax(buffer, args);
                    continue;
                }
                if (MAXCHILDREN != -1 && runningChildren >= MAXCHILDREN) {
                }
                TIMER_READ(startTime);

                pid = fork();
                if (pid < 0) {
                    perror("Failed to create new process.");
                    exit(EXIT_FAILURE);
                }

                if (pid > 0) {
                    runningChildren++;
                    child_t *child = malloc(sizeof(child_t));
                    
                    if (child == NULL) {
                        perror("Error allocating memory");
                        exit(EXIT_FAILURE);
                    }

                    child->pid = pid;
                    child->start = startTime;
                    vector_pushBack(children, child);

                    printf("%s: background child started with PID %d.\n\n", COMMAND_RUN, pid);
                } else {
                    char seqsolver[] = "../CircuitRouter-SeqSolver/CircuitRouter-SeqSolver";
                    char *newArgs[5] = {seqsolver, args[2], args[0], "pipe", NULL};

                    execv(seqsolver, newArgs);
                    perror("Error while executing child process"); // Nao deveria chegar aqui
                    exit(EXIT_FAILURE);
                }
            }
            else
            {     
                invalidSyntax(buffer, args);
                continue;
            }
        }
    }

    for (int i = 0; i < vector_getSize(children); i++) {
        free(vector_at(children, i));
    }
    vector_free(children);
    if(close(fdFIFO_IN) == -1)
    {
        printf("Error closing client pipe.\n");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}

