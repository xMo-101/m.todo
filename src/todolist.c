#include "include/todolist.h"
#include "include/ANSI.h"
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

list* LoadList(const char* dirpath) {
    char* pathtofirstlist = NULL;
    DIR* dir;
    struct dirent* entity;
    dir = opendir(dirpath);
    entity = readdir(dir);

    while (entity != NULL && *(entity->d_name) == '.')
    {
        entity = readdir(dir);
    }

    if (NULL == entity) return NULL;
    
    int len = strlen(dirpath) +strlen(entity->d_name) +1 +strlen("/");
    pathtofirstlist = calloc( len, sizeof(char) );
    if (NULL == pathtofirstlist) {
        printf(ERR_MALLOC);
        return NULL;
    }
    snprintf(pathtofirstlist, len, "%s/%s", dirpath, entity->d_name);

    list* MyList = NULL;
    MyList = malloc( sizeof(list) );
    
    if (NULL == MyList) {
        printf(ERR_MALLOC);
        return NULL;
    }

    MyList->csvfile = NULL;
    MyList->csvfile = fopen(pathtofirstlist, "r+");
    if (NULL == MyList->csvfile) {
        printf(ERR_MALLOC);
        return MyList;
    }

    MyList->csvpath = malloc( strlen(pathtofirstlist) + 1 * sizeof(char) );
    
    if (NULL == MyList->csvpath)
    {
        printf(ERR_MALLOC);
        return NULL;
    }
    
    strncpy(MyList->csvpath, pathtofirstlist, strlen(pathtofirstlist));

    MyList->nelements = GetListnElements(MyList->csvfile) + 1;
    free(pathtofirstlist);
    return MyList;
}

listelement* LoadListElement(list* MyList) {
    listelement* tmp = NULL;
    
    tmp = calloc( 1, sizeof(listelement) );
    if (NULL == tmp) {
        printf(ERR_MALLOC);
        return NULL;
    }
    fscanf(MyList->csvfile, "%i;%i;", &tmp->ID, &tmp->priority);
    if (!(tmp->ID))
    {
        free(tmp);
        return NULL;
    }
    
    int textlen = 0;

    char tmp2;
    while ((tmp2 = fgetc(MyList->csvfile)) != ';' && tmp2 != EOF)
    {
        textlen++;
    }

    tmp->text = calloc( textlen +1, sizeof(char) );
    fseek(MyList->csvfile, (-textlen)-1, SEEK_CUR);

    fgets(tmp->text, textlen+1, MyList->csvfile);
    fscanf(MyList->csvfile, ";%i;", &tmp->done);

    tmp->next = NULL;
    return tmp;
}

void DisplayList(const list* MyList) {
    /** -- ASCII: Box Drawing Characters -- */ 
    /** INFO: This design works fine with atleast 68 columns */
    listelement* current;
    current = MyList->head;
    
    printf("┌───┬────┬───────────────────────────────────────────────────┬─────┐\n");
    
    // Header row
    printf("│ %-2s│ %-3s│ %-50s│ %-3s│\n", "ID", "Pr", "Task", "Done");

    // Header and rows separator
    printf("├───┼────┼───────────────────────────────────────────────────┼─────┤\n");
    printf("│   │    │                                                   │     │\n");
    // Data rows
    while (current != NULL)
    {
        printf("│ %02i│ %02i │ %s%-50s%s│ %6s │\n", current->ID, current->priority, ANSI_GRAPHICS_BOLD ANSI_COLOR_FGCYAN, current->text, ANSI_COLOR_RESET, (current->done == 1) ? CHECKMARK : XMARK);
        current = current->next;
    }

    // Bottom border
    printf("│   │    │                                                   │     │\n");
    printf("└───┴────┴───────────────────────────────────────────────────┴─────┘\n\n");
}

/**
 * @brief   Uses LoadListElement() to continously load the tasks in a list in a linked list.
 * @param   MyList Current List
 * @return  head of linkedlist
 */
listelement* LoadListElements(list* MyList) {
    listelement* head = NULL;
    fseek(MyList->csvfile, 0, SEEK_SET);
    
    head = LoadListElement(MyList);
    if (NULL == head) {
        printf(ERR_MALLOC);
        return 0;
    }

    listelement* tmp = head;
    while (tmp != NULL)
    {
        tmp->next = LoadListElement(MyList);
        tmp = tmp->next;
    }

    return head;
}

int GetListnElements(FILE* listfile) {
    char tmp;
    int nelements = 0;
    while ( (tmp = fgetc(listfile)) != EOF ) {
        if (tmp == '\n') {
            nelements++;
        }
    }
    return nelements;
}

void AddNewTaskToList(list* MyList, listelement* NewTask) {
    if (NULL == MyList || NULL == NewTask) {
        if (NewTask != NULL) {
            free(NewTask->text);
            free(NewTask);
        }
        return;
    }
    NewTask->ID = ++MyList->nelements;
    NewTask->done = 0;
    NewTask->next = MyList->head;
    MyList->head = NewTask;
    SaveListState(MyList);
}

listelement* CreateNewTask(char* text) {
    listelement* tmp = NULL;
    tmp = calloc(1, sizeof(listelement));
    if (NULL == tmp) {
        printf(ERR_MALLOC);
        return NULL;
    }
    tmp->text = calloc(strlen(text) + 1, sizeof(char));

    *(text+strcspn(text, "\n")) = '\0'; // replace newline with terminator

    strncpy(tmp->text, text, strlen(text));
    
    printf("Enter priority: ");
    scanf("%2i", &tmp->priority);

    return tmp;
}

void NewTask(list* MyList) {
    printf("\033c");
    char task[TASK_MAXLEN+1];
    memset(task, 0, TASK_MAXLEN+1);
    printf("Task (max %i char): ", TASK_MAXLEN);
    
    do {
        fgets(task, TASK_MAXLEN, stdin);
    } while (task[0] == '\n');

    AddNewTaskToList(MyList,  CreateNewTask(task));
}

int GetTaskID() {
    int ID;
    scanf("%2i", &ID);
    return ID;
}

void CheckTask(list* MyList) {
    ChangeTaskDoneStatus(MyList, GetTaskID(), 1);
}

void UncheckTask(list* MyList) {
    ChangeTaskDoneStatus(MyList, GetTaskID(), 0);
}

void ChangeTaskDoneStatus(list* MyList, int ID, int mode) {
    listelement* tmp = MyList->head;

    while (tmp != NULL && ID != tmp->ID)
    {
        tmp = tmp->next;
    }

    if (NULL == tmp) return;

    tmp->done = (mode == 1) ? 1 : 0;
    SaveListState(MyList);
}

void SaveListState(list* MyList) {
    listelement* tmp = MyList->head;
    freopen(MyList->csvpath, "w+", MyList->csvfile);
    
    for (; NULL != tmp; tmp = tmp->next)
    {
        fprintf(MyList->csvfile, "%02i;%02i;%s;%1i;\n", tmp->ID, tmp->priority, tmp->text, tmp->done);
    }

    freopen(MyList->csvpath, "r+", MyList->csvfile);
}

void ListClean(list* MyList) {
    listelement* tmp = MyList->head;
    if (NULL == tmp) return;
    while (tmp != NULL)
    {
        listelement* next = tmp->next;
        free(tmp->text);
        free(tmp);
        tmp = next;
    }
    MyList->head = NULL;
}

void DeleteTask(list* MyList) {
    int ID = GetTaskID(); // Get ID of task to delete
    listelement* tmp = MyList->head;
    if (tmp == NULL) return;
    // Traverse to task
    while (tmp != NULL && tmp->ID != ID)
    {
        tmp = tmp->next;
    }
    if (NULL == tmp) return;
    // If task is head
    if (tmp == MyList->head)
    {
        MyList->head = tmp->next;
        free(tmp->text);
        free(tmp);
        SaveListState(MyList);
        return;
    }
    else
    {
        // If task is not head
        listelement *prev = MyList->head;
        while (prev->next != tmp)
        {
            prev = prev->next;
        }

        prev->next = tmp->next;
        free(tmp->text);
        free(tmp);
        SaveListState(MyList);
    }
    MyList->nelements--;
}