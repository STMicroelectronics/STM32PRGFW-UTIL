/**
  ******************************************************************************
  * @file    hash_interface.h
  * @author  MCD Application Team
  * @brief   Header for hash_interface.c module
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
#ifndef HASH_INTERFACE_H
#define HASH_INTERFACE_H

/* Includes ------------------------------------------------------------------*/
#ifdef USE_HASH_OVER_OTP
#include "otp_util.h"
#include "hash_util.h"
#include "otp_interface.h"
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void OPENBL_Hash_Init(void);
void OPENBL_Hash_DeInit(void);
int OPENBL_Hash_Start(OPENBL_Otp_TypeDef *Otp_p);
#endif /* USE_HASH_OVER_OTP */

#endif /* hash_interface */
