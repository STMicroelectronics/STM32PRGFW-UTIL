/**
  ******************************************************************************
  * @file    main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "string.h"
#include "stdlib.h"
#include <stdio.h>
#include "stm32mp2xx_hal.h"
#include "stm32mp2xx_ll_bus.h"
#if !defined (__CONSOLE__)
#include "app_openbootloader.h"
#endif /* !__CONSOLE__ */

/* Private includes ----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);
void System_DeInit(void);

/* Private defines -----------------------------------------------------------*/
#if defined (__CONSOLE__)
#define USARTx_TX_PIN                    MX_UART_TX_PIN
#define USARTx_TX_AF                     MX_UART_TX_AF
#define USARTx_RX_PIN                    MX_UART_RX_PIN
#define USARTx_RX_AF                     MX_UART_RX_AF

#define USARTx_TX_GPIO_PORT              MX_UART_TX_GPIO_PORT
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTx_RX_GPIO_PORT              MX_UART_RX_GPIO_PORT
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#endif

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
