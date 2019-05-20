#ifndef FUNCTIONS_H_   
#define FUNCTIONS_H_
#define INPUTSIZE 2000
#define MAXCONTACTS 1000


typedef struct Contact
{
    char *name;
    char *emailLocal;
    char *emailDomain;
    char *phoneNumber;   
    struct Contact* next;
    struct Contact* prev; 
    struct Contact* hashNameNext;
    struct Contact* hashNamePrev;
    struct Contact* hashDomainNext;
    struct Contact* hashDomainPrev;
}contact_t;


void addContact(contact_t **heads,contact_t **DomainHeads,contact_t **NameTails,contact_t **DomainTails,contact_t **head,contact_t **tail,char info[INPUTSIZE-2],int M1,int M2);
void printAllContacts(contact_t *head);
void printContact(contact_t **NameHeads,char* info,int M1);
void removeContact(contact_t **NameHeads,contact_t **DomainHeads,contact_t **NameTails,contact_t **DomainTails,contact_t **head, contact_t **tail,char* info,int M1,int M2);
void changeEmailAdress(contact_t **NameHeads,contact_t **DomainHeads,contact_t **DomainTails,char *info,int M1,int M2);
void emailOccurrence(contact_t **DomainHeads,char *info,int M2);
void insertEndList(contact_t **head,contact_t **tail, contact_t *newContact);
char * parseString(char info[INPUTSIZE-2],int *i);
void InsertInNameHash(contact_t **NameHeads,contact_t **NameTails,int M, contact_t *contact);
void removeInNameHash(contact_t **NameHeads,contact_t **Tails,int res,contact_t *contact);
void InsertInDomainHash(contact_t **DomainHeads,contact_t **DomainTails,int M2,contact_t *newContact);
void removeInDomainHash(contact_t **DomainHeads,contact_t **Tails,int res, contact_t *contact);
void removeInLinkedList(contact_t **head,contact_t **tail,contact_t *contact);
int hashU(char* value, int M);
void freeList(contact_t *head);


#endif
