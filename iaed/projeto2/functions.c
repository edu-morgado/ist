#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

/*These variables mean the same throughout the entire code.
**NameHeads->Array with heads for the Name hash table
**DomainHeads->Array with heads for the Domain hash table 
**head->head for the linked list
**tail->tail for the linked list
  info->string received from the input without the first two charaters 
  M1->The M value associated with the Name hash table
  M2->The M value associated with the Domain hash table*/



/* This function adds a contact to the linked list and to the hash tables. 
If the person was already inside the contact list, it will not be added */
void addContact(contact_t **NameHeads, contact_t **DomainHeads,contact_t **NameTails,contact_t **DomainTails, contact_t **head, contact_t **tail, char info[INPUTSIZE - 2], int M1, int M2)
{
    int i = 0;
    int key1, key2;
    contact_t *newContact = (contact_t *)malloc(sizeof(contact_t));
    contact_t *temp;
    newContact->name = parseString(info, &i);
    newContact->emailLocal = parseString(info, &i);
    newContact->emailDomain = parseString(info, &i);
    newContact->phoneNumber = parseString(info, &i);
    newContact->next = NULL;
    newContact->prev = NULL;
    newContact->hashNameNext = NULL;
    newContact->hashNamePrev = NULL;
    newContact->hashDomainNext = NULL;
    newContact->hashDomainPrev = NULL;
    key1 = hashU(newContact->name, M1);
    temp = NameHeads[key1];
    while (temp != NULL)
    {
        if (strcmp(newContact->name, temp->name) == 0)
        {
            free(newContact->emailDomain);
            free(newContact->emailLocal);
            free(newContact->name);
            free(newContact->phoneNumber);
            free(newContact);
            printf("Nome existente.\n");
            return;
        }
        temp = temp->hashNameNext;
    }


    insertEndList(head, tail, newContact);
    key1 = hashU(newContact->name, M1);
    InsertInNameHash(NameHeads,NameTails, key1, newContact);

    key2 = hashU(newContact->emailDomain, M2);
    InsertInDomainHash(DomainHeads,DomainTails, key2, newContact);
}


/*Prints all the elements of the linked list 
by the order they were added to the linked list */
void printAllContacts(contact_t *head)
{
    contact_t *aux;
    aux = head;
    while (aux != NULL)
    {
        printf("%s %s@%s %s\n", aux->name, aux->emailLocal, aux->emailDomain, aux->phoneNumber);
        aux = aux->next;
    }
}

/*It searches by name if the person exists. To do so, it uses the hash table that is organized by the person's name */
void printContact(contact_t **NameHeads, char *info, int M1)
{
    int res, i = 0, validation = 0;
    char *name;
    contact_t *aux;
    name = parseString(info, &i);
    res = hashU(name, M1);
    aux = NameHeads[res];
    while (aux != NULL)
    {
        if (strcmp(aux->name, name) == 0)
        {
            printf("%s %s@%s %s\n", aux->name, aux->emailLocal, aux->emailDomain, aux->phoneNumber);
            validation = 1;
            free(name);
            return;
        }
        aux = aux->hashNameNext;
    }
    if (validation == 0)
        printf("Nome inexistente.\n");
    free(name);
}

/*It uses the person's name hash table to find the person and 
removes the pointers from both hash lists and the linked list */
void removeContact(contact_t **NameHeads, contact_t **DomainHeads,contact_t **NameTails,contact_t **DomainTails,contact_t **head, contact_t **tail, char *info, int M1, int M2)
{
    int res1, res2, i = 0;
    int validation = 0;
    char *name;
    contact_t *aux;
    contact_t *temp;
    name = parseString(info, &i);
    res1 = hashU(name, M1);
    aux = NameHeads[res1];
    while (aux != NULL)
    {
        if (strcmp(aux->name, name) == 0)
        {
            temp = aux;
            validation = 1;
            break;
        }
        aux = aux->hashNameNext;
    }
    if (validation == 0)
    {
        printf("Nome inexistente.\n");
        free(name);
        return;
    }

    removeInNameHash(NameHeads,NameTails, res1, temp);

    res2 = hashU(temp->emailDomain, M2);
    removeInDomainHash(DomainHeads,DomainTails, res2, temp);
    
    removeInLinkedList(head,tail,temp);



    free(name);
    free(temp->emailDomain);
    free(temp->emailLocal);
    free(temp->name);
    free(temp->phoneNumber);
    free(temp);

}

void removeInLinkedList(contact_t **head,contact_t **tail,contact_t *contact)
{
    if (contact == *head)
    {
        *head = contact->next;
        if (*head == NULL)
        {
            *tail = NULL;
        }
        else
        {
            contact->next->prev = NULL;
        }
    }
    else if (contact == *tail)
    {
        *tail = contact->prev;
        if (*tail == NULL)
        {
            *head = *tail;
        }
        else
        {
            contact->prev->next = NULL;
        }
    }
    else
    {
        contact->next->prev = contact->prev;
        contact->prev->next = contact->next;
    }
}

/*Using the person's hash table finds and changes the Domain of that person.
It updates the Domain hash table,because one domain has been removed, and another was has been added */
void changeEmailAdress(contact_t **NameHeads, contact_t **DomainHeads,contact_t **DomainTails,char *info, int M1, int M2)
{
    char *name;
    int res, i = 0;
    int validation = 0;
    contact_t *aux;
    name = parseString(info, &i);
    res = hashU(name, M1);
    aux = NameHeads[res];
    while (aux != NULL)
    {
        if (strcmp(aux->name, name) == 0)
        {
            res = hashU(aux->emailDomain, M2);

            removeInDomainHash(DomainHeads,DomainTails, res, aux);
            aux->hashDomainNext = NULL;
            aux->hashDomainPrev = NULL;
            aux->emailLocal = parseString(info, &i);
            ;
            aux->emailDomain = parseString(info, &i);
            res = hashU(aux->emailDomain, M2);
            InsertInDomainHash(DomainHeads,DomainTails,res, aux);
            validation = 1;
        }
        aux = aux->hashNameNext;
    }
    if (validation == 0)
        printf("Nome inexistente.\n");
    free(name);
}

/*Using the Domain hash table, finds and counts how many of that specific Domain are there*/
void emailOccurrence(contact_t **DomainHeads, char *info, int M2)
{
    char *domain;
    int res, i = 0, solution = 0;
    contact_t *aux;
    domain = parseString(info, &i);
    res = hashU(domain, M2);

    aux = DomainHeads[res];
    while (aux != NULL)
    {
        if (strcmp(aux->emailDomain, domain) == 0)
            solution++;
        aux = aux->hashDomainNext;
    }
    printf("%s:%d\n", domain, solution);

    free(domain);
}


/* *i->int that keeps track of where to begin the reading in the "info" string
Clears and returns a string with the next piece of information */
char *parseString(char info[INPUTSIZE - 2], int *i)
{
    int j = 0;
    char *string = malloc(sizeof(char) * 1300);
    string[0] = '\0';
    while (info[*i] != ' ' && info[*i] != '\n' && info[*i] != '@')
    {
        string[j] = info[*i];
        *i = *i + 1;
        j++;
    }
    string[j] = '\0';
    *i = *i + 1;
    string = realloc(string, j + 1);
    return string;
}

/* *newContact->The new contact_t struct to be added to the list
Adds an element to the end of the Linked List */
void insertEndList(contact_t **head, contact_t **tail, contact_t *newContact)
{
    contact_t *aux;
    if (*head == NULL)
    {
        newContact->next = NULL;
        newContact->prev = NULL;
        *head = newContact;
        *tail = newContact;
        return;
    }
    aux = *tail;
    aux->next = newContact;
    aux->next->prev = aux;
    aux = aux->next;
    *tail = aux;
}

/* *contact->The new contact_t struct to be added to the Name hash table
Adds an element to the corresponding index of the Name hash table */
void InsertInNameHash(contact_t **Heads,contact_t **NameTails,int res, contact_t *contact)
{
    contact_t *aux;
    if (Heads[res] == NULL)
    {
        Heads[res] = contact;
        NameTails[res] = contact;
        return;
    }
    aux = NameTails[res];
    aux->hashNameNext = contact;
    contact->hashNamePrev = aux;
    NameTails[res] = contact;
}

/* *contact->The new contact_t struct to be added to the Domain hash table
Adds an element to the corresponding index of the Domain hash table */

void InsertInDomainHash(contact_t **Heads,contact_t **DomainTails,int res, contact_t *contact)
{
    contact_t *aux;

    if (Heads[res] == NULL)
    {
        Heads[res] = contact;
        DomainTails[res] = contact;
        return;
    }
    aux = DomainTails[res];
    aux->hashDomainNext = contact;
    contact->hashDomainPrev = aux;
    DomainTails[res]=contact;
}

/*  res-> The index of where in the array of NameHeads is our contact.
    *contact->The contact_t struct to be removed from the Name hash table
Removes an element to the corresponding to the contact name of the Name hash table */
void removeInNameHash(contact_t **Heads,contact_t **Tails, int res, contact_t *contact)
{

    if (contact == Heads[res])
    {
        Heads[res] = contact->hashNameNext;
        if (Heads[res] != NULL)
        {
            contact->hashNameNext->hashNamePrev = NULL;
        }
        else
        {
            Tails[res]=NULL;
        }
        

    }
    else if(contact ==Tails[res])
    {
        Tails[res] = contact-> hashNamePrev;
        if(Tails[res]!=NULL)
        {
            contact->hashNamePrev->hashNameNext = NULL;
        }
        else
        {
            Heads[res]=NULL;
        }
        
    }
    else
    {
        contact->hashNamePrev->hashNameNext = contact->hashNameNext;
        contact->hashNameNext->hashNamePrev = contact->hashNamePrev;
    }
}

/*  res-> The index of where in the array of NameHeads is our contact.
    *contact->The contact_t struct to be removed from the Name hash table
Removes an element to the corresponding to the contact name of the Domain hash table */
void removeInDomainHash(contact_t **Heads,contact_t **Tails, int res, contact_t *contact)
{
    if (contact == Heads[res])
    {

        Heads[res] = contact->hashDomainNext;
        if (Heads[res] != NULL)
        {
            contact->hashDomainNext->hashDomainPrev = NULL;
        }
        else
        {
            Tails[res]=NULL;
        }

        
    }
    else if (contact==Tails[res])
    {
        Tails[res] = contact ->hashDomainPrev;
        if(Tails[res]!=NULL)
        {
            contact->hashDomainPrev->hashDomainNext = NULL;
        }
        else
        {
            Heads[res]=NULL;
        }
    }
    else
    {
        contact->hashDomainPrev->hashDomainNext = contact->hashDomainNext;
        contact->hashDomainNext->hashDomainPrev = contact->hashDomainPrev;
    }
}

/* *v-> string to be operated on
    M-> M value associated with the hash table
Operates on a string and returns on which index of the hash table the string belongs to.*/
int hashU(char *v, int M) 
{
    int h, a = 31415, b = 27183;
    for (h = 0; *v != '\0'; v++, a = a * b % (M - 1))
        h = (a * h + *v) % M;
    return h;
}

/*Frees all blocks of memory by iterating the linked list*/
void freeList(contact_t *head)
{
    contact_t *aux;
    aux = head;
    while (aux != NULL)
    {
        free(aux->emailDomain);
        free(aux->emailLocal);
        free(aux->name);
        free(aux->phoneNumber);
        head = aux->next;
        free(aux);
        aux = head;
    }
}