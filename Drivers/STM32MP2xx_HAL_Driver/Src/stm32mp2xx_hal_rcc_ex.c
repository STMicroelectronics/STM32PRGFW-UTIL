/**
  ******************************************************************************
  * @file    stm32mp25_hal_rcc_ex.c
  * @author  MCD Application Team
  * @brief   Extended RCC HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities RCC extended peripheral:
  *           + Extended Peripheral Control functions
  *           + Extended Clock management functions
  *           + Extended Clock Recovery System Control functions
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32mp2xx_hal.h"

/** @addtogroup STM32MP25_HAL_Driver
  * @{
  */

/** @defgroup RCCEx RCCEx
  * @brief RCC Extended HAL module driver
  * @{
  */

#ifdef HAL_RCC_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/** @defgroup RCCEx_Private_Constants RCCEx Private Constants
  * @{
  */
/**
  * @}
  */
#define FINDIV_TIMEOUT_VALUE    (2U)    /* 2 ms (minimum Tick + 1) */
#define PREDIV_TIMEOUT_VALUE    (2U)    /* 2 ms (minimum Tick + 1) */
#define LOCK_TIMEOUT_VALUE      (2U)    /* 2 ms (minimum Tick + 1) */

#define PLL4                    0x0U
#define PLL5                    0x1U
#define PLL6                    0x2U
#define PLL7                    0x3U
#define PLL8                    0x4U

/* Private macros ------------------------------------------------------------*/
/** @addtogroup RCCEx_Private_Macros
  * @{
  */

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/** @defgroup RCCEx_Private_Functions RCCEx Private Functions
  * @{
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/

/** @defgroup RCCEx_Exported_Functions RCCEx Exported Functions
  * @{
  */

/** @defgroup RCCEx_Exported_Functions_Group1 Extended Peripheral Control functions
  *  @brief  Extended Peripheral Control functions
  *
@verbatim
 ===============================================================================
                ##### Extended Peripheral Control functions  #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to control the RCC Clocks
    frequencies.
    [..]
    (@) Important note: Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to
        select the RTC clock source; in this case the Backup domain will be reset in
        order to modify the RTC Clock source, as consequence RTC registers (including
        the backup registers) are set to their reset values.

@endverbatim
  * @{
  */
/**
  * @brief  Initialize the RCC extended peripherals clocks according to the specified
  *         parameters in the RCC_PeriphCLKInitTypeDef.
  * @param  pPeriphClkInit  pointer to an RCC_PeriphCLKInitTypeDef structure that
  *         contains a field PeriphClockSelection which needs to be a single selection of one the following values:
  *            @arg @ref RCC_PERIPHCLK_LPTIM1_2
  *            @arg @ref RCC_PERIPHCLK_UART2_4
  *            @arg @ref RCC_PERIPHCLK_UART3_5
  *            @arg @ref RCC_PERIPHCLK_SPI2_3
  *            @arg @ref RCC_PERIPHCLK_SPDIFRX
  *            @arg @ref RCC_PERIPHCLK_I2C1_2
  *            @arg @ref RCC_PERIPHCLK_I3C1_2
  *            @arg @ref RCC_PERIPHCLK_I2C3_5
  *            @arg @ref RCC_PERIPHCLK_I3C3
  *            @arg @ref RCC_PERIPHCLK_I2C4_6
  *            @arg @ref RCC_PERIPHCLK_I2C7
  *            @arg @ref RCC_PERIPHCLK_SPI1
  *            @arg @ref RCC_PERIPHCLK_SPI4_5
  *            @arg @ref RCC_PERIPHCLK_SPI6_7
  *            @arg @ref RCC_PERIPHCLK_USART1
  *            @arg @ref RCC_PERIPHCLK_USART6
  *            @arg @ref RCC_PERIPHCLK_UART7_8
  *            @arg @ref RCC_PERIPHCLK_UART9
  *            @arg @ref RCC_PERIPHCLK_SAI1_MDF1
  *            @arg @ref RCC_PERIPHCLK_SAI2
  *            @arg @ref RCC_PERIPHCLK_SAI3_4
  *            @arg @ref RCC_PERIPHCLK_FDCAN
  *            @arg @ref RCC_PERIPHCLK_LTDC
  *            @arg @ref RCC_PERIPHCLK_DSIPHY
  *            @arg @ref RCC_PERIPHCLK_DCMIPP
  *            @arg @ref RCC_PERIPHCLK_CSITXESC
  *            @arg @ref RCC_PERIPHCLK_CSIPHY
  *            @arg @ref RCC_PERIPHCLK_LVDSPHY
  *            @arg @ref RCC_PERIPHCLK_STGEN
  *            @arg @ref RCC_PERIPHCLK_USB3PCIEPHY
  *            @arg @ref RCC_PERIPHCLK_USBTC
  *            @arg @ref RCC_PERIPHCLK_I3C4
  *            @arg @ref RCC_PERIPHCLK_SPI8
  *            @arg @ref RCC_PERIPHCLK_I2C8
  *            @arg @ref RCC_PERIPHCLK_LPUART1
  *            @arg @ref RCC_PERIPHCLK_LPTIM3
  *            @arg @ref RCC_PERIPHCLK_LPTIM4_5
  *            @arg @ref RCC_PERIPHCLK_ADF1
  *            @arg @ref RCC_PERIPHCLK_TSDBG
  *            @arg @ref RCC_PERIPHCLK_TPIU
  *            @arg @ref RCC_PERIPHCLK_ATB
  *            @arg @ref RCC_PERIPHCLK_ADC12
  *            @arg @ref RCC_PERIPHCLK_ADC3
  *            @arg @ref RCC_PERIPHCLK_OSPI1
  *            @arg @ref RCC_PERIPHCLK_OSPI2
  *            @arg @ref RCC_PERIPHCLK_FMC
  *            @arg @ref RCC_PERIPHCLK_SDMMC1
  *            @arg @ref RCC_PERIPHCLK_SDMMC2
  *            @arg @ref RCC_PERIPHCLK_SDMMC3
  *            @arg @ref RCC_PERIPHCLK_ETH1_ETHSW
  *            @arg @ref RCC_PERIPHCLK_ETH2
  *            @arg @ref RCC_PERIPHCLK_ETH1PTP_ETH2PTP
  *            @arg @ref RCC_PERIPHCLK_USB2PHY1
  *            @arg @ref RCC_PERIPHCLK_USB2PHY2
  *            @arg @ref RCC_PERIPHCLK_ICN_M_GPU
  *            @arg @ref RCC_PERIPHCLK_ETHSWREF
  *            @arg @ref RCC_PERIPHCLK_MCO1
  *            @arg @ref RCC_PERIPHCLK_MCO2
  *            @arg @ref RCC_PERIPHCLK_CPU1_EXT2F
  *
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCCEx_PeriphCLKConfig(RCC_PeriphCLKInitTypeDef  *pPeriphClkInit)
{
  uint32_t       tickstart;
  uint32_t       xbar_channel;
  uint32_t       xbar_channel_msk;
  __IO uint32_t *p_SRreg;

  /* Check parameters */

  xbar_channel = pPeriphClkInit->XBAR_Channel;
  xbar_channel_msk = 0x1UL << (pPeriphClkInit->XBAR_Channel % 32);

  /* Remember that final clock gating is handled through (existing) macros */
  /* xbar/findiv gating is propagating the clocks by default; we do not provide
     any means to change this */

  if (pPeriphClkInit->Div <= 64)
  {
    RCC->PREDIVxCFGR[xbar_channel] = 0x0;
    RCC->FINDIVxCFGR[xbar_channel] = ((pPeriphClkInit->Div) - 1) | (1 << 6);
  }
  else if (pPeriphClkInit->Div <= 128)
  {
    RCC->FINDIVxCFGR[xbar_channel] = ((pPeriphClkInit->Div / 2) - 1) | (1 << 6);
    RCC->PREDIVxCFGR[xbar_channel] = 0x1;
  }
  else if (pPeriphClkInit->Div <= 256)
  {
    RCC->FINDIVxCFGR[xbar_channel] = ((pPeriphClkInit->Div / 4) - 1) | (1 << 6);
    RCC->PREDIVxCFGR[xbar_channel] = 0x3;
  }
  else
  {
    RCC->FINDIVxCFGR[xbar_channel] = ((pPeriphClkInit->Div / 1024) - 1) | (1 << 6);
    RCC->PREDIVxCFGR[xbar_channel] = 0x3FF;
  }

  p_SRreg = (xbar_channel < 32) ? &RCC->PREDIVSR1 : &RCC->PREDIVSR2;

  /* Wait for prediv to be ready */
  tickstart = HAL_GetTick();

  while ((*p_SRreg & xbar_channel_msk) != RESET)
  {
    if ((HAL_GetTick() - tickstart) > PREDIV_TIMEOUT_VALUE)
    {
      return HAL_TIMEOUT;
    }
  }

  p_SRreg = (xbar_channel < 32) ? &RCC->FINDIVSR1 : &RCC->FINDIVSR2;

  /* Wait for findiv to be ready */
  tickstart = HAL_GetTick();

  while ((*p_SRreg & xbar_channel_msk) != RESET)
  {
    if ((HAL_GetTick() - tickstart) > FINDIV_TIMEOUT_VALUE)
    {
      return HAL_TIMEOUT;
    }
  }

  LL_RCC_SetCrossbarSource(xbar_channel, pPeriphClkInit->XBAR_ClkSrc);

  /* Wait for xbar to be ready */
  tickstart = HAL_GetTick();

  while ((RCC->XBARxCFGR[xbar_channel] & 0x80) != RESET)
  {
    if ((HAL_GetTick() - tickstart) > XBAR_TIMEOUT_VALUE)
    {
      return HAL_TIMEOUT;
    }
  }

#if defined(CORE_CA35)
  if (pPeriphClkInit->XBAR_Channel == RCC_PERIPHCLK_STGEN)
  {
    /* Adapt Systick interrupt period */
    if (HAL_InitTick(TICK_INT_PRIORITY) != HAL_OK)
    {
      return HAL_ERROR;
    }
  }
#endif /* CORE_CA35 */

  return HAL_OK;
}

/**
  * @brief  Get the RCC_ClkInitStruct according to the internal RCC configuration registers.
  * @param  pPeriphClkInit  pointer to an RCC_PeriphCLKInitTypeDef structure that contains
  *         the peripheral id which needs to be a single selection of one the following values:
  *            @arg @ref RCC_PERIPHCLK_LPTIM1_2
  *            @arg @ref RCC_PERIPHCLK_UART2_4
  *            @arg @ref RCC_PERIPHCLK_UART3_5
  *            @arg @ref RCC_PERIPHCLK_SPI2_3
  *            @arg @ref RCC_PERIPHCLK_SPDIFRX
  *            @arg @ref RCC_PERIPHCLK_I2C1_2
  *            @arg @ref RCC_PERIPHCLK_I3C1_2
  *            @arg @ref RCC_PERIPHCLK_I2C3_5
  *            @arg @ref RCC_PERIPHCLK_I3C3
  *            @arg @ref RCC_PERIPHCLK_I2C4_6
  *            @arg @ref RCC_PERIPHCLK_I2C7
  *            @arg @ref RCC_PERIPHCLK_SPI1
  *            @arg @ref RCC_PERIPHCLK_SPI4_5
  *            @arg @ref RCC_PERIPHCLK_SPI6_7
  *            @arg @ref RCC_PERIPHCLK_USART1
  *            @arg @ref RCC_PERIPHCLK_USART6
  *            @arg @ref RCC_PERIPHCLK_UART7_8
  *            @arg @ref RCC_PERIPHCLK_UART9
  *            @arg @ref RCC_PERIPHCLK_SAI1_MDF1
  *            @arg @ref RCC_PERIPHCLK_SAI2
  *            @arg @ref RCC_PERIPHCLK_SAI3_4
  *            @arg @ref RCC_PERIPHCLK_FDCAN
  *            @arg @ref RCC_PERIPHCLK_LTDC
  *            @arg @ref RCC_PERIPHCLK_DSIPHY
  *            @arg @ref RCC_PERIPHCLK_DCMIPP
  *            @arg @ref RCC_PERIPHCLK_CSITXESC
  *            @arg @ref RCC_PERIPHCLK_CSIPHY
  *            @arg @ref RCC_PERIPHCLK_LVDSPHY
  *            @arg @ref RCC_PERIPHCLK_STGEN
  *            @arg @ref RCC_PERIPHCLK_USB3PCIEPHY
  *            @arg @ref RCC_PERIPHCLK_USBTC
  *            @arg @ref RCC_PERIPHCLK_I3C4
  *            @arg @ref RCC_PERIPHCLK_SPI8
  *            @arg @ref RCC_PERIPHCLK_I2C8
  *            @arg @ref RCC_PERIPHCLK_LPUART1
  *            @arg @ref RCC_PERIPHCLK_LPTIM3
  *            @arg @ref RCC_PERIPHCLK_LPTIM4_5
  *            @arg @ref RCC_PERIPHCLK_ADF1
  *            @arg @ref RCC_PERIPHCLK_TSDBG
  *            @arg @ref RCC_PERIPHCLK_TPIU
  *            @arg @ref RCC_PERIPHCLK_ATB
  *            @arg @ref RCC_PERIPHCLK_ADC12
  *            @arg @ref RCC_PERIPHCLK_ADC3
  *            @arg @ref RCC_PERIPHCLK_OSPI1
  *            @arg @ref RCC_PERIPHCLK_OSPI2
  *            @arg @ref RCC_PERIPHCLK_FMC
  *            @arg @ref RCC_PERIPHCLK_SDMMC1
  *            @arg @ref RCC_PERIPHCLK_SDMMC2
  *            @arg @ref RCC_PERIPHCLK_SDMMC3
  *            @arg @ref RCC_PERIPHCLK_ETH1_ETHSW
  *            @arg @ref RCC_PERIPHCLK_ETH2
  *            @arg @ref RCC_PERIPHCLK_ETH1PTP_ETH2PTP
  *            @arg @ref RCC_PERIPHCLK_USB2PHY1
  *            @arg @ref RCC_PERIPHCLK_USB2PHY2
  *            @arg @ref RCC_PERIPHCLK_ICN_M_GPU
  *            @arg @ref RCC_PERIPHCLK_ETHSWREF
  *            @arg @ref RCC_PERIPHCLK_MCO1
  *            @arg @ref RCC_PERIPHCLK_MCO2
  *            @arg @ref RCC_PERIPHCLK_CPU1_EXT2F
  * @retval None
  */
void HAL_RCCEx_GetPeriphCLKConfig(RCC_PeriphCLKInitTypeDef  *pPeriphClkInit)
{
  uint32_t xbar_channel;

  /* Check parameters */

  xbar_channel = pPeriphClkInit->XBAR_Channel;

  pPeriphClkInit->Div = (((RCC->FINDIVxCFGR[xbar_channel] & RCC_FINDIVxCFGR_FINDIVx_Msk) + 1) * ((RCC->PREDIVxCFGR[xbar_channel] & RCC_PREDIVxCFGR_PREDIVx_Msk) + 1));
  pPeriphClkInit->XBAR_ClkSrc = RCC->XBARxCFGR[xbar_channel] & RCC_XBARxCFGR_XBARxSEL_Msk;
}

#if defined(CORE_CA35)
HAL_StatusTypeDef HAL_RCCEx_CA35SS_SetCA35SSClockSource(uint32_t source)
{
  uint32_t tickstart;

  if (source)
  {
    PLL1_Control->CA35SS_SSC_CHGCLKREQ_WS1 = CA35SS_ARMCHGCLKREQ;
  }
  else
  {
    PLL1_Control->CA35SS_SSC_CHGCLKREQ_WC1 = CA35SS_ARMCHGCLKREQ;
  }

  tickstart = HAL_GetTick();

  /* Wait for mux to get ready */
  while (((PLL1_Control->CA35SS_SSC_CHGCLKREQ_RW & CA35SS_ARMCHGCLKACK) >> 0x1) != source)
  {
    if ((HAL_GetTick() - tickstart) > CHGCLK_TIMEOUT_VALUE)
    {
      return HAL_TIMEOUT;
    }
  }

  return HAL_OK;
}

HAL_StatusTypeDef HAL_RCCEx_CA35SS_GetCA35SSClockSource(uint32_t *source)
{
  *source = PLL1_Control->CA35SS_SSC_CHGCLKREQ_RW & CA35SS_ARMCHGCLKREQ;

  return HAL_OK;
}
#endif /* CORE_CA35 */

#if defined(CORE_CA35)
/**
  * @brief  Configures PLL1
  * @param  pll_config: pointer to an RCC_PLLInitTypeDef structure
  *
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCCEx_CA35SS_PLL1Config(const RCC_PLLInitTypeDef *pll_config)
{
  uint32_t tickstart;

  if ((pll_config->PLLState) == RCC_PLL_ON)
  {
    /* Setup PLL (source) */
    LL_RCC_PLL1_SetSource((pll_config->PLLSource) << RCC_MUXSELCFGR_MUXSEL5_Pos);

    /* Reset PLL1 */
    PLL1_Control->CA35SS_SSC_PLL_ENABLE_WC1 = CA35SS_PLLRST;

    /* Disable PLL1 */
    PLL1_Control->CA35SS_SSC_PLL_ENABLE_WC1 = CA35SS_PLLEN;

    /* Write PLL1 parameters */
    MODIFY_REG(PLL1_Control->CA35SS_SSC_PLL_FREQ1_RW, CA35SS_REFDIV, pll_config->FREFDIV << CA35SS_REFDIV_Pos);
    MODIFY_REG(PLL1_Control->CA35SS_SSC_PLL_FREQ1_RW, CA35SS_FBDIV, pll_config->FBDIV << CA35SS_FBDIV_Pos);
    MODIFY_REG(PLL1_Control->CA35SS_SSC_PLL_FREQ2_RW, CA35SS_POSTDIV1, pll_config->POSTDIV1 << CA35SS_POSTDIV1_Pos);
    MODIFY_REG(PLL1_Control->CA35SS_SSC_PLL_FREQ2_RW, CA35SS_POSTDIV2, pll_config->POSTDIV2 << CA35SS_POSTDIV2_Pos);

    /* Enable PLL1 */
    PLL1_Control->CA35SS_SSC_PLL_ENABLE_WS1 = CA35SS_PLLEN;

    /* Wait for PLL to lock */
    tickstart = HAL_GetTick();

    while ((PLL1_Control->CA35SS_SSC_PLL_ENABLE_RW & 0x2) == RESET)
    {
      if ((HAL_GetTick() - tickstart) > PLL1_LOCK_TIMEOUT_VALUE)
      {
        return HAL_TIMEOUT;
      }
    }

    /* deassert PLL1 reset */
    PLL1_Control->CA35SS_SSC_PLL_ENABLE_WS1 = CA35SS_PLLRST;
  }
  else /* PLL off */
  {
    HAL_RCCEx_CA35SS_SetCA35SSClockSource(A35_SOURCE_EXT);

    /* Reset PLL1 */
    PLL1_Control->CA35SS_SSC_PLL_ENABLE_WC1 = CA35SS_PLLRST;

    /* Disable PLL1 */
    PLL1_Control->CA35SS_SSC_PLL_ENABLE_WC1 = CA35SS_PLLEN;

    /* Wait for PLL to unlock */
    tickstart = HAL_GetTick();

    while ((PLL1_Control->CA35SS_SSC_PLL_ENABLE_RW & 0x2) != RESET)
    {
      if ((HAL_GetTick() - tickstart) > PLL1_LOCK_TIMEOUT_VALUE)
      {
        return HAL_TIMEOUT;
      }
    }
  }

  return HAL_OK;
}
#endif /* CORE_CA35 */

/**
  * @brief  Configures PLL2
  * @param  pll_config: pointer to an RCC_PLLInitTypeDef structure
  *
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCCEx_PLL2Config(RCC_PLLInitTypeDef *pll_config)
{
  uint32_t tickstart;
  uint32_t ddr_axi_clk = 0;

  /* Check parameters */

  /* Bypass PLL */
  LL_RCC_PLL2_FOUTPOSTDIV_FREF_Enable();

  /* Setup GFG (source) */
  LL_RCC_PLL2_SetSource((pll_config->PLLSource) << RCC_MUXSELCFGR_MUXSEL6_Pos);

  /* Setup VCO params */
  LL_RCC_PLL2_SetFREFDIV(pll_config->FREFDIV);
  LL_RCC_PLL2_SetFBDIV(pll_config->FBDIV);

  /* Setup postdiv */
  LL_RCC_PLL2_ConfigFREQDIV_L1(pll_config->POSTDIV1);
  LL_RCC_PLL2_ConfigFREQDIV_L2(pll_config->POSTDIV2);

  /* Setup fractional if requested, SSCG (spread spectrum) if required */

  if ((pll_config->PLLMode & RCC_PLL_SPREAD_SPECTRUM) == RCC_PLL_SPREAD_SPECTRUM)
  {
    LL_RCC_PLL2_ConfigCSG(pll_config->SSM_DIVVAL, pll_config->SSM_SPREAD);
    LL_RCC_PLL2_CSG_Enable();
    LL_RCC_PLL2_DSMEN_Disable();

    if (pll_config->SSM_Mode == RCC_PLL_CENTERSPREAD)
    {
      CLEAR_BIT(RCC->PLL2CFGR3, RCC_PLL2CFGR3_DOWNSPREAD);
    }
    else
    {
      SET_BIT(RCC->PLL2CFGR3, RCC_PLL2CFGR3_DOWNSPREAD);
    }
  }

  if (pll_config->FRACIN != 0)
  {
    LL_RCC_PLL2_SetFRACIN(pll_config->FRACIN);
    LL_RCC_PLL2_CSG_Disable();
    LL_RCC_PLL2_DSMEN_Enable();
  }

  /* Handle state (on/off) - if state is on, wait for lock bit */
  if (pll_config->PLLState == RCC_PLL_ON)
  {
    if (HAL_IS_BIT_CLR(RCC->DDRCPCFGR, RCC_DDRCPCFGR_DDRCPEN))
    {
      ddr_axi_clk = 1;

      /* This clock needs to be enabled to allow the PLL to lock */
      SET_BIT(RCC->DDRCPCFGR, RCC_DDRCPCFGR_DDRCPEN);
    }
    else
    {
      ddr_axi_clk = 0;
    }

    LL_RCC_PLL2_Enable();

    /* Wait for lock */
    tickstart = HAL_GetTick();

    while (LL_RCC_PLL2_IsReady() == RESET)
    {
      if ((HAL_GetTick() - tickstart) > LOCK_TIMEOUT_VALUE)
      {
        return HAL_TIMEOUT;
      }
    }

    /* Disable bypass */
    LL_RCC_PLL2_FOUTPOSTDIV_FREF_Disable();

    /* Enable postdiv1 & postdiv2, sscg (release ssmod), frac if PLL ON */
    if ((pll_config->PLLMode & RCC_PLL_SPREAD_SPECTRUM) == RCC_PLL_SPREAD_SPECTRUM)
    {
      LL_RCC_PLL2_CSG_Enable();
      LL_RCC_PLL2_SSMODRST_Release();
    }
    else
    {
      LL_RCC_PLL2_CSG_Disable();
      LL_RCC_PLL2_SSMODRST_Assert();
    }

    LL_RCC_PLL2_FOUTPOSTDIV_Enable();
  }
  else
  {
    if (ddr_axi_clk == 1)
    {
      CLEAR_BIT(RCC->DDRCPCFGR, RCC_DDRCPCFGR_DDRCPEN);
    }

    LL_RCC_PLL2_Disable();
  }

  return HAL_OK;
}

/**
  * @brief  Configures PLL3
  * @param  pll_config: pointer to an RCC_PLLInitTypeDef structure
  *
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCCEx_PLL3Config(RCC_PLLInitTypeDef *pll_config)
{
  uint32_t tickstart;

  /* Check parameters */

  /* Bypass PLL */
  LL_RCC_PLL3_FOUTPOSTDIV_FREF_Enable();

  /* Setup GFG (source) */
  LL_RCC_PLL3_SetSource((pll_config->PLLSource) << RCC_MUXSELCFGR_MUXSEL7_Pos);

  /* Setup VCO params */
  LL_RCC_PLL3_SetFREFDIV(pll_config->FREFDIV);
  LL_RCC_PLL3_SetFBDIV(pll_config->FBDIV);

  /* Setup postdiv */
  LL_RCC_PLL3_ConfigFREQDIV_L1(pll_config->POSTDIV1);
  LL_RCC_PLL3_ConfigFREQDIV_L2(pll_config->POSTDIV2);

  /* Setup fractional if requested, SSCG (spread spectrum) if required */

  if ((pll_config->PLLMode & RCC_PLL_SPREAD_SPECTRUM) == RCC_PLL_SPREAD_SPECTRUM)
  {
    LL_RCC_PLL3_ConfigCSG(pll_config->SSM_DIVVAL, pll_config->SSM_SPREAD);

    if (pll_config->SSM_Mode == RCC_PLL_CENTERSPREAD)
    {
      CLEAR_BIT(RCC->PLL3CFGR3, RCC_PLL3CFGR3_DOWNSPREAD);
    }
    else
    {
      SET_BIT(RCC->PLL3CFGR3, RCC_PLL3CFGR3_DOWNSPREAD);
    }
  }

  LL_RCC_PLL3_SetFRACIN(pll_config->FRACIN);

  /* Handle state (on/off) - if state is on, wait for lock bit */
  if (pll_config->PLLState == RCC_PLL_ON)
  {
    LL_RCC_PLL3_Enable();

    /* Wait for lock */
    tickstart = HAL_GetTick();

    while (LL_RCC_PLL3_IsReady() == RESET)
    {
      if ((HAL_GetTick() - tickstart) > LOCK_TIMEOUT_VALUE)
      {
        return HAL_TIMEOUT;
      }
    }

    /* Disable bypass */
    LL_RCC_PLL3_FOUTPOSTDIV_FREF_Disable();

    /* Enable postdiv1 & postdiv2, sscg (release ssmod), frac if PLL ON */
    if ((pll_config->PLLMode & RCC_PLL_SPREAD_SPECTRUM) == RCC_PLL_SPREAD_SPECTRUM)
    {
      LL_RCC_PLL3_CSG_Enable();
      LL_RCC_PLL3_SSMODRST_Release();
    }
    else
    {
      LL_RCC_PLL3_CSG_Disable();
      LL_RCC_PLL3_SSMODRST_Assert();
    }

    LL_RCC_PLL3_FOUTPOSTDIV_Enable();
  }
  else
  {
    LL_RCC_PLL3_Disable();
  }

  return HAL_OK;
}

/*
  * @brief  Configures PLL4
  * @param  pll_config: pointer to an RCC_PLLInitTypeDef structure
  *
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCCEx_PLL4Config(RCC_PLLInitTypeDef *pll_config)
{
  uint32_t tickstart;

  /* Bypass PLL */
  LL_RCC_PLL4_FOUTPOSTDIV_FREF_Enable();

  /* Setup GFG (source) */
  LL_RCC_PLL4_SetSource((pll_config->PLLSource) << RCC_MUXSELCFGR_MUXSEL0_Pos);

  /* Setup VCO params */
  LL_RCC_PLL4_SetFREFDIV(pll_config->FREFDIV);
  LL_RCC_PLL4_SetFBDIV(pll_config->FBDIV);

  /* Setup postdiv */
  LL_RCC_PLL4_ConfigFREQDIV_L1(pll_config->POSTDIV1);
  LL_RCC_PLL4_ConfigFREQDIV_L2(pll_config->POSTDIV2);

  /* Setup fractional if requested, SSCG (spread spectrum) if required */

  if ((pll_config->PLLMode & RCC_PLL_SPREAD_SPECTRUM) == RCC_PLL_SPREAD_SPECTRUM)
  {
    LL_RCC_PLL4_ConfigCSG(pll_config->SSM_DIVVAL, pll_config->SSM_SPREAD);
    LL_RCC_PLL4_CSG_Enable();
    LL_RCC_PLL4_DSMEN_Disable();

    if (pll_config->SSM_Mode == RCC_PLL_CENTERSPREAD)
    {
      CLEAR_BIT(RCC->PLL4CFGR3, RCC_PLL4CFGR3_DOWNSPREAD);
    }
    else
    {
      SET_BIT(RCC->PLL4CFGR3, RCC_PLL4CFGR3_DOWNSPREAD);
    }
  }

  if (pll_config->FRACIN != 0)
  {
    LL_RCC_PLL4_SetFRACIN(pll_config->FRACIN);
    LL_RCC_PLL4_CSG_Disable();
    LL_RCC_PLL4_DSMEN_Enable();
  }

  /* Handle state (on/off) - if state is on, wait for lock bit */
  if (pll_config->PLLState == RCC_PLL_ON)
  {
    LL_RCC_PLL4_Enable();

    /* Wait for lock */
    tickstart = HAL_GetTick();

    while (LL_RCC_PLL4_IsReady() == RESET)
    {
      if ((HAL_GetTick() - tickstart) > LOCK_TIMEOUT_VALUE)
      {
        return HAL_TIMEOUT;
      }
    }

    /* Disable bypass */
    LL_RCC_PLL4_FOUTPOSTDIV_FREF_Disable();

    /* Enable postdiv1 & postdiv2, sscg (release ssmod), frac if PLL ON */
    if ((pll_config->PLLMode & RCC_PLL_SPREAD_SPECTRUM) == RCC_PLL_SPREAD_SPECTRUM)
    {
      LL_RCC_PLL4_CSG_Enable();
      LL_RCC_PLL4_SSMODRST_Release();
    }
    else
    {
      LL_RCC_PLL4_CSG_Disable();
      LL_RCC_PLL4_SSMODRST_Assert();
    }

    LL_RCC_PLL4_FOUTPOSTDIV_Enable();
  }
  else
  {
    LL_RCC_PLL4_Disable();
  }

  return HAL_OK;
}

/**
  * @brief  Configures PLL5
  * @param  pll_config: pointer to an RCC_PLLInitTypeDef structure
  *
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCCEx_PLL5Config(RCC_PLLInitTypeDef *pll_config)
{
  uint32_t tickstart;

  /* Bypass PLL */
  LL_RCC_PLL5_FOUTPOSTDIV_FREF_Enable();

  /* Setup GFG (source) */
  LL_RCC_PLL5_SetSource((pll_config->PLLSource) << RCC_MUXSELCFGR_MUXSEL1_Pos);

  /* Setup VCO params */
  LL_RCC_PLL5_SetFREFDIV(pll_config->FREFDIV);
  LL_RCC_PLL5_SetFBDIV(pll_config->FBDIV);

  /* Setup postdiv */
  LL_RCC_PLL5_ConfigFREQDIV_L1(pll_config->POSTDIV1);
  LL_RCC_PLL5_ConfigFREQDIV_L2(pll_config->POSTDIV2);

  /* Setup fractionnal if requested, SSCG (spread spectrum) if required */
  if ((pll_config->PLLMode & RCC_PLL_SPREAD_SPECTRUM) == RCC_PLL_SPREAD_SPECTRUM)
  {
    LL_RCC_PLL5_ConfigCSG(pll_config->SSM_DIVVAL, pll_config->SSM_SPREAD);
    LL_RCC_PLL5_CSG_Enable();
    LL_RCC_PLL5_DSMEN_Disable();

    if (pll_config->SSM_Mode == RCC_PLL_CENTERSPREAD)
    {
      CLEAR_BIT(RCC->PLL5CFGR3, RCC_PLL5CFGR3_DOWNSPREAD);
    }
    else
    {
      SET_BIT(RCC->PLL5CFGR3, RCC_PLL5CFGR3_DOWNSPREAD);
    }
  }

  if (pll_config->FRACIN != 0)
  {
    LL_RCC_PLL5_SetFRACIN(pll_config->FRACIN);
    LL_RCC_PLL5_CSG_Disable();
    LL_RCC_PLL5_DSMEN_Enable();
  }

  /* Handle state (on/off) - if state is on, wait for lock bit */
  if (pll_config->PLLState == RCC_PLL_ON)
  {
    LL_RCC_PLL5_Enable();

    /* Wait for lock */
    tickstart = HAL_GetTick();

    while (LL_RCC_PLL5_IsReady() == RESET)
    {
      if ((HAL_GetTick() - tickstart) > LOCK_TIMEOUT_VALUE)
      {
        return HAL_TIMEOUT;
      }
    }

    /* Disable bypass */
    LL_RCC_PLL5_FOUTPOSTDIV_FREF_Disable();

    /* Enable postdiv1 & postdiv2, sscg (release ssmod), frac if PLL ON */
    if ((pll_config->PLLMode & RCC_PLL_SPREAD_SPECTRUM) == RCC_PLL_SPREAD_SPECTRUM)
    {
      LL_RCC_PLL5_CSG_Enable();
      LL_RCC_PLL5_SSMODRST_Release();
    }
    else
    {
      LL_RCC_PLL5_CSG_Disable();
      LL_RCC_PLL5_SSMODRST_Assert();
    }

    LL_RCC_PLL5_FOUTPOSTDIV_Enable();
  }
  else
  {
    LL_RCC_PLL5_Disable();
  }

  return HAL_OK;
}

/**
  * @brief  Configures PLL6
  * @param  pll_config: pointer to an RCC_PLLInitTypeDef structure
  *
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCCEx_PLL6Config(RCC_PLLInitTypeDef *pll_config)
{
  uint32_t tickstart;

  /* Bypass PLL */
  LL_RCC_PLL6_FOUTPOSTDIV_FREF_Enable();

  /* Setup GFG (source) */
  LL_RCC_PLL6_SetSource((pll_config->PLLSource) << RCC_MUXSELCFGR_MUXSEL2_Pos);

  /* Setup VCO params */
  LL_RCC_PLL6_SetFREFDIV(pll_config->FREFDIV);
  LL_RCC_PLL6_SetFBDIV(pll_config->FBDIV);

  /* Setup postdiv */
  LL_RCC_PLL6_ConfigFREQDIV_L1(pll_config->POSTDIV1);
  LL_RCC_PLL6_ConfigFREQDIV_L2(pll_config->POSTDIV2);

  /* Setup fractionnal if requested, SSCG (spread spectrum) if required */
  if ((pll_config->PLLMode & RCC_PLL_SPREAD_SPECTRUM) == RCC_PLL_SPREAD_SPECTRUM)
  {
    LL_RCC_PLL6_ConfigCSG(pll_config->SSM_DIVVAL, pll_config->SSM_SPREAD);
    LL_RCC_PLL6_CSG_Enable();
    LL_RCC_PLL6_DSMEN_Disable();

    if (pll_config->SSM_Mode == RCC_PLL_CENTERSPREAD)
    {
      CLEAR_BIT(RCC->PLL6CFGR3, RCC_PLL6CFGR3_DOWNSPREAD);
    }
    else
    {
      SET_BIT(RCC->PLL6CFGR3, RCC_PLL6CFGR3_DOWNSPREAD);
    }
  }

  if (pll_config->FRACIN != 0)
  {
    LL_RCC_PLL6_SetFRACIN(pll_config->FRACIN);
    LL_RCC_PLL6_CSG_Disable();
    LL_RCC_PLL6_DSMEN_Enable();
  }

  /* Handle state (on/off) - if state is on, wait for lock bit */
  if (pll_config->PLLState == RCC_PLL_ON)
  {
    LL_RCC_PLL6_Enable();

    /* Wait for lock */
    tickstart = HAL_GetTick();

    while (LL_RCC_PLL6_IsReady() == RESET)
    {
      if ((HAL_GetTick() - tickstart) > LOCK_TIMEOUT_VALUE)
      {
        return HAL_TIMEOUT;
      }
    }

    /* Disable bypass */
    LL_RCC_PLL6_FOUTPOSTDIV_FREF_Disable();

    /* Enable postdiv1 & postdiv2, sscg (release ssmod), frac if PLL ON */
    if ((pll_config->PLLMode & RCC_PLL_SPREAD_SPECTRUM) == RCC_PLL_SPREAD_SPECTRUM)
    {
      LL_RCC_PLL6_CSG_Enable();
      LL_RCC_PLL6_SSMODRST_Release();
    }
    else
    {
      LL_RCC_PLL6_CSG_Disable();
      LL_RCC_PLL6_SSMODRST_Assert();
    }

    LL_RCC_PLL6_FOUTPOSTDIV_Enable();
  }
  else
  {
    LL_RCC_PLL6_Disable();
  }
  return HAL_OK;
}

/**
  * @brief  Configures PLL7
  * @param  pll_config: pointer to an RCC_PLLInitTypeDef structure
  *
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCCEx_PLL7Config(RCC_PLLInitTypeDef *pll_config)
{
  uint32_t tickstart;

  /* Bypass PLL */
  LL_RCC_PLL7_FOUTPOSTDIV_FREF_Enable();

  /* Setup GFG (source) */
  LL_RCC_PLL7_SetSource((pll_config->PLLSource) << RCC_MUXSELCFGR_MUXSEL3_Pos);

  /* Setup VCO params */
  LL_RCC_PLL7_SetFREFDIV(pll_config->FREFDIV);
  LL_RCC_PLL7_SetFBDIV(pll_config->FBDIV);

  /* Setup postdiv */
  LL_RCC_PLL7_ConfigFREQDIV_L1(pll_config->POSTDIV1);
  LL_RCC_PLL7_ConfigFREQDIV_L2(pll_config->POSTDIV2);

  /* Setup fractionnal if requested, SSCG (spread spectrum) if required */
  if ((pll_config->PLLMode & RCC_PLL_SPREAD_SPECTRUM) == RCC_PLL_SPREAD_SPECTRUM)
  {
    LL_RCC_PLL7_ConfigCSG(pll_config->SSM_DIVVAL, pll_config->SSM_SPREAD);
    LL_RCC_PLL7_CSG_Enable();
    LL_RCC_PLL7_DSMEN_Disable();

    if (pll_config->SSM_Mode == RCC_PLL_CENTERSPREAD)
    {
      CLEAR_BIT(RCC->PLL7CFGR3, RCC_PLL7CFGR3_DOWNSPREAD);
    }
    else
    {
      SET_BIT(RCC->PLL7CFGR3, RCC_PLL7CFGR3_DOWNSPREAD);
    }
  }

  if (pll_config->FRACIN != 0)
  {
    LL_RCC_PLL7_SetFRACIN(pll_config->FRACIN);
    LL_RCC_PLL7_CSG_Disable();
    LL_RCC_PLL7_DSMEN_Enable();
  }

  /* Handle state (on/off) - if state is on, wait for lock bit */
  if (pll_config->PLLState == RCC_PLL_ON)
  {
    LL_RCC_PLL7_Enable();

    /* Wait for lock */
    tickstart = HAL_GetTick();

    while (LL_RCC_PLL7_IsReady() == RESET)
    {
      if ((HAL_GetTick() - tickstart) > LOCK_TIMEOUT_VALUE)
      {
        return HAL_TIMEOUT;
      }
    }

    /* Disable bypass */
    LL_RCC_PLL7_FOUTPOSTDIV_FREF_Disable();

    /* Enable postdiv1 & postdiv2, sscg (release ssmod), frac if PLL ON */
    if ((pll_config->PLLMode & RCC_PLL_SPREAD_SPECTRUM) == RCC_PLL_SPREAD_SPECTRUM)
    {
      LL_RCC_PLL7_CSG_Enable();
    }
    else
    {
      LL_RCC_PLL7_CSG_Disable();
      LL_RCC_PLL7_SSMODRST_Release();
    }

    LL_RCC_PLL7_FOUTPOSTDIV_Enable();
    LL_RCC_PLL7_SSMODRST_Assert();
  }
  else
  {
    LL_RCC_PLL7_Disable();
  }

  return HAL_OK;
}

/**
  * @brief  Configures PLL8
  * @param  pll_config: pointer to an RCC_PLLInitTypeDef structure
  *
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCCEx_PLL8Config(RCC_PLLInitTypeDef *pll_config)
{
  uint32_t tickstart;

  /* Bypass PLL */
  LL_RCC_PLL8_FOUTPOSTDIV_FREF_Enable();

  /* Setup GFG (source) */
  LL_RCC_PLL8_SetSource((pll_config->PLLSource) << RCC_MUXSELCFGR_MUXSEL4_Pos);

  /* Setup VCO params */
  LL_RCC_PLL8_SetFREFDIV(pll_config->FREFDIV);
  LL_RCC_PLL8_SetFBDIV(pll_config->FBDIV);

  /* Setup postdiv */
  LL_RCC_PLL8_ConfigFREQDIV_L1(pll_config->POSTDIV1);
  LL_RCC_PLL8_ConfigFREQDIV_L2(pll_config->POSTDIV2);

  /* Setup fractionnal if requested, SSCG (spread spectrum) if required */
  if ((pll_config->PLLMode & RCC_PLL_SPREAD_SPECTRUM) == RCC_PLL_SPREAD_SPECTRUM)
  {
    LL_RCC_PLL8_ConfigCSG(pll_config->SSM_DIVVAL, pll_config->SSM_SPREAD);
    LL_RCC_PLL8_CSG_Enable();
    LL_RCC_PLL8_DSMEN_Disable();

    if (pll_config->SSM_Mode == RCC_PLL_CENTERSPREAD)
    {
      CLEAR_BIT(RCC->PLL8CFGR3, RCC_PLL8CFGR3_DOWNSPREAD);
    }
    else
    {
      SET_BIT(RCC->PLL8CFGR3, RCC_PLL8CFGR3_DOWNSPREAD);
    }
  }

  if (pll_config->FRACIN != 0)
  {
    LL_RCC_PLL8_SetFRACIN(pll_config->FRACIN);
    LL_RCC_PLL8_CSG_Disable();
    LL_RCC_PLL8_DSMEN_Enable();
  }

  /* Handle state (on/off) - if state is on, wait for lock bit */
  if (pll_config->PLLState == RCC_PLL_ON)
  {
    LL_RCC_PLL8_Enable();

    /* Wait for lock */
    tickstart = HAL_GetTick();

    while (LL_RCC_PLL8_IsReady() == RESET)
    {
      if ((HAL_GetTick() - tickstart) > LOCK_TIMEOUT_VALUE)
      {
        return HAL_TIMEOUT;
      }
    }

    /* Disable bypass */
    LL_RCC_PLL8_FOUTPOSTDIV_FREF_Disable();

    /* Enable postdiv1 & postdiv2, sscg (release ssmod), frac if PLL ON */
    if ((pll_config->PLLMode & RCC_PLL_SPREAD_SPECTRUM) == RCC_PLL_SPREAD_SPECTRUM)
    {
      LL_RCC_PLL8_CSG_Enable();
      LL_RCC_PLL8_SSMODRST_Release();
    }
    else
    {
      LL_RCC_PLL8_CSG_Disable();
      LL_RCC_PLL8_SSMODRST_Assert();
    }

    LL_RCC_PLL8_FOUTPOSTDIV_Enable();
  }
  else
  {
    LL_RCC_PLL8_Disable();
  }

  return HAL_OK;
}

#if defined(CORE_CA35)
/**
  * @brief  Retrieve the PLL1 configuration settings
  * @param  pll_config: pointer to an RCC_PLLInitTypeDef structure
  *
  * @retval None
  */
void HAL_RCCEx_CA35SS_GetPLL1Config(RCC_PLLInitTypeDef *pll_config)
{
  pll_config->id = 1;
  pll_config->PLLState = ((PLL1_Control->CA35SS_SSC_PLL_ENABLE_RW & CA35SS_PLLEN) == CA35SS_PLLEN ? RCC_PLL_ON : RCC_PLL_OFF);
  pll_config->PLLSource = (RCC->MUXSELCFGR & RCC_MUXSELCFGR_MUXSEL5_Msk) >> RCC_MUXSELCFGR_MUXSEL5_Pos;
  pll_config->FREFDIV = (PLL1_Control->CA35SS_SSC_PLL_FREQ1_RW & CA35SS_REFDIV_Msk) >> CA35SS_REFDIV_Pos;
  pll_config->FBDIV = (PLL1_Control->CA35SS_SSC_PLL_FREQ1_RW & CA35SS_FBDIV_Msk) >> CA35SS_FBDIV_Pos;
  pll_config->POSTDIV1 = (PLL1_Control->CA35SS_SSC_PLL_FREQ2_RW & CA35SS_POSTDIV1_Msk) >> CA35SS_POSTDIV1_Pos;
  pll_config->POSTDIV2 = (PLL1_Control->CA35SS_SSC_PLL_FREQ2_RW & CA35SS_POSTDIV2_Msk) >> CA35SS_POSTDIV2_Pos;
  pll_config->SSM_Mode = 0;
  pll_config->SSM_SPREAD = 0;
  pll_config->SSM_DIVVAL = 0;
}
#endif /* CORE_CA35 */

/**
  * @brief  Retrieve the PLL2 configuration settings
  * @param  pll_config: pointer to an RCC_PLLInitTypeDef structure
  *
  * @retval None
  */
void HAL_RCCEx_GetPLL2Config(RCC_PLLInitTypeDef  *pll_config)
{
  pll_config->id = 2;
  pll_config->PLLState = ((RCC->PLL2CFGR1 & RCC_PLL2CFGR1_PLLEN) == RCC_PLL2CFGR1_PLLEN ? RCC_PLL_ON : RCC_PLL_OFF);
  pll_config->PLLSource = (RCC->MUXSELCFGR & RCC_MUXSELCFGR_MUXSEL6_Msk) >> RCC_MUXSELCFGR_MUXSEL6_Pos;
  pll_config->FREFDIV = (RCC->PLL2CFGR2 & RCC_PLL2CFGR2_FREFDIV_Msk) >> RCC_PLL2CFGR2_FREFDIV_Pos;
  pll_config->FBDIV = (RCC->PLL2CFGR2 & RCC_PLL2CFGR2_FBDIV_Msk) >> RCC_PLL2CFGR2_FBDIV_Pos;
  pll_config->FRACIN = (RCC->PLL2CFGR3 & RCC_PLL2CFGR3_FRACIN_Msk) >> RCC_PLL2CFGR3_FRACIN_Pos;
  pll_config->POSTDIV1 = (RCC->PLL2CFGR6 & RCC_PLL2CFGR6_POSTDIV1_Msk) >> RCC_PLL2CFGR6_POSTDIV1_Pos;
  pll_config->POSTDIV2 = (RCC->PLL2CFGR7 & RCC_PLL2CFGR7_POSTDIV2_Msk) >> RCC_PLL2CFGR7_POSTDIV2_Pos;
  pll_config->PLLMode = 0;
  if (LL_RCC_PLL2_CSG_IsEnabled())
  {
    pll_config->PLLMode |= RCC_PLL_SPREAD_SPECTRUM;
  }
}

/**
  * @brief  Retrieve the PLL3 configuration settings
  * @param  pll_config: pointer to an RCC_PLLInitTypeDef structure
  *
  * @retval None
  */
void HAL_RCCEx_GetPLL3Config(RCC_PLLInitTypeDef  *pll_config)
{
  pll_config->id = 3;
  pll_config->PLLState = ((RCC->PLL3CFGR1 & RCC_PLL3CFGR1_PLLEN) == RCC_PLL3CFGR1_PLLEN ? RCC_PLL_ON : RCC_PLL_OFF);
  pll_config->PLLSource = (RCC->MUXSELCFGR & RCC_MUXSELCFGR_MUXSEL7_Msk) >> RCC_MUXSELCFGR_MUXSEL7_Pos;
  pll_config->FREFDIV = (RCC->PLL3CFGR2 & RCC_PLL3CFGR2_FREFDIV_Msk) >> RCC_PLL3CFGR2_FREFDIV_Pos;
  pll_config->FBDIV = (RCC->PLL3CFGR2 & RCC_PLL3CFGR2_FBDIV_Msk) >> RCC_PLL3CFGR2_FBDIV_Pos;
  pll_config->FRACIN = (RCC->PLL3CFGR3 & RCC_PLL3CFGR3_FRACIN_Msk) >> RCC_PLL3CFGR3_FRACIN_Pos;
  pll_config->POSTDIV1 = (RCC->PLL3CFGR6 & RCC_PLL3CFGR6_POSTDIV1_Msk) >> RCC_PLL3CFGR6_POSTDIV1_Pos;
  pll_config->POSTDIV2 = (RCC->PLL3CFGR7 & RCC_PLL3CFGR7_POSTDIV2_Msk) >> RCC_PLL3CFGR7_POSTDIV2_Pos;
  pll_config->PLLMode = 0;
  if (LL_RCC_PLL3_CSG_IsEnabled())
  {
    pll_config->PLLMode |= RCC_PLL_SPREAD_SPECTRUM;
  }
}

/**
  * @brief  Retrieve the PLL4 configuration settings
  * @param  pll_config: pointer to an RCC_PLLInitTypeDef structure
  *
  * @retval None
  */
void HAL_RCCEx_GetPLL4Config(RCC_PLLInitTypeDef  *pll_config)
{
  pll_config->id = 4;
  pll_config->PLLState = ((RCC->PLL4CFGR1 & RCC_PLL4CFGR1_PLLEN) == RCC_PLL4CFGR1_PLLEN ? RCC_PLL_ON : RCC_PLL_OFF);
  pll_config->PLLSource = (RCC->MUXSELCFGR & RCC_MUXSELCFGR_MUXSEL0_Msk) >> RCC_MUXSELCFGR_MUXSEL0_Pos;
  pll_config->FREFDIV = (RCC->PLL4CFGR2 & RCC_PLL4CFGR2_FREFDIV_Msk) >> RCC_PLL4CFGR2_FREFDIV_Pos;
  pll_config->FBDIV = (RCC->PLL4CFGR2 & RCC_PLL4CFGR2_FBDIV_Msk) >> RCC_PLL4CFGR2_FBDIV_Pos;
  pll_config->FRACIN = (RCC->PLL4CFGR3 & RCC_PLL4CFGR3_FRACIN_Msk) >> RCC_PLL4CFGR3_FRACIN_Pos;
  pll_config->POSTDIV1 = (RCC->PLL4CFGR6 & RCC_PLL4CFGR6_POSTDIV1_Msk) >> RCC_PLL4CFGR6_POSTDIV1_Pos;
  pll_config->POSTDIV2 = (RCC->PLL4CFGR7 & RCC_PLL4CFGR7_POSTDIV2_Msk) >> RCC_PLL4CFGR7_POSTDIV2_Pos;
  pll_config->PLLMode = 0;
  if (LL_RCC_PLL4_CSG_IsEnabled())
  {
    pll_config->PLLMode |= RCC_PLL_SPREAD_SPECTRUM;
  }
}

/**
  * @brief  Retrieve the PLL5 configuration settings
  * @param  pll_config: pointer to an RCC_PLLInitTypeDef structure
  *
  * @retval None
  */
void HAL_RCCEx_GetPLL5Config(RCC_PLLInitTypeDef  *pll_config)
{
  pll_config->id = 5;
  pll_config->PLLState = ((RCC->PLL5CFGR1 & RCC_PLL5CFGR1_PLLEN) == RCC_PLL5CFGR1_PLLEN ? RCC_PLL_ON : RCC_PLL_OFF);
  pll_config->PLLSource = (RCC->MUXSELCFGR & RCC_MUXSELCFGR_MUXSEL1_Msk) >> RCC_MUXSELCFGR_MUXSEL1_Pos;
  pll_config->FREFDIV = (RCC->PLL5CFGR2 & RCC_PLL5CFGR2_FREFDIV_Msk) >> RCC_PLL5CFGR2_FREFDIV_Pos;
  pll_config->FBDIV = (RCC->PLL5CFGR2 & RCC_PLL5CFGR2_FBDIV_Msk) >> RCC_PLL5CFGR2_FBDIV_Pos;
  pll_config->FRACIN = (RCC->PLL5CFGR3 & RCC_PLL5CFGR3_FRACIN_Msk) >> RCC_PLL5CFGR3_FRACIN_Pos;
  pll_config->POSTDIV1 = (RCC->PLL5CFGR6 & RCC_PLL5CFGR6_POSTDIV1_Msk) >> RCC_PLL5CFGR6_POSTDIV1_Pos;
  pll_config->POSTDIV2 = (RCC->PLL5CFGR7 & RCC_PLL5CFGR7_POSTDIV2_Msk) >> RCC_PLL5CFGR7_POSTDIV2_Pos;
  pll_config->PLLMode = 0;
  if (LL_RCC_PLL5_CSG_IsEnabled())
  {
    pll_config->PLLMode |= RCC_PLL_SPREAD_SPECTRUM;
  }
}

/**
  * @brief  Retrieve the PLL6 configuration settings
  * @param  pll_config: pointer to an RCC_PLLInitTypeDef structure
  *
  * @retval None
  */
void HAL_RCCEx_GetPLL6Config(RCC_PLLInitTypeDef  *pll_config)
{
  pll_config->id = 6;
  pll_config->PLLState = ((RCC->PLL6CFGR1 & RCC_PLL6CFGR1_PLLEN) == RCC_PLL6CFGR1_PLLEN ? RCC_PLL_ON : RCC_PLL_OFF);
  pll_config->PLLSource = (RCC->MUXSELCFGR & RCC_MUXSELCFGR_MUXSEL2_Msk) >> RCC_MUXSELCFGR_MUXSEL2_Pos;
  pll_config->FREFDIV = (RCC->PLL6CFGR2 & RCC_PLL6CFGR2_FREFDIV_Msk) >> RCC_PLL6CFGR2_FREFDIV_Pos;
  pll_config->FBDIV = (RCC->PLL6CFGR2 & RCC_PLL6CFGR2_FBDIV_Msk) >> RCC_PLL6CFGR2_FBDIV_Pos;
  pll_config->FRACIN = (RCC->PLL6CFGR3 & RCC_PLL6CFGR3_FRACIN_Msk) >> RCC_PLL6CFGR3_FRACIN_Pos;
  pll_config->POSTDIV1 = (RCC->PLL6CFGR6 & RCC_PLL6CFGR6_POSTDIV1_Msk) >> RCC_PLL6CFGR6_POSTDIV1_Pos;
  pll_config->POSTDIV2 = (RCC->PLL6CFGR7 & RCC_PLL6CFGR7_POSTDIV2_Msk) >> RCC_PLL6CFGR7_POSTDIV2_Pos;
  pll_config->PLLMode = 0;
  if (LL_RCC_PLL6_CSG_IsEnabled())
  {
    pll_config->PLLMode |= RCC_PLL_SPREAD_SPECTRUM;
  }
}

/**
  * @brief  Retrieve the PLL7 configuration settings
  * @param  pll_config: pointer to an RCC_PLLInitTypeDef structure
  *
  * @retval None
  */
void HAL_RCCEx_GetPLL7Config(RCC_PLLInitTypeDef  *pll_config)
{
  pll_config->id = 7;
  pll_config->PLLState = ((RCC->PLL7CFGR1 & RCC_PLL7CFGR1_PLLEN) == RCC_PLL7CFGR1_PLLEN ? RCC_PLL_ON : RCC_PLL_OFF);
  pll_config->PLLSource = (RCC->MUXSELCFGR & RCC_MUXSELCFGR_MUXSEL3_Msk) >> RCC_MUXSELCFGR_MUXSEL3_Pos;
  pll_config->FREFDIV = (RCC->PLL7CFGR2 & RCC_PLL7CFGR2_FREFDIV_Msk) >> RCC_PLL7CFGR2_FREFDIV_Pos;
  pll_config->FBDIV = (RCC->PLL7CFGR2 & RCC_PLL7CFGR2_FBDIV_Msk) >> RCC_PLL7CFGR2_FBDIV_Pos;
  pll_config->FRACIN = (RCC->PLL7CFGR3 & RCC_PLL7CFGR3_FRACIN_Msk) >> RCC_PLL7CFGR3_FRACIN_Pos;
  pll_config->POSTDIV1 = (RCC->PLL7CFGR6 & RCC_PLL7CFGR6_POSTDIV1_Msk) >> RCC_PLL7CFGR6_POSTDIV1_Pos;
  pll_config->POSTDIV2 = (RCC->PLL7CFGR7 & RCC_PLL7CFGR7_POSTDIV2_Msk) >> RCC_PLL7CFGR7_POSTDIV2_Pos;
  pll_config->PLLMode = 0;
  if (LL_RCC_PLL7_CSG_IsEnabled())
  {
    pll_config->PLLMode |= RCC_PLL_SPREAD_SPECTRUM;
  }
}

/**
  * @brief  Retrieve the PLL8 configuration settings
  * @param  pll_config: pointer to an RCC_PLLInitTypeDef structure
  *
  * @retval None
  */
void HAL_RCCEx_GetPLL8Config(RCC_PLLInitTypeDef  *pll_config)
{
  pll_config->id = 8;
  pll_config->PLLState = ((RCC->PLL8CFGR1 & RCC_PLL8CFGR1_PLLEN) == RCC_PLL8CFGR1_PLLEN ? RCC_PLL_ON : RCC_PLL_OFF);
  pll_config->PLLSource = (RCC->MUXSELCFGR & RCC_MUXSELCFGR_MUXSEL4_Msk) >> RCC_MUXSELCFGR_MUXSEL4_Pos;
  pll_config->FREFDIV = (RCC->PLL8CFGR2 & RCC_PLL8CFGR2_FREFDIV_Msk) >> RCC_PLL8CFGR2_FREFDIV_Pos;
  pll_config->FBDIV = (RCC->PLL8CFGR2 & RCC_PLL8CFGR2_FBDIV_Msk) >> RCC_PLL8CFGR2_FBDIV_Pos;
  pll_config->FRACIN = (RCC->PLL8CFGR3 & RCC_PLL8CFGR3_FRACIN_Msk) >> RCC_PLL8CFGR3_FRACIN_Pos;
  pll_config->POSTDIV1 = (RCC->PLL8CFGR6 & RCC_PLL8CFGR6_POSTDIV1_Msk) >> RCC_PLL8CFGR6_POSTDIV1_Pos;
  pll_config->POSTDIV2 = (RCC->PLL8CFGR7 & RCC_PLL8CFGR7_POSTDIV2_Msk) >> RCC_PLL8CFGR7_POSTDIV2_Pos;
  pll_config->PLLMode = 0;
  if (LL_RCC_PLL8_CSG_IsEnabled())
  {
    pll_config->PLLMode |= RCC_PLL_SPREAD_SPECTRUM;
  }
}

#if defined(CORE_CA35)
/**
  * @brief  Compute the PLL1 frequency from real settings read from registers
  * @param  none
  *
  * @retval pll1 frequency
  */
uint32_t HAL_RCCEx_CA35SS_GetPLL1ClockFreq(void)
{
  RCC_PLLInitTypeDef  pll;
  uint32_t source_freq;
  float pll_vco;
  float pll_output;

  HAL_RCCEx_CA35SS_GetPLL1Config(&pll);

  /* pll freq = ((source * FBDIV) / (REFDIV)) / (postdiv1 * postdiv2)*/

#if defined(USE_STM32MP257CXX_FPGA)
  source_freq = 32000000UL;
#else /* USE_STM32MP257CXX_FPGA */
  switch (pll.PLLSource)
  {
    case RCC_PLLSOURCE_HSI:
      source_freq = HSI_VALUE;
      break;
    case RCC_PLLSOURCE_HSE:
      source_freq = HSE_VALUE;
      break;
    case RCC_PLLSOURCE_MSI:
      source_freq = MSI_VALUE;
      break;
    default:
      source_freq = 0UL;
      break;
  }
#endif /* else USE_STM32MP257CXX_FPGA */

  pll_vco = (source_freq * pll.FBDIV) / pll.FREFDIV;
  pll_output = pll_vco / (pll.POSTDIV1 * pll.POSTDIV2);

  return (uint32_t)pll_output;
}
#endif /* CORE_CA35 */

/**
  * @brief  Compute the PLL2 frequency from real settings read from registers
  * @param  none
  *
  * @retval pll2 frequency
  */
uint32_t HAL_RCCEx_GetPLL2ClockFreq(void)
{
  RCC_PLLInitTypeDef  pll;
  uint32_t source_freq;
  float pll_vco;
  float pll_output;

  HAL_RCCEx_GetPLL2Config(&pll);

  /* pll freq = ((source * FBDIV) / (FREFDIV)) / (postdiv1 * postdiv2)*/
  /* Integer mode as first step; fractional is next */

#if defined(USE_STM32MP257CXX_FPGA)
  source_freq = 32000000UL;
#else /* USE_STM32MP257CXX_FPGA */
  switch (pll.PLLSource)
  {
    case RCC_PLLSOURCE_HSI:
      source_freq = HSI_VALUE;
      break;
    case RCC_PLLSOURCE_HSE:
      source_freq = HSE_VALUE;
      break;
    case RCC_PLLSOURCE_MSI:
      source_freq = MSI_VALUE;
      break;
    default:
      source_freq = 0;
      break;
  }
#endif /* else USE_STM32MP257CXX_FPGA */

  pll_vco = (source_freq * pll.FBDIV) / pll.FREFDIV;
  pll_output = pll_vco / (pll.POSTDIV1 * pll.POSTDIV2);

  return (uint32_t)pll_output;
}

/**
  * @brief  Compute the PLL3 frequency from real settings read from registers
  * @param  none
  *
  * @retval pll3 frequency
  */
uint32_t HAL_RCCEx_GetPLL3ClockFreq(void)
{
  RCC_PLLInitTypeDef  pll;
  uint32_t source_freq;
  float pll_vco;
  float pll_output;

  HAL_RCCEx_GetPLL3Config(&pll);

  /* pll freq = ((source * FBDIV) / (FREFDIV)) / (postdiv1 * postdiv2)*/
  /* Integer mode as first step; fractional is next */

#if defined(USE_STM32MP257CXX_FPGA)
  source_freq = 32000000UL;
#else /* USE_STM32MP257CXX_FPGA */
  switch (pll.PLLSource)
  {
    case RCC_PLLSOURCE_HSI:
      source_freq = HSI_VALUE;
      break;
    case RCC_PLLSOURCE_HSE:
      source_freq = HSE_VALUE;
      break;
    case RCC_PLLSOURCE_MSI:
      source_freq = MSI_VALUE;
      break;
    default:
      source_freq = 0;
      break;
  }
#endif /* else USE_STM32MP257CXX_FPGA */

  pll_vco = (source_freq * pll.FBDIV) / pll.FREFDIV;
  pll_output = pll_vco / (pll.POSTDIV1 * pll.POSTDIV2);

  return (uint32_t)pll_output;
}

/**
  * @brief  Compute the PLL4 frequency from real settings read from registers
  * @param  none
  *
  * @retval pll4 frequency
  */
uint32_t HAL_RCCEx_GetPLL4ClockFreq(void)
{
  RCC_PLLInitTypeDef  pll;
  uint32_t source_freq;
  float pll_vco;
  float pll_output;

  HAL_RCCEx_GetPLL4Config(&pll);

  /* pll freq = ((source * FBDIV) / (FREFDIV)) / (postdiv1 * postdiv2)*/
  /* Integer mode as first step; fractional is next */

#if defined(USE_STM32MP257CXX_FPGA)
  source_freq = 32000000UL;
#else /* USE_STM32MP257CXX_FPGA */
  switch (pll.PLLSource)
  {
    case RCC_PLLSOURCE_HSI:
      source_freq = HSI_VALUE;
      break;
    case RCC_PLLSOURCE_HSE:
      source_freq = HSE_VALUE;
      break;
    case RCC_PLLSOURCE_MSI:
      source_freq = MSI_VALUE;
      break;
    default:
      source_freq = 0;
      break;
  }
#endif /* else USE_STM32MP257CXX_FPGA */

  pll_vco = (source_freq * pll.FBDIV) / pll.FREFDIV;
  pll_output = pll_vco / (pll.POSTDIV1 * pll.POSTDIV2);

  return (uint32_t)pll_output;
}

/**
  * @brief  Compute the PLL5 frequency from real settings read from registers
  * @param  none
  *
  * @retval pll5 frequency
  */
uint32_t HAL_RCCEx_GetPLL5ClockFreq(void)
{
  RCC_PLLInitTypeDef  pll;
  uint32_t source_freq;
  float pll_vco;
  float pll_output;

  HAL_RCCEx_GetPLL5Config(&pll);

  /* pll freq = ((source * FBDIV) / (FREFDIV)) / (postdiv1 * postdiv2)*/

#if defined(USE_STM32MP257CXX_FPGA)
  source_freq = 32000000UL;
#else /* USE_STM32MP257CXX_FPGA */
  switch (pll.PLLSource)
  {
    case RCC_PLLSOURCE_HSI:
      source_freq = HSI_VALUE;
      break;
    case RCC_PLLSOURCE_HSE:
      source_freq = HSE_VALUE;
      break;
    case RCC_PLLSOURCE_MSI:
      source_freq = MSI_VALUE;
      break;
    default:
      source_freq = 0;
      break;
  }
#endif /* else USE_STM32MP257CXX_FPGA */

  pll_vco = (source_freq * pll.FBDIV) / pll.FREFDIV;
  pll_output = pll_vco / (pll.POSTDIV1 * pll.POSTDIV2);

  return (uint32_t)pll_output;
}

/**
  * @brief  Compute the PLL6 frequency from real settings read from registers
  * @param  none
  *
  * @retval pll6 frequency
  */
uint32_t HAL_RCCEx_GetPLL6ClockFreq(void)
{
  RCC_PLLInitTypeDef  pll;
  uint32_t source_freq;
  float pll_vco;
  float pll_output;

  HAL_RCCEx_GetPLL6Config(&pll);

  /* pll freq = ((source * FBDIV) / (FREFDIV)) / (postdiv1 * postdiv2)*/

#if defined(USE_STM32MP257CXX_FPGA)
  source_freq = 32000000UL;
#else /* USE_STM32MP257CXX_FPGA */
  switch (pll.PLLSource)
  {
    case RCC_PLLSOURCE_HSI:
      source_freq = HSI_VALUE;
      break;
    case RCC_PLLSOURCE_HSE:
      source_freq = HSE_VALUE;
      break;
    case RCC_PLLSOURCE_MSI:
      source_freq = MSI_VALUE;
      break;
    default:
      source_freq = 0;
      break;
  }
#endif /* else USE_STM32MP257CXX_FPGA */

  pll_vco = (source_freq * pll.FBDIV) / pll.FREFDIV;
  pll_output = pll_vco / (pll.POSTDIV1 * pll.POSTDIV2);

  return (uint32_t)pll_output;
}

/**
  * @brief  Compute the PLL7 frequency from real settings read from registers
  * @param  none
  *
  * @retval pll7 frequency
  */
uint32_t HAL_RCCEx_GetPLL7ClockFreq(void)
{
  RCC_PLLInitTypeDef  pll;
  uint32_t source_freq;
  float pll_vco;
  float pll_output;

  HAL_RCCEx_GetPLL7Config(&pll);

  /* pll freq = ((source * FBDIV) / (FREFDIV)) / (postdiv1 * postdiv2)*/

#if defined(USE_STM32MP257CXX_FPGA)
  source_freq = 32000000UL;
#else /* USE_STM32MP257CXX_FPGA */
  switch (pll.PLLSource)
  {
    case RCC_PLLSOURCE_HSI:
      source_freq = HSI_VALUE;
      break;
    case RCC_PLLSOURCE_HSE:
      source_freq = HSE_VALUE;
      break;
    case RCC_PLLSOURCE_MSI:
      source_freq = MSI_VALUE;
      break;
    default:
      source_freq = 0;
      break;
  }
#endif /* else USE_STM32MP257CXX_FPGA */

  pll_vco = (source_freq * pll.FBDIV) / pll.FREFDIV;
  pll_output = pll_vco / (pll.POSTDIV1 * pll.POSTDIV2);

  return (uint32_t)pll_output;
}

/**
  * @brief  Compute the PLL8 frequency from real settings read from registers
  * @param  none
  *
  * @retval pll8 frequency
  */
uint32_t HAL_RCCEx_GetPLL8ClockFreq(void)
{
  RCC_PLLInitTypeDef  pll;
  uint32_t source_freq;
  float pll_vco;
  float pll_output;

  HAL_RCCEx_GetPLL8Config(&pll);

  /* pll freq = ((source * FBDIV) / (FREFDIV)) / (postdiv1 * postdiv2)*/

#if defined(USE_STM32MP257CXX_FPGA)
  source_freq = 32000000UL;
#else /* USE_STM32MP257CXX_FPGA */
  switch (pll.PLLSource)
  {
    case RCC_PLLSOURCE_HSI:
      source_freq = HSI_VALUE;
      break;
    case RCC_PLLSOURCE_HSE:
      source_freq = HSE_VALUE;
      break;
    case RCC_PLLSOURCE_MSI:
      source_freq = MSI_VALUE;
      break;
    default:
      source_freq = 0;
      break;
  }
#endif /* else USE_STM32MP257CXX_FPGA */

  pll_vco = (source_freq * pll.FBDIV) / pll.FREFDIV;
  pll_output = pll_vco / (pll.POSTDIV1 * pll.POSTDIV2);

  return (uint32_t)pll_output;
}

/**
  * @brief  Return the peripheral clock frequency for peripherals
  * @note   Return 0 if peripheral clock identifier not managed by this API
  * @param  PeriphClk  Peripheral clock identifier
  *         This parameter can be one of the following values and needs to be
  *         a single selection of one the following defines:
  *            @arg @ref RCC_PERIPHCLK_LPTIM1_2
  *            @arg @ref RCC_PERIPHCLK_UART2_4
  *            @arg @ref RCC_PERIPHCLK_UART3_5
  *            @arg @ref RCC_PERIPHCLK_SPI2_3
  *            @arg @ref RCC_PERIPHCLK_SPDIFRX
  *            @arg @ref RCC_PERIPHCLK_I2C1_2
  *            @arg @ref RCC_PERIPHCLK_I3C1_2
  *            @arg @ref RCC_PERIPHCLK_I2C3_5
  *            @arg @ref RCC_PERIPHCLK_I3C3
  *            @arg @ref RCC_PERIPHCLK_I2C4_6
  *            @arg @ref RCC_PERIPHCLK_I2C7
  *            @arg @ref RCC_PERIPHCLK_SPI1
  *            @arg @ref RCC_PERIPHCLK_SPI4_5
  *            @arg @ref RCC_PERIPHCLK_SPI6_7
  *            @arg @ref RCC_PERIPHCLK_USART1
  *            @arg @ref RCC_PERIPHCLK_USART6
  *            @arg @ref RCC_PERIPHCLK_UART7_8
  *            @arg @ref RCC_PERIPHCLK_UART9
  *            @arg @ref RCC_PERIPHCLK_SAI1_MDF1
  *            @arg @ref RCC_PERIPHCLK_SAI2
  *            @arg @ref RCC_PERIPHCLK_SAI3_4
  *            @arg @ref RCC_PERIPHCLK_FDCAN
  *            @arg @ref RCC_PERIPHCLK_LTDC
  *            @arg @ref RCC_PERIPHCLK_DSIPHY
  *            @arg @ref RCC_PERIPHCLK_DCMIPP
  *            @arg @ref RCC_PERIPHCLK_CSITXESC
  *            @arg @ref RCC_PERIPHCLK_CSIPHY
  *            @arg @ref RCC_PERIPHCLK_LVDSPHY
  *            @arg @ref RCC_PERIPHCLK_STGEN
  *            @arg @ref RCC_PERIPHCLK_USB3PCIEPHY
  *            @arg @ref RCC_PERIPHCLK_USBTC
  *            @arg @ref RCC_PERIPHCLK_I3C4
  *            @arg @ref RCC_PERIPHCLK_SPI8
  *            @arg @ref RCC_PERIPHCLK_I2C8
  *            @arg @ref RCC_PERIPHCLK_LPUART1
  *            @arg @ref RCC_PERIPHCLK_LPTIM3
  *            @arg @ref RCC_PERIPHCLK_LPTIM4_5
  *            @arg @ref RCC_PERIPHCLK_ADF1
  *            @arg @ref RCC_PERIPHCLK_TSDBG
  *            @arg @ref RCC_PERIPHCLK_TPIU
  *            @arg @ref RCC_PERIPHCLK_ATB
  *            @arg @ref RCC_PERIPHCLK_ADC12
  *            @arg @ref RCC_PERIPHCLK_ADC3
  *            @arg @ref RCC_PERIPHCLK_OSPI1
  *            @arg @ref RCC_PERIPHCLK_OSPI2
  *            @arg @ref RCC_PERIPHCLK_FMC
  *            @arg @ref RCC_PERIPHCLK_SDMMC1
  *            @arg @ref RCC_PERIPHCLK_SDMMC2
  *            @arg @ref RCC_PERIPHCLK_SDMMC3
  *            @arg @ref RCC_PERIPHCLK_ETH1_ETHSW
  *            @arg @ref RCC_PERIPHCLK_ETH2
  *            @arg @ref RCC_PERIPHCLK_ETH1PTP_ETH2PTP
  *            @arg @ref RCC_PERIPHCLK_USB2PHY1
  *            @arg @ref RCC_PERIPHCLK_USB2PHY2
  *            @arg @ref RCC_PERIPHCLK_ICN_M_GPU
  *            @arg @ref RCC_PERIPHCLK_ETHSWREF
  *            @arg @ref RCC_PERIPHCLK_MCO1
  *            @arg @ref RCC_PERIPHCLK_MCO2
  *            @arg @ref RCC_PERIPHCLK_CPU1_EXT2F
  * @retval Frequency in Hz
  */
uint32_t HAL_RCCEx_GetPeriphCLKFreq(uint32_t PeriphClk)
{
  uint32_t xbar_source;
  uint32_t xbar_source_freq;
  uint32_t periph_freq;

  /* Identify xbar source */
  xbar_source = RCC->XBARxCFGR[PeriphClk] & RCC_XBARxCFGR_XBARxSEL_Msk;

  /* Then retrieve the source frequency */
  switch (xbar_source)
  {
    case RCC_XBAR_CLKSRC_PLL4:
      xbar_source_freq = HAL_RCCEx_GetPLL4ClockFreq();
      break;
    case RCC_XBAR_CLKSRC_PLL5:
      xbar_source_freq = HAL_RCCEx_GetPLL5ClockFreq();
      break;
    case RCC_XBAR_CLKSRC_PLL6:
      xbar_source_freq = HAL_RCCEx_GetPLL6ClockFreq();
      break;
    case RCC_XBAR_CLKSRC_PLL7:
      xbar_source_freq = HAL_RCCEx_GetPLL7ClockFreq();
      break;
    case RCC_XBAR_CLKSRC_PLL8:
      xbar_source_freq = HAL_RCCEx_GetPLL8ClockFreq();
      break;
    case RCC_XBAR_CLKSRC_HSI:
    case RCC_XBAR_CLKSRC_HSI_KER:
      xbar_source_freq = HSI_VALUE;
      break;
    case RCC_XBAR_CLKSRC_MSI:
    case RCC_XBAR_CLKSRC_MSI_KER:
      xbar_source_freq = MSI_VALUE;
      break;
    case RCC_XBAR_CLKSRC_SPDIF:
      xbar_source_freq = 0;
      break;
    case RCC_XBAR_CLKSRC_I2S:
      xbar_source_freq = EXTERNAL_CLOCK_VALUE;
      break;
    case RCC_XBAR_CLKSRC_HSE:
    case RCC_XBAR_CLKSRC_HSE_KER:
      xbar_source_freq = HSE_VALUE;
      break;
    case RCC_XBAR_CLKSRC_LSI:
      xbar_source_freq = LSI_VALUE;
      break;
    case RCC_XBAR_CLKSRC_LSE:
      xbar_source_freq = LSE_VALUE;
      break;
    default:
      xbar_source_freq = 0;
      break;
  }

  /* Then apply the fgen dividers to calculate exact periph frequency */
  periph_freq = xbar_source_freq / (((RCC->FINDIVxCFGR[PeriphClk] & RCC_FINDIVxCFGR_FINDIVx_Msk) + 1) * ((RCC->PREDIVxCFGR[PeriphClk] & RCC_PREDIVxCFGR_PREDIVx_Msk) + 1));

  return (periph_freq);
}

/**
  * @brief  Return the timer clock frequency
  * @param  TimerId  Timer clock identifier
  *         This parameter can be one of the following values and needs to be
  *         a single selection of one the following defines:
  *            @arg @ref RCC_PERIPHCLK_TIM1
  *            @arg @ref RCC_PERIPHCLK_TIM2
  *            @arg @ref RCC_PERIPHCLK_TIM3
  *            @arg @ref RCC_PERIPHCLK_TIM4
  *            @arg @ref RCC_PERIPHCLK_TIM5
  *            @arg @ref RCC_PERIPHCLK_TIM6
  *            @arg @ref RCC_PERIPHCLK_TIM7
  *            @arg @ref RCC_PERIPHCLK_TIM8
  *            @arg @ref RCC_PERIPHCLK_TIM10
  *            @arg @ref RCC_PERIPHCLK_TIM11
  *            @arg @ref RCC_PERIPHCLK_TIM12
  *            @arg @ref RCC_PERIPHCLK_TIM13
  *            @arg @ref RCC_PERIPHCLK_TIM14
  *            @arg @ref RCC_PERIPHCLK_TIM15
  *            @arg @ref RCC_PERIPHCLK_TIM16
  *            @arg @ref RCC_PERIPHCLK_TIM17
  *            @arg @ref RCC_PERIPHCLK_TIM20
  * @retval Frequency in Hz
  */
uint32_t HAL_RCCEx_GetTimerCLKFreq(uint32_t TimerId)
{
  uint32_t apbdiv;
  uint32_t timprer;
  uint32_t timfreq = 0;
  uint32_t icn_ls_mcu;

  icn_ls_mcu = HAL_RCC_GetFreq(RCC_CLOCKTYPE_ICN_LS_MCU);

  switch (TimerId)
  {
    case RCC_PERIPHCLK_TIM2:
    case RCC_PERIPHCLK_TIM3:
    case RCC_PERIPHCLK_TIM4:
    case RCC_PERIPHCLK_TIM5:
    case RCC_PERIPHCLK_TIM6:
    case RCC_PERIPHCLK_TIM7:
    case RCC_PERIPHCLK_TIM10:
    case RCC_PERIPHCLK_TIM11:
    case RCC_PERIPHCLK_TIM12:
    case RCC_PERIPHCLK_TIM13:
    case RCC_PERIPHCLK_TIM14:
      apbdiv = LL_RCC_GetAPB1Prescaler();
      timprer = RCC->TIMG1PRER & RCC_TIMG1PRER_TIMG1PRE;
      break;
    case RCC_PERIPHCLK_TIM1:
    case RCC_PERIPHCLK_TIM8:
    case RCC_PERIPHCLK_TIM15:
    case RCC_PERIPHCLK_TIM16:
    case RCC_PERIPHCLK_TIM17:
    case RCC_PERIPHCLK_TIM20:
      apbdiv = LL_RCC_GetAPB2Prescaler();
      timprer = RCC->TIMG2PRER & RCC_TIMG2PRER_TIMG2PRE;
      break;
    default:
      apbdiv = 0UL;
      timprer = 0UL;
      break;
  }

  switch (apbdiv)
  {
    case LL_RCC_APB1_DIV_1:
    case LL_RCC_APB1_DIV_2:
      timfreq = icn_ls_mcu;
      break;
    case LL_RCC_APB1_DIV_4:
      if (timprer == 0)
      {
        timfreq = icn_ls_mcu / 2;
      }
      else
      {
        timfreq = icn_ls_mcu;
      }
      break;
    case LL_RCC_APB1_DIV_8:
      if (timprer == 0)
      {
        timfreq = icn_ls_mcu / 4;
      }
      else
      {
        timfreq = icn_ls_mcu / 2;
      }
      break;
    case LL_RCC_APB1_DIV_16:
      if (timprer == 0)
      {
        timfreq = icn_ls_mcu / 8;
      }
      else
      {
        timfreq = icn_ls_mcu / 4;
      }
      break;
    default:
      timfreq = 0;
      break;
  }

  return timfreq;
}

/**
  * @brief  Set the D3 clock source
  * @param  source: identifier of the requested source among:
  *         RCC_D3SOURCE_MSI
  *         RCC_D3SOURCE_LSI
  *         RCC_D3SOURCE_LSE
  *
  * @retval
  */
HAL_StatusTypeDef HAL_RCCEx_SetD3ClockSource(uint32_t source)
{
  assert_param(IS_RCC_D3_SOURCE(source));

  MODIFY_REG(RCC->D3DCR, RCC_D3DCR_D3PERCKSEL_Msk, source);

  return HAL_OK;
}

/**
  * @brief  Get the D3 clock source
  * @param  None
  *
  * @retval Clock source identifier among:
  *         RCC_D3SOURCE_MSI
  *         RCC_D3SOURCE_LSI
  *         RCC_D3SOURCE_LSE
  */
uint32_t HAL_RCCEx_GetD3ClockSource(void)
{
  return (RCC->D3DCR & RCC_D3DCR_D3PERCKSEL_Msk);
}

/**
  * @}
  */

#ifdef CORE_CA35
/**
  * @brief  Control the enable boot function when the system exits from STANDBY
  * @param  RCC_BootCx: Boot Core to be enabled (set to 1)
  *         This parameter can be one (or both) of the following values:
  *            @arg RCC_BOOT_C1: CA35 core selection
  *            @arg RCC_BOOT_C2: CM33 core selection
  *
  * @note   Next combinations are possible:
  *           RCC_BOOT_C1   RCC_BOOT_C2  Expected result
  *               0              0       MPU boots, MCU does not boot
  *               0              1       Only MCU boots
  *               1              0       Only MPU boots
  *               1              1       MPU and MCU boot
  *
  * @note   This function is reset when a system reset occurs, but not when the
  *         circuit exits from STANDBY (rst_app reset)
  * @note   This function can only be called by the CA35
  * @retval None
  */
void HAL_RCCEx_EnableBootCore(uint32_t RCC_BootCx)
{
  assert_param(IS_RCC_BOOT_CORE(RCC_BootCx));
  SET_BIT(RCC->STBYBOOTCR, RCC_BootCx);
}

/**
  * @brief  Control the disable boot function when the system exits from STANDBY
  * @param  RCC_BootCx: Boot Core to be disabled (set to 0)
  *         This parameter can be one (or both) of the following values:
  *            @arg RCC_BOOT_C1: CA35 core selection
  *            @arg RCC_BOOT_C2: CM33 core selection
  *
  * @note   Next combinations are possible:
  *           RCC_BOOT_C1   RCC_BOOT_C2  Expected result
  *               0              0       MPU boots, MCU does not boot
  *               0              1       Only MCU boots
  *               1              0       Only MPU boots
  *               1              1       MPU and MCU boot
  *
  * @note   This function is reset when a system reset occurs, but not when the
  *         circuit exits from STANDBY (rst_app reset)
  * @note   This function can only be called by the CA35
  * @retval None
  */
void HAL_RCCEx_DisableBootCore(uint32_t RCC_BootCx)
{
  assert_param(IS_RCC_BOOT_CORE(RCC_BootCx));
  CLEAR_BIT(RCC->STBYBOOTCR, RCC_BootCx);
}

/**
  * @brief  The MCU will be set in HOLD_BOOT when the next MCU core reset occurs
  * @retval None
  */
void HAL_RCCEx_HoldBootMCU(void)
{
  CLEAR_BIT(RCC->CPUBOOTCR, RCC_CPUBOOTCR_BOOT_CPU2);
}

/**
  * @brief  The MCU will not be in HOLD_BOOT mode when the next MCU core reset occurs.
  * @note   If the MCU was in HOLD_BOOT it will cause the MCU to boot.
  * @retval None
  */
void HAL_RCCEx_BootMCU(void)
{
  SET_BIT(RCC->CPUBOOTCR, RCC_CPUBOOTCR_BOOT_CPU2);
}

#endif /* CORE_CA35 */

/** @defgroup RCCEx_Exported_Functions_Group2 Extended Clock management functions
  *  @brief  Extended Clock management functions
  *
@verbatim
 ===============================================================================
                ##### Extended clock management functions  #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to control the
    CSS feature and the frequency measurement service
    Low speed clock output and clock after wake-up from STOP mode.
@endverbatim
  * @{
  */

/**
  * @brief  Measures the requested clock frequency
  * @note   This is a debug service - not to be used at runtime
  *         because the execution time is not easily predictible
  * @param  clk_id is the clock to be measured. Must follow CKINTSEL definition of refman
  *         ref_id is the reference clock. Must follow FCALCREFCKSEL definition of refman
  *         freq contains the result of the measurement
  * @retval fstatus
  */
int HAL_RCCEx_MeasureClockFreq(uint32_t clk_id, uint32_t ref_id, uint32_t *freq)
{
  int fstatus = 0;
  uint32_t freq_meas_complete_flag, fcalctwc_val, min_fcalctwc;
  uint32_t regval, fcalcsts_bit, ckintsel;
  uint32_t timeout_flag, timeout_in_ticks, tickstart;
  uint64_t ckin_freq_val;
  uint32_t ref_freq;
  uint32_t gfg, source = 0;

  gfg = RCC->MUXSELCFGR;

  /* Find reference clock frequency */
  switch (ref_id)
  {
    case PLL4:
      source = ((gfg & RCC_MUXSELCFGR_MUXSEL0_Msk) >> RCC_MUXSELCFGR_MUXSEL0_Pos);
      break;
    case PLL5:
      source = ((gfg & RCC_MUXSELCFGR_MUXSEL1_Msk) >> RCC_MUXSELCFGR_MUXSEL1_Pos);
      break;
    case PLL6:
      source = ((gfg & RCC_MUXSELCFGR_MUXSEL2_Msk) >> RCC_MUXSELCFGR_MUXSEL2_Pos);
      break;
    case PLL7:
      source = ((gfg & RCC_MUXSELCFGR_MUXSEL3_Msk) >> RCC_MUXSELCFGR_MUXSEL3_Pos);
      break;
    case PLL8:
      source = ((gfg & RCC_MUXSELCFGR_MUXSEL4_Msk) >> RCC_MUXSELCFGR_MUXSEL4_Pos);
      break;
    default:
      return 1;
  }

  if (source == 0)
  {
    ref_freq = HSI_VALUE;
  }
  else if (source == 1)
  {
    ref_freq = HSE_VALUE;
  }
  else if (source == 2)
  {
    ref_freq = MSI_VALUE;
  }
  else
  {
    ref_freq = 0;
  }

  freq_meas_complete_flag = 0;
  fcalctwc_val = 0xF;
  min_fcalctwc = 0x1;
  *freq = 0;

  while ((fcalctwc_val >= min_fcalctwc)
         && (freq_meas_complete_flag == 0))
  {

    /* set the FCALCRSTN bit in the RCC Clock Frequency Calculator and Observation 1
     * clock Configuration Register (RCC_FCALCOBS1CFGR) to enable the clock frequency
     * calculator.
     */
    SET_BIT(RCC->FCALCOBS1CFGR, (1 << 27));

    /* set the CKINTSEL (or CKEXTSEL if FCALCCKEXTSEL bit is set) field in the RCC
     * Clock Frequency Calculator and Observation 0 Clock Configuration Register
     * (RCC_FCALCOBS0CFGR) to select the clock frequency calculator input clock
     * (i.e. ckin).
     */
    ckintsel = clk_id;
    regval = RCC->FCALCOBS0CFGR;
    regval &= ~(0xFF);
    regval |= ckintsel;
    WRITE_REG(RCC->FCALCOBS0CFGR, regval);

    /* set the FCALCCKEN field in the RCC Clock Frequency Calculator and Observation 0
     * Clock Configuration Register (RCC_FCALCOBS0CFGR) to enable the selected input
     * clock at the input of clock frequency calculator.
     */
    regval = RCC->FCALCOBS0CFGR;
    regval |= (1 << 25);
    WRITE_REG(RCC->FCALCOBS0CFGR, regval);

    /* set the FCALCREFCKSEL field in the RCC Clock Frequency Calculator Reference
     * clock Configuration Register (RCC_FCALCREFCFGR) to select the reference clock
     * (i.e. ckref).
     */
    WRITE_REG(RCC->FCALCREFCFGR, 0); // OBI: 0 corresponds to PLL4 input as ref clock; make sure it is 'on'.

    /* set the clock frequency time window value via the FCALCTWC field in the RCC Clock
     * Frequency Calculator Control Register 2 (RCC_FCALCCR2)
     */
    regval = RCC->FCALCCR2;
    regval &= ~(0xF << 11);
    regval |= (fcalctwc_val << 11);
    WRITE_REG(RCC->FCALCCR2, regval);

    /* set the FCALCTYP field to 0x0C in the RCC Clock Frequency Calculator Control
     * Register 2 (RCC_FCALCCR2) to select the frequency value type.
     */
    regval = RCC->FCALCCR2;
    regval |= (0xC << 17);
    WRITE_REG(RCC->FCALCCR2, regval);

    /* select the application mode via the FCALCMD field in the RCC Clock Frequency
     * Calculator Control Register 2 (RCC_FCALCCR2).
     */
    regval = RCC->FCALCCR2;
    regval |= (0x3 << 3);
    WRITE_REG(RCC->FCALCCR2, regval);

    /* set the FCALCRUN bit in the RCC Clock Frequency Calculator Control Register 1
     * (RCC_FCALCCR1).
     */
    WRITE_REG(RCC->FCALCCR1, 1);

    /* poll the FCALCSTS bit in the RCC Clock Frequency Calculator Status Register
     * (RCC_FCALCSR) until the calculation is done.
     */
    fcalcsts_bit = 0;
    timeout_flag = 0;

    /* timeout value (in ms) to be tuned according */
    /* to lowest frequency to be measured */
    timeout_in_ticks = 100;

    tickstart = HAL_GetTick();
    while ((fcalcsts_bit == 0)
           && (timeout_flag == 0))
    {
      regval = RCC->FCALCSR;
      if ((regval & 0xFFFF) != 0)
      {
        fcalcsts_bit = (regval & (1 << 19)) >> 19;
      }
      /* detect timeout (time expired whereas counter still zero) */
      else if (HAL_GetTick() - tickstart > timeout_in_ticks)
      {
        timeout_flag = 1;
      }
    }

    /* read the measured value via the FVAL field in the RCC Clock Frequency Calculator
     * Status Register (RCC_FCALCSR). If FVAL[16] is 1 then the measured value is false.
     */
    /* detect overflow */
    if ((regval & (1 << 16)) == (1 << 16))
    {
      /* set status for next measurement iteration */
      fstatus = 0;
    }
    else if (1 == timeout_flag)
    {
      /* set status for leaving measurement algorithm */
      fstatus = -1;
    }
    else
    {
      if (0 == (regval & 0xFFFF))
      {
        /* set status for leaving measurement algorithm */
        fstatus = -1;
      }
      else
      {
        /* compute measured frequency */
        ckin_freq_val = (uint64_t)((regval & 0xFFFF) - 16) * (uint64_t)ref_freq;
        uint32_t denom_val = (1 << (fcalctwc_val + 1)) + 5;
        ckin_freq_val /= (uint64_t)denom_val;
        *freq = (uint32_t)ckin_freq_val;
        freq_meas_complete_flag = 1;
        fstatus = 0;
      }
    }

    /* Re-initialisation : */
    /* clear the FCALCRUN bit in the RCC Clock Frequency Calculator
     * Control Register 1 (RCC_FCALCCR1)
     */
    WRITE_REG(RCC->FCALCCR1, 0);

    /* clear the FCALCRSTN bit in the RCC Clock Frequency Calculator and Observation 1
     * clock Configuration Register (RCC_FCALCOBS1CFGR) to disable the clock frequency
     * calculator.
     */
    CLEAR_BIT(RCC->FCALCOBS1CFGR, (1 << 27));

    /* Waiting for status register to be cleared : NOT SPECIFIED */
    timeout_flag = 0;
    timeout_in_ticks = 100; /* TO BE TUNED */
    tickstart = HAL_GetTick();
    regval = READ_REG(RCC->FCALCSR);

    while (((regval & 0xffff) != 0)
           && (timeout_flag == 0))
    {
      regval = READ_REG(RCC->FCALCSR);
      /* detect timeout */
      if (HAL_GetTick() - tickstart > timeout_in_ticks)
      {
        timeout_flag = 1;
      }
    }

    if (1 == timeout_flag)
    {
      fstatus = -1;
    }

    /* decrease FCALCTWC value */
    fcalctwc_val -= 1;

    /* check status */
    if (-1 == fstatus)
    {
      break;
    }
  }

  return fstatus;
}
/**
  * @brief  Enable the LSE Clock Security System.
  * @note   Prior to enable the LSE Clock Security System, LSE oscillator is to be enabled
  *         with HAL_RCC_OscConfig() and the LSE oscillator clock is to be selected as RTC
  *         clock with HAL_RCCEx_PeriphCLKConfig().
  * @retval Status
  */
HAL_StatusTypeDef HAL_RCCEx_EnableLSECSS(void)
{
  uint32_t tickstart;
  uint32_t backup_domain;

  if (HAL_IS_BIT_CLR(PWR->BDCR1, PWR_BDCR1_DBD3P))
  {
    backup_domain = 1;

    /* Enable write access to Backup domain */
    SET_BIT(PWR->BDCR1, PWR_BDCR1_DBD3P);

    /* Wait for Backup domain Write protection disable */
    tickstart = HAL_GetTick();

    while ((PWR->BDCR1 & PWR_BDCR1_DBD3P) == RESET)
    {
      if ((HAL_GetTick() - tickstart) > DBP_TIMEOUT_VALUE)
      {
        return HAL_TIMEOUT;
      }
    }
  }
  else
  {
    backup_domain = 0;
  }

  LL_RCC_LSE_EnableCSS();

  if (backup_domain == 1)
  {
    CLEAR_BIT(PWR->BDCR1, PWR_BDCR1_DBD3P);
  }

  return HAL_OK;
}

/**
  * @brief  Disable the LSE Clock Security System.
  * @note   LSE Clock Security System can only be disabled after a LSE failure detection.
  * @retval None
  */
HAL_StatusTypeDef HAL_RCCEx_DisableLSECSS(void)
{
  uint32_t tickstart;
  uint32_t backup_domain;

  if (HAL_IS_BIT_CLR(PWR->BDCR1, PWR_BDCR1_DBD3P))
  {
    backup_domain = 1;

    /* Enable write access to Backup domain */
    SET_BIT(PWR->BDCR1, PWR_BDCR1_DBD3P);

    /* Wait for Backup domain Write protection disable */
    tickstart = HAL_GetTick();

    while ((PWR->BDCR1 & PWR_BDCR1_DBD3P) == RESET)
    {
      if ((HAL_GetTick() - tickstart) > DBP_TIMEOUT_VALUE)
      {
        return HAL_TIMEOUT;
      }
    }
  }
  else
  {
    backup_domain = 0;
  }

  LL_RCC_LSE_DisableCSS();

  if (backup_domain == 1)
  {
    CLEAR_BIT(PWR->BDCR1, PWR_BDCR1_DBD3P);
  }

  return HAL_OK;
}

/**
  * @brief  Enable the HSE Clock Security System.
  * @note   Prior to enable the HSE Clock Security System, HSE oscillator is to be enabled
  *         with HAL_RCC_OscConfig() and the HSE oscillator clock is to be selected as RTC
  *         clock with HAL_RCCEx_PeriphCLKConfig().
  * @retval None
  */
void HAL_RCCEx_EnableHSECSS(void)
{
  LL_RCC_HSE_EnableCSS();
}

/**
  * @brief Handle the RCC LSE Clock Security System interrupt request.
  * @retval None
  */
void HAL_RCCEx_LSECSS_IRQHandler(void)
{
  if (READ_BIT(RCC->BDCR, RCC_BDCR_LSECSSD) != 0U)
  {
    /* RCC LSE Clock Security System interrupt user callback */
    HAL_RCCEx_LSECSS_Callback();
  }
}

/**
  * @brief  RCCEx LSE Clock Security System interrupt callback.
  * @retval none
  */
__weak void HAL_RCCEx_LSECSS_Callback(void)
{
  /* NOTE : This function should not be modified, when the callback is needed,
            the @ref HAL_RCCEx_LSECSS_Callback should be implemented in the user file
   */
}

/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup RCCEx_Private_Functions
  * @{
  */

/**
  * @}
  */

#endif /* HAL_RCC_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
