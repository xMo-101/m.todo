#ifndef GETCH_H
#define GETCH_H

#ifdef _WIN32
#include <conio.h>
#else
int getch(void);
#endif // _WIN32
#endif // GETCH_H