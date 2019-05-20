#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "functions.h"
#define INFOSIZE 2048
#define MAXEVENTS 1000
#define PARTICIPANTMAXSIZE 300

/*
Prints all events inside the events struct.
*/
void PrintAllEvents(struct Event events[MAXEVENTS], int index)
{
    int i, j;

    for (i = 0; i < index; i++)
    {
        printf("%s %02d%02d%04d %02d%02d %d Sala%d %s\n", events[i].description, events[i].date.day, events[i].date.month, events[i].date.year, events[i].init.hours, events[i].init.minutes, events[i].duration, events[i].room, events[i].liableperson);
        printf("*");
        for (j = 0; j < 3; j++)
        {
            if (events[i].participants[j][0] != '\0')
                printf(" %s", events[i].participants[j]);
        }
        printf("\n");
    }
}

/*
validates and adds the new event to the saved events array 
*/
int command_a(struct Event events[MAXEVENTS], char input[INFOSIZE], int index)
{
    struct Event event;
    event = parse(input);
    return eventValidation(events, event, index, 0);
}
/*
sorts and prints all events
*/
int command_l(struct Event events[MAXEVENTS], int index)
{
    sort(events, index);
    PrintAllEvents(events, index);
    return 1;
}

/*
Creates an array with events on a specific room. Then applies command l to that array.
*/
void command_s(struct Event events[MAXEVENTS], char info[INFOSIZE], int index)
{
    struct Event room[100]; /*There can only be 100 events on one room at the same time*/
    int temp = 0;
    int aux;
    int idroom = atoi(info);
    for (aux = 0; aux <= index; aux++)
        if (events[aux].room == idroom)
            room[temp++] = events[aux];
    command_l(room, temp);
}

/*
Removes an event if it is possible to do so
*/
int command_r(struct Event events[MAXEVENTS], char input[INFOSIZE], int index)
{
    int aux, i;
    int validation = 0;
    strcpy(input, clearToken(input));
    for (aux = 0; aux <= index; aux++)
    {
        if (strcmp(events[aux].description, input) == 0)
        {
            validation = 1;
            for (i = aux; i < MAXEVENTS - 1; i++)
                events[i] = events[i + 1];
            return validation;
        }
    }
    printf("Evento %s inexistente.\n", input);
    return validation;
}

/*
Changes the hour of a specific event if possible
*/
int command_i(struct Event events[MAXEVENTS], char input[INFOSIZE], int index)
{
    struct Event event;
    char description[INFOSIZE];
    char Aux[64];
    int newHour, newMinutes;
    int year;
    int validation = 0;
    int aux = 0;
    smallParse(input, description, Aux);
    aux = atoi(Aux);
    newMinutes = aux % 100;
    aux = aux / 100;
    newHour = aux;
    for (aux = 0; aux < index; aux++)
    {
        if (strcmp(description, events[aux].description) == 0)
        {
            event = events[aux];
            event.init.hours = newHour;
            event.init.minutes = newMinutes;
            year = events[aux].date.year;
            events[aux].date.year = 0;
            validation = 1; /* o event existe */
            if (eventValidation(events, event, index, 0))
            {
                events[aux] = event;
                sort(events, index);
                return 1;
            }
            events[aux].date.year = year;
        }
    }
    if (!validation)
        printf("Evento %s inexistente.\n", description);
    return 0;
}

/*
Changes thge duration of an event, if possible
*/
int command_T(struct Event events[MAXEVENTS], char input[INFOSIZE], int index)
{
    struct Event event;
    char description[64];
    char Novaduration[64];
    int newduration;
    int validation = 0;
    int aux = 0;
    int year;
    smallParse(input, description, Novaduration);
    newduration = atoi(Novaduration);
    for (aux = 0; aux < index; aux++)
    {

        if (strcmp(description, events[aux].description) == 0)
        {
            event = events[aux];
            event.duration = newduration;
            year = events[aux].date.year;
            events[aux].date.year = 0;
            validation = 1; /* o event existe */
            if (eventValidation(events, event, index, 0))
            {
                events[aux].duration = newduration;
                events[aux].date.year = year;
                return 1;
            }
            events[aux].date.year = year;
        }
    }
    if (!validation)
        printf("Evento %s inexistente.\n", description);

    return 0;
}

/*
Changes the room of a specific event if possible
*/
int command_m(struct Event events[MAXEVENTS], char input[INFOSIZE], int index)
{
    struct Event event;
    char description[64];
    char Novaroom[3];
    int newroom;
    int validation = 0;
    int aux = 0;
    int year;
    smallParse(input, description, Novaroom);
    newroom = atoi(Novaroom);
    for (aux = 0; aux < index; aux++)
    {

        if (strcmp(description, events[aux].description) == 0)
        {
            event = events[aux];
            event.room = newroom;
            year = event.date.year;
            events[aux].date.year = 0;
            validation = 1; /* o event existe */
            if (eventValidation(events, event, index, 0))
            {
                events[aux].room = newroom;
                events[aux].date.year = year;
                return 1;
            }
            events[aux].date.year = year;
        }
    }
    if (!validation)
        printf("Evento %s inexistente.\n", description);

    return 0;
}

/*
Adds a participant, if he is not somewhere else, and the event can take one more participant
*/
int command_A(struct Event events[MAXEVENTS], char input[INFOSIZE], int index)
{
    struct Event event;
    char description[64];
    char novoparticipante[64];
    int validation = 0, validperson = 1;
    int aux = 0, i = 0;
    int year;
    smallParse(input, description, novoparticipante);
    for (aux = 0; aux < index; aux++)
    {
        if (strcmp(description, events[aux].description) == 0)
        {
            validation = 1; /* o event existe */
            event = events[aux];
            year = events[aux].date.year;
            events[aux].date.year = 0;
            if (event.nparticipants < 3)
            {
                strcpy(event.participants[event.nparticipants++], novoparticipante);
                if (eventValidation(events, event, index, 1))
                {
                    for (i = 0; i < events[aux].nparticipants; i++)
                    {
                        if (strcmp(novoparticipante, events[aux].participants[i]) == 0)
                            validperson = 0;
                    }
                    if (validperson)
                    {
                        strcpy(events[aux].participants[events[aux].nparticipants++], novoparticipante);
                        events[aux].date.year = year;
                        return 0;
                    }
                }
            }
            else if (event.nparticipants == 3)
                printf("Impossivel adicionar participante. Evento %s ja tem 3 participantes.\n", description);
            events[aux].date.year = year;
        }
    }
    if (!validation)
        printf("Evento %s inexistente.\n", description);

    return 0;
}

/*
Removes a participant from an event if possible
*/
int command_R(struct Event events[MAXEVENTS], char input[INFOSIZE], int index)
{

    char description[64];
    char retiraparticipante[64];
    int validation = 0;
    int aux, i, j;
    smallParse(input, description, retiraparticipante);
    for (aux = 0; aux < index; aux++)
    {
        if (strcmp(description, events[aux].description) == 0)
        {

            validation = 1; /* o event existe */
            for (i = 0; i < events[aux].nparticipants; i++)
            {
                if (strcmp(events[aux].participants[i], retiraparticipante) == 0)
                {
                    if (events[aux].nparticipants == 1)
                    {
                        printf("Impossivel remover participante. Participante %s e o unico participante no evento %s.\n", retiraparticipante, description);
                        return 0;
                    }
                    events[aux].participants[i][0] = '\0';
                    for (j = i; j < 3 - 1; j++)
                        strcpy(events[aux].participants[j], events[aux].participants[j + 1]);
                    events[aux].nparticipants--;
                    return 0;
                }
            }
        }
    }
    if (!validation)
        printf("Evento %s inexistente.\n", description);
    return 0;
}

void swap(struct Event events[MAXEVENTS], int index1, int index2)
{
    struct Event aux;
    aux = events[index1];
    events[index1] = events[index2];
    events[index2] = aux;
}

/*
Validates if the room is free at the pointed time, 
and checks if everyone is avaiable at that time to participate on the event
*/
int eventValidation(struct Event events[MAXEVENTS], struct Event event, int index, int modo)
{
    int aux;
    int hours, hoursi;
    int minutes, minutesi;
    int TinitEvent, TinitEvents, TfimEvent, TfimEvents;
    int validation = 1;

    for (aux = 0; aux <= index; aux++)
    {

        if (event.date.year == events[aux].date.year &&
            event.date.month == events[aux].date.month &&
            event.date.day == events[aux].date.day)
        {
            hours = event.duration / 60;
            minutes = event.duration % 60;
            hoursi = events[aux].duration / 60;
            minutesi = events[aux].duration % 60;

            TinitEvent = event.init.hours * 60 + event.init.minutes;
            TinitEvents = events[aux].init.hours * 60 + events[aux].init.minutes;
            TfimEvent = (event.init.hours + hours) * 60 + event.init.minutes + minutes;
            TfimEvents = (events[aux].init.hours + hoursi) * 60 + events[aux].init.minutes + minutesi;

            if (TinitEvent <= TinitEvents && event.room == events[aux].room)
            {
                if (TinitEvents < TfimEvent)
                {
                    printf("Impossivel agendar evento %s. Sala%d ocupada.\n", event.description, event.room);
                    return 0;
                }
            }
            else if (TinitEvents <= TinitEvent && event.room == events[aux].room)
            {
                if (TinitEvent < TfimEvents)
                {
                    printf("Impossivel agendar evento %s. Sala%d ocupada.\n", event.description, event.room);
                    return 0;
                }
            }
            if (TinitEvent <= TinitEvents && event.room != events[aux].room)
            {
                if (TinitEvents < TfimEvent)
                    if (!peopleValidation(events[aux], event, modo))
                        validation = 0;
            }
            else if (TinitEvents <= TinitEvent && event.room != events[aux].room)
                if (TinitEvent < TfimEvents)
                    if (!peopleValidation(events[aux], event, modo))
                        validation = 0;
        }
    }

    return validation;
}

/*
Compares everyone one with eveyone
*/
int peopleValidation(struct Event event1, struct Event event2, int modo)
{
    /*compara todos os dos do 1o e do 20 event e verifica se algum par de nomonth e igual
    Event 2 corresponde ao event a ser adicionado*/
    int validation = 1, i = 0;
    if (!comparePeople(event1.liableperson, event2, modo))
        validation = 0;
    while (i < 3)
    {
        if (!comparePeople(event1.participants[i], event2, modo))
            validation = 0;
        i++;
    }
    return validation;
}

/*
Compares 1 person from an event with everyone with the other event
*/
int comparePeople(char personName[64], struct Event event2, int modo)
{
    int aux = 0;
    int validation = 1;
    if (strcmp(personName, event2.liableperson) == 0)
    {
        validation = 0;
        printf("Impossivel agendar evento %s. Participante %s tem um evento sobreposto.\n", event2.description, event2.liableperson);
    }
    while (aux < 3)
    {
        if (strcmp(personName, event2.participants[aux]) == 0 && event2.participants[aux][0] != '\0')
        {
            validation = 0;
            if (modo == 0)
                printf("Impossivel agendar evento %s. Participante %s tem um evento sobreposto.\n", event2.description, personName);
            if (modo == 1)
                printf("Impossivel adicionar participante. Participante %s tem um evento sobreposto.\n", personName);
        }
        aux++;
    }
    return validation;
}

/*
reads the input line and saves the information in a struct Event
*/
struct Event parse(char input[INFOSIZE])
{
    const char s[2] = ":";
    const char end[2] = "\0";
    char *token;
    struct Event event;
    int aux;
    char temp[INFOSIZE];
    strcpy(temp, input);
    token = strtok(temp, s);
    strcpy(event.description, clearToken(token));

    token = strtok(NULL, s);
    aux = atoi(token);
    event.date.year = aux % 10000;
    aux = aux / 10000;
    event.date.month = aux % 100;
    aux = aux / 100;
    event.date.day = aux;
    token = strtok(NULL, s);
    aux = atoi(token);
    event.init.minutes = aux % 100;
    aux = aux / 100;
    event.init.hours = aux;
    token = strtok(NULL, s);
    event.duration = atoi(token);
    token = strtok(NULL, s);
    event.room = atoi(token);
    token = strtok(NULL, s);
    strcpy(event.liableperson, token);
    token = strtok(NULL, s);
    aux = 0;
    if (token != NULL)
    {
        strcpy(event.participants[0], clearToken(token));
        event.nparticipants = 1;
    }
    token = strtok(NULL, s);
    aux = 0;
    if (token != NULL)
    {
        strcpy(event.participants[1], clearToken(token));
        event.nparticipants++;
    }
    else
        event.participants[1][0] = '\0';

    token = strtok(NULL, end);
    aux = 0;
    if (token != NULL)
    {
        strcpy(event.participants[2], clearToken(token));
        event.nparticipants++;
    }
    else
        event.participants[2][0] = '\0';

    return event;
}

/*
reads the lines for everyone command that needs 2 inputs
*/
void smallParse(char input[INFOSIZE], char description[64], char extrainfo[64])
{
    const char s[2] = ":";
    const char end[2] = "\0";
    char *token = strtok(input, s);
    strcpy(description, token);
    token = strtok(NULL, end);
    strcpy(extrainfo, clearToken(token));
}

/*
sorts by date and time the envents
*/
void sort(struct Event events[MAXEVENTS], int index)
{
    int i, j;
    int init1, init2;
    int date1, date2;
    for (i = 0; i < index - 1; i++)
    {
        for (j = i + 1; j < index; j++)
        {
            date1 = events[i].date.year * 10000;
            date1 = date1 + events[i].date.month * 100;
            date1 = date1 + events[i].date.day;
            date2 = events[j].date.year * 10000;
            date2 = date2 + events[j].date.month * 100;
            date2 = date2 + events[j].date.day;
            if (date1 > date2)
                swap(events, i, j);
            else if (date1 == date2)
            {
                init1 = events[i].init.hours * 60 + events[i].init.minutes;
                init2 = events[j].init.hours * 60 + events[j].init.minutes;
                if (init1 > init2)
                {
                    swap(events, i, j);
                }
                if (init1 == init2)
                {
                    if (events[i].room > events[j].room)
                    {
                        swap(events, i, j);
                    }
                }
            }
        }
    }
}

/*
strtok leaves \n or \r on the last element that is copied. This functions cleans  the string
*/
char *clearToken(char token[64])
{
    int aux = 0;
    while (token[aux] != '\0' && token[aux] != '\r' && token[aux] != '\n')
        aux++;
    token[aux] = '\0';
    return token;
}