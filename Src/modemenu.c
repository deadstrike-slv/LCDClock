#include "modemenu.h"

void MenuOpen()
{
    LCD_Clear();
    LCD_SetPos(0, 0);
    LCD_SendChar('>');

    currMenuEntry = MENU_CLOCK;

    LCD_SetPos(2, 0);
    char *entry = "CLOCK";
    LCD_SendString(entry);

    LCD_SetPos(2, 1);
    entry = "STOPWATCH";
    LCD_SendString(entry);
}

// TODO upper and lower pos
void MenuMoveUp()
{
    char *entry1 = "";
    char *entry2 = "";
    switch (currMenuEntry)
    {
    case MENU_CLOCK:
        entry1 = "  SETTINGS      ";
        entry2 = "> CLOSE         ";
        currMenuEntry = MENU_CLOSE;
        break;
    case MENU_STOPWATCH:
        entry1 = "> CLOCK         ";
        entry2 = "  STOPWATCH     ";
        currMenuEntry = MENU_CLOCK;
        break;
    case MENU_TIMER:
        entry1 = "> STOPWATCH     ";
        entry2 = "  TIMER         ";
        currMenuEntry = MENU_STOPWATCH;
        break;
    case MENU_SETTINGS:
        entry1 = "> TIMER         ";
        entry2 = "  SETTINGS      ";
        currMenuEntry = MENU_TIMER;
        break;
    case MENU_CLOSE:
        entry1 = "> SETTINGS      ";
        entry2 = "  CLOSE         ";
        currMenuEntry = MENU_SETTINGS;
        break;
    };
    LCD_SetPos(0, 0);
    LCD_SendString(entry1);
    LCD_SetPos(0, 1);
    LCD_SendString(entry2);
}

void MenuMoveDown()
{
    char *entry1 = "";
    char *entry2 = "";
    switch (currMenuEntry)
    {
    case MENU_CLOCK:
        entry1 = "  CLOCK         ";
        entry2 = "> STOPWATCH     ";
        currMenuEntry = MENU_STOPWATCH;
        break;
    case MENU_STOPWATCH:
        entry1 = "  STOPWATCH     ";
        entry2 = "> TIMER         ";
        currMenuEntry = MENU_TIMER;
        break;
    case MENU_TIMER:
        entry1 = "  TIMER         ";
        entry2 = "> SETTINGS      ";
        currMenuEntry = MENU_SETTINGS;
        break;
    case MENU_SETTINGS:
        entry1 = "  SETTINGS      ";
        entry2 = "> CLOSE         ";
        currMenuEntry = MENU_CLOSE;
        break;
    case MENU_CLOSE:
        entry1 = "> CLOCK         ";
        entry2 = "  STOPWATCH     ";
        currMenuEntry = MENU_CLOCK;
        break;
    };
    LCD_SetPos(0, 0);
    LCD_SendString(entry1);
    LCD_SetPos(0, 1);
    LCD_SendString(entry2);
}

void MenuClick()
{
    switch (currMenuEntry)
    {
    case MENU_CLOCK:
        //switch mode to clock
        break;
    case MENU_STOPWATCH:
        //switch mode to stopwatch
        break;
    case MENU_TIMER:
        //switch mode to stopwatch
        break;
    case MENU_SETTINGS:
        //switch mode to stopwatch
        break;
    case MENU_CLOSE:
        //switch mode to current
        break;
    };
    currMenuEntry = 0;
}