/**
  ******************************************************************************
  * @file    openbl_usb_cmd.c
  * @author  MCD Application Team
  * @brief   Contains USB protocol commands
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
#include "openbl_usb_cmd.h"
#include "openbl_mem.h"
#include "openbootloader_conf.h"
#include "usb_interface.h"
#include "openbl_util.h"
#include "otp_interface.h"

/* External variables --------------------------------------------------------*/
extern OPENBL_Flashlayout_TypeDef FlashlayoutStruct;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint32_t i, otp_idx;
static uint8_t phase = PHASE_FLASHLAYOUT;
uint8_t count = 0;
static uint32_t cur_sector = 0;
static uint32_t last_sector = 0;
static uint8_t cur_part = PHASE_FLASHLAYOUT;
static bool is_start_operation = false;
uint32_t addr;
static OPENBL_Otp_TypeDef Otp;
/* Private function prototypes -----------------------------------------------*/
uint32_t OPENBL_USB_GetAddress(uint8_t Phase);
uint8_t OPENBL_USB_GetPhase(uint32_t Alt);

/* Exported functions---------------------------------------------------------*/
/**
  * @brief  Memory write routine.
  * @param  pSrc: Pointer to the source buffer. Address to be written to.
  * @param  Alt: USB Alternate.
  * @param  Length: Number of data to be written (in bytes).
  * @retval None.
  */
void OPENBL_USB_Download(uint8_t *pSrc, uint32_t Alt, uint32_t Length, uint32_t BlockNumber)
{
  switch (phase)
  {
  case PHASE_OTP:
    /* Set otp version */
    Otp.Version = (((uint32_t)pSrc[3] << 24) | ((uint32_t)pSrc[2] << 16) | ((uint32_t)pSrc[1] << 8) | (uint32_t)pSrc[0]);

    /* Set otp global state */
    Otp.GlobalState = (((uint32_t)pSrc[7] << 24) | ((uint32_t)pSrc[6] << 16) | ((uint32_t)pSrc[5] << 8) | (uint32_t)pSrc[4]);

	if (BlockNumber == 0)
	{
		/* Set otp values and status */
		for (i = 8, otp_idx = 0; (i < Length && (otp_idx < OTP_PART_SIZE)); i+=4, otp_idx++)
		{
		  Otp.OtpPart[otp_idx] = (((uint32_t)pSrc[i+3] << 24) | ((uint32_t)pSrc[i+2] << 16) | ((uint32_t)pSrc[i+1] << 8) | (uint32_t)pSrc[i]);
		}
	}
	else
	{
		for (i = 0, otp_idx = (254 + ((BlockNumber-1)*256)); (i < Length && (otp_idx < OTP_PART_SIZE)); i+=4, otp_idx++)
		{
		  Otp.OtpPart[otp_idx] = (((uint32_t)pSrc[i+3] << 24) | ((uint32_t)pSrc[i+2] << 16) | ((uint32_t)pSrc[i+1] << 8) | (uint32_t)pSrc[i]);
		}
	}

    /* Write otp */
    if (otp_idx == OTP_PART_SIZE)
    {
       OPENBL_OTP_Write(Otp);
    }
    break;

  case PHASE_0x3:
    /* Write memory */
    OPENBL_MEM_Write(addr, pSrc, Length);
    break;

  case PHASE_0x4:
    /* Init the external memories */
    OPENBL_MEM_Init(addr);

    /* Get the current sector */
    cur_sector = ((addr - EXT_MEMORY_START_ADDRESS) / SECTOR_SIZE) + 1;
    if (cur_sector > last_sector)
    {
      /* Erase sector */
      OPENBL_MEM_SectorErase(addr, addr, (addr + Length));

      /* Update last sector */
      last_sector = cur_sector;
    }

    /* Verify data write to memory */
    uint32_t res = OPENBL_MEM_Verify(addr, (uint32_t)pSrc, Length, 0);
    if ((res != 0) && (res < (addr + Length)))
    {
      /* Error */
      while(1){};
    }
    break;

  case PHASE_FLASHLAYOUT:
    /* first 256 is reserved for binary signature info */
    addr = addr + 256;

    /* Parse the flashlayout */
    if (parse_flash_layout((uint32_t)pSrc + 256, (Length - 256)) == PARSE_ERROR)
    {
      /* Error */
      while(1){};
    }
    break;

  default:
    break;
  }
}

/**
  * @brief  Memory read routine.
  * @param  USB Alternate
  * @param  pDest: Pointer to the destination buffer.
  * @param  Length: Number of data to be read (in bytes).
  * @retval Pointer to the physical address where data should be read.
  */
uint8_t *OPENBL_USB_ReadMemory(uint32_t Alt, uint8_t *pDest, uint32_t Length, uint32_t BlockNumber)
{
  phase = OPENBL_USB_GetPhase(Alt);
  switch (phase)
  {
  case PHASE_CMD:
    if (cur_part == PHASE_FLASHLAYOUT) /* Phase flashlayout */
    {
      phase = PHASE_FLASHLAYOUT;
    }
    else if (cur_part < FlashlayoutStruct.partsize) /* Phase external memories 0x3 & 0x4 */
    {
      phase = FlashlayoutStruct.id[cur_part];
    }
    else /* Phase END */
    {
      phase = PHASE_END;
    }

    /* Get the phase address */
    addr = OPENBL_USB_GetAddress(phase);

    /* Get phase command response */
    pDest[0] = phase;
    pDest[1] = (uint8_t)(addr >> 0);
    pDest[2] = (uint8_t)(addr >> 8);
    pDest[3] = (uint8_t)(addr >> 16);
    pDest[4] = (uint8_t)(addr >> 24);
    pDest[5] = 0x00;
    pDest[6] = 0x00;
    pDest[7] = 0x00;
    pDest[8] = 0x00;

    /* Start operation call after each phase operation */
    /* If current operation is phase operation, next operation is start operation */
    if (is_start_operation) /* Start operation */
    {
      /* Next operation is phase operation */
      is_start_operation = false;

      /* Update current partition */
      cur_part++;
    }
    else /* Phase operation */
    {
      /* Next operation is start operation */
      is_start_operation = true;
    }
    break;

  case PHASE_OTP:
    /* Read otp */
	if (BlockNumber == 0)
	{
		Otp = OPENBL_OTP_Read();


    /* Get otp version */
    pDest[0] = (uint8_t)Otp.Version;
    pDest[1] = (uint8_t)(Otp.Version >> 8);
    pDest[2] = (uint8_t)(Otp.Version >> 16);
    pDest[3] = (uint8_t)(Otp.Version >> 24);

    /* Get otp global state */
    pDest[4] = (uint8_t)Otp.GlobalState;
    pDest[5] = (uint8_t)(Otp.GlobalState >> 8);
    pDest[6] = (uint8_t)(Otp.GlobalState >> 16);
    pDest[7] = (uint8_t)(Otp.GlobalState >> 24);

    /* Get otp values and status */
    for (i = 8, otp_idx = 0; (i < Length && (otp_idx < OTP_PART_SIZE)); i+=4, otp_idx++)
    {
      /* 127 OTP sent for the first 1024 bytes block - */
      /* BlockNumber == 0 : otp_idx=0..251 - 127 OTP x 8 bytes */
      pDest[i] = (uint8_t)(Otp.OtpPart[otp_idx]);
      pDest[i+1] = (uint8_t)(Otp.OtpPart[otp_idx] >> 8);
      pDest[i+2] = (uint8_t)(Otp.OtpPart[otp_idx] >> 16);
      pDest[i+3] = (uint8_t)(Otp.OtpPart[otp_idx] >> 24);
    }

	}
	else
	{
	    for (i = 0, otp_idx = (254 + ((BlockNumber-1)*256)); (i < Length && (otp_idx < OTP_PART_SIZE)); i+=4, otp_idx++)
	    {

	      /*
	       * BlockNumber == 1 : otp_idx=254..505 - 128 OTP x 8 bytes = 1024 bytes - 8 bytes = 4 byes for OTP value & 4 bytes for status
	       * BlockNumber == 2 : otp_idx=506..761 - 1024 bytes
	       * BlockNumber == 3 : otp_idx=762..767 - 1024 bytes
	       */

	      pDest[i] = (uint8_t)(Otp.OtpPart[otp_idx]);
	      pDest[i+1] = (uint8_t)(Otp.OtpPart[otp_idx] >> 8);
	      pDest[i+2] = (uint8_t)(Otp.OtpPart[otp_idx] >> 16);
	      pDest[i+3] = (uint8_t)(Otp.OtpPart[otp_idx] >> 24);
	    }
	}
    break;
  default:

    break;
  }

  /* Return a valid address to avoid HardFault */
  return pDest;
}

/**
  * @brief  Link between USB Alternate and STM32CubeProgrammer phase
  * @param  Alt: USB Alternate.
  * @retval STM32CubeProgramer Phase.
  */
uint8_t OPENBL_USB_GetPhase(uint32_t Alt)
{
  uint8_t ret;

  switch (Alt)
  {
  case 0:
    ret = PHASE_FLASHLAYOUT;
    break;

  case 1:
    ret = PHASE_0x3;
    break;

  case 2:
    ret = PHASE_0x4;
    break;

  case 3:
    ret = PHASE_CMD;
    break;

  case 4:
    ret = PHASE_OTP;
    break;

  default:
    ret = PHASE_END;
    break;
  }

  return ret;
}

/**
  * @brief  Get Address of STM32CubeProgrammer phase
  * @param  Phase: USB Alternate.
  * @retval STM32CubeProgrammer Phase.
  */
uint32_t OPENBL_USB_GetAddress(uint8_t Phase)
{
  uint32_t ret;

  switch (Phase)
  {
  case 0x00:
    ret = FLASHLAYOUT_RAM_ADDRESS;
    break;

  case 0x03:
    ret = RAM_WRITE_ADDRESS;
    break;

  case 0x04:
    ret = EXT_MEMORY_START_ADDRESS;
    break;

  default:
    ret = UNDEF_ADDRESS;
    break;
  }

  return ret;
}
