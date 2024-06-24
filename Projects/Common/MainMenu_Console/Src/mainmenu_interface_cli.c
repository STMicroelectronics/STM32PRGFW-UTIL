/**
  ******************************************************************************
  * @file    mainmenu_interface_cli.c
  * @author  MCD Application Team
  * @brief   Main Menu Console Interface CLI
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
#include "main.h"
#include "mainmenu_interface_cli.h"
#if defined (__CONSOLE__)
#include "console_util.h"
#include "otp_interface_cli.h"
#include "pmic_interface_cli.h"
#endif /* __CONSOLE__ */
#include <limits.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
  OTP_INTERFACE = '1',
  PMIC_INTERFACE = '2',
  MAX_INTERFACE,
} cli_interface_type;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void print_command_error(void);
/* Exported variables --------------------------------------------------------*/
static bool ExitConsole = false;

static void print_console_header(void)
{
  printf("\n\r******OTP/PMIC Programming console ********\n\r");
  printf("\n\rPress (1) for OTP interface\n\r");
  printf("\n\rPress (2) for PMIC interface\n\r");
  printf("\rUser>\n\r");
}

/**
  * @brief print command error message
  * @param None
  * @retval None
  */
static void print_command_error(void)
{
  printf("\n\rError: Wrong value\n\r");
  return;
}


/* Exported functions --------------------------------------------------------*/
void MainMenu_interface_start(void)
{
  char choice = 0;
  print_console_header();
  choice = Serial_Scanf(255);
  fflush(stdin);

  switch (choice)
  {
    case OTP_INTERFACE:
      otp_print_header();
      ExitConsole = false;
      while (ExitConsole == false)
      {
        ExitConsole = otp_commands_interactive();
      }
      break;

    case PMIC_INTERFACE:
      pmic_print_header();
      ExitConsole = false;
      while (ExitConsole == false)
      {
        ExitConsole = pmic_commands_interactive();
      }
      break;

    default:
      print_command_error();
      break;
  }
}


