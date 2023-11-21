/**
  ******************************************************************************
  * @file    otp_interface_cli.c
  * @author  MCD Application Team
  * @brief   OTP Interface CLI
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
#include "otp_interface_cli.h"
#include "otp_interface_cli_util.h"
#include "otp_util.h"
#include <limits.h>
#include <errno.h>
#include <stdbool.h>

/* Private typedef -----------------------------------------------------------*/
typedef enum {
  OTP_CMD_HELP = 0,
  OTP_CMD_DISPL,
  OTP_CMD_WRITE,
  OTP_CMD_LOCK,
  OTP_CMD_MAX,
} otp_cmd_id;

typedef struct {
  char *str;
  uint32_t param_min;
  uint32_t param_max;
} otp_command;

typedef struct {
  uint32_t off;
  uint32_t len;
} otp_arg;

const otp_command otp_cmd[OTP_CMD_MAX] = {
    [OTP_CMD_HELP]         = { "help"        , 0, 0 },
    [OTP_CMD_DISPL]        = { "displ"       , 0, OTP_VALUE_SIZE },
    [OTP_CMD_WRITE]        = { "write"       , 2, ((2 * OTP_VALUE_SIZE) + 2)},
    [OTP_CMD_LOCK]         = { "lock"        , 1, 2 },
};

/* Private define ------------------------------------------------------------*/
#define CMD_MAX_LEN 1024
#define CMD_MAX_ARG 255
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static Otp_TypeDef Otp;
static uint32_t idx;

static char *cmd;
static char *val_str;
static char *end_ptr;
static uint32_t val;
static bool once;
static char entry[CMD_MAX_LEN];
static bool auto_conf;
uint32_t prev_opt_cmd;

/* Private function prototypes -----------------------------------------------*/
static void print_help(void);
static void free_args(int argc, char *argv[]);
static void get_entry_string(char *entry);
static int parse_entry_string(char *entry, size_t size, int *command, char *argv[]);
static void print_displ(int argc, char *argv[]);
static void print_command_error(void);
static void print_outofrange_error(uint32_t num);
static void write_otp(uint32_t word, uint32_t val, bool lock);
static void lock_otp(uint32_t word);
static void print_otp_more_status(Otp_TypeDef Otp, uint32_t word);

/* Exported variables --------------------------------------------------------*/

/**
  * @brief print help message
  * @param None
  * @retval None
  */
static void print_help(void)
{
  printf("commands:\n\r");
  printf("-help                      : This command allows the user to display help menu\n\r");
  printf("-displ                     : This command allows the user to display all or\n\r");
  printf("                             part of the OTP structure\n\r");
  printf(" [word=<id>]               : {Optional} display a specific OTP word {values and\n\r");
  printf("                             status} up to %d words [0 to %d]. <id> value in\n\r",  OTP_VALUE_SIZE, (OTP_VALUE_SIZE - 1));
  printf("                             dec/hex/oct format. Multiple words can be displayed\n\r");
  printf("                             at the same time (i.e. displ word=0 word=2)\n\r");
  printf("-write                     : This command allows to fuse or update OTP words up\n\r");
  printf("                             to %d words [0 to %d] at the same command line\n\r", OTP_VALUE_SIZE, (OTP_VALUE_SIZE - 1));
  printf(" [-y]                      : {Optional} enable auto confirmation\n\r");
  printf(" [lock]                    : {Optional} indicate the operation type, update or\n\r");
  printf("                             permanent lock\n\r");
  printf(" [word=<id>]               : This field contains the OTP word number in\n\r");
  printf("                             dec/hex/oct format\n\r");
  printf(" [value=<value>]           : Value to be written in dec/hex/oct format. Multiple\n\r");
  printf("                             words can be written at the same time\n\r");
  printf("                             (i.e. write word=0 value=0xff word=2 value=0x2F)\n\r");
  printf("-lock                      : This command allows to fuse permanently OTP words\n\r");
  printf("                             up to %d words [0 to %d] at the same command line\n\r", OTP_VALUE_SIZE, (OTP_VALUE_SIZE - 1));
  printf(" [-y]                      : {Optional} enable auto confirmation\n\r");
  printf(" [word=<id>]               : This field contains the OTP word number in\n\r");
  printf("                             dec/hex/oct format.\n\r");
}

/**
  * @brief print otp status description
  * @param Otp: otp structure
  * 	   word: otp word
  * @retval None
  */
static void print_otp_more_status(Otp_TypeDef Otp, uint32_t word)
{
  if ((Otp.OtpPart[word] & OTP_READ_ERROR) == OTP_READ_ERROR)
  {
    printf("\n\r                                   |_[00] Invalid");
  }
  if ((Otp.OtpPart[word] & OTP_LOCK_ERROR) == OTP_LOCK_ERROR)
  {
    printf("\n\r                                   |_[26] Lock error");
  }
  if ((Otp.OtpPart[word] & OTP_STICKY_PROG_LOCK_MASK) == OTP_STICKY_PROG_LOCK_MASK)
  {
    printf("\n\r                                   |_[27] Sticky programming lock");
  }
  if ((Otp.OtpPart[word] & OTP_STICKY_WRITE_LOCK_MASK) == OTP_STICKY_WRITE_LOCK_MASK)
  {
    printf("\n\r                                   |_[28] Shadow write sticky lock");
  }
  if ((Otp.OtpPart[word] & OTP_STICKY_READ_LOCK_MASK) == OTP_STICKY_READ_LOCK_MASK)
  {
    printf("\n\r                                   |_[29] Shadow read sticky lock");
  }
  if ((Otp.OtpPart[word] & OTP_PERM_LOCK_MASK) == OTP_PERM_LOCK_MASK)
  {
    printf("\n\r                                   |_[30] Permanent programming lock");
  }
}

/**
  * @brief print the otp
  * @param argc:
  * 	   argv:
  * @retval None
  */
static void print_displ(int argc, char *argv[])
{
  errno = 0;
  once = true;

  /* Read otp */
  Otp = OTP_Util_Read();

  /* In case of specific otp words display */
  if (argc >= 2)
  {
    for (int idx = 0; idx < argc - 1; idx++)
    {
      /* find the '=' character */
      cmd = strtok(argv[idx], "=");
      val_str = strtok(NULL, "=");

      /* if the left '=' string is "word" */
      if (!strcmp(cmd, "word"))
      {
        /* Convert the right '=' string to a number */
        val = strtoul(val_str, &end_ptr, 0);

        /* if string to number conversion is OK */
        if (end_ptr != val_str)
        {
          /* if the number is not valid */
          if ((val == 0 || val == ULONG_MAX) && errno == ERANGE)
          {
            /* print error command */
            print_command_error();
          }

          /* if value is in otp word range */
          if ((val >= 0) && (val <= OTP_VALUE_SIZE))
          {
            /* print this once */
            if (once)
            {
              printf("\n\rOTP DATA");
              printf("\n\r---------------------------------------------------------------------");
              printf("\n\r    ID    |      VALUE       |     STATUS     ");
              once = false;
            }
            printf("\n\r---------------------------------------------------------------------");
            printf("\n\r    %02ld    |    0x%08lX    |    0x%08lX    ", val, Otp.OtpPart[val*2], Otp.OtpPart[val*2+1]);
            /* print otp status description */
            print_otp_more_status(Otp, val*2+1);
          }
          else /* if value is out of otp word range */
          {
            /* print out of range error message */
            print_outofrange_error(val);
          }
        }
      }
      else /* if the left '=' string is not "word" */
      {
        /* print command error message */
        print_command_error();
      }
    }
    printf("\n\r---------------------------------------------------------------------\n\r");
  }
  else /* In case of all otp display */
  {
    printf("\n\rOTP GLOBAL STATE");
    if (Otp.GlobalState == BSEC_SEC_OTP_OPEN)
    {
      printf("\n\r |_ State            : Secure Open");
    }
    else if (Otp.GlobalState == BSEC_SEC_OTP_CLOSED)
    {
      printf("\n\r |_ State            : Secure Closed");
    }
    else
    {
      printf("\n\r |_ State            : Invalid");
    }

    printf("\n\r |_ Hardware Key Set : N");
    printf("\n\r |_ Encrypted data   : N\n\r");

    printf("\n\rOTP DATA");
    printf("\n\r---------------------------------------------------------------------");
    printf("\n\r    ID    |      VALUE       |     STATUS     ");
    printf("\n\r---------------------------------------------------------------------");
    for (idx = 0; idx < OTP_PART_SIZE; idx+=2)
    {
      printf("\n\r    %02lu    |    0x%08lX    |    0x%08lX    ", (idx/2), Otp.OtpPart[idx], Otp.OtpPart[idx+1]);
      /* print otp status description */
      print_otp_more_status(Otp, idx+1);
    }
    printf("\n\r---------------------------------------------------------------------\n\r");
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
  * @param num: user otp word number
  * @retval None
  */
static void print_outofrange_error(uint32_t num)
{
  printf("\n\rError: word=0x%08lX is out of range, it will be ignored", num);
  printf("\n\rPlease refer to help for the supported ranges\n\r");
  return;
}

/**
  * @brief print help message
  * @param word: otp word
  * 	   val: otp value
  * 	   lock: otp lock status
  * @retval None
  */
static void write_otp(uint32_t word, uint32_t val, bool lock)
{
  /* Read the otp */
  Otp = OTP_Util_Read();

  /* Change the otp value */
  Otp.OtpPart[word * 2] = val;

  /* if lock is enabled */
  if (lock)
  {
    /* lock the otp */
    Otp.OtpPart[word * 2 + 1] |= OTP_PERM_LOCK_MASK;
  }

  /* Apply the change */
  Otp.OtpPart[word * 2 + 1] |= OTP_REQUEST_UPDATE_MASK;

  /* Write the otp */
  if (!OTP_Util_Write(Otp))
  {
    /* In case of success */
    printf("\n\rSUCCESS\n\r");
  }
  else
  {
    /* In case of failure */
    printf("\n\rFAIL\n\r");
  }
}

/**
  * @brief print help message
  * @param word: otp word
  * @retval None
  */
static void lock_otp(uint32_t word)
{
  /* Read the otp */
  Otp = OTP_Util_Read();

  /* Lock the otp */
  Otp.OtpPart[word * 2 + 1] |= OTP_PERM_LOCK_MASK;

  /* Apply the change */
  Otp.OtpPart[word * 2 + 1] |= OTP_REQUEST_UPDATE_MASK;

  /* Write the otp */
  if (!OTP_Util_Write(Otp))
  {
    /* In case of success */
    printf("\n\rSUCCESS\n\r");
  }
  else
  {
    /* In case of failure */
    printf("\n\rFAIL\n\r");
  }
}

/**
  * @brief print help message
  * @param argc:
  * 	     argv:
  * @retval None
  */
static void print_write(int argc, char *argv[])
{
  uint32_t opt_word;
  bool lock = false;
  auto_conf = false;
  prev_opt_cmd = 0;
  errno = 0;

  /* if arguments size is less than 3 */
  if (argc < 3)
  {
    /* print command error message */
    print_command_error();
  }

  /* if arguments size is equal or superior to 4 */
  if (argc >= 4)
  {
    /* if arguments size is pair */
    if (argc % 2 == 0)
    {
      /* if auto confirmation is enabled */
      if(!strcmp(argv[0], "-y"))
      {
        auto_conf = true;
        prev_opt_cmd++;
        printf("Warning: Auto confirmation is enabled.\n\r");
      }
      else if (!strcmp(argv[0], "lock")) /* if lock is enabled */
      {
        lock = true;
        prev_opt_cmd++;
      }
      else /* error case as none of auto confirmation or lock command is enabled and size is pair */
      {
        /* print error command message */
        print_command_error();
      }
    }
    else /* if arguments size is impair */
    {
      /* if auto confirmation is enabled */
      if(!strcmp(argv[0], "-y"))
      {
        /* if lock is enabled */
        if(!strcmp(argv[1], "lock"))
        {
          lock = true;
        }
        else /* if lock is not enabled */
        {
          /* print command error message */
          print_command_error();
        }
        auto_conf = true;
        prev_opt_cmd = 2;
        printf("Warning: Auto confirmation is enabled.\n\r");
      }
    }
  }

  for (int idx = 0; idx < argc - 1 - prev_opt_cmd; idx += 2)
  {
    /* find the '=' character */
    cmd = strtok(argv[idx + prev_opt_cmd], "=");
    val_str = strtok(NULL, "=");

    /* if the left '=' string is "word" */
    if (!strcmp(cmd, "word"))
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

        /* if value is in otp word range */
        if ((val >= 0) && (val <= OTP_VALUE_SIZE))
        {
          opt_word = val;

          /* find the '=' character in the next argument */
          cmd = strtok(argv[idx + 1 + prev_opt_cmd], "=");
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
                printf("\n\rOTP Write command:");
                printf("\n\rYou are trying to write OTP with the following inputs:");
                printf("\n\r--------------------------");
                printf("\n\r Word     |  Value        ");
                printf("\n\r--------------------------");
                printf("\n\r %02ld       |  0x%08lX         ", opt_word, val);
                if (lock)
                {
                  printf("\n\r Lock     |  YES          ");
                }
                else
                {
                  printf("\n\r Lock     |  NO          ");
                }
                printf("\n\r--------------------------");
                printf("\n\rWarning: This operation cannot be reverted and may brick your device.");
                /* if auto confirmation is enabled */
                if (auto_conf)
                {
                  /* write the otp */
                  write_otp(opt_word, val, lock);
                }
                else
                {
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
                    printf("The operation was confirmed...");
                    write_otp(opt_word, val, lock);
                  }
                  else /* if entry is no */
                  {
                    printf("Warning: The operation was cancelled...\n\r");
                  }
                }
              }
            }
            else /* if string to number conversion is not OK */
            {
              /* print error command message */
              print_command_error();
            }
          }
        }
        else /* if value is in otp word range */
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
    else /* if the left '=' string is not "word" */
    {
      /* print error command message */
      print_command_error();
    }
  }
}

/**
  * @brief print help message
  * @param argc:
  * 	   argv:
  * @retval None
  */
static void print_lock(int argc, char *argv[])
{
  auto_conf = false;
  prev_opt_cmd = 0;
  errno = 0;

  /* if arguments size is equal to 3 */
  if (argc == 3)
  {
    /* if auto confirmation is enabled */
    if(!strcmp(argv[0], "-y"))
    {
      auto_conf = true;
      prev_opt_cmd++;
      printf("Warning: Auto confirmation is enabled.\n\r");
    }
    else /* if auto confirmation is not enabled */
    {
      /* print error command message */
      print_command_error();
    }
  }

  /* find the '=' character of the next argument */
  cmd = strtok(argv[0 + prev_opt_cmd], "=");
  val_str = strtok(NULL, "=");

  /* if the left '=' string is "word" */
  if (!strcmp(cmd, "word"))
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

      /* if value is in otp word range */
      if ((val >= 0) && (val <= OTP_VALUE_SIZE))
      {
        /* Read the otp */
        Otp = OTP_Util_Read();

        printf("OTP Lock command:");
        printf("\n\rYou are trying to lock some OTP words with the following inputs:");
        printf("\n\r--------------------------");
        printf("\n\r Word     |  Value        ");
        printf("\n\r--------------------------");
        printf("\n\r %02ld       |  0x%08lX         ", val, Otp.OtpPart[2 *val]);
        printf("\n\r Lock     |  YES          ");
        printf("\n\r--------------------------");
        printf("\n\rWarning: This operation cannot be reverted and may brick your device.");

        /* if auto confirmation is enabled */
        if (auto_conf)
        {
          lock_otp(val);
        }
        else /* if auto confirmation is not enabled */
        {
          printf("\n\rWarning: Do you confirm?  [y/n]\n\r");
          /* get the user entry */
          get_entry_string(entry);

          /* while the user entry is not the one expected */
          while ((entry[0] != 'y') && (entry[0] != 'Y') && (entry[0] != 'n') && (entry[0] != 'N'))
          {
            printf("Error: type 'y' or 'Y' for yes and type 'n' or 'N' for no\n\r");
            /* get the user entry */
            get_entry_string(entry);
          }
          /* if entry is yes */
          if ((entry[0] == 'y') || (entry[0] == 'Y'))
          {
            printf("The operation was confirmed...");
            /* lock the otp */
            lock_otp(val);
          }
          else /* if entry is no */
          {
            printf("Warning: The operation was cancelled...\n\r");
          }
        }
      }
      else /* if value is out of otp word range */
      {
        /* print out of range error message */
        print_outofrange_error(val);
      }
    }
    else /* if string to number conversion is not OK */
    {
      /* print command error message */
      print_command_error();
    }
  }
  else /* if the left '=' string is not "word" */
  {
    /* print command error message */
    print_command_error();
  }
}

/**
  * @brief free the argv
  * @param argc:
  * 	     argv:
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
#endif

  while(user_entry_valid == 0)
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
    if ( (user_entry_value == 0xa) || (user_entry_value == 0xd) )
    {
      entry[i] = '\0';
      printf("\r\n");
      user_entry_valid = 1;
    }
#endif
    else /* if user keep writing */
    {
      entry[i++] = user_entry_value;
    }
  }
}

/**
  * @brief parse the entry string
  * @param entry: user entry
  * 	   size: user entry size
  * 	   command: user command
  * 	   argv:
  * @retval number of arguments
  */
static int parse_entry_string(char *entry, size_t size, int *command, char *argv[])
{
  int i = 0;
  int n = 0;
  otp_arg cmd = {0, 0};
  otp_arg arg = {0, 0};

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
  for (i = 0; i < OTP_CMD_MAX; i++)
  {
    if (!strncmp(&entry[cmd.off], otp_cmd[i].str, cmd.len)
        && (cmd.len == strlen(otp_cmd[i].str)))
    {
      *command = i;
      break;
    }
  }

  /* Check number of parameters */
  if ((n - 1) < otp_cmd[*command].param_min)
  {
    printf("Error: %s: argument missing (min=%lu)\n\r", otp_cmd[*command].str,
                                                otp_cmd[*command].param_min);
    printf("Please refer to help for the supported commands\n\r");
    return -3;
  }
  else if ((n - 1) > otp_cmd[*command].param_max)
  {
    printf("Error: %s: too many arguments (max=%lu)\n\r", otp_cmd[*command].str,
                                                  otp_cmd[*command].param_max);
    printf("Please refer to help for the supported commands\n\r");
    return -4;
  }

  /* return number of arguments, including command and parameters */
  return n;
}

/* Exported functions --------------------------------------------------------*/

/**
  * @brief print console header
  * @param None
  * @retval None
  */
void print_header(void)
{
  /* Output a message on Hyperterminal using printf function */
  printf("\n\r=============== OTP Serial Interface ===============\r");
  printf("\n\rTarget: %s \r", MX_MODEL);
  printf("\n\rPrint 'help' to see OTP commands\n\r");
}

/**
  * @brief print help message
  * @param None
  * @retval None
  */
void otp_commands_interactive(void)
{
  char buffer[CMD_MAX_LEN];
  char *argv[CMD_MAX_ARG + 1]; /* NULL terminated */
  int argc;
  int cmd;

  /* Init the otp */
  OTP_Util_Init();

  /* get the entry string */
  get_entry_string(buffer);

  /* get the arguments size */
  argc = parse_entry_string(buffer, strlen(buffer), &cmd, argv);

  /* if the size is not valid */
  if (argc <= 0)
  {
    return;
  }

  /* Check the command */
  switch (cmd)
  {
  case OTP_CMD_HELP:
    print_help();
    break;

  case OTP_CMD_DISPL:
    print_displ(argc, argv);
    break;

  case OTP_CMD_WRITE:
    print_write(argc, argv);
    break;

  case OTP_CMD_LOCK:
    print_lock(argc, argv);
    break;

  default:
    break;
  }

  /* reset argc and argv */
  free_args(argc, argv);
}
