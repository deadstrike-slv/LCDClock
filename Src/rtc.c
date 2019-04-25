#include "rtc.h"

uint8_t local_buf[1] = {0};

void RTC_read_reg(uint16_t MemAddr, uint8_t *Buffer)
{
    HAL_I2C_Mem_Read(&hi2c2, RTC_ADDR, MemAddr, 1, Buffer, 1, HAL_MAX_DELAY);
}

void RTC_write_reg(uint16_t MemAddr, uint8_t value)
{
    local_buf[0] = value;
    HAL_I2C_Mem_Write(&hi2c2, RTC_ADDR, MemAddr, 1, local_buf, 1, HAL_MAX_DELAY);
}

HAL_StatusTypeDef ReadRTCRegisters(RTCDateTimeTypeDef *data)
{
    HAL_StatusTypeDef res;
    res = HAL_I2C_IsDeviceReady(&hi2c2, RTC_ADDR, 1, HAL_MAX_DELAY);

    if (res == HAL_OK)
    {
        RTC_read_reg(SEC_REG, &(data->seconds));
        RTC_read_reg(MIN_REG, &(data->minutes));
        RTC_read_reg(HOUR_REG, &(data->hours));
        RTC_read_reg(DAY_REG, &(data->day));
        RTC_read_reg(DATE_REG, &(data->date));
        RTC_read_reg(MON_REG, &(data->month));
        RTC_read_reg(YEAR_REG, &(data->year));
    }
    return res;
}

//TODO some shit
void WriteClockData(RTCDateTimeTypeDef *data, uint8_t bit_ch)
{
    RTC_write_reg(SEC_REG, (bit_ch << 7) | (data->seconds & 0x7F));
    RTC_write_reg(MIN_REG, data->minutes);
    RTC_write_reg(HOUR_REG, data->hours);
    RTC_write_reg(DAY_REG, data->day);
    RTC_write_reg(DATE_REG, data->date);
    RTC_write_reg(MON_REG, data->month);
    RTC_write_reg(YEAR_REG, data->year);
}

void RTC_WriteTimeToRAM(RTCDateTimeTypeDef *data, uint8_t bit_ch)
{
    RTCDateTimeTypeDef initTime;
    initTime.seconds = 0x00;
    initTime.minutes = 0x33;
    initTime.hours = 0x13;
    initTime.day = 0x03;
    initTime.date = 0x24;
    initTime.month = 0x04;
    initTime.year = 0x19;

    WriteClockData(&initTime, 0);
}


