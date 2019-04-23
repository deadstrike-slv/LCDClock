#ifndef DS18B20_H_

#define DS18B20_H_
//--------------------------------------------------

#define SKIP_ROM 0
#define NO_SKIP_ROM 1

#define RESOLUTION_9BIT 0x1F  // (0001 1111)
#define RESOLUTION_10BIT 0x3F // (0011 1111)
#define RESOLUTION_11BIT 0x5F // (0101 1111)
#define RESOLUTION_12BIT 0x7F // (0111 1111)

#include "stm32f1xx_hal.h"

void ds18b20_port_init(void);
uint8_t ds18b20_init(uint8_t mode);
void ds18b20_MeasureTemperCmd(uint8_t mode, uint8_t DevNum);
void ds18b20_ReadStratcpad(uint8_t mode, uint8_t *Data, uint8_t DevNum);
uint8_t ds18b20_GetSign(uint16_t dt);
uint8_t ds18b20_ConvertDiv(uint16_t dt);
//--------------------------------------------------

#endif /* DS18B20_H_ */
