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
#ifndef PMIC_INTERFACE_CLI_H
#define PMIC_INTERFACE_CLI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
/* Private includes ----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
bool pmic_commands_interactive(void);
void pmic_print_header(void);
/* Private defines -----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* PMIC_INTERFACE_CLI_H */
