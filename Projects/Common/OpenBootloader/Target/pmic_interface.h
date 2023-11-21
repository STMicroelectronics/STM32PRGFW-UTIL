/**
  ******************************************************************************
  * @file    pmic_interface.h
  * @author  GPM Application Team
  * @brief   Header for pmic_interface.c module
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
#ifndef PMIC_INTERFACE_H
#define PMIC_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "pmic_util.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void OPENBL_PMIC_Init(void);
void OPENBL_PMIC_Read(uint8_t *pDest);
void OPENBL_PMIC_Write(uint8_t *pSource);

#ifdef __cplusplus
}
#endif

#endif /* PMIC_INTERFACE_H */
