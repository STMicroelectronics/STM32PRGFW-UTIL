/**
  ******************************************************************************
  * @file    openbl_usart_cmd.c
  * @author  MCD Application Team
  * @brief   Contains USART protocol commands
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
#include "openbl_mem.h"
#include "openbl_usart_cmd.h"
#include "openbootloader_conf.h"
#include "app_openbootloader.h"
#include "usart_interface.h"
#include "external_memory_interface.h"
#include "otp_interface.h"
#include "openbl_util.h"

/* External variables --------------------------------------------------------*/
extern OPENBL_Flashlayout_TypeDef FlashlayoutStruct;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define OPENBL_USART_COMMANDS_NB          8U       /* Number of supported commands */

#define USART_RAM_BUFFER_SIZE             1024U    /* Size of USART buffer used to store received data from the host */

#define OPENBL_USART_PACKET_SIZE          256      /* Size of USART Packet send by the host */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Buffer used to store received data from the host */
static uint8_t USART_RAM_Buf[USART_RAM_BUFFER_SIZE];
static uint8_t phase = PHASE_FLASHLAYOUT;
static uint32_t destination = RAM_WRITE_ADDRESS;
static uint32_t cur_sector = 0;
static uint32_t last_sector = 0;
static uint8_t operation;
static uint32_t packet_number = 0;
static bool is_fl = true;
static uint32_t cur_part = 1;
static OPENBL_Otp_TypeDef Otp;
static uint32_t otp_idx_rp = 0;
static uint32_t otp_idx_wm = 0;

/* Private function prototypes -----------------------------------------------*/
static void OPENBL_USART_GetCommand(void);
static void OPENBL_USART_GetVersion(void);
static void OPENBL_USART_GetID(void);
static void OPENBL_USART_GetPhase(void);
static void OPENBL_USART_ReadMemory(void);
static void OPENBL_USART_Download(void);
static void OPENBL_USART_ReadPartition(void);
static void OPENBL_USART_Start(void);
static uint8_t OPENBL_USART_GetAddress(uint32_t *Address);

/* Exported variables --------------------------------------------------------*/
OPENBL_CommandsTypeDef OPENBL_USART_Commands =
{
  OPENBL_USART_GetCommand,
  OPENBL_USART_GetVersion,
  OPENBL_USART_GetID,
  OPENBL_USART_GetPhase,
  OPENBL_USART_ReadMemory,
  OPENBL_USART_Download,
  OPENBL_USART_ReadPartition,
  OPENBL_USART_Start
};

/* Exported functions---------------------------------------------------------*/
/**
  * @brief  This function is used to get a pointer to the structure that contains the available USART commands.
  * @return Returns a pointer to the OPENBL_USART_Commands struct.
  */
OPENBL_CommandsTypeDef *OPENBL_USART_GetCommandsList(void)
{
  return (&OPENBL_USART_Commands);
}

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  This function is used to get the list of the available USART commands
  * @retval None.
  */
static void OPENBL_USART_GetCommand(void)
{
  uint32_t counter;

  /* Init usefull gloabl varialble */
  otp_idx_rp = 0;

  const uint8_t a_OPENBL_USART_CommandsList[OPENBL_USART_COMMANDS_NB] =
  {
    CMD_GET_COMMAND,
    CMD_GET_VERSION,
    CMD_GET_ID,
    CMD_GET_PHASE,
    CMD_READ_MEMORY,
    CMD_READ_PARTITION,
    CMD_START,
    CMD_DOWNLOAD
  };

  /* Send Acknowledge byte to notify the host that the command is recognized */
  OPENBL_USART_SendByte(ACK_BYTE);

  /* Send the number of commands supported by the USART protocol */
  OPENBL_USART_SendByte(OPENBL_USART_COMMANDS_NB);

  /* Send USART protocol version */
  OPENBL_USART_SendByte(OPENBL_USART_VERSION);

  /* Send the list of supported commands */
  for (counter = 0U; counter < OPENBL_USART_COMMANDS_NB; counter++)
  {
    OPENBL_USART_SendByte(a_OPENBL_USART_CommandsList[counter]);
  }

  /* Send last Acknowledge synchronization byte */
  OPENBL_USART_SendByte(ACK_BYTE);
}

/**
  * @brief  This function is used to get the USART protocol version.
  * @retval None.
  */
static void OPENBL_USART_GetVersion(void)
{
  /* Send Acknowledge byte to notify the host that the command is recognized */
  OPENBL_USART_SendByte(ACK_BYTE);

  /* Send USART protocol version */
  OPENBL_USART_SendByte(OPENBL_USART_VERSION);

  /* Send the Option bytes */
  OPENBL_USART_SendByte(0x00);
  OPENBL_USART_SendByte(0x00);

  /* Send last Acknowledge synchronization byte */
  OPENBL_USART_SendByte(ACK_BYTE);
}

/**
  * @brief  This function is used to get the device ID.
  * @retval None.
  */
static void OPENBL_USART_GetID(void)
{
  /* Send Acknowledge byte to notify the host that the command is recognized */
  OPENBL_USART_SendByte(ACK_BYTE);

  /* Send the number of following bytes - 1 */
  OPENBL_USART_SendByte(0x01);

  /* Send the device ID starting by the MSB byte then the LSB byte */
  OPENBL_USART_SendByte(DEVICE_ID_MSB);
  OPENBL_USART_SendByte(DEVICE_ID_LSB);

  /* Send last Acknowledge synchronization byte */
  OPENBL_USART_SendByte(ACK_BYTE);
}

/**
  * @brief  This function is used to get the phase ID.
  * @retval None.
  */
static void OPENBL_USART_GetPhase(void)
{
  uint8_t length = 1;

  /* First phase is reserved for flashlayout phase */
  if (phase == PHASE_FLASHLAYOUT)
  {
    destination = FLASHLAYOUT_RAM_ADDRESS;
  }
  else /* Other phase and after flashlayout parsing */
  {
    /* Check if there is available partition */
    if (cur_part < FlashlayoutStruct.partsize)
    {
      /* Get the current partition phase ID */
      phase = FlashlayoutStruct.id[cur_part];

      /* Get the destination address based on current partion ip */
      if (!strcmp(FlashlayoutStruct.ip[cur_part], "none"))
      {
        /* If partition ip is "none" destination is RAM address */
        destination = RAM_WRITE_ADDRESS;
      }
      else if (!strcmp(FlashlayoutStruct.ip[cur_part], "nor"))
      {
        /* If partiton ip is "nor" destination is external memory address (qspi nor) */
        destination = EXT_MEMORY_START_ADDRESS;
      }
      else /* Only none, nor are supported */
      {
    	  OPENBL_USART_SendByte(NACK_BYTE);
      }

      /* Go to the next partition */
      cur_part++;

      /* Init the external memories */
      OPENBL_MEM_Init(destination);
    }
    else
    {
      /* End operation after this phase */
      phase = PHASE_END;
    }
  }

  /* Send Acknowledge byte to notify the host that the command is recognized */
  OPENBL_USART_SendByte(ACK_BYTE);

  OPENBL_USART_SendByte(length + 5);               /* Total length */

  OPENBL_USART_SendByte(phase & 0xFF);             /* partition ID */

  OPENBL_USART_SendByte(destination);              /* byte 1 of address */
  OPENBL_USART_SendByte(destination >> 8);         /* byte 2 of address */
  OPENBL_USART_SendByte(destination >> 16);        /* byte 3 of address */
  OPENBL_USART_SendByte(destination >> 24);        /* byte 4 of address */

  OPENBL_USART_SendByte(length);                   /* Information length */

  OPENBL_USART_SendByte(0);                        /* Send information */

  /* Send last Acknowledge synchronization byte */
  OPENBL_USART_SendByte(ACK_BYTE);

  phase++;
}

/**
 * @brief  This function is used to read memory from the device.
 * @retval None.
 */
static void OPENBL_USART_ReadMemory(void)
{
  uint32_t address;
  uint32_t counter;
  uint32_t memory_index;
  uint8_t data;
  uint8_t tmpXOR;

  OPENBL_USART_SendByte(ACK_BYTE);

  /* Get the memory address */
  if (OPENBL_USART_GetAddress(&address) == NACK_BYTE)
  {
    OPENBL_USART_SendByte(NACK_BYTE);
  }
  else
  {
    OPENBL_USART_SendByte(ACK_BYTE);

    /* Get the number of bytes to be received */
    data = OPENBL_USART_ReadByte();
    tmpXOR  = ~data;

    /* Check data integrity */
    if (OPENBL_USART_ReadByte() != tmpXOR)
    {
      OPENBL_USART_SendByte(NACK_BYTE);
    }
    else
    {
      OPENBL_USART_SendByte(ACK_BYTE);

      /* Get the memory index to know from which memory we will read */
      memory_index = OPENBL_MEM_GetMemoryIndex(address);

      /* Read the data (data + 1) from the memory and send them to the host */
      for (counter = ((uint32_t)data + 1U); counter != 0U; counter--)
      {
        OPENBL_USART_SendByte(OPENBL_MEM_Read(address, memory_index));
        address++;
      }
    }
  }
}

/**
 * @brief  This function is used to write in to device memory.
 * @retval None.
 */
static void OPENBL_USART_Download(void)
{
  uint32_t address;
  uint32_t tmpXOR;
  uint32_t counter;
  uint32_t codesize;
  uint8_t *ramaddress;
  uint8_t data;
  uint32_t res;
  uint32_t offset = 0;

  OPENBL_USART_SendByte(ACK_BYTE);

  /* Get the memory address */
  if (OPENBL_USART_GetAddress(&address) == NACK_BYTE)
  {
    OPENBL_USART_SendByte(NACK_BYTE);
  }
  else
  {
    OPENBL_USART_SendByte(ACK_BYTE);

    /* Read number of bytes to be written and data */
    ramaddress = (uint8_t *)USART_RAM_Buf;

    /* Read the number of bytes to be written: Max number of data = data + 1 = 256 */
    data = OPENBL_USART_ReadByte();

    /* Number of data to be written = data + 1 */
    codesize = (uint32_t)data + 1U;

    /* Checksum Initialization */
    tmpXOR = data;

    /* UART receive data and send to RAM Buffer */
    for (counter = codesize; counter != 0U ; counter--)
    {
      data    = OPENBL_USART_ReadByte();
      tmpXOR ^= data;

      *(__IO uint8_t *)(ramaddress) = data;

      ramaddress++;
    }

    /* Send NACK if Checksum is incorrect */
    if (OPENBL_USART_ReadByte() != tmpXOR)
    {
      OPENBL_USART_SendByte(NACK_BYTE);
    }
    else
    {
      /* If otp operation */
      if (operation == PHASE_OTP)
      {
    	  /* If first otp packet */
        if (packet_number == 0)
        {
          /* Check otp version */
          Otp.Version = (USART_RAM_Buf[0] << 0) | (USART_RAM_Buf[1] << 8) | (USART_RAM_Buf[2] << 16) | (USART_RAM_Buf[3] << 24);

          /* NACK if not expected version */
          if (Otp.Version != OPENBL_OTP_VERSION)
          {
            OPENBL_USART_SendByte(NACK_BYTE);
          }

          /* Get global state */
          Otp.GlobalState = (USART_RAM_Buf[4] << 0) | (USART_RAM_Buf[5] << 8) | (USART_RAM_Buf[6] << 16) | (USART_RAM_Buf[7] << 24);

          /* Set offset to 8, bytes number for version and global state */
          offset = 8;
        }

        /* Get otp word and status */
        for (counter = 0 + offset; (counter < codesize) && (otp_idx_wm < OTP_PART_SIZE); counter+=4)
        {
          Otp.OtpPart[otp_idx_wm] = (USART_RAM_Buf[counter] << 0) | (USART_RAM_Buf[counter+1] << 8) | (USART_RAM_Buf[counter+2] << 16) | (USART_RAM_Buf[counter+3] << 24);
          otp_idx_wm++;
        }

        /* Write otp since otp structure is full */
        if (otp_idx_wm == OTP_PART_SIZE)
        {
          OPENBL_OTP_Write(Otp);

          /* Reset otp index */
          otp_idx_wm = 0;
          otp_idx_rp = 0;
        }
      }
      else /* If normal download operation */
      {
        /* If External memory download, erase the sector */
        if (address >= EXT_MEMORY_START_ADDRESS && address <= EXT_MEMORY_END_ADDRESS)
        {
          cur_sector = ((address - EXT_MEMORY_START_ADDRESS) / SECTOR_SIZE) + 1;
          if (cur_sector > last_sector)
          {
            /* Erase sector */
            OPENBL_MEM_SectorErase(address, address, (address + codesize));
            last_sector = cur_sector;
          }
        }

        /* Write data to memory */
        OPENBL_MEM_Write(address, (uint8_t *)USART_RAM_Buf, codesize);

        /* First write memory operation is reserved for the flashlayout */
        if (is_fl)
        {
          /* First packet number of flashlayout download is reserved for ST binary signature */
          if (packet_number == 0)
          {
            /* Skip ST binary signature */
          }
          else
          {
            /* Parse the flashlayout */
            if (parse_flash_layout(address, codesize) == PARSE_ERROR)
            {
              OPENBL_USART_SendByte(NACK_BYTE);
            }
            is_fl = false; /* Leave the flashlayout parsing */
          }
        }

        /* If External memory download, verify data write to memory */
        if (address >= EXT_MEMORY_START_ADDRESS && address <= EXT_MEMORY_END_ADDRESS)
        {
          /* Verify data write to memory */
          res = OPENBL_MEM_Verify(address, (uint32_t)USART_RAM_Buf, codesize, 0);
          if ((res != 0) && (res < (address + codesize)))
          {
            OPENBL_USART_SendByte(NACK_BYTE);
          }
        }
      }

      /* Send last Acknowledge synchronization byte */
      OPENBL_USART_SendByte(ACK_BYTE);
    }
  }
}

/**
 * @brief  This function is used to read data from NVM, Flash.
 * @retval None.
 */
static void OPENBL_USART_ReadPartition(void)
{
  uint32_t offset;
  uint32_t i;
  uint32_t tmpXOR;
  uint32_t codesize;
  uint8_t data;
  uint8_t tmpOffset[4] = {0, 0, 0, 0};
  uint8_t partId;

  OPENBL_USART_SendByte(ACK_BYTE);

  /* Get partition ID byte */
  partId = OPENBL_USART_ReadByte();

  /* Get offset address bytes */
  tmpOffset[3] = OPENBL_USART_ReadByte();
  tmpOffset[2] = OPENBL_USART_ReadByte();
  tmpOffset[1] = OPENBL_USART_ReadByte();
  tmpOffset[0] = OPENBL_USART_ReadByte();

  tmpXOR = tmpOffset[3] ^ tmpOffset[2] ^ tmpOffset[1] ^ tmpOffset[0] ^ partId;

  /* Check the integrity of received data */
  if (OPENBL_USART_ReadByte() != tmpXOR)
  {
    OPENBL_USART_SendByte(NACK_BYTE);
  }
  else
  {
	  /* Get offset value */
    offset = (((uint32_t)tmpOffset[3] << 24) | ((uint32_t)tmpOffset[2] << 16) | ((uint32_t)tmpOffset[1] << 8) | (uint32_t)tmpOffset[0]);

    /* Check if the partion ID is supported */
    switch (partId)
    {
    case PHASE_OTP:
      break;
    default:
    	OPENBL_USART_SendByte(NACK_BYTE);
    }

    /* Save the operation type */
    operation = partId;

    /* Save the packet number */
    packet_number = offset / OPENBL_USART_PACKET_SIZE;

    OPENBL_USART_SendByte(ACK_BYTE);

    /* NbrOfData to read = data + 1 */
    data = OPENBL_USART_ReadByte();

    /* Number of data to be written = data + 1 */
    codesize = (uint32_t)data + 1U;

    /* Size for Word writing */
    codesize /= 4;

    /* Checksum Initialization */
    tmpXOR = data;

    /* Get the data */
    data = OPENBL_USART_ReadByte();

    /* Check the integrity of received data */
    if ((data ^ tmpXOR) != 0xFF)
    {
      OPENBL_USART_SendByte(NACK_BYTE);
    }

    OPENBL_USART_SendByte(ACK_BYTE);

    /* Read otp */
    Otp = OPENBL_OTP_Read();

    /* Check if first otp packet */
    if (offset == 0)
    {
      /* Send the otp version */
      OPENBL_USART_SendWord(Otp.Version);

      /* Send the global state */
      OPENBL_USART_SendWord(Otp.GlobalState);

      /* Update codesize */
      codesize -= 2;
    }

    /* Send otp words */
    for (i=0; i < codesize; i++)
    {
      /* Send otp words until its end and send 0 after to fill */
      if (otp_idx_rp < OTP_PART_SIZE)
      {
        OPENBL_USART_SendWord(Otp.OtpPart[otp_idx_rp]);
        otp_idx_rp++;
      }
      else
      {
        OPENBL_USART_SendWord(0);
      }
    }
  }
}

/**
  * @brief  This function is used to jump to the user application.
  * @retval None.
  */
static void OPENBL_USART_Start(void)
{
  uint32_t address;

  OPENBL_USART_SendByte(ACK_BYTE);

  /* Get memory address */
  if (OPENBL_USART_GetAddress(&address) == NACK_BYTE)
  {
    OPENBL_USART_SendByte(NACK_BYTE);
  }
  else
  {
   /* If the jump address is valid then send ACK */
    OPENBL_USART_SendByte(ACK_BYTE);
  }
}

/**
 * @brief  This function is used to get a valid address.
 * @retval Returns NACK status in case of error else returns ACK status.
 */
static uint8_t OPENBL_USART_GetAddress(uint32_t *Address)
{
  uint8_t tmpAddress[4] = {0, 0, 0, 0};
  uint8_t status;
  uint8_t tmpXOR;

  tmpAddress[3] = OPENBL_USART_ReadByte();
  tmpAddress[2] = OPENBL_USART_ReadByte();
  tmpAddress[1] = OPENBL_USART_ReadByte();
  tmpAddress[0] = OPENBL_USART_ReadByte();

  tmpXOR = tmpAddress[3] ^ tmpAddress[2] ^ tmpAddress[1] ^ tmpAddress[0];

  /* Check the integrity of received data */
  if (OPENBL_USART_ReadByte() != tmpXOR)
  {
    status = NACK_BYTE;
  }
  else
  {
	  /* Get the start address */
    *Address = (((uint32_t)tmpAddress[3] << 24) | ((uint32_t)tmpAddress[2] << 16) | ((uint32_t)tmpAddress[1] << 8) | (uint32_t)tmpAddress[0]);

    /* Get the operation type */
    operation = (uint8_t)(*Address >> 24);

    /* Get the packet number */
    packet_number = (uint32_t)((*Address << 8) >> 8);

    /* Check if the operation is supported */
    switch (operation)
    {
    case PHASE_FLASHLAYOUT:
      break;
    default:
      status = NACK_BYTE;
    }

    /* Check if the address is supported */
    if (*Address == 0xFFFFFFFF)
    {
      status = ACK_BYTE;
    }
    else
    {
      /* Build the real memory address */
      *Address = destination + (packet_number * OPENBL_USART_PACKET_SIZE);

      /* Check if the memory address is valid */
      if (OPENBL_MEM_GetAddressArea(*Address) == AREA_ERROR)
      {
        status = NACK_BYTE;
      }
      else
      {
        status = ACK_BYTE;
      }
    }
  }

  return status;
}
