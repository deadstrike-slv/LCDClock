/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define INIT_TIME 0
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
RTCDateTimeTypeDef currentTime, rtcTime;
uint8_t b[1] = {0};

//for db18b20
char str1[60];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void RTC_InitTime(void);
void LCD_PostInit(void);
void LCD_WriteSecondsOrMinutes(uint8_t val, uint8_t x, uint8_t y);
void LCD_WriteHours24(uint8_t val, uint8_t x, uint8_t y);
void LCD_WriteDay(uint8_t val, uint8_t x, uint8_t y);
void LCD_WriteDate(uint8_t val, uint8_t x, uint8_t y);
void LCD_WriteMonth(uint8_t val, uint8_t x, uint8_t y, uint8_t textmode);
void LCD_WriteYear(uint8_t val, uint8_t x, uint8_t y, uint8_t mode);
void LCD_WriteTemp(uint16_t val, uint8_t x, uint8_t y);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  uint8_t status;
  uint8_t dt[8];
  uint16_t raw_temper;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  currentTime.seconds = 0xFF;
  currentTime.minutes = 0xFF;
  currentTime.hours = 0xFF;
  currentTime.day = 0xFF;
  currentTime.date = 0xFF;
  currentTime.month = 0xFF;
  currentTime.year = 0xFF;
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */
  //db1820 init
  ds18b20_port_init();
  status = ds18b20_init(SKIP_ROM);

  LCD_Init();
  HAL_Delay(100);
  LCD_PostInit();

  if (INIT_TIME){
    RTC_InitTime();
  };
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    if (!status){
        ds18b20_MeasureTemperCmd(SKIP_ROM, 0);
        HAL_Delay(800);
        ds18b20_ReadStratcpad(SKIP_ROM, dt, 0);

        raw_temper = ((uint16_t)dt[1] << 8) | dt[0];
        HAL_Delay(150);

        LCD_WriteTemp(raw_temper, 3, 0);
    }

    if (ReadRTCRegisters(&rtcTime) == HAL_OK)
    {
      if (currentTime.seconds != rtcTime.seconds)
      {
        currentTime.seconds = rtcTime.seconds;
        LCD_WriteSecondsOrMinutes(currentTime.seconds, 14, 0);
      };

      if (currentTime.minutes != rtcTime.minutes)
      {
        currentTime.minutes = rtcTime.minutes;
        LCD_WriteSecondsOrMinutes(currentTime.minutes, 11, 0);
      };

      if (currentTime.hours != rtcTime.hours)
      {
        currentTime.hours = rtcTime.hours;
        LCD_WriteHours24(currentTime.hours, 8, 0);
      };

      if (currentTime.day != rtcTime.day)
      {
        currentTime.day = rtcTime.day;
        LCD_WriteDay(currentTime.day, 1, 1);
      };

      if (currentTime.date != rtcTime.date)
      {
        currentTime.date = rtcTime.date;
        LCD_WriteDate(currentTime.date, 5, 1);
      };

      if (currentTime.month != rtcTime.month)
      {
        currentTime.month = rtcTime.month;
        LCD_WriteMonth(currentTime.month, 8, 1, 1);
      };

      if (currentTime.year != rtcTime.year)
      {
        currentTime.year = rtcTime.year;
        LCD_WriteYear(currentTime.year, 12, 1, 1);
      };
    };
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

void RTC_InitTime()
{
  RTCDateTimeTypeDef initTime;
  initTime.seconds = 0x00;
  initTime.minutes = 0x33;
  initTime.hours   = 0x13;
  initTime.day     = 0x03;
  initTime.date    = 0x24;
  initTime.month   = 0x04;
  initTime.year    = 0x19;

  WriteClockData(&initTime, 0);
}
/* USER CODE BEGIN 4 */
void LCD_PostInit()
{
  LCD_Clear();
  LCD_SetPos(1, 0);
  LCD_SendChar('t');
  LCD_SendChar('=');

  LCD_SetPos(10, 0);
  LCD_SendChar(':');
  LCD_SetPos(13, 0);
  LCD_SendChar(':');
}

void LCD_WriteSecondsOrMinutes(uint8_t val, uint8_t x, uint8_t y)
{
  LCD_SetPos(x, y);
  LCD_SendChar(((val >> 4) & 0x07) + '0');
  LCD_SendChar((val & 0x0F) + '0');
}

void LCD_WriteHours24(uint8_t val, uint8_t x, uint8_t y)
{
  LCD_SetPos(x, y);
  LCD_SendChar(((val >> 4) & 0x03) + '0');
  LCD_SendChar((val & 0x0F) + '0');
}

void LCD_WriteDay(uint8_t val, uint8_t x, uint8_t y)
{
  char *day = "";
  switch (val)
  {
  case 1:
    day = "Mon";
    break;
  case 2:
    day = "Tue";
    break;
  case 3:
    day = "Wed";
    break;
  case 4:
    day = "Thu";
    break;
  case 5:
    day = "Fri";
    break;
  case 6:
    day = "Sat";
    break;
  case 7:
    day = "Sun";
    break;
  default:
    day = "Unk";
  };
  LCD_SetPos(x, y);
  LCD_SendString(day);
}

void LCD_WriteDate(uint8_t val, uint8_t x, uint8_t y)
{
  LCD_SetPos(x, y);
  LCD_SendChar(((val >> 4) & 0x03) + '0');
  LCD_SendChar((val & 0x0F) + '0');
}

void LCD_WriteMonth(uint8_t val, uint8_t x, uint8_t y, uint8_t textmode)
{
  LCD_SetPos(x, y);
  if (textmode == 1)
  {
    char *mon = "";
    switch (((val >> 4) & 0x01) * 10 + (val & 0x0F))
    {
    case 1:
      mon = "Jan";
      break;
    case 2:
      mon = "Feb";
      break;
    case 3:
      mon = "Mar";
      break;
    case 4:
      mon = "Apr";
      break;
    case 5:
      mon = "May";
      break;
    case 6:
      mon = "Jun";
      break;
    case 7:
      mon = "Jul";
      break;
    case 8:
      mon = "Aug";
      break;
    case 9:
      mon = "Sep";
      break;
    case 10:
      mon = "Oct";
      break;
    case 11:
      mon = "Now";
      break;
    case 12:
      mon = "Dec";
      break;
    default:
      mon = "Unk";
    };
    LCD_SendString(mon);
  }
  else
  {
    LCD_SendChar(((val >> 4) & 0x01) + '0');
    LCD_SendChar((val & 0x0F) + '0');
  }
}

void LCD_WriteYear(uint8_t val, uint8_t x, uint8_t y, uint8_t mode)
{
  LCD_SetPos(x, y);
  if (mode == 1)
  {
    // 4 digit mode
    if (((val >> 4) * 10 + (val & 0x0F)) > 20)
    {
      //19XX
      LCD_SendChar('1');
      LCD_SendChar('9');
    }
    else
    {
      //20XX
      LCD_SendChar('2');
      LCD_SendChar('0');
    }
  };
  LCD_SendChar((val >> 4) + '0');
  LCD_SendChar((val & 0x0F) + '0');
}

void LCD_WriteTemp(uint16_t val, uint8_t x, uint8_t y)
{
  LCD_SetPos(x, y);
  if (ds18b20_GetSign(val))
    LCD_SendChar('-');
  else
    LCD_SendChar('+');
  uint8_t t = ds18b20_ConvertDiv(val);
  uint8_t fc = (uint8_t)(t / 10);
  if (fc != 0)
    LCD_SendChar(fc + '0');
  else
    LCD_SetPos(x + 2, y);
  char sc = (uint8_t)(t % 10) + '0';
  LCD_SendChar(sc);
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
