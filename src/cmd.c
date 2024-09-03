#include "include/cmd.h"
#include "include/ANSI.h"
#include "include/getch.h"
#include <stdio.h>

void cmd_settings() {
    printf("%s", CMD_POS);
    printf("%s", ANSI_CURSOR_1UP);
    printf(ANSI_COLOR_FGRED);
    printf("'%c' for help", CMD_HELP);
    printf("%s", ANSI_CURSOR_DOWN(1));
    printf("%s", CMD_POS);
    printf("%s ", CMD_INDICATOR);
}

void cmd_help() {
    printf("%s", ANSI_PRIVATE_SAVESCREEN);
    printf(ANSI_ERASE_SCREEN2);
    printf("\b[?]\t→\thelp\n");
    printf("[+]\t→\tnew task\n");
    printf("[-]\t→\tremove task\n");
    printf("[.]\t→\tclear\n");
    printf("[#]\t→\tCheck off task\n");
    printf("[*]\t→\tUncheck task\n");
    printf("[q]\t→\texit\n");
    printf("\nEnter to continue ");
    getch();
    printf("%s", ANSI_PRIVATE_RESTORESCREEN);
}

void cmd_exit() {
    printf("%s", ANSI_COLOR_RESET);
    printf("%s", ANSI_CURSOR_HOME);
}

void cmd_clear() {
    printf(ANSI_ERASE_SCREEN2);
}

void cmd_eval(CMDS command, list* MyList) {
    switch (command)
    {
    case CMD_HELP:
        cmd_help();
        break;
    case CMD_NEWTASK:
        NewTask(MyList);
        break;
    case CMD_REMOVETASK:
        DeleteTask(MyList);
        break;
    case CMD_CLEARS:
        cmd_clear();
        break;
    case CMD_CHECKTASK:
        CheckTask(MyList);
        break;
    case CMD_UNCHECKTASK:
        UncheckTask(MyList);
        break;
    default:
        break;
    }
}