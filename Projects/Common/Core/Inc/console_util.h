/**
  ******************************************************************************
  * @file    console_util.h
  * @author  MCD Application Team
  * @brief   provide console utilities header
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
#ifndef CONSOLE_UTIL_H
#define CONSOLE_UTIL_H

/* Private includes ----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
void Serial_Putchar(char value);
void Serial_Printf(char *value, int len);
uint32_t Serial_Scanf(uint32_t value);
void UART_Config(void);

/* Private defines -----------------------------------------------------------*/
/* UART related configuration */
#if defined (STM32MP257Cxx)
#define USARTx                           USART2
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART2_CLK_ENABLE()
#define USARTx_FORCE_RESET()             __HAL_RCC_USART2_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART2_RELEASE_RESET()

#define MX_UART_INSTANCE        USART2
#define MX_UART_TX_PIN          GPIO_PIN_4
#define MX_UART_TX_GPIO_PORT    GPIOA
#define MX_UART_TX_AF           GPIO_AF6_USART2
#define MX_UART_RX_PIN          GPIO_PIN_8
#define MX_UART_RX_GPIO_PORT    GPIOA
#define MX_UART_RX_AF           GPIO_AF8_USART2
#define MX_MODEL                "STM32MP2XX"
#elif (STM32MP157Cxx)
#define USARTx                           UART4
#define USARTx_CLK_ENABLE()              __HAL_RCC_UART4_CLK_ENABLE()
#define USARTx_FORCE_RESET()             __HAL_RCC_UART4_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_UART4_RELEASE_RESET()

#define MX_UART_INSTANCE        UART4
#define MX_UART_TX_PIN          GPIO_PIN_11
#define MX_UART_TX_GPIO_PORT    GPIOG
#define MX_UART_TX_AF           GPIO_AF6_UART4
#define MX_UART_RX_PIN          GPIO_PIN_2
#define MX_UART_RX_GPIO_PORT    GPIOB
#define MX_UART_RX_AF           GPIO_AF8_UART4
#define MX_MODEL                "STM32MP15XX"
#else
#define USARTx                           UART4
#define USARTx_CLK_ENABLE()              __HAL_RCC_UART4_CLK_ENABLE()
#define USARTx_FORCE_RESET()             __HAL_RCC_UART4_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_UART4_RELEASE_RESET()

#define MX_UART_INSTANCE        UART4
#define MX_UART_TX_PIN          GPIO_PIN_6
#define MX_UART_TX_GPIO_PORT    GPIOD
#define MX_UART_TX_AF           GPIO_AF8_UART4
#define MX_UART_RX_PIN          GPIO_PIN_8
#define MX_UART_RX_GPIO_PORT    GPIOD
#define MX_UART_RX_AF           GPIO_AF8_UART4
#define MX_MODEL                "STM32MP13XX"
#endif

#define MX_UART_BAUDRATE        115200
#define MX_UART_WORDLENGTH      UART_WORDLENGTH_8B
#define MX_UART_STOPBITS        UART_STOPBITS_1
#define MX_UART_PARITY          UART_PARITY_NONE
#define MX_UART_HWFLOWCTL       UART_HWCONTROL_NONE
#define MX_UART_MODE            UART_MODE_TX_RX
#define MX_UART_OVERSAMPLING    UART_OVERSAMPLING_16

#endif /* CONSOLE_UTIL_H */
