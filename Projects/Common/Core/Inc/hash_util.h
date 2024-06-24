/**
  ******************************************************************************
  * @file    hash_util.h
  * @author  MCD Application Team
  * @brief   Header for hash_util.c module
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
#ifndef HASH_UTIL_H
#define HASH_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#if defined(USE_HASH_OVER_OTP)
#include "main.h"
#include "otp_util.h"
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
#define HASH_ERROR                       -1
#define HASH_OK                           0
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void HASH_Util_Init(void);
void HASH_Util_DeInit(void);
HAL_StatusTypeDef HASH_Util_calculate(Otp_TypeDef *Otp);

#endif /* USE_HASH_OVER_OTP */

#ifdef __cplusplus
}
#endif

#endif /* HASH_UTIL_H */
