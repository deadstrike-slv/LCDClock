#include "lcd.h"

extern I2C_HandleTypeDef hi2c2;
uint8_t buf[1] = {0};
char strl[100];
uint8_t portlcd = 0; // WTF???

void WriteByteI2CLCD(uint8_t bt)
{
    buf[0] = bt;
    HAL_I2C_Master_Transmit(&hi2c2, LCD_ADDR, buf, 1, HAL_MAX_DELAY);
}

void sendhalfbyte(uint8_t c)
{
    c = c << 4;
    e_set();
    HAL_Delay(LCD_DELAY_MS);
    WriteByteI2CLCD(portlcd | c);
    e_reset();
    HAL_Delay(LCD_DELAY_MS);
}

void sendbyte(uint8_t c, uint8_t mode)
{
    if (mode == 0)
    {
        rs_reset();
    }
    else
    {
        rs_set();
    }
    uint8_t hc = c >> 4;
    sendhalfbyte(hc);
    sendhalfbyte(c);
}

void LCD_Clear(void)
{
    sendbyte(0x01, 0);
    HAL_Delay(15);
}

void LCD_SendChar(char ch)
{
    sendbyte(ch, 1);
}

void LCD_SetPos(uint8_t x, uint8_t y)
{
    switch (y)
    {
    /***** DDRAM adresses
         * 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F
         * 40 41 42 43 44 45 46 47 48 49 4A 4B 4C 4D 4E 4F
         * 
         * */
    case 0:
        sendbyte(x | 0x80, 0);
        HAL_Delay(1);
        break;
    case 1:
        sendbyte((0x40 + x) | 0x80, 0);
        HAL_Delay(1);
        break;
    }
}

void LCD_Init(void)
{
    HAL_Delay(15);
    sendhalfbyte(0x03);
    HAL_Delay(4);
    sendhalfbyte(0x03);
    HAL_Delay(1);
    sendhalfbyte(0x03);
    HAL_Delay(1);
    sendhalfbyte(0x02);
    HAL_Delay(1);
    sendbyte(0x28, 0); //4бит-режим (DL=0) и 2 линии (N=1)
    HAL_Delay(1);
    sendbyte(0x0C, 0); //включаем изображение на дисплее (D=1), курсоры никакие не включаем (C=0, B=0)
    HAL_Delay(1);
    sendbyte(0x6, 0); //курсор (хоть он у нас и невидимый) будет двигаться влево
    HAL_Delay(1);
    set_led();   //подсветка
    set_write(); //запись
}

void LCD_SendCharAt(char ch, uint8_t x, uint8_t y)
{
    LCD_SetPos(x, y);
    LCD_SendChar(ch);
}

void LCD_SendString(char *st)
{
    uint8_t i = 0;
    while (st[i] != 0)
    {
        sendbyte((uint8_t)st[i], 1);
        i++;
    };
}

// LCD write clock data func
void LCD_ClockInit(uint8_t cfg)
{
    LCD_Clear();
    if (cfg & CFG_TEMP) {
        LCD_SendCharAt('t', 1, 0);
        LCD_SendChar('=');
    }
    
    LCD_SendCharAt(':', 10, 0); // H:M

    if (cfg & CFG_SECONDS){
        LCD_SetPos(13, 0);
        LCD_SendChar(':'); // M:S
    }
}

void LCD_WriteSecondsOrMinutes(uint8_t val, uint8_t x, uint8_t y)
{
    LCD_SetPos(x, y);
    LCD_SendChar(((val >> 4) & 0x07) + '0');
    LCD_SendChar((val & 0x0F) + '0');
}

void LCD_WriteHours24(uint8_t val, uint8_t x, uint8_t y)
{
    LCD_SetPos(x, y);
    LCD_SendChar(((val >> 4) & 0x03) + '0');
    LCD_SendChar((val & 0x0F) + '0');
}

void LCD_WriteDay(uint8_t val, uint8_t x, uint8_t y)
{
    char *day = "";
    switch (val)
    {
    case 1:
        day = "Mon";
        break;
    case 2:
        day = "Tue";
        break;
    case 3:
        day = "Wed";
        break;
    case 4:
        day = "Thu";
        break;
    case 5:
        day = "Fri";
        break;
    case 6:
        day = "Sat";
        break;
    case 7:
        day = "Sun";
        break;
    default:
        day = "Unk";
    };
    LCD_SetPos(x, y);
    LCD_SendString(day);
}

void LCD_WriteDate(uint8_t val, uint8_t x, uint8_t y)
{
    LCD_SetPos(x, y);
    LCD_SendChar(((val >> 4) & 0x03) + '0');
    LCD_SendChar((val & 0x0F) + '0');
}

void LCD_WriteMonth(uint8_t val, uint8_t x, uint8_t y, uint8_t textmode)
{
    LCD_SetPos(x, y);
    if (textmode == 1)
    {
        char *mon = "";
        switch (((val >> 4) & 0x01) * 10 + (val & 0x0F))
        {
        case 1:
            mon = "Jan";
            break;
        case 2:
            mon = "Feb";
            break;
        case 3:
            mon = "Mar";
            break;
        case 4:
            mon = "Apr";
            break;
        case 5:
            mon = "May";
            break;
        case 6:
            mon = "Jun";
            break;
        case 7:
            mon = "Jul";
            break;
        case 8:
            mon = "Aug";
            break;
        case 9:
            mon = "Sep";
            break;
        case 10:
            mon = "Oct";
            break;
        case 11:
            mon = "Now";
            break;
        case 12:
            mon = "Dec";
            break;
        default:
            mon = "Unk";
        };
        LCD_SendString(mon);
    }
    else
    {
        LCD_SendChar(((val >> 4) & 0x01) + '0');
        LCD_SendChar((val & 0x0F) + '0');
    }
}

void LCD_WriteYear(uint8_t val, uint8_t x, uint8_t y, uint8_t mode)
{
    LCD_SetPos(x, y);
    if (mode == 1)
    {
        // 4 digit mode
        if (((val >> 4) * 10 + (val & 0x0F)) > 20)
        {
            //19XX
            LCD_SendChar('1');
            LCD_SendChar('9');
        }
        else
        {
            //20XX
            LCD_SendChar('2');
            LCD_SendChar('0');
        }
    };
    LCD_SendChar((val >> 4) + '0');
    LCD_SendChar((val & 0x0F) + '0');
}

void LCD_WriteTemp(uint16_t val, uint8_t x, uint8_t y)
{
    LCD_SetPos(x, y);
    if (ds18b20_GetSign(val))
        LCD_SendChar('-');
    else
        LCD_SendChar('+');
    uint8_t t = ds18b20_ConvertDiv(val);
    uint8_t fc = (uint8_t)(t / 10);
    if (fc != 0)
        LCD_SendChar(fc + '0');
    else
        LCD_SetPos(x + 2, y);
    char sc = (uint8_t)(t % 10) + '0';
    LCD_SendChar(sc);
}
