#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>
#include <sys/select.h>

#define MAXARGS 3
#define BUFFER_SIZE 100
#define INTERRUPTEDFLAG -1
#define PIPEFLAG -2
  /************************************************************+

    retirar os imports que nao sao necessarios
    a criar o pipe ja nao tem o .pipe

  **************************************************************/


void signalHandler(int dummy);
void signalChecking(char *outPath);

int signalFlag=0;

int main(int argc, char** argv){
    int fd;
    int out;
    char *fifoPath;
    char buffer[BUFFER_SIZE];
    char outPath[BUFFER_SIZE];
    fd_set fdset;
    struct sigaction s;

    sigemptyset(&s.sa_mask);
    s.sa_flags = SA_NOCLDSTOP | SA_RESTART;
    s.sa_handler = signalHandler;
    sigaction(SIGPIPE, &s, NULL);
    sigaction(SIGINT, &s, NULL);


    if(argv[1] == NULL){
        printf("Invalid argument at program start.");
        exit(EXIT_FAILURE);
    }
    fifoPath = argv[1];
    printf("%s\n", fifoPath);
    if((fd = open(fifoPath, O_NONBLOCK | O_WRONLY)) == -1){
        printf("Error connecting to main pipe.\n");
        exit(EXIT_FAILURE);
    }
    int pid = getpid();
    sprintf(outPath, "%d.pipe", pid);

    if(mkfifo(outPath, 0777) == -1)
    {
        printf("Error creating client pipe.\n");
        exit(EXIT_FAILURE);
    }

    if((out = open(outPath,O_NONBLOCK | O_RDONLY)) == -1){
        printf("Error connecting to client pipe.\n");
        exit(EXIT_FAILURE);
    }

    while(1)
    {
        signalChecking(outPath);
        FD_ZERO(&fdset);
        FD_SET(STDIN_FILENO, &fdset);
        FD_SET(out, &fdset);

        select(out+1, &fdset, NULL, NULL, NULL);

        if(FD_ISSET(STDIN_FILENO, &fdset))
        {
            char auxbuffer[BUFFER_SIZE];
            sprintf(buffer,"%s ", outPath);
            fgets(auxbuffer, BUFFER_SIZE, stdin);
            strcat(buffer, auxbuffer);

            int aux;

            if((aux = write(fd, buffer, BUFFER_SIZE)) == -1){
                printf("Error writing to main pipe.\n");
                exit(EXIT_FAILURE);
            }
            else if(aux == 0)
            {
                if(close(fd) == -1)
                {
                    printf("Error closing client pipe.\n");
                    exit(EXIT_FAILURE);
                }
                if((out = open(fifoPath, O_NONBLOCK | O_RDONLY)) == -1)
                {
                    printf("Error reconnecting to main pipe.\n");
                    exit(EXIT_FAILURE);
                }
            }
        }
        else if(FD_ISSET(out, &fdset))
        {
            int bytes;

            if((bytes = read(out, buffer, BUFFER_SIZE)) == -1)
            {
                printf("Error reading from client pipe.\n");
                exit(EXIT_FAILURE);
            }
            else if(bytes == 0)
            {
                if(close(out) == -1)
                {
                    printf("Error closing client pipe.\n");
                    exit(EXIT_FAILURE);
                }
                if((out = open(outPath, O_NONBLOCK | O_RDONLY)) == -1){
                    printf("Error connecting to client pipe.\n");
                    exit(EXIT_FAILURE);
                }
            }
            else
                printf("%s", buffer);
        }
    }     
}

void signalHandler(int dummy){
    if(dummy==SIGINT){ 
        signalFlag=INTERRUPTEDFLAG;
    }
    if(dummy==SIGPIPE){
        signalFlag=PIPEFLAG;
    }
}

void signalChecking(char *outPath){
    if(signalFlag==-1){
        printf("\n/****************************************************************\nCircuitRouter-Client will now close the pipe and will exit.\n****************************************************************\n");
        unlink((const char *)outPath);
        exit(0);
    }
    if(signalFlag==-2){
        printf("\n/****************************************************************\nThe server is not reading.CircuitRouter-Client will now close the pipe and will exit.\n****************************************************************\n");
        unlink((const char *)outPath);
        exit(0);
    }
}