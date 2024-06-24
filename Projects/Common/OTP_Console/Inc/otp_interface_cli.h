/**
  ******************************************************************************
  * @file    otp_interface_cli.h
  * @author  MCD Application Team
  * @brief   Header for otp_interface_cli.c module
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
#ifndef OTP_INTERFACE_CLI_H
#define OTP_INTERFACE_CLI_H

/* Private includes ----------------------------------------------------------*/
#include <stdbool.h>
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
bool otp_commands_interactive(void);
void otp_print_header(void);
/* Private defines -----------------------------------------------------------*/

#endif /* OTP_INTERFACE_CLI_H */
