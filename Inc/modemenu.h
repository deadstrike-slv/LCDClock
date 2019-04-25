#ifndef MENU_MODE_H
#define MENU_MODE_H

#include "lcd.h"

//user code here
#define MENU_CLOCK      1
#define MENU_STOPWATCH  2
#define MENU_TIMER      3
#define MENU_SETTINGS   4
#define MENU_CLOSE      5

uint8_t currMenuEntry = 0;

void MenuOpen(void);
void MenuMoveUp(void);
void MenuMoveDown(void);
void MenuClick(void);

#endif


