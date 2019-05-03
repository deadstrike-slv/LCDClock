#include "menu.h"

/*
    --- MAIN MENU --- 
     * CLOCK
     * STOPWATCH
     * TIMER
     * SETTINGS
     * CLOSE
*/
void MenuMainOpen(uint8_t *prev, uint8_t *sel, uint8_t *currEntry)
{
    (*prev) = (*sel);
    (*sel) = MODE_MENU;

    char *entry = "> CLOCK         ";
    LCD_SendString(entry);
    entry = "  STOPWATCH     ";
    LCD_SendString(entry);
    (*currEntry) = MENU_CLOCK;
}

void MenuMainUp(uint8_t *currEntry)
{
    char *entry1 = "";
    char *entry2 = "";
    switch (*currEntry)
    {
    case MENU_CLOCK:
        entry1 = STR_MENU_EXIT;
        entry1[0] = '>';
        entry2 = STR_MENU_CLOCK;
        (*currEntry) = MENU_EXIT;
        break;
    case MENU_STOPWATCH:
        entry1 = STR_MENU_CLOCK;
        entry1[0] = '>';
        entry2 = STR_MENU_STOPWATCH;
        (*currEntry) = MENU_CLOCK;
        break;
    case MENU_TIMER:
        entry1 = STR_MENU_STOPWATCH;
        entry1[0] = '>';
        entry2 = STR_MENU_TIMER;
        (*currEntry) = MENU_STOPWATCH;
        break;
    case MENU_SETTINGS:
        entry1 = STR_MENU_TIMER;
        entry1[0] = '>';
        entry2 = STR_MENU_SETTINGS;
        (*currEntry) = MENU_TIMER;
        break;
    case MENU_EXIT:
        entry1 = STR_MENU_SETTINGS;
        entry1[0] = '>';
        entry2 = STR_MENU_EXIT;
        (*currEntry) = MENU_SETTINGS;
        break;
    };
    LCD_SetPos(0, 0);
    LCD_SendString(entry1);
    LCD_SetPos(0, 1);
    LCD_SendString(entry2);
}

void MenuMainDown(uint8_t *currEntry)
{
    char *entry1 = "";
    char *entry2 = "";
    switch (*currEntry)
    {
    case MENU_CLOCK:
        entry1 = STR_MENU_STOPWATCH;
        entry1[0] = '>';
        entry2 = STR_MENU_TIMER;
        (*currEntry) = MENU_STOPWATCH;
        break;
    case MENU_STOPWATCH:
        entry1 = STR_MENU_TIMER;
        entry1[0] = '>';
        entry2 = STR_MENU_SETTINGS;
        (*currEntry) = MENU_TIMER;
        break;
    case MENU_TIMER:
        entry1 = STR_MENU_SETTINGS;
        entry1[0] = '>';
        entry2 = STR_MENU_EXIT;
        (*currEntry) = MENU_SETTINGS;
        break;
    case MENU_SETTINGS:
        entry1 = STR_MENU_EXIT;
        entry1[0] = '>';
        entry2 = STR_MENU_CLOCK;
        (*currEntry) = MENU_EXIT;
        break;
    case MENU_EXIT:
        entry1 = STR_MENU_CLOCK;
        entry1[0] = '>';
        entry2 = STR_MENU_STOPWATCH;
        (*currEntry) = MENU_CLOCK;
        break;
    };
    LCD_SetPos(0, 0);
    LCD_SendString(entry1);
    LCD_SetPos(0, 1);
    LCD_SendString(entry2);
}

/*
    --- SETTINGS MENU --- 
     * SET TIME
     * Day      On/Off
     * Date     On/Off
     * Seconds  On/Off
     * Temp     On/Off
     * SAVE     On/Off
     * CANCEL   On/Off
*/

char *entrySetTime = "  SET TIME      ";
volatile char *entryDay = "";
volatile char *entryDate = "";
volatile char *entrySeconds = "";
volatile char *entryTemp = "";
char *entrySave = "  SAVE          ";
char *entryCancel = "  CANCEL        ";

void MenuSettingsUpdateStrings(uint8_t cfg)
{
    entryDay = cfg & CFG_DAY ? "  Day       ON  " : "  Day       OFF ";
    entryDate = cfg & CFG_DATE ? "  Date      ON  " : "  Date      OFF ";
    entrySeconds = cfg & CFG_SECONDS ? "  Seconds   ON  " : "  Seconds   OFF ";
    entryTemp = cfg & CFG_TEMP ? "  Temp      ON  " : "  Temp      OFF ";
}

void MenuSettingsOpen(uint8_t *sel, uint8_t *currEntry, uint8_t cfg)
{
    //TODO correct strings in menu entries
    (*sel) = MODE_SETTINGS;
    UpdateStrings(cfg);

    char *entry1 = entrySetTime;
    entry1[0] = '>';
    char *entry2 = entryDay;

    LCD_SendString(entry1);
    LCD_SendString(entry2);
    (*currEntry) = SETTINGS_TIME;
}

void MenuSettingsUp(uint8_t *currEntry)
{
    char *entry1 = "";
    char *entry2 = "";
    switch (*currEntry & 0x0F)
    {
    case SETTINGS_TIME:
        entry1 = entryCancel;
        entry1[0] = '>';
        entry2 = entrySetTime;
        (*currEntry) = SETTINGS_CANCEL;
        break;
    case SETTINGS_DAY:
        entry1 = entrySetTime;
        entry1[0] = '>';
        entry2 = entryDay;
        (*currEntry) = SETTINGS_TIME;
        break;
    case SETTINGS_DATE:
        entry1 = entryDay;
        entry1[0] = '>';
        entry2 = entryDate;
        (*currEntry) = SETTINGS_DAY;
        break;
    case SETTINGS_SECONDS:
        entry1 = entryDate;
        entry1[0] = '>';
        entry2 = entrySeconds;
        (*currEntry) = SETTINGS_DATE;
        break;
    case SETTINGS_TEMP:
        entry1 = entrySeconds;
        entry1[0] = '>';
        entry2 = entryTemp;
        (*currEntry) = SETTINGS_SECONDS;
        break;
    case SETTINGS_SAVE:
        entry1 = entryTemp;
        entry1[0] = '>';
        entry2 = entrySave;
        (*currEntry) = SETTINGS_TEMP;
        break;
    case SETTINGS_CANCEL:
        entry1 = entrySave;
        entry1[0] = '>';
        entry2 = entryCancel;
        (*currEntry) = SETTINGS_SAVE;
        break;
    };
    LCD_SetPos(0, 0);
    LCD_SendString(entry1);
    LCD_SetPos(0, 1);
    LCD_SendString(entry2);
}

void MenuSettingsDown(uint8_t *currEntry)
{
    char *entry1 = "";
    char *entry2 = "";
    switch (*currEntry)
    {
    case SETTINGS_TIME:
        entry1 = entryDay;
        entry1[0] = '>';
        entry2 = entryDate;
        (*currEntry) = SETTINGS_DAY;
        break;
    case SETTINGS_DAY:
        entry1 = entryDate;
        entry1[0] = '>';
        entry2 = entrySeconds;
        (*currEntry) = SETTINGS_DATE;
        break;
    case SETTINGS_DATE:
        entry1 = entrySeconds;
        entry1[0] = '>';
        entry2 = entryTemp;
        (*currEntry) = SETTINGS_SECONDS;
        break;
    case SETTINGS_SECONDS:
        entry1 = entryTemp;
        entry1[0] = '>';
        entry2 = entrySave;
        (*currEntry) = SETTINGS_TEMP;
        break;
    case SETTINGS_TEMP:
        entry1 = entrySave;
        entry1[0] = '>';
        entry2 = entryCancel;
        (*currEntry) = SETTINGS_SAVE;
        break;
    case SETTINGS_SAVE:
        entry1 = entryCancel;
        entry1[0] = '>';
        entry2 = entrySetTime;
        (*currEntry) = SETTINGS_CANCEL;
        break;
    case SETTINGS_CANCEL:
        entry1 = entrySetTime;
        entry1[0] = '>';
        entry2 = entryDay;
        (*currEntry) = SETTINGS_TIME;
        break;
    };
    LCD_SetPos(0, 0);
    LCD_SendString(entry1);
    LCD_SetPos(0, 1);
    LCD_SendString(entry2);
}