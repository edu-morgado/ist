#ifndef CIRCUITROUTER_SHELL_H
#define CIRCUITROUTER_SHELL_H

#include "lib/vector.h"
#include <sys/types.h>

typedef struct {
    pid_t pid;
    int status;
    struct timeval start;
    struct timeval stop;
} child_t;

void checkErrors();
void waitForChild(int signo);
void printChildren(vector_t *children);
void invalidSyntax(char buffer[], char *args[]);
int parsePipeCommand(char **argVector, int vectorSize, char *input, int bufferSize);


#endif /* CIRCUITROUTER_SHELL_H */
