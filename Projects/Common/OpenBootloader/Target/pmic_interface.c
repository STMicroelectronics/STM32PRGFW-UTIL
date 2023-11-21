/**
  ******************************************************************************
  * @file    pmic_interface.c
  * @author  GPM Application Team
  * @brief   common interface for configuring pmic nvm.
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
#include "platform.h"
#include "pmic_interface.h"

/* Global variables ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t PMIC_Initilized = 0U;
/* Private function prototypes -----------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/

/**
  * @brief  Interface for OpenBootloader to initialize PMIC.
  * @param  none
  * @param  none
  * @retval
  */
void OPENBL_PMIC_Init(void)
{
  PMIC_Util_Init();

  PMIC_Initilized = 1U;
}

/**
  * @brief  Interface for OpenBootloader to Read PMIC.
  * @param  pDest : address of memory in RAM to write the read data form PMIC
  * @retval none
  */
void OPENBL_PMIC_Read(uint8_t *pDest)
{
  if (PMIC_Initilized == 0)
  {
    OPENBL_PMIC_Init();
  }

  PMIC_Util_ReadWrite(pDest, PMIC_SHADOW_READ);
}

/**
  * @brief  Interface for OpenBootloader to Write PMIC.
  * @param  pSource : address of memory in RAM to read data for writing to PMIC.
  * @retval none
  */
void OPENBL_PMIC_Write(uint8_t *pSource)
{
  if (PMIC_Initilized == 0)
  {
    OPENBL_PMIC_Init();
  }

  PMIC_Util_ReadWrite(pSource, PMIC_SHADOW_WRITE);

  return;
}

