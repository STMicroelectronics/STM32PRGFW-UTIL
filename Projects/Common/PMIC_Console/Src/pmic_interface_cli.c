/**
  ******************************************************************************
  * @file    pmic_interface_cli.c
  * @author  MCD Application Team
  * @brief   PMIC Interface CLI
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
#include "pmic_interface_cli.h"
#include "console_util.h"
#include "pmic_util.h"
#include <limits.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
  PMIC_CMD_HELP = 0,
  PMIC_CMD_DISPL,
  PMIC_CMD_WRITE,
  PMIC_CMD_UPDATE,
  PMIC_CMD_EXIT,
  PMIC_CMD_MAX,
} pmic_cmd_id;

typedef struct
{
  char *str;
  uint32_t param_min;
  uint32_t param_max;
} pmic_command;

typedef struct
{
  uint32_t off;
  uint32_t len;
} pmic_arg;

static pmic_command pmic_cmd[PMIC_CMD_MAX];
/* Private define ------------------------------------------------------------*/
#define CMD_MAX_LEN 1024
#define CMD_MAX_ARG 255
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

static char *cmd;
static char *val_str;
static char *end_ptr;
static uint8_t val;
static bool once;
static char entry[CMD_MAX_LEN];
static bool auto_conf;
static uint8_t nvm_buffer[MAX_PMIC_NVM_SIZE] = {0};
uint32_t prev_pmic_cmd;
static bool once_nvm_read_in_buffer = false;
static pmic_data_t identified_pmic;

/* Private function prototypes -----------------------------------------------*/
static void print_help(void);
static void free_args(int argc, char *argv[]);
static void get_entry_string(char *entry);
static int parse_entry_string(char *entry, size_t size, int *command, char *argv[]);
static void print_displ(int argc, char *argv[]);
static bool print_exit(int argc, char *argv[]);
static void print_command_error(void);
static void print_outofrange_error(uint8_t num);
static void print_modified_values(void);
static void read_nvm_in_buffer(void);

/* Exported variables --------------------------------------------------------*/

/**
  * @brief print help message
  * @param None
  * @retval None
  */
static void print_help(void)
{
  printf("commands:\n\r");
  printf(">help                      : This command allows the user to display help menu\n\r");
  printf(">displ                     : This command allows the user to display NVM register\n\r");
  printf(">write                     : This command allows to update NVM registers  up\n\r");
  printf(" [addr=<address>]          : This field contains the PMIC NVM registers addresses in\n\r");
  printf("                             dec/hex/oct format\n\r");
  printf(" [value=<value>]           : Value to be written in dec/hex/oct format. Multiple\n\r");
  printf("                             addresses can be written at the same time\n\r");
  printf("                             (i.e. write addr=0 value=0xff addr=2 value=0x2F)\n\r");
  printf(">update                    : This command starts programming of PMIC NVM with shadow registers\n\r");
  printf("                             %d bytes starting from 0x%x at the same command line\n\r",
         identified_pmic.NVMSize, identified_pmic.NVMStartAddress);
  printf(">exit                      : This command return to the Main menu\n\r");
}

/**
  * @brief print the pmic nvm
  * @param argc:
  *      argv:
  * @retval None
  */
static void print_displ(int argc, char *argv[])
{
  errno = 0;
  once = true;
  uint8_t idx;
  uint8_t pmic_nvm_shadow[identified_pmic.NVMSize];

  PMIC_Util_ReadWrite(pmic_nvm_shadow, PMIC_SHADOW_READ, &identified_pmic);

  printf("address | value\n\r");
  printf("----------------\n\r");

  for (idx = 0; idx < identified_pmic.NVMSize; idx++)
  {
    printf(" 0x%x   | 0x%x \n\r", (identified_pmic.NVMStartAddress + idx), pmic_nvm_shadow[idx]);
    printf("----------------\n\r");
  }
}

/**
  * @brief print command error message
  * @param None
  * @retval None
  */
static void print_command_error(void)
{
  printf("\n\rError: Wrong command");
  printf("\n\rPlease refer to help for the supported commands\n\r");
  return;
}

/**
  * @brief print out of range error message
  * @param num: user pmic nvm address
  * @retval None
  */
static void print_outofrange_error(uint8_t address)
{
  printf("\n\rError: addr=0x%x is out of range, it will be ignored", address);
  printf("\n\rPlease refer to help for the supported ranges\n\r");
  return;
}

/**
  * @brief print help message
  * @param argc:
  *        argv:
  * @retval None
  */
static void print_write(int argc, char *argv[])
{
  uint8_t nvm_reg_addr;
  auto_conf = false;
  prev_pmic_cmd = 0;
  errno = 0;
  bool show_notification = false;


  /* if arguments size is less than 3 */
  if (argc < 3)
  {
    /* print command error message */
    print_command_error();
  }

  for (int idx = 0; idx < argc - 1 - prev_pmic_cmd; idx += 2)
  {
    /* find the '=' character */
    cmd = strtok(argv[idx + prev_pmic_cmd], "=");
    val_str = strtok(NULL, "=");

    /* if the left '=' string is "addr" */
    if (!strcmp(cmd, "addr"))
    {
      /* Convert string to number */
      val = strtoul(val_str, &end_ptr, 0);

      /* if string to number conversion is OK */
      if (end_ptr != val_str)
      {
        /* if the number is not valid */
        if ((val == 0 || val == ULONG_MAX) && errno == ERANGE)
        {
          /* print error command message */
          print_command_error();
        }

        /* if value is in NVM address range */
        if ((val >= identified_pmic.NVMStartAddress)
            && (val <= (identified_pmic.NVMStartAddress + identified_pmic.NVMSize) - 1U))
        {
          nvm_reg_addr = val;

          /* find the '=' character in the next argument */
          cmd = strtok(argv[idx + 1 + prev_pmic_cmd], "=");
          val_str = strtok(NULL, "=");

          if (!strcmp(cmd, "value"))
          {
            /* Convert string to number */
            val = strtoul(val_str, &end_ptr, 0);

            /* if string to number conversion is OK */
            if (end_ptr != val_str)
            {
              /* if the value is not valid */
              if ((val == 0 || val == ULONG_MAX) && errno == ERANGE)
              {
                /* print error command message */
                print_command_error();
              }
              else
              {
                nvm_buffer[nvm_reg_addr - identified_pmic.NVMStartAddress] = val;
                show_notification = true;
              }
            }
            else /* if string to number conversion is not OK */
            {
              /* print error command message */
              print_command_error();
            }
          }
        }
        else /* if value is not in address range */
        {
          /* print out of range error message */
          print_outofrange_error(val);
        }
      }
      else /* if string to number conversion is not OK */
      {
        /* print error command message */
        print_command_error();
      }
    }
    else /* if the left '=' string is not "addr" */
    {
      /* print error command message */
      print_command_error();
    }
  }

  if (show_notification == true)
  {
    printf("\n\rNote: This only updates the local copy.\n\r");
    printf("\n\rTo Program the NVM use \"update\" command \n\r");
  }
}

static void print_update(int argc, char *argv[])
{
  printf("\n\rPlease verify the data below..?\n\r");
  print_modified_values();
  uint8_t tmp[identified_pmic.NVMSize];

  printf("\n\rWarning: Do you confirm?  [y/n]\n\r");
  /* Get the user entry */
  get_entry_string(entry);

  /* while the entry is not the expected one */
  while ((entry[0] != 'y') && (entry[0] != 'Y') && (entry[0] != 'n') && (entry[0] != 'N'))
  {
    printf("Error: type 'y' or 'Y' for yes and type 'n' or 'N' for no\n\r");
    /* Get the user entry */
    get_entry_string(entry);
  }
  /* if entry is yes */
  if ((entry[0] == 'y') || (entry[0] == 'Y'))
  {
    printf("\n\rThe operation was confirmed...\n\r");
    PMIC_Util_ReadWrite(nvm_buffer, PMIC_SHADOW_WRITE, &identified_pmic);
    PMIC_Util_ReadWrite(tmp, PMIC_SHADOW_READ, &identified_pmic);
    if (memcmp((const void *) nvm_buffer, (const void *)tmp, identified_pmic.NVMSize) == 0U)
    {
      printf("\n\rThe operation was success...\n\r");
    }
    else
    {
      printf("\n\rThe operation Failed...\n\r");
    }

  }
  else /* if entry is no */
  {
    printf("\n\rWarning: The operation was cancelled...\n\r");
    printf("\n\rAttention: Do you want to discard local copy?  [y/n]\n\r");

    /* Get the user entry */
    get_entry_string(entry);
    /* while the entry is not the expected one */

    while ((entry[0] != 'y') && (entry[0] != 'Y') && (entry[0] != 'n') && (entry[0] != 'N'))
    {
      printf("Error: type 'y' or 'Y' for yes and type 'n' or 'N' for no\n\r");
      /* Get the user entry */
      get_entry_string(entry);
    }
    /* if entry is yes */
    if ((entry[0] == 'y') || (entry[0] == 'Y'))
    {
      /* Clear the buffer by reading nvm */
      PMIC_Util_ReadWrite((uint8_t *)&nvm_buffer, PMIC_SHADOW_READ, &identified_pmic);
    }
  }
}

static bool print_exit(int argc, char *argv[])
{
  once_nvm_read_in_buffer = false;
  return true;
}

static void print_modified_values(void)
{
  uint8_t idx;
  uint8_t pmic_nvm_shadow[identified_pmic.NVMSize];

  PMIC_Util_ReadWrite(pmic_nvm_shadow, PMIC_SHADOW_READ, &identified_pmic);

  printf("address | value\n\r");
  printf("----------------\n\r");
  for (idx = 0; idx < identified_pmic.NVMSize; idx++)
  {
    if (nvm_buffer[idx] == pmic_nvm_shadow[idx])
    {
      printf(" 0x%x   | 0x%x \n\r", (identified_pmic.NVMStartAddress + idx), pmic_nvm_shadow[idx]);
    }
    else
    {
      printf(" 0x%x   | 0x%x -> (0x%x)\n\r", (identified_pmic.NVMStartAddress + idx), pmic_nvm_shadow[idx],
             nvm_buffer[idx]);
    }
    printf("----------------\n\r");
  }
}
/**
  * @brief free the argv
  * @param argc:
  *        argv:
  * @retval None
  */
static void free_args(int argc, char *argv[])
{
  int i;

  for (i = 1; i < argc; i++)
  {
    free(argv[i - 1]);
  }
}

/**
  * @brief print help message
  * @param entry: user entry
  * @retval None
  */
static void get_entry_string(char *entry)
{
  uint8_t user_entry_valid = 0;
  char user_entry_value = 0;
  int i = 0;

#ifndef __TERMINAL_IO__
  Serial_Putchar(0xd);
  Serial_Putchar('U');
  Serial_Putchar('s');
  Serial_Putchar('e');
  Serial_Putchar('r');
  Serial_Putchar('>');
#else
  printf("\rUser>\n\r");
#endif /* __TERMINAL_IO__ */

  while (user_entry_valid == 0)
  {
    /* Scan for user entry */
    user_entry_value = Serial_Scanf(255);

#ifndef __TERMINAL_IO__
    /* Print the entry character */
    Serial_Putchar(user_entry_value);

    /* if carriage return is detected */
    if (user_entry_value == 0xd)
    {
      entry[i] = '\0';
      printf("\r\n");
      user_entry_valid = 1;
    }
    /* if cancel or delete is detected */
    else if (user_entry_value == 0x18 || user_entry_value == 0x7f)
    {
      i--;
    }
#else
    /* if new line is detected */
    if ((user_entry_value == 0xa) || (user_entry_value == 0xd))
    {
      entry[i] = '\0';
      printf("\r\n");
      user_entry_valid = 1;
      fflush(stdin);
    }
#endif /* __TERMINAL_IO__  */
    else /* if user keep writing */
    {
      entry[i++] = user_entry_value;
    }
  }
}

/**
  * @brief parse the entry string
  * @param entry: user entry
  *      size: user entry size
  *      command: user command
  *      argv:
  * @retval number of arguments
  */
static int parse_entry_string(char *entry, size_t size, int *command, char *argv[])
{
  int i = 0;
  int n = 0;
  pmic_arg cmd = {0, 0};
  pmic_arg arg = {0, 0};

  /* Identify command and parameters */
  for (i = 0; i < (int)size; i++)
  {
    /* if end of string is detected */
    if (entry[i] == '\0')
    {
      break;
    }

    /* if space is detected */
    if (entry[i] == ' ')
    {
      continue;
    }

    /* if command is not yet detected */
    if (n == 0)
    {
      cmd.off = i;
    }
    else /* if command is already detected */
    {
      arg.off = i;
    }

    /* while entry is not space or end of string */
    while ((entry[i] != ' ') && (entry[i] != '\0'))
    {
      i++;
    }

    /* if command is not yet detected */
    if (n == 0)
    {
      cmd.len = i - cmd.off;
    }
    else /* if command is already detected */
    {
      arg.len = i - arg.off;
      argv[n - 1] = (char *)malloc(arg.len + 1);
      strncpy(argv[n - 1], &entry[arg.off], arg.len);
      strncpy(argv[n - 1] + arg.len, "\0", 1);
    }

    n++;

    if (n > CMD_MAX_ARG + 1)
    {
      printf("Error: too many arguments (max=%d)\n\r", CMD_MAX_ARG);
      printf("\n\rPlease refer to help for the supported commands\n\r");
      return -1;
    }

    i--;
  }

  /* Case of empty string, simple ignore, no error */
  if ((n == 0) && (cmd.len == 0))
  {
    return 0;
  }

  /* Get command */
  for (i = 0; i < PMIC_CMD_MAX; i++)
  {
    if (!strncmp(&entry[cmd.off], pmic_cmd[i].str, cmd.len)
        && (cmd.len == strlen(pmic_cmd[i].str)))
    {
      *command = i;
      break;
    }
  }

  /* Check number of parameters */
  if ((n - 1) < pmic_cmd[*command].param_min)
  {
    printf("Error: %s: argument missing (min=%lu)\n\r", pmic_cmd[*command].str,
           pmic_cmd[*command].param_min);
    printf("Please refer to help for the supported commands\n\r");
    return -3;
  }
  else if ((n - 1) > pmic_cmd[*command].param_max)
  {
    printf("Error: %s: too many arguments (max=%lu)\n\r", pmic_cmd[*command].str,
           pmic_cmd[*command].param_max);
    printf("Please refer to help for the supported commands\n\r");
    return -4;
  }

  /* return number of arguments, including command and parameters */
  return n;
}

/**
  * @brief reads the entire pmic nvm
  * @param void
  * @retval void
  */
static void read_nvm_in_buffer(void)
{
  PMIC_Util_ReadWrite((uint8_t *)&nvm_buffer, PMIC_SHADOW_READ, &identified_pmic);
}

/* Exported functions --------------------------------------------------------*/

/**
  * @brief print help message
  * @param None
  * @retval None
  */
bool pmic_commands_interactive(void)
{
  char buffer[CMD_MAX_LEN];
  char *argv[CMD_MAX_ARG + 1]; /* NULL terminated */
  int argc;
  int cmd;
  bool ret = false;

  /* Init the PMIC */
  PMIC_Util_Init();

  if (once_nvm_read_in_buffer == false)
  {
    if (PMIC_Util_Detect_PMIC(&identified_pmic) == PMIC_ERROR_NONE)
    {
      printf("\n\r----------------------------------\n\r");
      printf("*** PMIC Detected : STPMIC%s ***\n\r", identified_pmic.DisplayString);
      printf("*** NVM SIZE: %d ***\n\r", identified_pmic.NVMSize);
      printf("*** I2C Address Configured: 0x%x ***\n\r", (PMIC_I2C_ADDRESS >> 1));
      printf("----------------------------------\n\r");
      if (identified_pmic.Supported == PMIC_NOT_SUPPORTED)
      {
        printf("\n\r ERROR!!: This PMIC is currently not supported \n\r");
        ret = true;
        return ret;
      }

      read_nvm_in_buffer();
      once_nvm_read_in_buffer = true;

      pmic_cmd[PMIC_CMD_HELP].str       = "help";
      pmic_cmd[PMIC_CMD_HELP].param_min = 0;
      pmic_cmd[PMIC_CMD_HELP].param_max = 0;
      pmic_cmd[PMIC_CMD_DISPL].str  = "displ";
      pmic_cmd[PMIC_CMD_DISPL].param_min  = 0;
      pmic_cmd[PMIC_CMD_DISPL].param_max  = 0;
      pmic_cmd[PMIC_CMD_WRITE].str  = "write";
      pmic_cmd[PMIC_CMD_WRITE].param_min  = 2;
      pmic_cmd[PMIC_CMD_WRITE].param_max  = ((2 * identified_pmic.NVMSize) + 2);
      pmic_cmd[PMIC_CMD_UPDATE].str = "update";
      pmic_cmd[PMIC_CMD_UPDATE].param_min = 0;
      pmic_cmd[PMIC_CMD_UPDATE].param_max = 0;
      pmic_cmd[PMIC_CMD_EXIT].str         =  "exit";
      pmic_cmd[PMIC_CMD_EXIT].param_min   = 0;
      pmic_cmd[PMIC_CMD_EXIT].param_max   = 0;

    }
    else
    {
      printf("\n\r ERROR!!: Unable to detect PMIC \n\r");
      ret = true;
      return ret;
    }
  }

  /* get the entry string */
  get_entry_string(buffer);

  /* get the arguments size */
  argc = parse_entry_string(buffer, strlen(buffer), &cmd, argv);

  /* if the size is not valid */
  if (argc <= 0)
  {
    return ret;
  }

  /* Check the command */
  switch (cmd)
  {
    case PMIC_CMD_HELP:
      print_help();
      break;

    case PMIC_CMD_DISPL:
      print_displ(argc, argv);
      break;

    case PMIC_CMD_WRITE:
      print_write(argc, argv);
      break;

    case PMIC_CMD_UPDATE:
      print_update(argc, argv);
      break;

    case PMIC_CMD_EXIT:
      ret = print_exit(argc, argv);
    default:
      break;
  }

  /* reset argc and argv */
  free_args(argc, argv);

  return ret;
}

/**
  * @brief print pmic header
  * @param None
  * @retval None
  */
void pmic_print_header(void)
{
  printf("\n\r=========== PMIC Command Line interface ===========\n\r");
}
