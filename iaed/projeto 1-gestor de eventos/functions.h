#ifndef FUNCTIONS_H_   /* Include guard */
#define FUNCTIONS_H_

#define INFOSIZE 2048
#define MAXEVENTS 1000


struct Date{
    int year;
    int month;
    int day;
};



struct Hour{
    int hours;
    int minutes;
};


struct Event{
    
    struct Date date;/*ddmmaaaa*/
    struct Hour init;/*hhmm*/
    int room;
    char participants[3][64];
    int nparticipants;
    int duration;/*in minutes*/
    char description[64], liableperson[64];
    
};

int command_a(struct Event events[MAXEVENTS],char input[INFOSIZE],int index);
int command_l(struct Event events[MAXEVENTS],int index);
void command_s(struct Event events[MAXEVENTS],char info[INFOSIZE], int index);
int command_r(struct Event events[MAXEVENTS],char info[INFOSIZE],int index);
int command_i(struct Event events[MAXEVENTS],char info[INFOSIZE],int index);
int command_T(struct Event events[MAXEVENTS],char info[INFOSIZE],int index);
int command_m(struct Event events[MAXEVENTS],char input[INFOSIZE],int index);
int command_A(struct Event events[MAXEVENTS], char input[INFOSIZE], int index);
int command_R(struct Event events[MAXEVENTS], char input[INFOSIZE], int index);
void PrintAllEvents(struct Event events[MAXEVENTS], int index);
void swap(struct Event events[MAXEVENTS],int index1, int index2);
int eventValidation(struct Event events[MAXEVENTS],struct Event event,int index,int mode);
int peopleValidation(struct Event event1,struct Event event2,int modo);
int comparePeople(char personName[64],struct Event event2,int modo);
struct Event parse(char input[INFOSIZE]);
void smallParse(char input[INFOSIZE],char description[64],char extrainfo[64]);
void sort(struct Event events[MAXEVENTS], int index);
char *clearToken(char token[64]);




#endif