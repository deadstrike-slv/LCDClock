#ifndef MENU_H
#define MENU_H

#include "lcd.h"
// modes values
#define MODE_CLOCK 0x01
#define MODE_STOPWATCH 0x02
#define MODE_TIMER 0x03
#define MODE_SETTINGS 0x04
#define MODE_MENU 0x05

//menu entries
#define MENU_CLOCK 0x11
#define MENU_STOPWATCH 0x12
#define MENU_TIMER 0x13
#define MENU_SETTINGS 0x14
#define MENU_EXIT 0x15

#define STR_MENU_CLOCK "  Clock         "
#define STR_MENU_STOPWATCH "  Stopwatch     "
#define STR_MENU_TIMER "  Timer         "
#define STR_MENU_SETTINGS "  Settings      "
#define STR_MENU_EXIT "  Exit          "

//menu settings entries
#define SETTINGS_TIME 0x20
#define SETTINGS_DAY 0x21
#define SETTINGS_DATE 0x22
#define SETTINGS_SECONDS 0x23
#define SETTINGS_TEMP 0x24
#define SETTINGS_SAVE 0x25
#define SETTINGS_CANCEL 0x26

void MenuMainOpen(uint8_t *prev, uint8_t *sel, uint8_t *currEntry);
void MenuMainUp(uint8_t *currEntry);
void MenuMainDown(uint8_t *currEntry);

void MenuSettingsUpdateStrings(uint8_t cfg);
void MenuSettingsOpen(uint8_t *sel, uint8_t *currEntry, uint8_t cfg);
void MenuSettingsUp(uint8_t *currEntry);
void MenuSettingsDown(uint8_t *currEntry);
#endif