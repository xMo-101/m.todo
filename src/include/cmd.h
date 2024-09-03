#include "ANSI.h"
#include "todolist.h"

#ifndef CMD_H
#define CMD_H

#define CMD_INDICATOR   "mo.todo>"
#define CMD_POS         (ANSI_CURSOR_XY(999, 0))

typedef enum CLI {
    CMD_NONE            = (-999),
    CMD_HELP            = '?',
    CMD_NEWTASK         = '+',
    CMD_REMOVETASK      = '-',
    CMD_CLEARS          = '.',
    CMD_CHECKTASK       = '#',
    CMD_UNCHECKTASK     = '*',

    CMD_EXIT            = 'q',
}   CMDS;

void cmd_settings();
void cmd_exit();
void cmd_help();
void cmd_clear();
void cmd_eval(CMDS command, list* MyList);

#endif // CMD_H