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
#else
#include "usb_device.h"
#endif /* __CONSOLE__ */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Clock config and protocol init already done by ROM code in serial-boot */
#if !defined (__CP_SERIAL_BOOT__)
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
extern void initialise_monitor_handles(void);
#endif /* !__CP_SERIAL_BOOT__ */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
#if !defined (__CP_SERIAL_BOOT__)
  /* Reset of all peripherals, Initialize the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Configure the periph clock */
  PeriphCommonClock_Config();
#endif /* !__CP_SERIAL_BOOT__ */

#if defined (__CONSOLE__)
#if defined(__TERMINAL_IO__) || defined(__VALID_OUTPUT_TERMINAL_IO__)
  /* Activate semihosting to get printf traces output from console tab */
  initialise_monitor_handles();
#else
  /* Configure the UART */
  UART_Config();
#endif

  /* Infinite loop */
  while (1)
  {
    /* Wait for the user to interact with the console */
    MainMenu_interface_start();
  }
#else /* __CONSOLE__ */

  /* Initialize openbootloader */
  OpenBootloader_Init();

  /* Infinite loop */
  while (1)
  {
    /* Wait for a protocol detection */
    OpenBootloader_ProtocolDetection();
  }
#endif /* __CONSOLE__ */
}

#if !defined (__CP_SERIAL_BOOT__)
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /**Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_MEDIUMHIGH);

  /**Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSE
                                     | RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS_DIG;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.HSIDivValue = RCC_HSI_DIV1;

  /**PLL1 Config
  */
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLL12SOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 3;
  RCC_OscInitStruct.PLL.PLLN = 81;
  RCC_OscInitStruct.PLL.PLLP = 1;
  RCC_OscInitStruct.PLL.PLLQ = 1;
  RCC_OscInitStruct.PLL.PLLR = 1;
  RCC_OscInitStruct.PLL.PLLFRACV = 0x800;
  RCC_OscInitStruct.PLL.PLLMODE = RCC_PLL_FRACTIONAL;
  RCC_OscInitStruct.PLL.RPDFN_DIS = RCC_RPDFN_DIS_DISABLED;
  RCC_OscInitStruct.PLL.TPDFN_DIS = RCC_TPDFN_DIS_DISABLED;

  /**PLL2 Config
    */
  RCC_OscInitStruct.PLL2.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL2.PLLSource = RCC_PLL12SOURCE_HSE;
  RCC_OscInitStruct.PLL2.PLLM = 3;
  RCC_OscInitStruct.PLL2.PLLN = 66;
  RCC_OscInitStruct.PLL2.PLLP = 2;
  RCC_OscInitStruct.PLL2.PLLQ = 1;
  RCC_OscInitStruct.PLL2.PLLR = 1;
  RCC_OscInitStruct.PLL2.PLLFRACV = 0x1400;
  RCC_OscInitStruct.PLL2.PLLMODE = RCC_PLL_FRACTIONAL;
  RCC_OscInitStruct.PLL2.RPDFN_DIS = RCC_RPDFN_DIS_DISABLED;
  RCC_OscInitStruct.PLL2.TPDFN_DIS = RCC_TPDFN_DIS_DISABLED;

  /**PLL3 Config
    */
  RCC_OscInitStruct.PLL3.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL3.PLLSource = RCC_PLL3SOURCE_HSE;
  RCC_OscInitStruct.PLL3.PLLM = 2;
  RCC_OscInitStruct.PLL3.PLLN = 34;
  RCC_OscInitStruct.PLL3.PLLP = 2;
  RCC_OscInitStruct.PLL3.PLLQ = 17;
  RCC_OscInitStruct.PLL3.PLLR = 37;
  RCC_OscInitStruct.PLL3.PLLRGE = RCC_PLL3IFRANGE_1;
  RCC_OscInitStruct.PLL3.PLLFRACV = 0x1A04;
  RCC_OscInitStruct.PLL3.PLLMODE = RCC_PLL_FRACTIONAL;
  RCC_OscInitStruct.PLL3.RPDFN_DIS = RCC_RPDFN_DIS_DISABLED;
  RCC_OscInitStruct.PLL3.TPDFN_DIS = RCC_TPDFN_DIS_DISABLED;

  /**PLL4 Config
    */
  RCC_OscInitStruct.PLL4.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL4.PLLSource = RCC_PLL4SOURCE_HSE;
  RCC_OscInitStruct.PLL4.PLLM = 4;
  RCC_OscInitStruct.PLL4.PLLN = 99;
  RCC_OscInitStruct.PLL4.PLLP = 6;
  RCC_OscInitStruct.PLL4.PLLQ = 8;
  RCC_OscInitStruct.PLL4.PLLR = 8;
  RCC_OscInitStruct.PLL4.PLLRGE = RCC_PLL4IFRANGE_0;
  RCC_OscInitStruct.PLL4.PLLFRACV = 0;
  RCC_OscInitStruct.PLL4.PLLMODE = RCC_PLL_INTEGER;
  RCC_OscInitStruct.PLL4.RPDFN_DIS = RCC_RPDFN_DIS_DISABLED;
  RCC_OscInitStruct.PLL4.TPDFN_DIS = RCC_TPDFN_DIS_DISABLED;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**RCC Clock Config
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_ACLK
                                | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2
                                | RCC_CLOCKTYPE_PCLK3 | RCC_CLOCKTYPE_PCLK4
                                | RCC_CLOCKTYPE_PCLK5 | RCC_CLOCKTYPE_MPU;
  RCC_ClkInitStruct.MPUInit.MPU_Clock = RCC_MPUSOURCE_PLL1;
  RCC_ClkInitStruct.MPUInit.MPU_Div = RCC_MPU_DIV2;
  RCC_ClkInitStruct.AXISSInit.AXI_Clock = RCC_AXISSOURCE_PLL2;
  RCC_ClkInitStruct.AXISSInit.AXI_Div = RCC_AXI_DIV1;
  RCC_ClkInitStruct.MCUInit.MCU_Clock = RCC_MCUSSOURCE_PLL3;
  RCC_ClkInitStruct.MCUInit.MCU_Div = RCC_MCU_DIV1;
  RCC_ClkInitStruct.APB4_Div = RCC_APB4_DIV2;
  RCC_ClkInitStruct.APB5_Div = RCC_APB5_DIV4;
  RCC_ClkInitStruct.APB1_Div = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2_Div = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB3_Div = RCC_APB3_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /**Set the HSE division factor for RTC clock
  */
  __HAL_RCC_RTC_HSEDIV(24);
}


/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /* Initializes the common periph clock */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_UART24;
  PeriphClkInit.Uart24ClockSelection = RCC_UART24CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}
#endif /* !__CP_SERIAL_BOOT__ */

#if defined (__CONSOLE__)
#ifdef __GNUC__
/* With GCC/RAISONANCE, small log_info (option LD Linker->Libraries->Small log_info
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __attribute__(( weak )) __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int __attribute__(( weak )) fputc(int ch, FILE *f)
#endif /* __GNUC__ */

#if defined (__LOG_UART_IO_) || defined (__LOG_TRACE_IO_)
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART1 and Loop until the end of transmission */
#if defined (__LOG_UART_IO_)
  extern UART_HandleTypeDef huart;
  HAL_UART_Transmit(&huart, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
#endif
#if defined (__LOG_TRACE_IO_)
  log_buff(ch);
#endif
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
