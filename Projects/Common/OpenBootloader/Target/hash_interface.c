/**
  ******************************************************************************
  * @file    Hash_interface.c
  * @author  MCD Application Team
  * @brief   Contains common functions used by different interfaces
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
#include "platform.h"
#ifdef USE_HASH_OVER_OTP
#include "hash_interface.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/**
  * @brief Init the otp
  * @param None
  * @retval None
  */
void OPENBL_Hash_Init(void)
{
  HASH_Util_Init();
}

/**
  * @brief De-init the otp
  * @param None
  * @retval None
  */
void OPENBL_Hash_DeInit(void)
{
  HASH_Util_DeInit();
}

/**
  * @brief Write the otp
  * @param Otp: the otp structure
  * @retval OTP_OK: if no error
  *         other: if error
  */
int OPENBL_Hash_Start(Otp_TypeDef *Otp_p)
{
  HAL_StatusTypeDef status = HAL_ERROR;
  status = HASH_Util_calculate(Otp_p);
  /* Compute SHA256 */
  if (status != HAL_OK)
  {
    return HASH_ERROR;
  }
  return HASH_OK;
}
#endif /* USE_HASH_OVER_OTP*/
