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
extern int8_t pmic_nvm_str[];
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t PMIC_Initialized = 0U;
static uint8_t pmic_status = PMIC_ERROR_NONE;
static pmic_data_t identified_pmic;

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

  pmic_status = PMIC_Util_Detect_PMIC(&identified_pmic);

  if (pmic_status == PMIC_ERROR_INVALID_ARG)
  {
	  while(1); /* coding defect halt the code */
  }
  else if (pmic_status == PMIC_ERROR_NO_PMIC)
  {
	  /* Do nothing size default to 8 bytes for protocol header */
  }
  else if (pmic_status == PMIC_ERROR_INVALID_PMIC)
  {
	  /* Do nothing size default to 8 bytes for protocol header */
  }
  else
  {
	  uint8_t tmp;
	  tmp = (identified_pmic.NVMSize + PMIC_PROTOCOL_HEADER_SIZE);
	  pmic_nvm_str[14] = (tmp/10) + 48U;
	  pmic_nvm_str[15] = (tmp%10) + 48U;
  }

  PMIC_Initialized = 1U;
}

/**
  * @brief  Interface for OpenBootloader to Read PMIC.
  * @param  pDest : address of memory in RAM to write the read data form PMIC
  * @retval none
  */
void OPENBL_PMIC_Read(uint8_t *pDest)
{
  uint8_t pmic_info;

  if (PMIC_Initialized == 0)
  {
    OPENBL_PMIC_Init();
  }

  /* protocol version specified in Macro PMIC_PROTOCOL_VERSION */
  pmic_info = PMIC_Util_GetNVMID();

  *pDest = PMIC_PROTOCOL_VERSION;

  if (pmic_status == PMIC_ERROR_NO_PMIC)
  {
	  *(pDest + 1) = 0xFF;
  }
  else
  {
    *(pDest + 1) = (pmic_info & 0xF0) >> 4;
    *(pDest + 3) = (pmic_info & 0x0F);
    *(pDest + 4) = (PMIC_I2C_ADDRESS >> 1);
    PMIC_Util_ReadWrite(pDest + PMIC_PROTOCOL_HEADER_SIZE, PMIC_SHADOW_READ, &identified_pmic);
  }

}

/**
  * @brief  Interface for OpenBootloader to Write PMIC.
  * @param  pSource : address of memory in RAM to read data for writing to PMIC.
  * @retval none
  */
void OPENBL_PMIC_Write(uint8_t *pSource)
{
  if (PMIC_Initialized == 0)
  {
    OPENBL_PMIC_Init();
  }

  PMIC_Util_ReadWrite(pSource, PMIC_SHADOW_WRITE, &identified_pmic);
}

/**
  * @brief  Interface for OpenBootloader to Get PMIC NVM size.
  * @param  none
  * @retval Size of identified PMIC NVM
  */
uint32_t OPENBL_PMIC_Get_NVM_Size(void)
{
  if (PMIC_Initialized == 0)
  {
    OPENBL_PMIC_Init();
  }

  return (identified_pmic.NVMSize);
}

