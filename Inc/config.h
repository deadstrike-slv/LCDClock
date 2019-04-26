#ifndef CONFIG_H
#define CONFIG_H

#include "stm32f1xx_hal.h"
#include "rtc.h"

// using rtc as BKP
// BKP addresses
#define ADDR_CFG_TEMP   0x08
#define ADDR_CFG_DAY    0x09
#define ADDR_CFG_DATE   0x0A
#define ADDR_CFG_SECONDS 0x0B

typedef struct {
    volatile uint8_t temp;          // 0-OFF 1-ON
    volatile uint8_t day;           // 0-OFF 1-ON
    volatile uint8_t date;          // 0-OFF 1-ON
    volatile uint8_t seconds;       // 0-OFF 1-ON
} ConfigTypeDef;

void ReadConfig(ConfigTypeDef *cfg);
void WriteConfig(ConfigTypeDef *cfg);

#endif
