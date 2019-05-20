#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

int main()
{
    char choice;
    char input[INPUTSIZE];
    char info[INPUTSIZE];
    int i;
    int M1 = 20051;
    int M2 = 10163;
    /*Initializing the variables for the hash tables and for the linked list */
    contact_t **NameHeads;
    contact_t **DomainHeads;
    contact_t **NameTails;
    contact_t **DomainTails;
    contact_t *head = NULL;
    contact_t *tail = NULL;
    NameHeads = (contact_t **)malloc(M1 * sizeof(contact_t *));
    DomainHeads = (contact_t **)malloc(M2 * sizeof(contact_t *));
    NameTails= (contact_t **)malloc(M1*sizeof(contact_t*));
    DomainTails = (contact_t **)malloc(M2 * sizeof(contact_t *));

    for (i = 0; i < M1; i++)
    {
        NameHeads[i] = NULL;
        NameTails[i]=NULL;
    }
    for (i = 0; i < M2; i++)
    {
        DomainHeads[i] = NULL;
        DomainTails[i]=NULL;
    }
    head = NULL;
    tail = NULL;
    /*The program takes in the input here and directs the action according to the input */
    while (fgets(input, INPUTSIZE, stdin) != NULL)
    {
        choice = input[0];
        for (i = 0; i < INPUTSIZE; i++)
            info[i] = input[i + 2];
        switch (choice)
        {
        case 'a':
            addContact(NameHeads, DomainHeads,NameTails,DomainTails, &head, &tail, info, M1, M2);
            break;
        case 'l':
            printAllContacts(head);
            break;
        case 'p':
            printContact(NameHeads, info, M1);
            break;
        case 'r':
            removeContact(NameHeads, DomainHeads,NameTails,DomainTails,&head, &tail, info, M1, M2);
            break;
        case 'e':
            changeEmailAdress(NameHeads, DomainHeads,DomainTails, info, M1, M2);
            break;
        case 'c':
            emailOccurrence(DomainHeads, info, M2);
            break;
        case 'x':
            freeList(head);
            free(NameHeads);
            free(DomainHeads);
            free(NameTails);
            free(DomainTails);
            break;
            /* exit(0); */
        }
    }

    return 0;
}