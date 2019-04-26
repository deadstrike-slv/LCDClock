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
#include "lcd.h"
#include "rtc.h"
#include "ds18b20.h"
#include "config.h"
#include "modeclock.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//menu entries
#define MENU_CLOCK 1
#define MENU_STOPWATCH 2
#define MENU_TIMER 3
#define MENU_SETTINGS 4
#define MENU_CLOSE 5

// modes values
#define MODE_CLOCK 1
#define MODE_STOPWATCH 2
#define MODE_TIMER 3
#define MODE_SETTINGS 4
#define MODE_MENU 5
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
RTCDateTimeTypeDef currentTime, rtcTime;
ConfigTypeDef globalCfg;
uint8_t b[1] = {0};
volatile uint8_t selectedMode;

volatile uint8_t btnMenuCLicked;
volatile uint8_t btnLeftCLicked;
volatile uint8_t btnRightCLicked;

uint8_t currMenuEntry = 0;
uint8_t prev_mode;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

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

    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_I2C2_Init();
    /* USER CODE BEGIN 2 */
    LCD_Init();
    HAL_Delay(100);

    ds18b20_port_init();
    uint8_t ds18b20status = ds18b20_init(SKIP_ROM);

    ReadConfig(&globalCfg);

    InitCurrentTime(&currentTime);
    LCD_ClockInit(&globalCfg);

    selectedMode = MODE_CLOCK;
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    btnMenuCLicked = 0;
    btnLeftCLicked = 0;
    btnRightCLicked = 0;
    while (1)
    {
        if (selectedMode == MODE_CLOCK)
        {
            if (ReadRTCRegisters(&rtcTime) == HAL_OK)
            {
                UpdateClock(&currentTime, &rtcTime, &globalCfg);
            }
            if (globalCfg.temp && ds18b20status)
            {
                UpdateTemp();
            }
        }
        /* USER CODE END WHILE */
        if (btnMenuCLicked)
        {
            if (selectedMode == MODE_MENU) // if menu is already opened
            {
                //Open current menu entry
                switch (currMenuEntry)
                {
                case MENU_CLOCK:
                    LCD_Clear();
                    InitCurrentTime(&currentTime);
                    LCD_ClockInit(&globalCfg);
                    selectedMode = MODE_CLOCK;
                    break;
                case MENU_STOPWATCH:
                    //TODO switch mode to stopwatch
                    selectedMode = MODE_STOPWATCH;
                    break;
                case MENU_TIMER:
                    //TODO switch mode to stopwatch
                    break;
                case MENU_SETTINGS:
                    //TODO switch mode to stopwatch
                    break;
                case MENU_CLOSE:
                    //TODO
                    LCD_Clear();
                    selectedMode = prev_mode;
                    break;
                };
                currMenuEntry = 0;
            }
            else
            {
                //Open menu
                prev_mode = selectedMode;
                selectedMode = MODE_MENU;

                LCD_Clear();
                LCD_SetPos(0, 0);
                LCD_SendChar('>');

                currMenuEntry = MENU_CLOCK;

                LCD_SetPos(2, 0);
                char *entry = "CLOCK";
                LCD_SendString(entry);

                LCD_SetPos(2, 1);
                entry = "STOPWATCH";
                LCD_SendString(entry);
            }
            btnMenuCLicked = 0;
        }
        if (btnLeftCLicked)
        {
            if (selectedMode == MODE_MENU)
            {
                //Move menu up
                char *entry1 = "";
                char *entry2 = "";
                switch (currMenuEntry)
                {
                case MENU_CLOCK:
                    entry1 = "  SETTINGS      ";
                    entry2 = "> CLOSE         ";
                    currMenuEntry = MENU_CLOSE;
                    break;
                case MENU_STOPWATCH:
                    entry1 = "> CLOCK         ";
                    entry2 = "  STOPWATCH     ";
                    currMenuEntry = MENU_CLOCK;
                    break;
                case MENU_TIMER:
                    entry1 = "> STOPWATCH     ";
                    entry2 = "  TIMER         ";
                    currMenuEntry = MENU_STOPWATCH;
                    break;
                case MENU_SETTINGS:
                    entry1 = "> TIMER         ";
                    entry2 = "  SETTINGS      ";
                    currMenuEntry = MENU_TIMER;
                    break;
                case MENU_CLOSE:
                    entry1 = "> SETTINGS      ";
                    entry2 = "  CLOSE         ";
                    currMenuEntry = MENU_SETTINGS;
                    break;
                };
                LCD_SetPos(0, 0);
                LCD_SendString(entry1);
                LCD_SetPos(0, 1);
                LCD_SendString(entry2);
            }
            btnLeftCLicked = 0;
        }
        if (btnRightCLicked)
        {
            if (selectedMode == MODE_MENU)
            {
                //Move menu down
                char *entry1 = "";
                char *entry2 = "";
                switch (currMenuEntry)
                {
                case MENU_CLOCK:
                    entry1 = "  CLOCK         ";
                    entry2 = "> STOPWATCH     ";
                    currMenuEntry = MENU_STOPWATCH;
                    break;
                case MENU_STOPWATCH:
                    entry1 = "  STOPWATCH     ";
                    entry2 = "> TIMER         ";
                    currMenuEntry = MENU_TIMER;
                    break;
                case MENU_TIMER:
                    entry1 = "  TIMER         ";
                    entry2 = "> SETTINGS      ";
                    currMenuEntry = MENU_SETTINGS;
                    break;
                case MENU_SETTINGS:
                    entry1 = "  SETTINGS      ";
                    entry2 = "> CLOSE         ";
                    currMenuEntry = MENU_CLOSE;
                    break;
                case MENU_CLOSE:
                    entry1 = "> CLOCK         ";
                    entry2 = "  STOPWATCH     ";
                    currMenuEntry = MENU_CLOCK;
                    break;
                };
                LCD_SetPos(0, 0);
                LCD_SendString(entry1);
                LCD_SetPos(0, 1);
                LCD_SendString(entry2);
            }
            btnRightCLicked = 0;
        }
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

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == btnLeft_Pin)
    {
        btnLeftCLicked = 1;
    };

    if (GPIO_Pin == btnRight_Pin)
    {
        btnRightCLicked = 1;
    }

    if (GPIO_Pin == btnCenter_Pin)
    {
        btnMenuCLicked = 1;
    }
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
