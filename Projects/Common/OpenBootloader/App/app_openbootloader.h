/**
  ******************************************************************************
  * @file    app_openbootloader.h
  * @author  MCD Application Team
  * @brief   Header for app_openbootloader.c module
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
#ifndef APP_OPENBOOTLOADER_H
#define APP_OPENBOOTLOADER_H

/* Includes ------------------------------------------------------------------*/
#include "openbl_mem.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void OpenBootloader_Init(void);
void OpenBootloader_DeInit(void);
void OpenBootloader_ProtocolDetection(void);

/* External variables --------------------------------------------------------*/
extern OPENBL_MemoryTypeDef FLASH_Descriptor;
extern OPENBL_MemoryTypeDef RAM_Descriptor;
extern OPENBL_MemoryTypeDef EB_Descriptor;
extern OPENBL_MemoryTypeDef EXTERNAL_MEMORY_Descriptor;

#endif /* APP_OPENBOOTLOADER_H */
