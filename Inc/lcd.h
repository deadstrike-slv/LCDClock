#ifndef LCD_H
#define LCD_H

#include "i2c.h"
#include "ds18b20.h"
#include "config.h"

#define LCD_ADDR (0x27 << 1)

#define PIN_RS (1 << 0)
#define PIN_RW (1 << 1)
#define PIN_EN (1 << 2)
#define BACKLIGHT (1 << 3)

#define LCD_DELAY_MS 5

#define e_set() WriteByteI2CLCD(portlcd |= PIN_EN)      //set E to 1
#define e_reset() WriteByteI2CLCD(portlcd &= ~PIN_EN)   //set E to 0
#define rs_set() WriteByteI2CLCD(portlcd |= PIN_RS)     //set RS to 1
#define rs_reset() WriteByteI2CLCD(portlcd &= ~PIN_RS)  //set RS to 0
#define set_led() WriteByteI2CLCD(portlcd |= BACKLIGHT) //set BACKLIGHT
#define set_write() WriteByteI2CLCD(portlcd &= ~PIN_RW) //set mode to write
/*
HAL_StatusTypeDef LCD_SendInternal(uint8_t lcd_addr, uint8_t data, uint8_t flags);

void LCD_SendCommand(uint8_t lcd_addr, uint8_t cmd);

void LCD_SendData(uint8_t lcd_addr, uint8_t data);

void LCD_SendString(uint8_t lcd_addr, char *str);

void LCD_Init(uint8_t lcd_addr);
*/

void LCD_Clear(void);
void LCD_SendChar(char ch);
void LCD_SetPos(uint8_t x, uint8_t y);
void LCD_SendCharAt(char ch, uint8_t x, uint8_t y);
void LCD_Init(void);
void LCD_SendString(char *st);

//lcd write data functions
void LCD_ClockInit(uint8_t cfg);
void LCD_WriteSecondsOrMinutes(uint8_t val, uint8_t x, uint8_t y);
void LCD_WriteHours24(uint8_t val, uint8_t x, uint8_t y);
void LCD_WriteDay(uint8_t val, uint8_t x, uint8_t y);
void LCD_WriteDate(uint8_t val, uint8_t x, uint8_t y);
void LCD_WriteMonth(uint8_t val, uint8_t x, uint8_t y, uint8_t textmode);
void LCD_WriteYear(uint8_t val, uint8_t x, uint8_t y, uint8_t mode);
void LCD_WriteTemp(uint16_t val, uint8_t x, uint8_t y);

#endif
