/**
  ******************************************************************************
  * @file    hash_util.c
  * @author  GPM Application Team
  * @brief   Hash utility functions
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

/* Includes ------------------------------------------------------------------*/
#if defined(USE_HASH_OVER_OTP)
#include "hash_util.h"

/* Global variables ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static HASH_HandleTypeDef hhash;
static uint8_t Hash_Initialized = 0U;
/* Private function prototypes -----------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/
/**
  * @brief  Intialize the hash instance.
  * @param  None
  * @retval None.
  */
void HASH_Util_Init(void)
{
  hhash.Instance = HASH;
  HAL_HASH_DeInit(&hhash);
  hhash.Init.DataType = HASH_NO_SWAP;
  hhash.Init.Algorithm = HASH_ALGOSELECTION_SHA256;
  HAL_HASH_Init(&hhash);
  Hash_Initialized = 1U;
}
/**
  * @brief  DeIntialize the hash instance.
  * @param  None
  * @retval None.
  */
void HASH_Util_DeInit(void)
{
  HAL_HASH_DeInit(&hhash);
  Hash_Initialized = 0;
}
/**
  * @brief  calculate the hash over OTP.
  * @param  OTP structure pointer
  * @retval Status.
  */
HAL_StatusTypeDef HASH_Util_calculate(Otp_TypeDef *Otp_p)
{
  HAL_StatusTypeDef status = HAL_ERROR;
  volatile uint32_t LocalOtpPart[OTP_HASH_PART_SIZE / 2] = {0};
  if (Hash_Initialized == 0)
  {
    HASH_Util_Init();
  }
  /* copy only OTP values not status for 376 words */
  for (uint32_t i = 0, index = 0; index < OTP_HASH_PART_SIZE; index += 2, i++)
  {
    LocalOtpPart[i] = Otp_p->OtpPart[index];
  }
  status = HAL_HASH_Start(&hhash, (uint8_t *)&LocalOtpPart[0], sizeof(LocalOtpPart),
		                                   (uint8_t *)&Otp_p->Sha256Hash[0], 1000);
  return status;
}
#endif /* USE_HASH_OVER_OTP */
