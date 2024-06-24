/**
  ******************************************************************************
  * @file    stm32mp2xx_hal_pwr_ex.c
  * @author  MCD Application Team
  * @brief   Extended PWR HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of PWR extension peripheral:
  *           + Peripheral Extended features functions
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32mp2xx_hal.h"

/** @addtogroup STM32MP2xx_HAL_Driver
  * @{
  */

/** @defgroup PWREx PWREx
  * @brief PWR HAL module driver
  * @{
  */

#ifdef HAL_PWR_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @addtogroup PWREx_Private_Constants PWREx Private Constants
  * @{
  */


/** @defgroup PWREx_FLAG_REG_SET_TIMEOUT PWR Extended Flag Setting Time Out Value
  * @{
  */
#define PWREx_FLAG_SETTING_DELAY_US  ((uint32_t)1000U)
/**
  * @}
  */

/**
  * @}
  */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup PWREx_Private_Functions PWREx Private Functions
  * @{
  */

/** @defgroup PWREx_Group1 Peripheral Extended features functions
  *  @brief Peripheral Extended features functions
  *
@verbatim

 ===============================================================================
                 ##### Peripheral extended features functions #####
 ===============================================================================

    *** Regulators configuration ***
    ================================================
    [..]
      (+) The backup domain includes 4 Kbytes of backup SRAM accessible only from
          the CPU, and address in 32-bit, 16-bit or 8-bit mode. Its content is
          retained even in Standby or VBAT mode when the low power backup regulator
          is enabled. It can be considered as an internal EEPROM when VBAT is
          always present. You can use the HAL_PWREx_EnableBkUpReg() function to
          enable the low power backup regulator.

      (+) When the backup domain is supplied by VDD (analog switch connected to VDD)
          the backup SRAM is powered from VDD which replaces the VBAT power supply to
          save battery life.

      (+) The backup SRAM is not mass erased by a tamper event. It is read
          protected to prevent confidential data, such as cryptographic private
          key, from being accessed.

      (+) Additionally 128 kbytes (32-bit, 16-bit or 8-bit accessible ) of retention ram is available, its content is
          retained even in Standby or VBAT mode when the low power retention ram regulator
          is enabled (HAL_PWREx_EnableRetRam)

      (+) Additionally 8 kbytes (32-bit, 16-bit or 8-bit accessible ) of Lpsram1 is available,
          its content is retained even in Standby or VBAT mode when the low power Lpsram1 ram regulator
          is enabled (HAL_PWR_EnableCpu3LpsramReg)

        Refer to the product datasheets for more details.

     *** VBAT and Temperature supervision ***
    ========================================
    [..]
      (+) The VBAT battery voltage supply can be monitored by comparing it with two threshold
          levels: VBAThigh and VBATlow. VBATH flag and VBATL flags in the PWR control register 2
          (PWR_CR2), indicate if VBAT is higher or lower than the threshold.
      (+) The temperature can be monitored by comparing it with two threshold levels, TEMPhigh
          and TEMPlow. TEMPH and TEMPL flags, in the PWR control register 2 (PWR_CR2),
          indicate whether the device temperature is higher or lower than the threshold.
      (+) The VBAT and the temperature monitoring is enabled by HAL_PWREx_EnableVbatTempMonitoring()
          function and disabled by HAL_PWREx_DisableVbatTempMonitoring() function.
      (+) The HAL_PWREx_GetVbatLevel() function return the VBAT level which can be:
          PWR_VBAT_BELOW_LOW_THRESHOLD or PWR_VBAT_ABOVE_HIGH_THRESHOLD or
          PWR_VBAT_BETWEEN_HIGH_LOW_THRESHOLD.
      (+) The HAL_PWREx_GetTemperatureLevel() function return the Temperature level which
          can be: PWR_TEMP_BELOW_LOW_THRESHOLD or PWR_TEMP_ABOVE_HIGH_THRESHOLD or
          PWR_TEMP_BETWEEN_HIGH_LOW_THRESHOLD.

    *** PVM configuration ***
    =========================
    [..]
      (+) The PVM is used to monitor  power supply of peripheral by comparing it to a
          fix threshold (PVM threshold is  equal 1.6 V and cannot be modified)
      (+) A GPVMO flag is available to indicate when a periph voltage lower
          than the PVM threshold. This event is internally connected to the EXTI
          line to generate an interrupt if enabled.
          It is configurable through __HAL_PWR_PVM_EXTI_ENABLE_IT() macro.
      (+) The PVM is stopped in System Standby mode.


    *** USB Regulator supervision ***
    ===================================
    [..]
      (+) When the USB regulator is enabled, the VDD33USB supply level detector shall
          be enabled through  HAL_PWREx_EnableUSBVoltageDetector() function and disabled by
          HAL_PWREx_DisableUSBVoltageDetector() function.

@endverbatim
  * @{
  */



/**
  * @brief  Enables backup ram content retention when system is in standby or Vbat mode
  *         This feature is implemented  thru  usage of backup Regulator for backup ram supply in standby/Vbat
  * @note   After reset PWR_CR2/PWR_CR9/PWR_CR10 registers are write-protected, see HAL_PWR_EnableBkUpD3Access/HAL_PWR_DisableBkUpD3Access.
  * @note   As this function accesses  to one of these registers, user shall check write protection is removed before calling this function
  * @note   (e.g. call to HAL_PWR_EnableBkUpD3Access during start of user application)
  * @retval void
  */
void HAL_PWREx_EnableBkUpRamContentStandbyRetention(void)
{
  SET_BIT(PWR->CR9, PWR_CR9_BKPRBSEN);
}

/**
  * @brief  @brief  Disables backup ram content retention when system is in standby and Vbat mode
  * @note   As this function accesses  to one of these registers, user shall check write protection is removed before calling this function
  * @note   (e.g. call to HAL_PWR_EnableBkUpD3Access during start of user application)
  * @retval void
  */
void HAL_PWREx_DisableBkUpRamContentStandbyRetention(void)
{
  CLEAR_BIT(PWR->CR9, PWR_CR9_BKPRBSEN);
}

/**
  * @brief  Enables retention ram content retention when system is in standby and/or Vbat mode (see param)
  *         This feature is implemented  thru  usage of backup Regulator for retention ram supply in standby/Vbat
  * @param  RetentionRamPowerSupplyMode: Specifies mode where retention RAM is powered thru backup regualtor.
  *          This parameter can be one of the following values:
  *            @arg PWR_RETENTION_RAM_SUPPLY_ON_STDBYONLY i.e. powering with backup regulator in STANDBY mode only
  *            @arg PWR_RETENTION_RAM_SUPPLY_ON_STDBY_VBAT i.e. powering with backup regulator in STANDBY and VBAT mode
  * @note   As this function access to one of this register, user shall check write protection is removed (e.g. call to HAL_PWR_EnableBkUpD3Access
  * @note   at start of user application)
  * @retval void
  */
void HAL_PWREx_EnableRetRamContentStandbyRetention(int RetentionRamPowerSupplyMode)
{
  assert_param(IS_PWR_RETENTION_RAM_SUPPLY(RetentionRamPowerSupplyMode));
  MODIFY_REG(PWR->CR10, PWR_CR10_RETRBSEN_Msk, RetentionRamPowerSupplyMode);
}

/**
  * @brief  Disables retention ram content retention  when system is in standby  or vbat
  * @note   As this function access to one of this register, user shall check write protection is removed (e.g. call to HAL_PWR_EnableBkUpD3Access
  * @note   at start of user application)
  * @retval void
  */
void HAL_PWREx_DisableRetRamContentStandbyRetention(void)
{
  MODIFY_REG(PWR->CR10, PWR_CR10_RETRBSEN_Msk, PWR_RETENTION_RAM_SUPPLY_OFF);
}

/**
  * @brief  Enables lpsram1 content retention when system is in standby2 and Vbat mode
  *         This feature is implemented  thru  usage of backup Regulator for lpsram ram supply in standby/Vbat
  * @note   when system is in standby1, M0P can R/W lpsram1 whatever this function is called or not.
  * @note   As this function access to one of this register, user shall check write protection is removed (e.g. call to HAL_PWR_EnableBkUpD3Access
  * @note   at start of user application)
  * @retval void
  */
void HAL_PWREx_EnableLpsram1ContentStandby2Retention(void)
{
  SET_BIT(PWR->CR9, PWR_CR9_LPR1BSEN);
}

/**
  * @brief  Disables lpsram1 content retention when system is in standby2 and Vbat mode
  * @note   As this function access to one of this register, user shall check write protection is removed (e.g. call to HAL_PWR_EnableBkUpD3Access
  * @note   at start of user application)
  * @retval void
  */
void HAL_PWREx_DisableLpsram1ContentStandby2Retention(void)
{
  CLEAR_BIT(PWR->CR9, PWR_CR9_LPR1BSEN);
}


/**
  * @brief  Valid GPU supply.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PWREx_EnableGPUSupply(void)
{
  uint32_t tickstart = 0;
  uint32_t maskEn, maskRdy;

#if ! defined(STM32MP2XX_ASSY2_3_2)
  maskEn = PWR_CR12_GPUSV;
  maskRdy = PWR_CR12_VDDGPURDY;
#else /* ! defined(STM32MP2XX_ASSY2_3_2) */
  maskEn = PWR_CR12_GPUPDEN;
  maskRdy = PWR_CR12_GPUPDRDY;
#endif /* else ! defined(STM32MP2XX_ASSY2_3_2) */

  /* swich on the GPU voltage  */
  SET_BIT(PWR->CR12, maskEn);

  /* Get tick */
  tickstart = HAL_GetTick();

  /* Wait until GPU regulator ready flag is set */
  while ((PWR->CR12 & maskRdy) != maskRdy)
  {
    if ((HAL_GetTick() - tickstart) > PWREx_FLAG_SETTING_DELAY_US)
    {
      return HAL_TIMEOUT;
    }
  }
  return HAL_OK;
}

/**
  * @brief  GPU supply not present.
  * @retval HAL status
  */
void HAL_PWREx_DisableGPUSupply(void)
{
#if ! defined(STM32MP2XX_ASSY2_3_2)
  /* swich off the GPU voltage  */
  CLEAR_BIT(PWR->CR12, PWR_CR12_GPUSV);
#else /* ! defined(STM32MP2XX_ASSY2_3_2) */
  /* swich off the GPU voltage  */
  CLEAR_BIT(PWR->CR12, PWR_CR12_GPUPDEN);
#endif /* else ! defined(STM32MP2XX_ASSY2_3_2) */
}


/**
  * @brief  Enable monitoring on GPU supply.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PWREx_EnableGPUSupplyMonitoring(void)
{
  SET_BIT(PWR->CR12, PWR_CR12_GPUVMEN);
  return HAL_OK;
}

/**
  * @brief  Disable monitoring on GPU supply.
  * @retval HAL status
  */
void HAL_PWREx_DisableGPUSupplyMonitoring(void)
{
  CLEAR_BIT(PWR->CR12, PWR_CR12_GPUVMEN);
}

#if defined (HAL_RTC_MODULE_ENABLED)
/**
  * @brief  Enable the VBAT and temperature monitoring.
  * @note   After reset PWR_CR2/PWR_CR9/PWR_CR10 registers are write-protected, see HAL_PWR_EnableBkUpD3Access/HAL_PWR_DisableBkUpD3Access
  * @note   As this function accesses  to one of these registers, user shall check write protection is removed before calling this function
  * @note   (e.g. call to HAL_PWR_EnableBkUpD3Access during start of user application)
  * @retval HAL status
  */
void HAL_PWREx_EnableVbatTempMonitoring(void)
{
  RTC_InternalTamperTypeDef sIntTamper = {0};
  RTC_HandleTypeDef          RTCHandle = {0};

  /* enable  VBAT/TEMPERATURE  internal tamper */
  sIntTamper.IntTamper = PWREx_VBAT_TEMP_TAMPER_EVENT;
  sIntTamper.TimeStampOnTamperDetection = RTC_TIMESTAMPONTAMPERDETECTION_DISABLE;
  HAL_RTCEx_SetInternalTamper_IT(&RTCHandle, &sIntTamper);

  /*activation of NS_TAMPER event on EXTI if not yet done*/
   __HAL_PWR_NS_TAMPER_EXTI_ENABLE_IT();

  //AAC to do : enable internal tamper ligne2 event for temperature, see RTC doc
  //AAC to do : issue with Vbat, BATH/BATL available in RTC according to PWR spec, not founded in RTC spec ...
//  HAL_RTCEx_SetInternalTamper_IT(&RTCHandle, PWREx_VBAT_TEMP_TAMPER_EVENT);


  /* Enable the VBAT and Temperature monitoring */
  SET_BIT(PWR->CR2, PWR_CR2_MONEN);
}

/**
  * @brief  Disable the VBAT and temperature monitoring.
  * @note   After reset PWR_CR2/PWR_CR9/PWR_CR10 registers are write-protected, see HAL_PWR_EnableBkUpD3Access/HAL_PWR_DisableBkUpD3Access
  * @note   As this function accesses  to one of these registers, user shall check write protection is removed before calling this function
  * @note   (e.g. call to HAL_PWR_EnableBkUpD3Access during start of user application)
  * @retval HAL status
  */
void HAL_PWREx_DisableVbatTempMonitoring(void)
{
  RTC_HandleTypeDef          RTCHandle = {0};

  /*Deactivate VBAT/TEMPERATURE tamper event, should not be done if tamper line yet active before VBAT/TEMP monitoring*/
//AAC shall be rework as NS internal TAMPER event can be used for other purpose than VBAT/TEMPERATURE
//AAC so de-activation shall be done only if there is no more usage of this IT
//AAC check that IT handler of this event parse all possible source for this event
  __HAL_PWR_NS_TAMPER_EXTI_DISABLE_IT();
  HAL_RTCEx_DeactivateInternalTamper(&RTCHandle, PWREx_VBAT_TEMP_TAMPER_EVENT);

  /* Disable the VBAT and Temperature monitoring */
  CLEAR_BIT(PWR->CR2, PWR_CR2_MONEN);
}

#endif /*defined (HAL_RTC_MODULE_ENABLED)*/


/**
  * @brief  Indicate whether the junction temperature is between, above or below the threshold.
  * @retval Temperature level.
  */
uint32_t HAL_PWREx_GetTemperatureLevel(void)
{
  uint32_t regValue = PWR->CR2;
  uint32_t result   = PWR_NO_TEMP_MEASUREMENT_AVAILABLE;

  /* Check VBAT/TEMP monitoring is ON */
  if ((regValue & PWR_CR2_MONEN) != PWR_CR2_MONEN)
  {
    result = PWR_NO_TEMP_MEASUREMENT_AVAILABLE;
  }

  /* Compare the read value to the temperature threshold */
  else if (((regValue & PWR_CR2_TEMPH) != PWR_CR2_TEMPH)&&((regValue & PWR_CR2_TEMPL) != PWR_CR2_TEMPL))
  {
    result = PWR_TEMP_BETWEEN_HIGH_LOW_THRESHOLD;
  }

  else if ((regValue & PWR_CR2_TEMPL) == PWR_CR2_TEMPL)
  {
    result = PWR_TEMP_BELOW_LOW_THRESHOLD;
  }

  else if ((regValue & PWR_CR2_TEMPH) == PWR_CR2_TEMPH)
  {
    result = PWR_TEMP_ABOVE_HIGH_THRESHOLD;
  }

  return result;
}


/**
  * @brief  Indicate whether the Battery voltage level is between, above or below the threshold.
  * @retval VBAT level.
  */

uint32_t HAL_PWREx_GetVbatLevel(void)
{
  uint32_t regValue = PWR->CR2;
  uint32_t result   = PWR_NO_VBAT_MEASUREMENT_AVAILABLE;

  /* check VBAT/TEMP monitoring is ON */
  if ((regValue & PWR_CR2_MONEN) != PWR_CR2_MONEN)
  {
    result   =  PWR_NO_VBAT_MEASUREMENT_AVAILABLE;
  }

  /* Compare the read value to the Vbat threshold */
  else if (((regValue & PWR_CR2_VBATH) != PWR_CR2_VBATH)&&((regValue & PWR_CR2_VBATL) != PWR_CR2_VBATL))
  {
    result   =  PWR_VBAT_BETWEEN_HIGH_LOW_THRESHOLD;
  }

  else if ((regValue & PWR_CR2_VBATL) == PWR_CR2_VBATL)
  {
    result   =  PWR_VBAT_BELOW_LOW_THRESHOLD;
  }

  else if ((regValue & PWR_CR2_VBATH) == PWR_CR2_VBATH)
  {
    result   =  PWR_VBAT_ABOVE_HIGH_THRESHOLD;
  }

  return result;
}



/**
  * @brief  This function handles the PWR VBAT/TEMPERATURE TAMPER interrupt request.
  * @note   This API should be called under the TAMPER_IRQHandler().
  * @retval None
  */
void HAL_PWREx_TEMP_VBAT_IRQHandler(void)
{
  uint32_t value;

  value = HAL_PWREx_GetVbatLevel();
  switch (value)
  {
    case PWR_VBAT_BETWEEN_HIGH_LOW_THRESHOLD :
      HAL_PWREx_VBAT_INRANGECallback();
      break;
    case PWR_VBAT_BELOW_LOW_THRESHOLD :
      HAL_PWREx_VBAT_LowerCallback();
      break;
    case PWR_VBAT_ABOVE_HIGH_THRESHOLD :
      HAL_PWREx_VBAT_HIGHERCallback();
      break;
    default :
      break;
  }

  value = HAL_PWREx_GetTemperatureLevel();
  switch (value)
  {
    case PWR_TEMP_BETWEEN_HIGH_LOW_THRESHOLD :
      HAL_PWREx_TEMP_INRANGECallback();
      break;
    case PWR_TEMP_BELOW_LOW_THRESHOLD :
      HAL_PWREx_TEMP_LowerCallback();
      break;
    case PWR_TEMP_ABOVE_HIGH_THRESHOLD :
      HAL_PWREx_TEMP_HIGHERCallback();
      break;
    default :
      break;
  }

    /* Clear TAMPER event */
//AAC need to clear VBAT/TEMPERATURE internal TAMPER

}

/**
  * @brief  PWR VBAT interrupt callback
  * @retval None
  */
__weak void HAL_PWREx_VBAT_HIGHERCallback(void)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_PWREx_VBAT_HIGHERCallback could be implemented in the user file
   */
}
__weak void HAL_PWREx_VBAT_LowerCallback(void)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_PWREx_VBAT_LowerCallback could be implemented in the user file
   */
}
__weak void HAL_PWREx_VBAT_INRANGECallback(void)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_PWR_BAT_TAMPER_INRANGECallback could be implemented in the user file
   */
}


/**
  * @brief  PWR TEMP interrupt callback
  * @retval None
  */
__weak void HAL_PWREx_TEMP_HIGHERCallback(void)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_PWREx_TEMP_HIGHERCallback could be implemented in the user file
   */
}
__weak void HAL_PWREx_TEMP_LowerCallback(void)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_PWREx_TEMP_LowerCallback could be implemented in the user file
   */
}
__weak void HAL_PWREx_TEMP_INRANGECallback(void)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_PWREx_TEMP_INRANGECallback could be implemented in the user file
   */
}



/* Power VDDcpu detection functions */

/**
  * @brief  Configure the VDDcpu threshold detection.
  * @param  sConfigVDDcpuD: pointer to an PWREx_DetectionTypeDef structure that contains the configuration
  *                     information for the VDDcpu detection.
  * @note   Refer to the electrical characteristics of your device datasheet for more details
  *         about the voltage threshold corresponding to each detection level.
  * @note   choice is to use EXTI interruption for notification and not a tamper event
  * @retval None
  */
void HAL_PWREx_ConfigVDDcpuD(PWREx_DetectionTypeDef *sConfigVDDcpuD)
{
  /* Check the parameters */
  assert_param(IS_PWR_VDDcpuD_LEVEL(sConfigVDDcpuD->Level));
  assert_param(IS_PWR_VDDcpuD_MODE(sConfigVDDcpuD->Mode));

  /* update the VCPULLS bit according to Level value */
  MODIFY_REG(PWR->CR6, PWR_CR6_VCPUH, sConfigVDDcpuD->Level);


  /* Clear any previous config. Keep it clear if no IT mode is selected */
  __HAL_PWR_VDDcpuD_EXTI_DISABLE_IT();
  __HAL_PWR_VDDcpuD_EXTI_DISABLE_RISING_FALLING_EDGE();


  /* Configure the interrupt mode */
  if (IS_AN_INTERRUPT_PWR_VDDcpuD_MODE(sConfigVDDcpuD->Mode))
  {
    __HAL_PWR_VDDcpuD_EXTI_ENABLE_IT();
  }

  /* Configure the edge */
  if ((sConfigVDDcpuD->Mode ) == PWR_VDDcpuD_MODE_IT_RISING)
  {
    __HAL_PWR_VDDcpuD_EXTI_ENABLE_RISING_EDGE();
  }

  if ((sConfigVDDcpuD->Mode ) == PWR_VDDcpuD_MODE_IT_FALLING)
  {
    __HAL_PWR_VDDcpuD_EXTI_ENABLE_FALLING_EDGE();
  }

  if ((sConfigVDDcpuD->Mode ) == PWR_VDDcpuD_MODE_IT_RISING_FALLING)
  {
    __HAL_PWR_VDDcpuD_EXTI_ENABLE_RISING_FALLING_EDGE();
  }
}

/**
  * @brief  Enable the VDDcpu Detector.
  * @retval None
  */
void HAL_PWREx_EnableVDDcpuD(void)
{
  SET_BIT(PWR->CR6, PWR_CR6_VCPUMONEN);
}

/**
  * @brief  disable the VDDcpu Detector.
  * @retval None
  */
void HAL_PWREx_DisableVDDcpuD(void)
{
  CLEAR_BIT(PWR->CR6, PWR_CR6_VCPUMONEN);
}

/**
  * @brief  Indicate whether the VDDcpu voltage level is between, above or below the threshold.
  * @retval VDDcpu level.
  */
uint32_t HAL_PWREx_GetVDDcpuRange(void)
{
  uint32_t regValue = PWR->CR6;
  uint32_t result   = PWR_NO_VDDCPU_MEASUREMENT_AVAILABLE;

  /* check VDDcpu monitoring is ON */
  if ((regValue & PWR_CR6_VCPUMONEN) != PWR_CR6_VCPUMONEN)
  {
    result   =  PWR_NO_VDDCPU_MEASUREMENT_AVAILABLE;
  }

  /* Compare the read value to the VDDCPU threshold */
  else if (((regValue & PWR_CR6_VCPUH) != PWR_CR6_VCPUH)&&((regValue & PWR_CR6_VCPUL) != PWR_CR6_VCPUL))
  {
    result   =  PWR_VDDCPU_BETWEEN_HIGH_LOW_THRESHOLD;
  }

  else if ((regValue & PWR_CR6_VCPUL) == PWR_CR6_VCPUL)
  {
    result   =  PWR_VDDCPU_BELOW_LOW_THRESHOLD;
  }

  else if ((regValue & PWR_CR6_VCPUH) == PWR_CR6_VCPUH)
  {
    result   =  PWR_VDDCPU_ABOVE_HIGH_THRESHOLD;
  }

  return result;
}


/**
  * @brief  This API should be called under HAL_EXTI_Handler() when line VDDcpu trigg
  * @note   //AAC, pas clair l'appellant sous HAL_EXTI, a checker ...
  * @retval None
  */
void HAL_PWREx_VDDcpuD_IRQHandler(void)
{
  uint32_t value;

  value = HAL_PWREx_GetVDDcpuRange();
  switch (value)
  {
    case PWR_VDDCPU_BETWEEN_HIGH_LOW_THRESHOLD :
      HAL_PWREx_VDDcpuD_INRANGECallback();
      break;
    case PWR_VDDCPU_BELOW_LOW_THRESHOLD :
      HAL_PWREx_VDDcpuD_LowerCallback();
      break;
    case PWR_VDDCPU_ABOVE_HIGH_THRESHOLD :
      HAL_PWREx_VDDcpuD_HIGHERCallback();
      break;
    default :
      break;
  }

  /* Clear PWR VDDcpuD EXTI pending bit */
  __HAL_PWR_VDDcpuD_EXTI_CLEAR_FLAG();
}

/**
  * @brief  PWR VDDcpuD interrupt callback
  * @retval None
  */
__weak void HAL_PWREx_VDDcpuD_HIGHERCallback(void)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_PWREx_VDDcpuD_HIGHERCallback could be implemented in the user file
   */
}
__weak void HAL_PWREx_VDDcpuD_LowerCallback(void)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_PWREx_VDDcpuD_LowerCallback could be implemented in the user file
   */
}

__weak void HAL_PWREx_VDDcpuD_INRANGECallback(void)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_PWREx_VDDcpuD_INRANGECallback could be implemented in the user file
   */
}


/* Power VDDcore detection functions */

/**
  * @brief  Configure the VDDcore threshold detection
  * @param  sConfigVDDcoreD: pointer to an PWREx_DetectionTypeDef structure that contains the configuration
  *                     information for the VDDcore detection.
  * @note   Refer to the electrical characteristics of your device datasheet for more details
  *         about the voltage threshold corresponding to each detection level.
  * @retval None
  */
void HAL_PWREx_ConfigVDDcoreD(PWREx_DetectionTypeDef *sConfigVDDcoreD)
{
  /* Check the parameters, no level threshold parameter for VDDcore */
  assert_param(IS_PWR_VDDcoreD_MODE(sConfigVDDcoreD->Mode));

  /* Clear any previous config. Keep it clear if no IT mode is selected */
  __HAL_PWR_VDDcoreD_EXTI_DISABLE_IT();
  __HAL_PWR_VDDcoreD_EXTI_DISABLE_RISING_FALLING_EDGE();

  /* Configure the interrupt mode */
  if (IS_AN_INTERRUPT_PWR_VDDcoreD_MODE(sConfigVDDcoreD->Mode))
  {
    __HAL_PWR_VDDcoreD_EXTI_ENABLE_IT();
  }

  /* Configure the edge */
  if ((sConfigVDDcoreD->Mode ) == PWR_VDDcoreD_MODE_IT_RISING)
  {
    __HAL_PWR_VDDcoreD_EXTI_ENABLE_RISING_EDGE();
  }

  if ((sConfigVDDcoreD->Mode ) == PWR_VDDcoreD_MODE_IT_FALLING)
  {
    __HAL_PWR_VDDcoreD_EXTI_ENABLE_FALLING_EDGE();
  }

  if ((sConfigVDDcoreD->Mode ) == PWR_VDDcoreD_MODE_IT_RISING_FALLING)
  {
    __HAL_PWR_VDDcoreD_EXTI_ENABLE_RISING_FALLING_EDGE();
  }
}

/**
  * @brief  Enable the VDDcore Detector.
  * @retval None
  */
void HAL_PWREx_EnableVDDcoreD(void)
{
  SET_BIT(PWR->CR5, PWR_CR5_VCOREMONEN);
}

/**
  * @brief  Disable the VDDcore Detector.
  * @retval None
  */
void HAL_PWREx_DisableVDDcoreD(void)
{
  CLEAR_BIT(PWR->CR5, PWR_CR5_VCOREMONEN);
}

/**
  * @brief  Indicate whether the VDDcore voltage level is between, above or below the threshold.
  * @retval VDDcore level.
  */
uint32_t HAL_PWREx_GetVDDcoreRange(void)
{
  uint32_t regValue = PWR->CR5;
  uint32_t result   = PWR_NO_TEMP_MEASUREMENT_AVAILABLE;

  /* check VDDcore monitoring is ON */
  if ((regValue & PWR_CR5_VCOREMONEN) != PWR_CR5_VCOREMONEN)
  {
    result   =  PWR_NO_VDDCORE_MEASUREMENT_AVAILABLE;
  }

  /* Compare the read value to the VDDCORE threshold */
  else if (((regValue & PWR_CR5_VCOREH) != PWR_CR5_VCOREH)&&((regValue & PWR_CR5_VCOREL) != PWR_CR5_VCOREL))
  {
    result   =  PWR_VDDCORE_BETWEEN_HIGH_LOW_THRESHOLD;
  }

  else if ((regValue & PWR_CR5_VCOREL) == PWR_CR5_VCOREL)
  {
    result   =  PWR_VDDCORE_BELOW_LOW_THRESHOLD;
  }

  else if ((regValue & PWR_CR5_VCOREH) == PWR_CR5_VCOREH)
  {
    result   =  PWR_VDDCORE_ABOVE_HIGH_THRESHOLD;
  }

  return result;
}

/**
  * @brief  This API should be called under HAL_EXTI_Handler() when line VDDcore trigg
  * @note   //AAC, pas clair l'appellant sous HAL_EXTI, a checker ...
  * @retval None
  */
void HAL_PWREx_VDDcoreD_IRQHandler(void)
{
  uint32_t value;

  value = HAL_PWREx_GetVDDcoreRange();
  switch (value)
  {
    case PWR_VDDCORE_BETWEEN_HIGH_LOW_THRESHOLD :
      HAL_PWREx_VDDcoreD_INRANGECallback();
      break;
    case PWR_VDDCORE_BELOW_LOW_THRESHOLD :
      HAL_PWREx_VDDcoreD_LowerCallback();
      break;
    case PWR_VDDCORE_ABOVE_HIGH_THRESHOLD :
      HAL_PWREx_VDDcoreD_HIGHERCallback();
      break;
    default :
      break;
  }

  /* Clear PWR VDDcoreD EXTI pending bit */
  __HAL_PWR_VDDcoreD_EXTI_CLEAR_FLAG();
}

/**
  * @brief  PWR VDDcoreD interrupt callback
  * @retval None
  */
__weak void HAL_PWREx_VDDcoreD_HIGHERCallback(void)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_PWREx_VDDcoreD_HIGHERCallback could be implemented in the user file
   */
}
__weak void HAL_PWREx_VDDcoreD_LowerCallback(void)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_PWREx_VDDcoreD_LowerCallback could be implemented in the user file
   */
}

__weak void HAL_PWREx_VDDcoreD_INRANGECallback(void)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_PWREx_VDDcoreD_INRANGECallback could be implemented in the user file
   */
}



#if defined(CORE_CM33)||defined(CORE_CA35)
typedef struct
{
  uint32_t activ;
  uint32_t ready;
  uint32_t monitor;
  uint32_t low;
  uint32_t lowStandby;
} pvmPosType;


void pvmPosition(uint32_t periph, __IO uint32_t *regName, pvmPosType *pvmPos)
{
  assert_param(IS_PWR_PVM_PERIPH(periph));

  switch (periph)
  {
    case PWR_PVM_VDDIO1 :
      *regName = PWR->CR8;
      pvmPos->activ      = PWR_CR8_VDDIO1SV_Pos;
      pvmPos->ready      = PWR_CR8_VDDIO1RDY_Pos;
      pvmPos->monitor    = PWR_CR8_VDDIO1VMEN_Pos;
      pvmPos->low        = PWR_CR8_VDDIO1VRSEL_Pos;
      pvmPos->lowStandby = PWR_CR8_VDDIO1VRSTBY_Pos;
      break;
    case PWR_PVM_VDDIO2 :
      *regName = PWR->CR7;
      pvmPos->activ      = PWR_CR7_VDDIO2SV_Pos;
      pvmPos->ready      = PWR_CR7_VDDIO2RDY_Pos;
      pvmPos->monitor    = PWR_CR7_VDDIO2VMEN_Pos;
      pvmPos->low        = PWR_CR7_VDDIO2VRSEL_Pos;
      pvmPos->lowStandby = PWR_CR7_VDDIO2VRSTBY_Pos;
      break;
    case PWR_PVM_VDDIO3 :
      *regName = PWR->CR1;
      pvmPos->activ   = PWR_CR1_VDDIO3SV_Pos;
      pvmPos->ready   = PWR_CR1_VDDIO3RDY_Pos;
      pvmPos->monitor = PWR_CR1_VDDIO3VMEN_Pos;
      break;
    case PWR_PVM_VDDIO4 :
      *regName = PWR->CR1;
      pvmPos->activ   = PWR_CR1_VDDIO4SV_Pos;
      pvmPos->ready   = PWR_CR1_VDDIO4RDY_Pos;
      pvmPos->monitor = PWR_CR1_VDDIO4VMEN_Pos;
      break;
    case PWR_PVM_USB33 :
      *regName = PWR->CR1;
      pvmPos->activ   = PWR_CR1_USB33SV_Pos;
      pvmPos->ready   = PWR_CR1_USB33RDY_Pos;
      pvmPos->monitor = PWR_CR1_USB33VMEN_Pos;
      break;
    case PWR_PVM_UCPD :
      *regName = PWR->CR1;
      pvmPos->activ   = PWR_CR1_UCPDSV_Pos;
      pvmPos->ready   = PWR_CR1_UCPDRDY_Pos;
      pvmPos->monitor = PWR_CR1_UCPDVMEN_Pos;
      break;
    case PWR_PVM_A :
      *regName = PWR->CR1;
      pvmPos->activ   = PWR_CR1_ASV_Pos;
      pvmPos->ready   = PWR_CR1_ARDY_Pos;
      pvmPos->monitor = PWR_CR1_AVMEN_Pos;
      break;
    default :
      break;
    }
}


/**
  * @brief  Configure a periph voltage  monitoring(PVM).
  * @param  mode: indicates if an interruption is raised when PVM status changes.
  *          This parameter can be one of the following values:
  *            @arg PWR_PVM_MODE_NORMAL                              no interruption
  *            @arg PWR_PVM_MODE_IT_RISING                           interruption when all PVM periph voltage become equals or higher than PVM threshold
  *            @arg PWR_PVM_MODE_IT_FALLING                          interruption when at least one  PVM periph voltage become lower  than PVM threshold
  *            @arg PWR_PVM_MODE_IT_RISING_FALLING                   combined cases
  * @note   Voltage threshold is 1.6 volt whatever periph is.
  * @retval None
  */
void HAL_PWREx_ConfigurePVM(uint32_t mode)
{
  assert_param(IS_PWR_PVM_MODE(mode));

  /* Clear any previous config. Keep it clear if no IT mode is selected */
  __HAL_PWR_PVM_EXTI_DISABLE_IT();
  __HAL_PWR_PVM_EXTI_DISABLE_RISING_FALLING_EDGE();

  /* Configure interrupt mode */
  if (IS_AN_INTERRUPT_PWR_PVM_MODE(mode))
  {
    __HAL_PWR_PVM_EXTI_ENABLE_IT();
  }

  /* Configure the edge */
  if ((mode ) == PWR_PVM_MODE_IT_RISING)
  {
    __HAL_PWR_PVM_EXTI_ENABLE_RISING_EDGE();
  }

  if ((mode ) == PWR_PVM_MODE_IT_FALLING)
  {
    __HAL_PWR_PVM_EXTI_ENABLE_FALLING_EDGE();
  }

  if ((mode ) == PWR_PVM_MODE_IT_RISING_FALLING)
  {
    __HAL_PWR_PVM_EXTI_ENABLE_RISING_FALLING_EDGE();
  }
}

/**
  * @brief  Set  voltage  for VDDIO1/2.
  * @param  periph (single), should be IO1 or IO2
  * @param  setLow , if true low voltage is requested (1.8) else high voltage (3.3, default)
  * @param  setLowStandby , if true low voltage is requested (1.8) in standby and standby exit else high voltage (3.3, default)
  * @retval None
  */
void HAL_PWREx_Set_IO_Voltage(uint32_t periph, uint32_t setLow, uint32_t setLowStandby)
{
  __IO uint32_t regName;
  pvmPosType pvmPos;

  assert_param(IS_PWR_CONFIGURABLE_IO(periph));
  pvmPosition( periph, &regName, &pvmPos);
  setLow        ? (SET_BIT(regName, 1U << (pvmPos.low)))        : (CLEAR_BIT(regName, 1U << (pvmPos.low)));
  setLowStandby ? (SET_BIT(regName, 1U << (pvmPos.lowStandby))) : (CLEAR_BIT(regName, 1U << (pvmPos.lowStandby)));
}

/**
  * @brief  Get  voltage  for VDDIO1/2.
  * @param  periph (single), should be IO1 or IO2
  * @param  isLow , true if low voltage is set  (1.8) else high voltage (3.3, default)
  * @param  isLowStandby , true low voltage is set (1.8) in standby and standby exit else high voltage (3.3, default)
  * @retval None
  */
void HAL_PWREx_Get_IO_Voltage(uint32_t periph, uint32_t *isLow, uint32_t *isLowStandby)
{
  __IO uint32_t regName;
  pvmPosType pvmPos;

  assert_param(IS_PWR_CONFIGURABLE_IO(periph));
  pvmPosition( periph, &regName, &pvmPos);
  *isLow        = READ_BIT(regName, 1U << (pvmPos.low))        ? 1 : 0;
  *isLowStandby = READ_BIT(regName, 1U << (pvmPos.lowStandby)) ? 1 : 0;
}

/**
  * @brief  Validate a Peripheral Supply Voltage (PVS)
  * @note   Before using a  peripheral, its supply shall be validated (remove electrical and logical isolation)
  * @param  periph (single), for all PVM periph value, see  macro IS_PWR_PVM_PERIPH
  * @retval None
  */
HAL_StatusTypeDef HAL_PWREx_EnableSupply(uint32_t periph)
{
  __IO uint32_t regName;
  pvmPosType pvmPos = {0,0,0,0,0};

  assert_param(IS_PWR_PVM_PERIPH(periph));
  pvmPosition( periph, &regName, &pvmPos);
  /* Active regulator Wait until ready flag is set */
  SET_BIT(regName, 1U << (pvmPos.activ));
  return HAL_OK;
}


/**
  * @brief  unValidate a Peripheral Supply Voltage (PVS)
  * @note   After  using a PVM peripheral, its supply shall be de-validated (set electrical and logical isolation)
  * @param  periph (single), for all PVM periph value, see  macro IS_PWR_PVM_PERIPH
  * @retval None
  */
void HAL_PWREx_DisableSupply(uint32_t periph)
{
  __IO uint32_t regName;
  pvmPosType pvmPos = {0,0,0,0,0};

  assert_param(IS_PWR_PVM_PERIPH(periph));
  pvmPosition( periph, &regName, &pvmPos);
  CLEAR_BIT(regName, 1U << (pvmPos.activ));
}


/**
  * @brief  Enable a Peripheral Supply Monitoring (PVM)
  * @note   For a peripheral, enable monitoring of its power suply
  * @param  periph (single), for all PVM periph value, see  macro IS_PWR_PVM_PERIPH
  * @retval None
  */
void HAL_PWREx_EnableSupplyMonitoring(uint32_t periph)
{
  __IO uint32_t regName;
  pvmPosType pvmPos = {0,0,0,0,0};

  assert_param(IS_PWR_PVM_PERIPH(periph));
  pvmPosition( periph, &regName, &pvmPos);
  SET_BIT(regName, 1U << (pvmPos.monitor));
}


/**
  * @brief  Disable a Peripheral Supply Monitoring (PVM)
  * @note   For a peripheral, enable monitoring of its power suply
  * @param  periph (single), for all PVM periph value, see  macro IS_PWR_PVM_PERIPH
  * @retval None
  */
void HAL_PWREx_DisableSupplyMonitoring(uint32_t periph)
{
  __IO uint32_t regName;
  pvmPosType pvmPos = {0,0,0,0,0};

  assert_param(IS_PWR_PVM_PERIPH(periph));
  pvmPosition( periph, &regName, &pvmPos);
  CLEAR_BIT(regName, 1U << (pvmPos.monitor));
}


/**
  * @brief  return TRUE (not zero) if supply value for a peripheral is below PVM threshold
  * @param  periph (single), for all PVM periph value, see  macro IS_PWR_PVM_PERIPH
  * @note   Voltage threshold is 1.6 volt whatever periph is.
  * @retval return 0 if periph voltage is equal or  higher to PVM threshold
  */
uint32_t HAL_PWREx_PVM_IsBelowThreshold(uint32_t periph)
{
  __IO uint32_t regName;
  pvmPosType pvmPos = {0,0,0,0,0};

  assert_param(IS_PWR_PVM_PERIPH(periph));
  pvmPosition( periph, &regName, &pvmPos);
  /*return true if Ready bit not set*/
  return (regName & (1U << pvmPos.ready)) != (1U << (pvmPos.ready)) ;
}


/**
  * @brief  This function handles the PWR PVM interrupt request.
  * @note   This API should be called under the PVM_IRQHandler().
  * @retval None
  */
void HAL_PWREx_PVM_IRQHandler(void)
{
  if (((EXTI1->RPR1 & PWR_EXTI_LINE_PVM) == PWR_EXTI_LINE_PVM))
  /*interruption on rising  edge, thus one (or several) supply is lower than PVM threshold*/
  {
    HAL_PWREx_PVM_LowerCallback();
  }

  if (((EXTI1->FPR1 & PWR_EXTI_LINE_PVM) == PWR_EXTI_LINE_PVM))
  {
  /*interruption on falling  edge, thus all supplies are equal or greater under PVM threshold*/
    HAL_PWREx_PVM_EqualHigherCallback();
  }
  /* Clear PWR PVD EXTI pending bit */
  __HAL_PWR_PVM_EXTI_CLEAR_FLAG();
}

/**
  *@brief PWR PVM rising interruption callback
  *@note  This function is called if :
           -PVM interruption on rising edge has been requested
           -one (or several)  monitored PVM periph power supply  becomes lower than PVM threshold
  *@note  If needed, it is under user responsability to handle level of each PVM periph  thru state variables.
       HAL_PWREx_PVM_LowerCallback and HAL_PWREx_PVM_EqualHigherCallback  callback enable such processing.
  *@retval None
  */
__weak void HAL_PWREx_PVM_LowerCallback(void)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_PWREx_PVM_LowerCallback could be implemented in the user file */
}

/**
  *@brief PWR PVM falling interruption callback
  *@note  This function is called if :
           -PVM interruption on falling edge has been requested
           -power supply of one (or several) monitored PVM periph becomes greater or equal  than PVM threshold
           and power supply for all other monitored PVM peripherals is also greater or equal  than PVM threshold
  *@note  If needed, it is under user responsability to handle level of each PVM periph  thru state variables.
       HAL_PWREx_PVM_LowerCallback and HAL_PWREx_PVM_EqualHigherCallback  callback enable such processing.
  *@retval None
  */
__weak void HAL_PWREx_PVM_EqualHigherCallback(void)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_PWREx_PVM_EqualHigherCallback could be implemented in the user file */
}

#endif /*defined(CORE_CM33)||defined(CORE_CA35)*/

/**
  * @brief  PWR WKUP interrupt callback
  * @note   In case of M0+ wakeup from tamper, Callback shall manage wakeup tamper event
  *         (which one has trigged, clear operation, ..)
  *         Not managed  at PWR level as power module.
  * @param  PWR wakeup line number (only case M33 or A35 wakeup)
  * @retval None
  */
#if defined(CORE_CM33)||defined(CORE_CA35)
__weak void HAL_PWREx_WKUP_Callback(__attribute__((unused))uint16_t line)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_PWREx_WKUP1Callback could be implemented in the user file
  */
}
#endif /*defined(CORE_CM33)||defined(CORE_CA35)*/


#if !defined(CORE_CM0PLUS)
/**
  * @brief  This function handles the PWR wakeup (or external TAMPER) detection  interruption
  * Parse all wakeup lines in order to find out which one has detected on wakeup event
  * then calls associated callback.
  * @note   This API should be called under the CPUx_WAKEUP_PIN_IRQn.
  * @retval None
  */
void HAL_PWREx_WAKEUP_PIN_IRQHandler(void)
{
#define WKUPCR_ADD_OFFSET      0x01u

  /* Wakeup event  for current CPU detected on a (or several) PWR line(s), line shall be identified, associated flag cleared and callback invoked */
  __IO uint32_t *regaddr;

  for (uint16_t index = PWR_WAKEUP_PIN_ID1; index <= PWR_WAKEUP_PIN_ID6; index++)
  {
    regaddr = &PWR->WKUPCR1 + (WKUPCR_ADD_OFFSET*index);        /* Selected register corresponding to input wakeup line*/
    if (READ_BIT(*regaddr, PWR_WKUPCR1_WKUPF) != RESET)
    {
    /* Clear PWR WKUPF#i flag */
      SET_BIT(*regaddr, PWR_WKUPCR1_WKUPC);
    /* PWR WKUP#i interrupt user callback */
      HAL_PWREx_WKUP_Callback(index);
    }
  }
}
#endif /*!defined(CORE_CM0PLUS)*/


/**
  * @brief Enters MPU  CSTOP allowing system stop but denying system standby
  * @param Regulator: Specifies which power state regulator supplying current CPU domain can reach
  *            This parameter can be one of the following values:
  *            @arg PWR_REGULATOR_LP_OFF i.e. full power
  *            @arg PWR_REGULATOR_LP_ON_LV_OFF i.e. can be low power but nominal voltage
  *            @arg PWR_REGULATOR_LP_ON_LV_ON  i.e. can be low power and  low voltage
  * @param STOPEntry: Specifies if Stop mode in entered with WFI or WFE instruction.
  *          This parameter can be one of the following values:
  *            @arg PWR_STOPENTRY_WFI: Enter Stop mode with WFI instruction
  *            @arg PWR_STOPENTRY_WFE: Enter Stop mode with WFE instruction
  * @retval None
  */
void HAL_PWREx_EnterCSTOPAllowSTOP(uint32_t Regulator, uint8_t STOPEntry)
{
  HAL_PWR_EnterSTOPMode(Regulator, STOPEntry);
}




#if defined(CORE_CM33)||defined(CORE_CA35)
/**
  * @brief Enters MPU  CSTOP allowing system Standby mode (1 or 2 see param)
  * @param STANDBYType: Specifies standby mode that can be reached
  *            This parameter can be one of the following values:
  *            @arg PWR_STANDBY_1 i.e. D3 still open (but D1 and D2 OFF)
  *            @arg PWR_STANDBY_2 i.e. D3 and D2 and D1 OFF
  * @retval None
  */
void HAL_PWREx_EnterCSTOPAllowSTANDBY(uint8_t STANDBYType)
{
  HAL_PWR_EnterSTANDBYMode(STANDBYType);
}
#endif


/**
  * @brief  This function clear all status flag set by PWR IP
  * @note   before entering standby user shall call this function other flag may not be relevant
  * @retval None
  */
void HAL_PWREx_ClearStatusFlag (void)
{
#if defined(CORE_CA35)
  CLEAR_BIT(PWR->CPU1CR, PWR_CPU1CR_CSSF);
#endif
#if defined(CORE_CM33)
  CLEAR_BIT(PWR->CPU2CR, PWR_CPU2CR_CSSF);
#endif
#if defined(CORE_CM0PLUS)
  CLEAR_BIT(PWR->CPU3CR, PWR_CPU3CR_CSSF);
#endif
}

/**
  * @}
  */

/**
  * @}
  */

#endif /* HAL_PWR_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
