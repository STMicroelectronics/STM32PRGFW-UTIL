/**
  ******************************************************************************
  * @file    pmic_util.h
  * @author  GPM Application Team
  * @brief   Header for pmic_util.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef PMIC_UTIL_H
#define PMIC_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdbool.h>


/* Exported types ------------------------------------------------------------*/

typedef enum
{
  PMIC_SHADOW_WRITE,
  PMIC_SHADOW_READ,
} pmic_nvm_ops_t;

typedef struct
{
  int8_t     Supported;
  uint8_t    Identifier;
  uint8_t    NVMSize;
  uint8_t    DisplayString[3];
  uint8_t    NVMStartAddress;
  uint8_t    NVMSRRegisterAddr;
  uint8_t    NVMCRRegisterAddr;
} pmic_data_t;

/* Exported constants --------------------------------------------------------*/
#if defined(STPMIC1)

typedef enum
{
	PMIC_STPMIC1,
	PMIC_MAX,
}pmic_types;

#elif defined(STPMIC2)
typedef enum
{
	PMIC_STPMIC25,
	PMIC_MAX,
}pmic_types;
#endif

/* Exported variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

#if defined(STPMIC1)
#define PMIC_VERSION_ID_SR_ADDR     0x06
#elif defined(STPMIC2)
#define PMIC_PRODUCT_ID_SR_ADDR     0x00
#endif /* STPMIC1 */

#define OWN_I2C_SLAVE_ADDRESS       0x33
#define PMIC_NVM_BUSY_MSK           0x01
#define PMIC_I2C_ADDRESS        ((0x33U & 0x7FU) << 1) /* NVM Default   */
#define PMIC_SR_REG_TIMEOUT        1000U
#define MAX_PMIC_NVM_SIZE            64
#if defined (STM32MP157Cxx)
#define BUS_I2C_INSTANCE                      I2C4
#define BUS_I2C_CLK_ENABLE()                  __HAL_RCC_I2C4_CLK_ENABLE()
#define BUS_I2C_CLK_DISABLE()                 __HAL_RCC_I2C4_CLK_DISABLE()
#define BUS_I2C_SCL_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOZ_CLK_ENABLE()
#define BUS_I2C_SCL_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOZ_CLK_DISABLE()
#define BUS_I2C_SDA_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOZ_CLK_ENABLE()
#define BUS_I2C_SDA_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOZ_CLK_DISABLE()

#define BUS_I2C_FORCE_RESET()                 __HAL_RCC_I2C4_FORCE_RESET()
#define BUS_I2C_RELEASE_RESET()               __HAL_RCC_I2C4_RELEASE_RESET()

/* Definition for I2Cx Pins */
#define BUS_I2C_SCL_PIN                       GPIO_PIN_4
#define BUS_I2C_SCL_GPIO_PORT                 GPIOZ
#define BUS_I2C_SDA_PIN                       GPIO_PIN_5
#define BUS_I2C_SDA_GPIO_PORT                 GPIOZ
#define BUS_I2C_SCL_AF                        GPIO_AF6_I2C4
#define BUS_I2C_SDA_AF                        GPIO_AF6_I2C4

/* I2C interrupt requests */
#define BUS_I2C_EV_IRQn                       I2C4_EV_IRQn
#define BUS_I2C_ER_IRQn                       I2C4_ER_IRQn

/* I2C TIMING Register define when I2C clock source is SYSCLK */
/* I2C TIMING is calculated from Bus clock (HSI) = 64 MHz */

#ifndef BUS_I2Cx_TIMING
#define BUS_I2Cx_TIMING                      ((uint32_t)0x10805E89U)
#endif /* BUS_I2Cx_TIMING */

#elif defined (STM32MP135Fxx)
#define BUS_I2C_INSTANCE                      I2C4
#define BUS_I2C_CLK_ENABLE()                  __HAL_RCC_I2C4_CLK_ENABLE()
#define BUS_I2C_CLK_DISABLE()                 __HAL_RCC_I2C4_CLK_DISABLE()
#define BUS_I2C_SCL_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOE_CLK_ENABLE()
#define BUS_I2C_SCL_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOE_CLK_DISABLE()
#define BUS_I2C_SDA_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOB_CLK_ENABLE()
#define BUS_I2C_SDA_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOB_CLK_DISABLE()

#define BUS_I2C_FORCE_RESET()                 __HAL_RCC_I2C4_FORCE_RESET()
#define BUS_I2C_RELEASE_RESET()               __HAL_RCC_I2C4_RELEASE_RESET()

/* Definition for I2Cx Pins */
#define BUS_I2C_SCL_PIN                       GPIO_PIN_15
#define BUS_I2C_SCL_GPIO_PORT                 GPIOE
#define BUS_I2C_SDA_PIN                       GPIO_PIN_9
#define BUS_I2C_SDA_GPIO_PORT                 GPIOB
#define BUS_I2C_SCL_AF                        GPIO_AF6_I2C4
#define BUS_I2C_SDA_AF                        GPIO_AF6_I2C4

/* I2C interrupt requests */
#define BUS_I2C_EV_IRQn                       I2C4_EV_IRQn
#define BUS_I2C_ER_IRQn                       I2C4_ER_IRQn

/* I2C TIMING Register define when I2C clock source is SYSCLK */
/* I2C TIMING is calculated from Bus clock (HSI) = 64 MHz */

#ifndef BUS_I2Cx_TIMING
#define BUS_I2Cx_TIMING                      ((uint32_t)0x10805E89U)
#endif /* BUS_I2Cx_TIMING */
#elif defined(STM32MP257Cxx)
#define BUS_I2C_INSTANCE                      I2C7
#define BUS_I2C_CLK_ENABLE()                  __HAL_RCC_I2C7_CLK_ENABLE()
#define BUS_I2C_CLK_DISABLE()                 __HAL_RCC_I2C7_CLK_DISABLE()
#define BUS_I2C_SCL_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOD_CLK_ENABLE()
#define BUS_I2C_SCL_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOD_CLK_DISABLE()
#define BUS_I2C_SDA_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOD_CLK_ENABLE()
#define BUS_I2C_SDA_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOD_CLK_DISABLE()

#define BUS_I2C_FORCE_RESET()                 __HAL_RCC_I2C7_FORCE_RESET()
#define BUS_I2C_RELEASE_RESET()               __HAL_RCC_I2C7_RELEASE_RESET()

/* Definition for I2Cx Pins */
#define BUS_I2C_SCL_PIN                       GPIO_PIN_15
#define BUS_I2C_SCL_GPIO_PORT                 GPIOD
#define BUS_I2C_SDA_PIN                       GPIO_PIN_14
#define BUS_I2C_SDA_GPIO_PORT                 GPIOD
#define BUS_I2C_SCL_AF                        GPIO_AF10_I2C7
#define BUS_I2C_SDA_AF                        GPIO_AF10_I2C7

/* I2C interrupt requests */
#define BUS_I2C_EV_IRQn                       I2C7_EV_IRQn
#define BUS_I2C_ER_IRQn                       I2C7_ER_IRQn

/* I2C TIMING Register define when I2C clock source is SYSCLK */
/* I2C TIMING is calculated from Bus clock (HSI) = 64 MHz */

#ifndef BUS_I2Cx_TIMING
#define BUS_I2Cx_TIMING                      ((uint32_t)0x10805E89U)
#endif  /* BUS_I2Cx_TIMING */

#endif /* STM32MP135Fxx */
#define PMIC_NOT_SUPPORTED       (-1)
#define PMIC_SUPPORTED           (0)
/* Exported functions ------------------------------------------------------- */
void PMIC_Util_Init(void);
void PMIC_Util_ReadWrite(uint8_t *addr, pmic_nvm_ops_t ops, pmic_data_t * pmic_data);
bool PMIC_Util_Detect_PMIC(pmic_data_t * pmic_detected);

#ifdef __cplusplus
}
#endif

#endif /* PMIC_UTIL_H */
