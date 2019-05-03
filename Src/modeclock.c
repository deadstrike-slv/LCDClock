#include "modeclock.h"

void InitCurrentTime(RTCDateTimeTypeDef *curr)
{
    curr->seconds = 0xFF;
    curr->minutes = 0xFF;
    curr->hours = 0xFF;
    curr->day = 0xFF;
    curr->date = 0xFF;
    curr->month = 0xFF;
    curr->year = 0xFF;
}

void UpdateClock(RTCDateTimeTypeDef *currTime, RTCDateTimeTypeDef *rtcTime, uint8_t cfg)
{
    if (currTime->seconds != rtcTime->seconds)
    {
        if (cfg & CFG_SECONDS)
        {
            currTime->seconds = rtcTime->seconds;
            LCD_WriteSecondsOrMinutes(currTime->seconds, 14, 0);
        }
        else
        {
            LCD_SendCharAt(':', 10, 0); // H:M
        }
    }

    if (currTime->minutes != rtcTime->minutes)
    {
        currTime->minutes = rtcTime->minutes;
        LCD_WriteSecondsOrMinutes(currTime->minutes, 11, 0);
    };

    if (currTime->hours != rtcTime->hours)
    {
        currTime->hours = rtcTime->hours;
        LCD_WriteHours24(currTime->hours, 8, 0);
    };

    if ((cfg & CFG_DAY) && currTime->day != rtcTime->day)
    {
        currTime->day = rtcTime->day;
        LCD_WriteDay(currTime->day, 1, 1);
    };

    if (cfg & CFG_DATE)
    {
        if (currTime->date != rtcTime->date)
        {
            currTime->date = rtcTime->date;
            LCD_WriteDate(currTime->date, 5, 1);
        };

        if (currTime->month != rtcTime->month)
        {
            currTime->month = rtcTime->month;
            LCD_WriteMonth(currTime->month, 8, 1, 1);
        };

        if (currTime->year != rtcTime->year)
        {
            currTime->year = rtcTime->year;
            LCD_WriteYear(currTime->year, 12, 1, 1);
        };
    };
}

void UpdateTemp()
{
    uint8_t dt[8];
    uint16_t raw_temper;
    ds18b20_MeasureTemperCmd(SKIP_ROM, 0);
    HAL_Delay(800);
    ds18b20_ReadStratcpad(SKIP_ROM, dt, 0);

    raw_temper = ((uint16_t)dt[1] << 8) | dt[0];
    HAL_Delay(150);

    LCD_WriteTemp(raw_temper, 3, 0);
}
