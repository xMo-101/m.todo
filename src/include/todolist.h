#include <stdio.h>

#ifndef TODOLIST_H
#define TODOLIST_H

// format for storing listelement in .csv file:
// id;priority;text;done

#define ERR_MALLOC      "Failed to allocate memory!\n"
#define XMARK           "\xF0\x90\x84\x82"
#define CHECKMARK       "\xF0\x9F\x97\xB9"

typedef struct liste
{
    char* text;
    int priority;
    int done;
    int ID;
    struct liste* next;
}   listelement;

typedef struct list
{
    listelement* head;
    int nelements;
    FILE* csvfile;
    char* csvpath;
}   list;

#define TASK_MAXLEN     60

int GetListnElements(FILE* listfile); // Get number of list elements in file
listelement* LoadListElement(list* MyList); // Load 1 list element from csv file (make sure file cursor is placed)
listelement* LoadListElements(list* MyList); // All list elements from csv file into a linked list and return head
list* LoadList(const char* dirpath);
void NewTask(list* MyList);
void DeleteTask(list* MyList);
void DisplayList(const list* MyList);
void SaveListState(list* MyList);
void CheckTask(list* MyList);
void UncheckTask(list* MyList);
void ChangeTaskDoneStatus(list* MyList, int ID, int mode);
void ListClean(list* MyList);

#endif