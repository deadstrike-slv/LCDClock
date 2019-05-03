#ifndef CONFIG_H
#define CONFIG_H

#include "stm32f1xx_hal.h"
#include "rtc.h"

// using rtc as BKP
// BKP addresses
#define ADDR_CFG    0x08
/* 
* BKP register config values
* BIT 7 0
* BIT 6 0
* BIT 5 0
* BIT 4 0
* BIT 3 Temp    0-Off / 1-On
* BIT 2 Date    0-Off / 1-On
* BIT 1 Day     0-Off / 1-On
* BIT 0 Seconds 0-Off / 1-On
*/
#define ADDR_CFG   0x08
#define CFG_SECONDS 0x00000001
#define CFG_DAY     0b00000010
#define CFG_DATE    0b00000100
#define CFG_TEMP    0b00001000

HAL_StatusTypeDef ReadConfig(uint8_t *cfg);
void WriteConfig(uint8_t cfg);

#endif
