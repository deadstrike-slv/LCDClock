#include "config.h"

void ReadConfig(ConfigTypeDef *cfg)
{
    RTC_read_reg(ADDR_CFG_TEMP,     &(cfg->temp));
    RTC_read_reg(ADDR_CFG_DAY,      &(cfg->day));
    RTC_read_reg(ADDR_CFG_DATE,     &(cfg->date));
    RTC_read_reg(ADDR_CFG_SECONDS,  &(cfg->seconds));
}

void WriteConfig(ConfigTypeDef *cfg)
{
    RTC_write_reg(ADDR_CFG_TEMP,     cfg->temp);
    RTC_write_reg(ADDR_CFG_DAY,      cfg->day);
    RTC_write_reg(ADDR_CFG_DATE,     cfg->date);
    RTC_write_reg(ADDR_CFG_SECONDS,  cfg->seconds);
}
