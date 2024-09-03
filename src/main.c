#include "include/getch.h"
#include "include/ANSI.h"
#include "include/cmd.h"
#include "include/todolist.h"

#include <stdlib.h>

#define RELATIVEPATH_LISTS      "./tododir"

// make; ./todo
// gcc -c src/main.c src/todolist.c src/cmd.c src/getch.c;gcc main.o todolist.o cmd.o getch.o -o todo.exe; ./todo.exe

int main() {
#ifdef _WIN32
    system("CHCP 65001");
#endif
    printf(ANSI_ERASE_SCREEN2);

    list* MyList = NULL;
    MyList = LoadList(RELATIVEPATH_LISTS);
    if (NULL == MyList)
    {
        printf("Failed to load list!\n");
        return EXIT_FAILURE;
    }
    MyList->head = LoadListElements(MyList);

    CMDS MyCommand = CMD_NONE;

    while (MyCommand != CMD_EXIT)
    {
        printf(ANSI_ERASE_SCREEN2);
        DisplayList(MyList);
        cmd_settings();
        MyCommand = getch();
        putchar(MyCommand);
        cmd_eval(MyCommand, MyList);
        cmd_exit(); 
    }
    
    SaveListState(MyList);
    ListClean(MyList);
    fclose(MyList->csvfile);
    free(MyList->csvpath);
    free(MyList);
    printf(ANSI_ERASE_SCREEN2);
    return EXIT_SUCCESS;
}