#include "config.h"

HAL_StatusTypeDef ReadConfig(uint8_t *cfg)
{
    HAL_StatusTypeDef res = HAL_I2C_IsDeviceReady(&hi2c2, RTC_ADDR, 1, HAL_MAX_DELAY);

    if (res == HAL_OK)
    {
        RTC_read_reg(ADDR_CFG, cfg);   
    }
    return res;
}

void WriteConfig(uint8_t cfg)
{
    RTC_write_reg(ADDR_CFG, cfg);
}
