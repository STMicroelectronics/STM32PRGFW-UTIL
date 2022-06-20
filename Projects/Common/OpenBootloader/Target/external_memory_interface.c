/**
  ******************************************************************************
  * @file    external_memory_interface.c
  * @author  MCD Application Team
  * @brief   Contains External memory access functions
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
#include "platform.h"
#include "openbl_mem.h"
#include "app_openbootloader.h"
#include "common_interface.h"
#include "external_memory_interface.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Base address of Ext flash loader function, can change if Ext flash loader project is modified */
#define INIT_BASE_ADDR                0x2fffa83e
#define READ_BASE_ADDR                0xffffffff /* Fake address, not used */
#define WRITE_BASE_ADDR               0x2fffa872
#define SECTOR_ERASE_BASE_ADDR        0x2fffa8b2
#define MASS_ERASE_BASE_ADDR          0x2fffa8ec
#define VERIFY_BASE_ADDR              0x2fffa9fc
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/  
uint8_t memory_initilized = 0; 

/* Private function prototypes -----------------------------------------------*/
static int (*Init)        (void)                                                      = (int     (*) (void)) (INIT_BASE_ADDR + 1); /* +1 for thumb */
static int (*Read)        (uint32_t Address,   uint32_t DataLength,   uint8_t *pData) = (int     (*) (uint32_t,   uint32_t,   uint8_t*)) (READ_BASE_ADDR + 1);
static int (*Write)       (uint32_t Address,   uint32_t DataLength,   uint8_t *pData) = (int     (*) (uint32_t,   uint32_t,   uint8_t*)) (WRITE_BASE_ADDR + 1);
static int (*SectorErase) (uint32_t StartAddress,   uint32_t EndAddress)              = (int     (*) (uint32_t,   uint32_t)) (SECTOR_ERASE_BASE_ADDR + 1);
static int (*MassErase)   (void)                                                      = (int     (*) (void)) (MASS_ERASE_BASE_ADDR + 1);
static int (*Verify)      (uint32_t Address, uint32_t DataAddr, uint32_t DataLength, uint32_t missalignement) = (int     (*) (uint32_t, uint32_t, uint32_t, uint32_t)) (VERIFY_BASE_ADDR + 1);

static uint32_t function_is_in_RAM(uint32_t);

/* Exported variables --------------------------------------------------------*/
/* In the future the items of this structure will be automatically getting from the external loader*/
OPENBL_MemoryTypeDef EXTERNAL_MEMORY_Descriptor =
{
  EXT_MEMORY_START_ADDRESS,
  EXT_MEMORY_END_ADDRESS,
  EXT_MEMORY_SIZE,
  EXTERNAL_MEMORY_AREA,
  OPENBL_ExtMem_Init,
  OPENBL_ExtMem_Read,
  OPENBL_ExtMem_Write,
  OPENBL_ExtMem_JumpToAddress,
  OPENBL_ExtMem_MassErase,
  OPENBL_ExtMem_SectorErase,
  OPENBL_ExtMem_Verify
};

/* Exported functions --------------------------------------------------------*/
/**
  * @brief  This function is used for initializing external memory.
  * @param  None
  * @retval int.
  */
void OPENBL_ExtMem_Init(uint32_t Address)
{
  /*Make sur the Init points to RAM*/
  if (function_is_in_RAM((uint32_t)Init))
    Init();
}

/**
  * @brief  This function is used to read data from a given address.
  * @param  Address The address to be read.
  * @retval Returns the read value.
  */
uint8_t OPENBL_ExtMem_Read(uint32_t Address)
{
    /* Check if the External memory has a Read function or not  */
    if (NULL != Read)
    {
      static uint8_t  tempData[1];
      /* Read one byte of data from external memory */
      if (function_is_in_RAM((uint32_t)Read))
        Read(Address, 1, tempData);
      
      /*Return the read data*/
      return (*tempData);
    }
    else 
    {      
      /*return the read value*/
      return (*(uint8_t *)(Address));     
    }
}

/**
  * @brief  This function is used to write data in external memory.
  * @param  Address The address where that data will be written.
  * @param  Data The data to be written.
  * @param  DataLength The length of the data to be written.
  * @retval None.
  */
void OPENBL_ExtMem_Write(uint32_t Address, uint8_t *Data, uint32_t DataLength)
{
    /* Write data to external Memory*/
  if (function_is_in_RAM((uint32_t)Write))
    Write(Address, DataLength, Data);
}

/**
  * @brief  Verify flash memory with RAM buffer and calculates checksum value of
  * the programmed memory
  * @param  Address The Flash address
  * @param  DataAddr The RAM buffer addres
  * @param  DataLength The Size (in WORD)
  * @param  missalignement The Initial CRC value
  * @retval R0: Operation failed (address of failure)
  *         R1: Checksum value
  */
uint64_t OPENBL_ExtMem_Verify(uint32_t Address, uint32_t DataAddr, uint32_t DataLength, uint32_t missalignement)
{
    /* Write data to external Memory*/
  if (function_is_in_RAM((uint32_t)Verify))
    return (Verify(Address, DataAddr, DataLength, missalignement));

  return 0;
}

/**
  * @brief  This function is used to jump to a given address.
  * @param  Address The address where the function will jump.
  * @retval None.
  */
void OPENBL_ExtMem_JumpToAddress(uint32_t Address)
{
  Function_Pointer jump_to_address;
  /* Deinitialize all HW resources used by the Bootloader to their reset values */
 OpenBootloader_DeInit();

  /* Enable IRQ */
 Common_EnableIrq();
          
  jump_to_address = (Function_Pointer)(*(__IO uint32_t *)(Address + 4U));

  /* Initialize user application's stack pointer */
  Common_SetMsp(*(__IO uint32_t *) Address);

  jump_to_address();
}

/**
  * @brief  This function is used to erase the chip.
  * @param  None.
  * @retval None.
  */
void OPENBL_ExtMem_MassErase(uint32_t Address)
{
  if (function_is_in_RAM((uint32_t)MassErase))
    MassErase();
}

/**
  * @brief  This function is used to erase the sector.
  * @param  EraseStartAddress erase start address
  * @param  EraseEndAddress erase end address
  * @retval None.
  */
void OPENBL_ExtMem_SectorErase(uint32_t EraseStartAddress, uint32_t EraseEndAddress)
{
  if (function_is_in_RAM((uint32_t)SectorErase))
  {
    SectorErase(EraseStartAddress, EraseEndAddress);
  }
}

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  This function is used to erase the sector.
  * @param  address
  * @retval uint32_t.
  */
static uint32_t function_is_in_RAM(uint32_t Address)
{
 return((Address>RAM_START_ADDRESS)&&(Address<RAM_END_ADDRESS));
}
