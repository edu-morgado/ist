#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "functions.h"
#define INFOSIZE 2048
#define MAXEVENTS 1000
int main()
{
    char choice;
    char input[INFOSIZE+2];
    char info[INFOSIZE];
    struct Event events[MAXEVENTS];
    int i;
    int index = 0;
    while (fgets(input, INFOSIZE+2, stdin) != NULL)
    {
        choice = input[0];
        for (i = 0; i < INFOSIZE; i++)
            info[i] = input[i + 2];
        switch (choice)
        {
        case 'a':
            if (command_a(events, info, index))
            {
                events[index] = parse(info);
                index++;
            }
            break;
        case 'l':
            command_l(events, index);
            break;
        case 's':
            command_s(events, info, index);
            break;
        case 'r':
            if (command_r(events, info, index))
                index--;
            break;
        case 'i':
            command_i(events, info, index);
            break;
        case 't':
            command_T(events, info, index);
            break;
        case 'm':
            command_m(events, info, index);
            break;
        case 'A':
            command_A(events, info, index);
            break;
        case 'R':
            command_R(events, info, index);
            break;
        case 'x':
            exit(0);
            break;
        }
    }
    return 0;
}