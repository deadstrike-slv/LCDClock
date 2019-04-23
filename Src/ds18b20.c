#include "ds18b20.h"

__STATIC_INLINE void DelayMicro(__IO uint32_t micros)
{
    micros *= (SystemCoreClock / 1000000) / 9;
    /* Wait till done */
    while (micros--)
        ;
}

void ds18b20_port_init(void)
{
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11);
    GPIOA->CRL |= GPIO_CRL_MODE0;
    GPIOA->CRL |= GPIO_CRL_CNF0_0;
    GPIOA->CRL &= ~GPIO_CRL_CNF0_1;
}

uint8_t ds18b20_Reset(void)
{
    uint16_t status;
    GPIOA->ODR &= ~GPIO_ODR_ODR0; //низкий уровень
    DelayMicro(485);               //задержка как минимум на 480 микросекунд

    GPIOA->ODR |= GPIO_ODR_ODR0; //высокий уровень
    DelayMicro(65);               //задержка как минимум на 60 микросекунд

    status = GPIOA->IDR & GPIO_IDR_IDR0; //проверяем уровень
    DelayMicro(500);                      //задержка как минимум на 480 микросекунд

    //(на всякий случай подождём побольше, так как могут быть неточности в задержке)
    return (status ? 1 : 0); //вернём результат
}

uint8_t ds18b20_ReadBit(void)
{
    uint8_t bit = 0;
    GPIOA->ODR &= ~GPIO_ODR_ODR0; //низкий уровень
    DelayMicro(2);
    GPIOA->ODR |= GPIO_ODR_ODR0; //высокий уровень
    DelayMicro(13);
    bit = (GPIOA->IDR & GPIO_IDR_IDR0 ? 1 : 0); //проверяем уровень
    DelayMicro(45);
    return bit;
}

uint8_t ds18b20_ReadByte(void)
{
    uint8_t data = 0;
    for (uint8_t i = 0; i <= 7; i++)
        data += ds18b20_ReadBit() << i;
    return data;
}

void ds18b20_WriteBit(uint8_t bit)
{
    GPIOA->ODR &= ~GPIO_ODR_ODR0; //низкий уровень
    DelayMicro(bit ? 3 : 65);
    GPIOA->ODR |= GPIO_ODR_ODR0; //высокий уровень
    DelayMicro(bit ? 65 : 3);
}

void ds18b20_WriteByte(uint8_t dt)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        ds18b20_WriteBit(dt >> i & 1);
        //Delay Protection
        DelayMicro(5);
    }
}

uint8_t ds18b20_init(uint8_t mode)
{
    if (ds18b20_Reset())
        return 1;
    if (mode == SKIP_ROM)
    {
        //SKIP ROM
        ds18b20_WriteByte(0xCC);
        //WRITE SCRATCHPAD
        ds18b20_WriteByte(0x4E);
        //TH REGISTER 100 градусов
        ds18b20_WriteByte(0x64);
        //TL REGISTER -30 градусов
        ds18b20_WriteByte(0x9E);
        //Resolution 12 bit
        ds18b20_WriteByte(RESOLUTION_12BIT);
    }
    return 0;
}

void ds18b20_MeasureTemperCmd(uint8_t mode, uint8_t DevNum)
{
    ds18b20_Reset();
    if (mode == SKIP_ROM)
    {
        //SKIP ROM
        ds18b20_WriteByte(0xCC);
    }
    //CONVERT T
    ds18b20_WriteByte(0x44);
}

void ds18b20_ReadStratcpad(uint8_t mode, uint8_t *Data, uint8_t DevNum)
{
    uint8_t i;
    ds18b20_Reset();
    if (mode == SKIP_ROM)
    {
        //SKIP ROM
        ds18b20_WriteByte(0xCC);
    }
    //READ SCRATCHPAD
    ds18b20_WriteByte(0xBE);
    for (i = 0; i < 8; i++)
    {
        Data[i] = ds18b20_ReadByte();
    }
}

uint8_t ds18b20_GetSign(uint16_t dt)
{
    //Проверим 11-й бит
    if (dt & (1 << 11))
        return 1;
    else
        return 0;
}

uint8_t ds18b20_ConvertDiv(uint16_t dt)
{
    return ((dt & 0x07FF) >> 4); //отборосим знаковые и дробные биты
}
