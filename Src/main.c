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
#include "menu.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
RTCDateTimeTypeDef currentTime, rtcTime;
uint8_t b[1] = {0};

volatile uint8_t btnMenuCLicked;
volatile uint8_t btnLeftCLicked;
volatile uint8_t btnRightCLicked;
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

    uint8_t config_reg = 0b00001111;
    uint8_t config_tmp = config_reg;
    ReadConfig(&config_reg);

    InitCurrentTime(&currentTime);
    LCD_ClockInit(config_reg);

    uint8_t selectedMode = MODE_CLOCK;
    uint8_t prev_mode = MODE_CLOCK;
    uint8_t currMenuEntry = 0;
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
                UpdateClock(&currentTime, &rtcTime, config_reg);
            }
            if ((config_reg & CFG_TEMP) && ds18b20status)
            {
                UpdateTemp();
            }
        }

        if (selectedMode == MODE_STOPWATCH)
        {
            //TODO mode STOPWATCH
        }

        if (selectedMode == MODE_TIMER)
        {
            //TODO mode TIMER
        }
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
        // btn center clicked
        if (btnMenuCLicked)
        {
            switch (selectedMode)
            {
            case MODE_MENU:
                switch (currMenuEntry)
                {
                case MENU_CLOCK:
                    LCD_Clear();
                    InitCurrentTime(&currentTime);
                    LCD_ClockInit(config_reg);
                    selectedMode = MODE_CLOCK;
                    break;

                case MENU_STOPWATCH:
                    //TODO switch mode to stopwatch
                    selectedMode = MODE_STOPWATCH;
                    break;

                case MENU_TIMER:
                    //TODO switch mode to timer
                    break;

                case MENU_SETTINGS:
                    MenuSettingsOpen(&selectedMode, &currMenuEntry, config_reg);
                    break;
                case MENU_EXIT:
                    //TODO menu CLOSE
                    LCD_Clear();
                    selectedMode = prev_mode;
                    break;
                };
                currMenuEntry = 0;
                break;

            case MODE_SETTINGS:
                switch (currMenuEntry)
                {
                case SETTINGS_TIME:
                    //TODO open time settings menu
                    break;
                case SETTINGS_DAY:
                    //TODO update menu entry
                    config_tmp ^= CFG_DAY; 
                    break;
                case SETTINGS_DATE:
                    //TODO update menu entry
                    config_tmp ^= CFG_DAY; 
                    break;
                case SETTINGS_SECONDS:
                    //TODO update menu entry
                    config_tmp ^= CFG_DAY; 
                    break;
                case SETTINGS_TEMP:
                    //TODO update menu entry
                    config_tmp ^= CFG_DAY; 
                    break;
                case SETTINGS_SAVE:
                    config_reg = config_tmp;
                    WriteConfig(config_reg);
                    
                    //TODO prev mode
                    LCD_Clear();
                    selectedMode = prev_mode;

                    break;
                case SETTINGS_CANCEL:
                    config_tmp = config_reg;
                    
                    //TODO prev mode
                    LCD_Clear();
                    selectedMode = prev_mode;
                    break;
                }

                currMenuEntry = 0;
                break;

            default:
                MenuMainOpen(&prev_mode, &selectedMode, &currMenuEntry);
            }
            btnMenuCLicked = 0;
        }

        //btn Left (Up) clicked
        if (btnLeftCLicked)
        {
            switch (selectedMode)
            {
            case MODE_MENU:
                MenuMainUp(&currMenuEntry);
                break;
            case MODE_SETTINGS:
                MenuSettingsUp(&currMenuEntry);
                break;

            default:
                break;
            }
            btnLeftCLicked = 0;
        }

        //btn Right (Down) clicked
        if (btnRightCLicked)
        {
            switch (selectedMode)
            {
            case MODE_MENU:
                MenuMainDown(&currMenuEntry);
                break;
            case MODE_SETTINGS:
                MenuSettingsDown(&currMenuEntry);
                break;
            default:
                break;
            }
            btnRightCLicked = 0;
        }
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
        if (!btnLeftCLicked)
        {
            btnLeftCLicked = 1;
        }
    };

    if (GPIO_Pin == btnRight_Pin)
    {
        if (!btnRightCLicked)
        {
            btnRightCLicked = 1;
        }
    }

    if (GPIO_Pin == btnCenter_Pin)
    {
        if (!btnMenuCLicked)
        {
            btnMenuCLicked = 1;
        }
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
