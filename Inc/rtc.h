#ifndef RTC_H
#define RTC_H

#include "i2c.h"

#define RTC_ADDR (0x68 << 1) // 011 1000

#define SEC_REG 0x00
#define MIN_REG 0x01
#define HOUR_REG 0x02
#define DAY_REG 0x03
#define DATE_REG 0x04
#define MON_REG 0x05
#define YEAR_REG 0x06

typedef struct
{                             //  DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
    volatile uint8_t seconds; //  CH  X   X   X | X   X   X   X   00-59
    volatile uint8_t minutes; //  0   X   X   X | X   X   X   X   00-59
    volatile uint8_t hours;   //  0   2/4 A/P X | X   X   X   X   1-12+AM/PM or 00-23

    volatile uint8_t day;     //  0   0   0   0   0   X   X   X    01-07

    volatile uint8_t date;    //  0   0   X   X | X   X   X   X    01-31
    volatile uint8_t month;   //  0   0   0   X | X   X   X   X    01-12
    volatile uint8_t year;    //  X   X   X   X | X   X   X   X    00-99
} RTCDateTimeTypeDef;

HAL_StatusTypeDef ReadRTCRegisters(RTCDateTimeTypeDef *data);

void RTC_read_reg(uint16_t MemAddr, uint8_t *Buffer);
void RTC_write_reg(uint16_t MemAddr, uint8_t value);
void WriteClockData(RTCDateTimeTypeDef *data, uint8_t bit_ch);

#endif
