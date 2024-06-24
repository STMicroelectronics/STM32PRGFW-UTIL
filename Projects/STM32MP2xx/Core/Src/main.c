/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#if defined (__CONSOLE__)
#include "mainmenu_interface_cli.h"
#include "console_util.h"
#endif
#if defined (ENABLE_USB_IF)
#include "usb_device.h"
#endif /* __CONSOLE__ */


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
#if defined (__CONSOLE__)
extern void initialise_monitor_handles(void);
#endif /* !__CP_SERIAL_BOOT__ */
void SystemClock_Config(void);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* Reset of all peripherals, Initialize the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

#if defined (__CONSOLE__)
#if defined(__TERMINAL_IO__)
  /* Activate semihosting to get printf traces output from console tab */
  initialise_monitor_handles();
#elif defined (__VALID_OUTPUT_UART__)
  UART_Config();
#endif


  while (1)
  {
    MainMenu_interface_start();
  }
#elif defined(__CP_SERIAL_BOOT__) || defined (__CP_DEV_BOOT__)

  /* Initialize openbootloader */
  OpenBootloader_Init();

  /* Infinite loop */
  while (1)
  {
    OpenBootloader_ProtocolDetection();
  }
#endif /* __CP_SERIAL_BOOT__  ||  __CP_DEV_BOOT__  */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{


  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_PLLInitTypeDef RCC_Pll1InitStruct = {0};
  /* PLL 2 is configured by DDR initialization code */
  /* PLL 3 is configured by GPU initialization code */
  RCC_PLLInitTypeDef RCC_Pll4InitStruct = {0};
  RCC_PLLInitTypeDef RCC_Pll5InitStruct = {0};
  RCC_PLLInitTypeDef RCC_Pll6InitStruct = {0};
  RCC_PLLInitTypeDef RCC_Pll7InitStruct = {0};
  RCC_PLLInitTypeDef RCC_Pll8InitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSE |
                                     RCC_OSCILLATORTYPE_LSE | RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_OFF;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
  RCC_OscInitStruct.LSEDriveValue = RCC_LSEDRIVE_MEDIUMHIGH;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    printf("SystemClock_Config: Failed to enable oscillators.\n\r");
    while (1);
  }

#if defined(CORE_CA35)
  /* 1000MHz */
  RCC_Pll1InitStruct.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_Pll1InitStruct.PLLMode = 0;
  RCC_Pll1InitStruct.FBDIV = 25;
  RCC_Pll1InitStruct.FREFDIV = 1;
  RCC_Pll1InitStruct.POSTDIV1 = 1;
  RCC_Pll1InitStruct.POSTDIV2 = 1;
  RCC_Pll1InitStruct.PLLState = RCC_PLL_ON;
#endif

  /* PLL 2 is configured by DDR initialization code */
  /* PLL 3 is configured by GPU initialization code */

  /* 1200MHz */
  RCC_Pll4InitStruct.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_Pll4InitStruct.PLLMode = 0;
  RCC_Pll4InitStruct.FBDIV = 60;
  RCC_Pll4InitStruct.FREFDIV = 2;
  RCC_Pll4InitStruct.POSTDIV1 = 1;
  RCC_Pll4InitStruct.POSTDIV2 = 1;
  RCC_Pll4InitStruct.PLLState = RCC_PLL_ON;

  /* 532MHz */
  RCC_Pll5InitStruct.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_Pll5InitStruct.PLLMode = 0;
  RCC_Pll5InitStruct.FBDIV = 53;
  RCC_Pll5InitStruct.FREFDIV = 4;
  RCC_Pll5InitStruct.FRACIN = 3355500;
  RCC_Pll5InitStruct.POSTDIV1 = 1;
  RCC_Pll5InitStruct.POSTDIV2 = 1;
  RCC_Pll5InitStruct.PLLState = RCC_PLL_ON;

  /* 500MHz */
  RCC_Pll6InitStruct.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_Pll6InitStruct.PLLMode = 0;
  RCC_Pll6InitStruct.FBDIV = 25;
  RCC_Pll6InitStruct.FREFDIV = 1;
  RCC_Pll6InitStruct.POSTDIV1 = 2;
  RCC_Pll6InitStruct.POSTDIV2 = 1;
  RCC_Pll6InitStruct.PLLState = RCC_PLL_ON;

  /* 491.5MHz */
  RCC_Pll7InitStruct.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_Pll7InitStruct.PLLMode = 0;
  RCC_Pll7InitStruct.FBDIV = 49;
  RCC_Pll7InitStruct.FREFDIV = 2;
  RCC_Pll7InitStruct.FRACIN = 2516600;
  RCC_Pll7InitStruct.POSTDIV1 = 2;
  RCC_Pll7InitStruct.POSTDIV2 = 1;
  RCC_Pll7InitStruct.PLLState = RCC_PLL_ON;

  /* 451.5MHz */
  RCC_Pll8InitStruct.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_Pll8InitStruct.PLLMode = 0;
  RCC_Pll8InitStruct.FBDIV = 45;
  RCC_Pll8InitStruct.FREFDIV = 2;
  RCC_Pll8InitStruct.FRACIN = 2516600;
  RCC_Pll8InitStruct.POSTDIV1 = 2;
  RCC_Pll8InitStruct.POSTDIV2 = 1;
  RCC_Pll8InitStruct.PLLState = RCC_PLL_ON;

  /* Feed A35 on another source than PLL1 */
  HAL_RCCEx_CA35SS_SetCA35SSClockSource(A35_SOURCE_EXT);

  if (HAL_RCCEx_CA35SS_PLL1Config(&RCC_Pll1InitStruct) != HAL_OK)
  {
    printf("SystemClock_Config: Failed to enable PLL1.\n\r");
    printf("SystemClock_Config: Staying on external clock.\n\r");
    while (1);
  }

  /* Switch back A35 on PLL1 */
  if (HAL_RCCEx_CA35SS_SetCA35SSClockSource(A35_SOURCE_PLL1) != HAL_OK)
  {
    printf("HAL_RCCEx_CA35SS_SetCA35SSClockSource: Failed to switch to PLL1.\n\r");
    while (1);
  }

  /* PLL 2 is configured by DDR initialization code */
  /* PLL 3 is configured by GPU initialization code */

  if (HAL_RCCEx_PLL4Config(&RCC_Pll4InitStruct) != HAL_OK)
  {
    printf("SystemClock_Config: Failed to enable PLL4.\n\r");
    while (1);
  }

  if (HAL_RCCEx_PLL5Config(&RCC_Pll5InitStruct) != HAL_OK)
  {
    printf("SystemClock_Config: Failed to enable PLL5.\n\r");
    while (1);
  }

  if (HAL_RCCEx_PLL6Config(&RCC_Pll6InitStruct) != HAL_OK)
  {
    printf("SystemClock_Config: Failed to enable PLL6.\n\r");
    while (1);
  }

  if (HAL_RCCEx_PLL7Config(&RCC_Pll7InitStruct) != HAL_OK)
  {
    printf("SystemClock_Config: Failed to enable PLL7.\n\r");
    while (1);
  }

  if (HAL_RCCEx_PLL8Config(&RCC_Pll8InitStruct) != HAL_OK)
  {
    printf("SystemClock_Config: Failed to enable PLL8.\n\r");
    while (1);
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_ICN_HS_MCU | RCC_CLOCKTYPE_ICN_LS_MCU |
                                RCC_CLOCKTYPE_ICN_SDMMC | RCC_CLOCKTYPE_ICN_DDR |
                                RCC_CLOCKTYPE_ICN_DISPLAY | RCC_CLOCKTYPE_ICN_HCL |
                                RCC_CLOCKTYPE_ICN_NIC | RCC_CLOCKTYPE_ICN_VID |
                                RCC_CLOCKTYPE_ICN_APB1 | RCC_CLOCKTYPE_ICN_APB2 |
                                RCC_CLOCKTYPE_ICN_APB3 | RCC_CLOCKTYPE_ICN_APB4 |
                                RCC_CLOCKTYPE_ICN_APBDBG;

  RCC_ClkInitStruct.ICN_HS_MCU.XBAR_ClkSrc = RCC_XBAR_CLKSRC_PLL4;
  RCC_ClkInitStruct.ICN_HS_MCU.Div = 3;
  RCC_ClkInitStruct.ICN_SDMMC.XBAR_ClkSrc = RCC_XBAR_CLKSRC_PLL4;
  RCC_ClkInitStruct.ICN_SDMMC.Div = 6;
  RCC_ClkInitStruct.ICN_DDR.XBAR_ClkSrc = RCC_XBAR_CLKSRC_PLL4;
  RCC_ClkInitStruct.ICN_DDR.Div = 2;
  RCC_ClkInitStruct.ICN_DISPLAY.XBAR_ClkSrc = RCC_XBAR_CLKSRC_PLL4;
  RCC_ClkInitStruct.ICN_DISPLAY.Div = 4;
  RCC_ClkInitStruct.ICN_HCL.XBAR_ClkSrc = RCC_XBAR_CLKSRC_PLL4;
  RCC_ClkInitStruct.ICN_HCL.Div = 4;
  RCC_ClkInitStruct.ICN_NIC.XBAR_ClkSrc = RCC_XBAR_CLKSRC_PLL4;
  RCC_ClkInitStruct.ICN_NIC.Div = 3;
  RCC_ClkInitStruct.ICN_VID.XBAR_ClkSrc = RCC_XBAR_CLKSRC_PLL4;
  RCC_ClkInitStruct.ICN_VID.Div = 2;
  RCC_ClkInitStruct.ICN_LSMCU_Div = RCC_LSMCU_DIV2;
  RCC_ClkInitStruct.APB1_Div = RCC_APB1_DIV1;
  RCC_ClkInitStruct.APB2_Div = RCC_APB2_DIV1;
  RCC_ClkInitStruct.APB3_Div = RCC_APB3_DIV1;
  RCC_ClkInitStruct.APB4_Div = RCC_APB4_DIV1;
  RCC_ClkInitStruct.APBDBG_Div = RCC_APBDBG_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, 0) != HAL_OK)
  {
    printf("SystemClock_Config: Failed to configure bus clocks.\n\r");
    while (1);
  }
}

#if defined (__CONSOLE__)
extern UART_HandleTypeDef huart;
#ifdef __GNUC__
int __io_putchar(int ch)
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to EVAL_COM1 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart, (uint8_t *)&ch, 1, HAL_MAX_DELAY);

  return ch;
}

int __io_getchar(void)
{
  uint8_t ch = 0;
  /* We received the charracter on the handler of the EVAL_COM1 */
  /* The handler must be initialise before */
  HAL_UART_Receive(&huart, (uint8_t *)&ch, 1, HAL_MAX_DELAY);

  return ch;
}
#endif
#endif /* __CONSOLE__ */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  while (1)
  {
  }
}

#if defined (__CONSOLE__)
#if defined(__TERMINAL_IO__)
int _kill(int pid, int sig)
{
  return -1;
}

#endif
#endif

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* Infinite loop */
  while (1)
  {
  }
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
