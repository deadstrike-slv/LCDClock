#ifndef CLOCK_MODE_H
#define CLOCK_MODE_H

#include "rtc.h"
#include "ds18b20.h"
#include "lcd.h"
#include "config.h"

/**
 * @brief Initialize struct of current timestamp
 * @note Initialize struct of current timestamp with default 0xFF value. 
 * Comparing with this value further will be guarantly replaced with real value
 * @param  *curr: pointer to structure
 * @retval None
 */
void InitCurrentTime(RTCDateTimeTypeDef *curr);

/**
 * @brief  Update Clock data on LCD
 * @note   Updateting clock data on LCD with selected configures. 
 * This func compare current value with data from RTC-module. If it diff -> update data on LCD.
 * @param  *currTime: struct with currently setted data on LCD 
 * @param  *rtcTime: struct recieved from RTC-module
 * @param  *cfg: struct with selected configures (ON/OFF some param to display)
 * @retval None
 */
void UpdateClock(RTCDateTimeTypeDef *currTime, RTCDateTimeTypeDef *rtcTime, ConfigTypeDef *cfg);

/**
 * @brief  Update Temp data on LCD
 * @note   Updating temperature data on LCD with selected configures. Read data and write it to LCD 
 * @retval None
 */
void UpdateTemp();

#endif
