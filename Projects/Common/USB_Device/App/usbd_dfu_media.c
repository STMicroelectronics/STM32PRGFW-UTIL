/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    USB_Device/App/usbd_dfu_media.c
  * @author  MCD Application Team
  * @brief   This file implements the USB Device
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "openbl_usb_cmd.h"
#include "usb_interface.h"
#include "openbl_mem.h"
#include "usbd_dfu_media.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
#define VIRTUAL_DESC_STR    "@virtual /0xF1/1*512Be"
#define VIRTUAL_DESC_SIZE    (512)
extern USBD_HandleTypeDef hUsbDeviceHS;
int8_t pmic_nvm_str[] = "@PMIC /0xF4/1*08Be";

/* USER CODE END PV */

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
static uint16_t USB_DFU_If_Init(void);
static uint16_t USB_DFU_If_Write(uint8_t *pSrc, uint32_t alt, uint32_t Len, uint32_t BlockNumber);
static uint8_t *USB_DFU_If_Read(uint32_t alt, uint8_t *pDest, uint32_t Len, uint32_t BlockNumber);
static uint16_t USB_DFU_If_DeInit(void);
static inline uint32_t USBD_DFU_GetPartSize(uint8_t alt, uint32_t blocknumber);
USBD_DFU_MediaTypeDef USBD_DFU_MEDIA_fops =
{
  (uint8_t *)VIRTUAL_DESC_STR,
  USB_DFU_If_Init,
  USB_DFU_If_DeInit,
  NULL,
  USB_DFU_If_Write,
  USB_DFU_If_Read,
  NULL

};

/**
  * @brief  Memory write routine.
  * @param  src: Pointer to the source buffer. Address to be written to.
  * @param  dest: Pointer to the destination buffer.
  * @param  Len: Number of data to be written (in bytes).
  * @retval USBD_OK if operation is successful, MAL_FAIL else.
  */
uint16_t USB_DFU_If_Write(uint8_t *pSrc, uint32_t alt, uint32_t Len, uint32_t BlockNumber)
{
  OPENBL_USB_Download(pSrc, alt, Len, BlockNumber);

  return 0;
}

/**
  * @brief  Memory read routine.
  * @param  src: Pointer to the source buffer. Address to be written to.
  * @param  dest: Pointer to the destination buffer.
  * @param  Len: Number of data to be read (in bytes).
  * @retval Pointer to the physical address where data should be read.
  */
uint8_t *USB_DFU_If_Read(uint32_t alt, uint8_t *pDest, uint32_t Len, uint32_t BlockNumber)
{
  uint32_t partition_size = USBD_DFU_GetPartSize(alt, BlockNumber);

  if (Len > partition_size)
  {
	  ((USBD_DFU_HandleTypeDef *)hUsbDeviceHS.pClassDataCmsit[0])->wlength =
			  partition_size;
	  Len = partition_size;
  }

  return OPENBL_USB_ReadMemory(alt, pDest, Len, BlockNumber);
}

/**
  * @brief  Memory initialization routine.
  * @retval USBD_OK if operation is successful, MAL_FAIL else.
  */
uint16_t USB_DFU_If_Init(void)
{
  return 0;
}

/**
  * @brief  De-Initializes Memory
  * @retval USBD_OK if operation is successful, MAL_FAIL else
  */
uint16_t USB_DFU_If_DeInit(void)
{
  return 0;
}

static inline uint32_t USBD_DFU_GetPartSize(uint8_t alt, uint32_t blocknumber)
{
	uint32_t part_size = USBD_DFU_XFER_SIZE;

  switch(alt)
  {
				  case 0:
					  part_size =  FL_DESC_PARTSIZE;
					  break;
				  case 1:
					  part_size = FSBL_EXT_PARTSIZE;
					  break;
				  case 2:
					  part_size =  FSBL_APP_DESC_PARTSIZE;
					  break;
				  case 3:
					  part_size = VIRTUAL_DESC_SIZE;
					  break;
				  case 4:
					  part_size = OTP_DESC_PARTSIZE;
					  break;
				  case 5:
					  part_size = ((pmic_nvm_str[14] - 0x30) * 10) + (pmic_nvm_str[15] - 0x30);
					  break;

				  default:
					  break;
  }

  if ((((blocknumber + 1) * USBD_DFU_XFER_SIZE) < part_size) || (part_size == USBD_DFU_XFER_SIZE))
    return USBD_DFU_XFER_SIZE;
  else if (part_size > (blocknumber * USBD_DFU_XFER_SIZE))
  {
	return (part_size - (blocknumber * USBD_DFU_XFER_SIZE));
  }
  else
  {
	return part_size;
  }

  return 0;
}
