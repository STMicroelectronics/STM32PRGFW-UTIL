/**
  ******************************************************************************
  * @file    stm32mp25_hal_rcc_ex.h
  * @author  MCD Application Team
  * @brief   Header file of RCC HAL Extended  module.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32MP2xx_HAL_RCC_EX_H
#define STM32MP2xx_HAL_RCC_EX_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32mp2xx_hal_def.h"

/** @addtogroup STM32MP2xx_HAL_Driver
  * @{
  */

/** @addtogroup RCCEx
  * @{
  */

/** @defgroup RCCEx_RCC_BootCx RCC BootCx
  * @{
  */
#define RCC_BOOT_C1                     RCC_STBYBOOTCR_CPU1_HW_BEN
#define RCC_BOOT_C2                     RCC_STBYBOOTCR_CPU2_HW_BEN

#define IS_RCC_BOOT_CORE(CORE)          (((CORE) == RCC_BOOT_C1)  || \
                                         ((CORE) == RCC_BOOT_C2)  || \
                                         ((CORE) == (RCC_BOOT_C1 | RCC_BOOT_C2) ))

/**
  * @}
  */

/** @defgroup RCCEx_RCC_XBar RCC XBar Div
  * @{
  */
#define IS_RCC_XBARDIV(__DIV__) ((((__DIV__) >=    1UL) && ((__DIV__) <=    64UL)) || \
                                 (((__DIV__) >=    2UL) && ((__DIV__) <=   128UL) && (((__DIV__) %    2UL) == 0UL)) || \
                                 (((__DIV__) >=    4UL) && ((__DIV__) <=   256UL) && (((__DIV__) %    4UL) == 0UL)) || \
                                 (((__DIV__) >= 1024UL) && ((__DIV__) <= 65536UL) && (((__DIV__) % 1024UL) == 0UL)))

#define IS_RCC_D3_SOURCE(SOURCE) (((SOURCE) == RCC_D3SOURCE_MSI) || ((SOURCE) == RCC_D3SOURCE_LSI) || \
                                  ((SOURCE) == RCC_D3SOURCE_LSE))

/**
  * @}
  */

#if defined(CORE_CA35)
/* CA35SS Clock Lock time outs */
#define PLL1_LOCK_TIMEOUT_VALUE    (2U)    /* 2 ms (minimum Tick + 1) but real lock time is less than one tick */
#define CHGCLK_TIMEOUT_VALUE       (2U)    /* 2 ms (minimum Tick + 1) but real lock time is less than one tick */

#define SSCG_ON                   0x1U
#define FRAC_ON                   0x1U
#define A35_SOURCE_PLL1           0x0U
#define A35_SOURCE_EXT            0x1U
#endif /* CORE_CA35 */

#if defined(CORE_CA35)
// To be moved to CMSIS Device
#define CA35SS_BASE  0x48800000

typedef struct
{
  /* PLL1 registers - not in RCC */
	  __IO uint32_t CA35SS_SSC_CHGCLKREQ_RW;
	  __IO uint32_t CA35SS_SSC_CHGCLKREQ_WS1;
	  __IO uint32_t CA35SS_SSC_CHGCLKREQ_WC1;
	  uint32_t RESERVED1[29];
	  __IO uint32_t CA35SS_SSC_PLL_FREQ1_RW;
	  __IO uint32_t CA35SS_SSC_PLL_FREQ1_WS1;
	  __IO uint32_t CA35SS_SSC_PLL_FREQ1_WC1;
	  uint32_t RESERVED2[1];
	  __IO uint32_t CA35SS_SSC_PLL_FREQ2_RW;
	  __IO uint32_t CA35SS_SSC_PLL_FREQ2_WS1;
	  __IO uint32_t CA35SS_SSC_PLL_FREQ2_WC1;
	  uint32_t RESERVED3[1];
	  __IO uint32_t CA35SS_SSC_PLL_ENABLE_RW;
	  __IO uint32_t CA35SS_SSC_PLL_ENABLE_WS1;
	  __IO uint32_t CA35SS_SSC_PLL_ENABLE_WC1;
} PLL1_Typedef;

#define PLL1_Control          ((PLL1_Typedef*)CA35SS_BASE)
#define CA35SS_ARMCHGCLKREQ_Pos    0
#define CA35SS_ARMCHGCLKREQ_Msk   (0x1 << CA35SS_ARMCHGCLKREQ_Pos)
#define CA35SS_ARMCHGCLKREQ        CA35SS_ARMCHGCLKREQ_Msk
#define CA35SS_ARMCHGCLKACK_Pos    1
#define CA35SS_ARMCHGCLKACK_Msk   (0x1 << CA35SS_ARMCHGCLKACK_Pos)
#define CA35SS_ARMCHGCLKACK        CA35SS_ARMCHGCLKACK_Msk
#define CA35SS_PLLEN_Pos           0
#define CA35SS_PLLEN_Msk          (0x1 << CA35SS_PLLEN_Pos)
#define CA35SS_PLLEN               CA35SS_PLLEN_Msk
#define CA35SS_PLLRST_Pos          2
#define CA35SS_PLLRST_Msk         (0x1 << CA35SS_PLLRST_Pos)
#define CA35SS_PLLRST              CA35SS_PLLRST_Msk
#define CA35SS_FBDIV_Pos           0
#define CA35SS_FBDIV_Msk           (0xFFF << CA35SS_FBDIV_Pos)
#define CA35SS_FBDIV               CA35SS_FBDIV_Msk
#define CA35SS_REFDIV_Pos          16
#define CA35SS_REFDIV_Msk         (0x3F << CA35SS_REFDIV_Pos)
#define CA35SS_REFDIV              CA35SS_REFDIV_Msk
#define CA35SS_POSTDIV1_Pos        0
#define CA35SS_POSTDIV1_Msk        (0x7 << CA35SS_POSTDIV1_Pos)
#define CA35SS_POSTDIV1            CA35SS_POSTDIV1_Msk
#define CA35SS_POSTDIV2_Pos        3
#define CA35SS_POSTDIV2_Msk       (0x7 << CA35SS_POSTDIV2_Pos)
#define CA35SS_POSTDIV2            CA35SS_POSTDIV2_Msk
#endif

/** @defgroup RCC_PLL_State  RCC PLL State
  * @{
  */
#define RCC_PLL_OFF                  0x00000000U
#define RCC_PLL_ON                   0x00000001U

/**
  * @}
  */

/** @defgroup RCC_PLL_Mode  RCC PLL Mode
  * @{
  */
#define RCC_PLL_SPREAD_SPECTRUM 0x01U
/**
  * @}
  */

/** @defgroup RCC_PLL_Spread_Spectrum_Mode  RCC PLL Spread Spectrum mode
  * @{
  */
#define RCC_PLL_CENTERSPREAD    0x00U
#define RCC_PLL_DOWNSPREAD      0x01U
/**
  * @}
  */

/** @defgroup RCC_PLL_Clock_Source  RCC PLL Clock Source
  * @{
  */
#define RCC_PLLSOURCE_HSI       0x00000000U
#define RCC_PLLSOURCE_HSE       0x00000001U
#define RCC_PLLSOURCE_MSI       0x00000002U
/**
  * @}
  */

/** @defgroup RCC_D3_Clock_Source  RCC D3 Clock Source
  * @{
  */
#define RCC_D3SOURCE_MSI       0x00000000U
#define RCC_D3SOURCE_LSI       RCC_D3DCR_D3PERCKSEL_0
#define RCC_D3SOURCE_LSE       RCC_D3DCR_D3PERCKSEL_1
/**
  * @}
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup RCCEx_Exported_Types RCCEx Exported Types
  * @{
  */

/**
  * @brief  RCC PLL configuration structure definition
  */
typedef struct
{
  uint32_t id;          /*!< The PLL id from 2 to 8       */
  uint32_t PLLSource;   /*!< The new source of the PLL.
                            This parameter can be a value of @ref RCC_PLL_Clock_Source                */
  uint8_t  PLLState;    /*!< The new state of the PLL.
                            This parameter can be a value of @ref RCC_PLL_State                       */
  uint8_t  PLLMode;     /*!< The new state of the PLL.
                            This parameter can be a value of @ref RCC_PLL_Mode                        */
  uint8_t  FREFDIV;
  uint16_t FBDIV;
  uint32_t FRACIN;
  uint32_t POSTDIV1;
  uint32_t POSTDIV2;
  uint32_t SSM_Mode;     /*!< The new state of the PLL.
                            This parameter can be a value of @ref RCC_PLL_Spread_Spectrum_Mode        */

  uint32_t SSM_SPREAD;
  uint32_t SSM_DIVVAL;
} RCC_PLLInitTypeDef;

#define RCC_PeriphCLKInitTypeDef        RCC_FlexgenTypeDef

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup RCCEx_Exported_Constants RCCEx Exported Constants
  * @{
  */

/** @defgroup RCCEx_Periph_Clock_Selection  RCCEx Periph Clock Selection
  * @{
  */
#define RCC_PERIPHCLK_LPTIM1_2          7U
#define RCC_PERIPHCLK_UART2_4           8U
#define RCC_PERIPHCLK_UART3_5           9U
#define RCC_PERIPHCLK_SPI2_3            10U
#define RCC_PERIPHCLK_SPDIFRX           11U
#define RCC_PERIPHCLK_I2C1_2            12U
#define RCC_PERIPHCLK_I3C1_2            12U
#define RCC_PERIPHCLK_I2C3_5            13U
#define RCC_PERIPHCLK_I3C3              13U
#define RCC_PERIPHCLK_I2C4_6            14U
#define RCC_PERIPHCLK_I2C7              15U
#define RCC_PERIPHCLK_SPI1              16U
#define RCC_PERIPHCLK_SPI4_5            17U
#define RCC_PERIPHCLK_SPI6_7            18U
#define RCC_PERIPHCLK_USART1            19U
#define RCC_PERIPHCLK_USART6            20U
#define RCC_PERIPHCLK_UART7_8           21U
#define RCC_PERIPHCLK_UART9             22U
#define RCC_PERIPHCLK_SAI1_MDF1         23U
#define RCC_PERIPHCLK_SAI2              24U
#define RCC_PERIPHCLK_SAI3_4            25U
#define RCC_PERIPHCLK_FDCAN             26U
#define RCC_PERIPHCLK_LTDC              27U
#define RCC_PERIPHCLK_DSIPHY            28U
#define RCC_PERIPHCLK_DCMIPP            29U
#define RCC_PERIPHCLK_CSITXESC          30U
#define RCC_PERIPHCLK_CSIPHY            31U
#define RCC_PERIPHCLK_LVDSPHY           32U
#define RCC_PERIPHCLK_STGEN             33U
#define RCC_PERIPHCLK_USB3PCIEPHY       34U
#define RCC_PERIPHCLK_USBTC             35U
#define RCC_PERIPHCLK_I3C4              36U
#define RCC_PERIPHCLK_SPI8              37U
#define RCC_PERIPHCLK_I2C8              38U
#define RCC_PERIPHCLK_LPUART1           39U
#define RCC_PERIPHCLK_LPTIM3            40U
#define RCC_PERIPHCLK_LPTIM4_5          41U
#define RCC_PERIPHCLK_ADF1              42U
#define RCC_PERIPHCLK_TSDBG             43U
#define RCC_PERIPHCLK_TPIU              44U
#define RCC_PERIPHCLK_ATB               45U
#define RCC_PERIPHCLK_ADC12             46U
#define RCC_PERIPHCLK_ADC3              47U
#define RCC_PERIPHCLK_OSPI1             48U
#define RCC_PERIPHCLK_OSPI2             49U
#define RCC_PERIPHCLK_FMC               50U
#define RCC_PERIPHCLK_SDMMC1            51U
#define RCC_PERIPHCLK_SDMMC2            52U
#define RCC_PERIPHCLK_SDMMC3            53U
#define RCC_PERIPHCLK_ETH1_ETHSW        54U
#define RCC_PERIPHCLK_ETH2              55U
#define RCC_PERIPHCLK_ETH1PTP_ETH2PTP   56U
#define RCC_PERIPHCLK_USB2PHY1          57U
#define RCC_PERIPHCLK_USB2PHY2          58U
#define RCC_PERIPHCLK_ICN_M_GPU         59U
#define RCC_PERIPHCLK_ETHSWREF          60U
#define RCC_PERIPHCLK_MCO1              61U
#define RCC_PERIPHCLK_MCO2              62U
#define RCC_PERIPHCLK_CPU1_EXT2F        63U
#define RCC_PERIPHCLK_TIM1              64U
#define RCC_PERIPHCLK_TIM2              65U
#define RCC_PERIPHCLK_TIM3              66U
#define RCC_PERIPHCLK_TIM4              67U
#define RCC_PERIPHCLK_TIM5              68U
#define RCC_PERIPHCLK_TIM6              69U
#define RCC_PERIPHCLK_TIM7              70U
#define RCC_PERIPHCLK_TIM8              71U
#define RCC_PERIPHCLK_TIM10             72U
#define RCC_PERIPHCLK_TIM11             73U
#define RCC_PERIPHCLK_TIM12             74U
#define RCC_PERIPHCLK_TIM13             75U
#define RCC_PERIPHCLK_TIM14             76U
#define RCC_PERIPHCLK_TIM15             77U
#define RCC_PERIPHCLK_TIM16             78U
#define RCC_PERIPHCLK_TIM17             79U
#define RCC_PERIPHCLK_TIM20             80U
/**
  * @}
  */

/* Exported macros -----------------------------------------------------------*/
/** @defgroup RCCEx_Exported_Macros RCCEx Exported Macros
  * @{
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup RCCEx_Exported_Functions
  * @{
  */

/** @addtogroup RCCEx_Exported_Functions_Group1
  * @{
  */

HAL_StatusTypeDef HAL_RCCEx_PeriphCLKConfig(RCC_PeriphCLKInitTypeDef  *PeriphClkInit);
void              HAL_RCCEx_GetPeriphCLKConfig(RCC_PeriphCLKInitTypeDef  *PeriphClkInit);
uint32_t          HAL_RCCEx_GetPeriphCLKFreq(uint32_t PeriphClk);
uint32_t          HAL_RCCEx_GetTimerCLKFreq(uint32_t TimerId);

#if defined(CORE_CA35)
HAL_StatusTypeDef HAL_RCCEx_CA35SS_SetCA35SSClockSource(uint32_t source);
HAL_StatusTypeDef HAL_RCCEx_CA35SS_GetCA35SSClockSource(uint32_t *source);
#endif /* CORE_CA35 */

HAL_StatusTypeDef HAL_RCCEx_SetD3ClockSource(uint32_t source);
uint32_t HAL_RCCEx_GetD3ClockSource(void);

#if defined(CORE_CA35)
HAL_StatusTypeDef HAL_RCCEx_CA35SS_PLL1Config(const RCC_PLLInitTypeDef *pll_config);
#endif /* CORE_CA35 */
HAL_StatusTypeDef HAL_RCCEx_PLL2Config(RCC_PLLInitTypeDef *pll_config);
HAL_StatusTypeDef HAL_RCCEx_PLL3Config(RCC_PLLInitTypeDef *pll_config);
HAL_StatusTypeDef HAL_RCCEx_PLL4Config(RCC_PLLInitTypeDef *pll_config);
HAL_StatusTypeDef HAL_RCCEx_PLL5Config(RCC_PLLInitTypeDef *pll_config);
HAL_StatusTypeDef HAL_RCCEx_PLL6Config(RCC_PLLInitTypeDef *pll_config);
HAL_StatusTypeDef HAL_RCCEx_PLL7Config(RCC_PLLInitTypeDef *pll_config);
HAL_StatusTypeDef HAL_RCCEx_PLL8Config(RCC_PLLInitTypeDef *pll_config);

#if defined(CORE_CA35)
void              HAL_RCCEx_CA35SS_GetPLL1Config(RCC_PLLInitTypeDef *pll_config);
#endif /* CORE_CA35 */
void              HAL_RCCEx_GetPLL2Config(RCC_PLLInitTypeDef  *pll_config);
void              HAL_RCCEx_GetPLL3Config(RCC_PLLInitTypeDef  *pll_config);
void              HAL_RCCEx_GetPLL4Config(RCC_PLLInitTypeDef  *pll_config);
void              HAL_RCCEx_GetPLL5Config(RCC_PLLInitTypeDef  *pll_config);
void              HAL_RCCEx_GetPLL6Config(RCC_PLLInitTypeDef  *pll_config);
void              HAL_RCCEx_GetPLL7Config(RCC_PLLInitTypeDef  *pll_config);
void              HAL_RCCEx_GetPLL8Config(RCC_PLLInitTypeDef  *pll_config);

#if defined(CORE_CA35)
uint32_t          HAL_RCCEx_CA35SS_GetPLL1ClockFreq(void);
#endif /* CORE_CA35 */
uint32_t          HAL_RCCEx_GetPLL2ClockFreq(void);
uint32_t          HAL_RCCEx_GetPLL3ClockFreq(void);
uint32_t          HAL_RCCEx_GetPLL4ClockFreq(void);
uint32_t          HAL_RCCEx_GetPLL5ClockFreq(void);
uint32_t          HAL_RCCEx_GetPLL6ClockFreq(void);
uint32_t          HAL_RCCEx_GetPLL7ClockFreq(void);
uint32_t          HAL_RCCEx_GetPLL8ClockFreq(void);

int HAL_RCCEx_MeasureClockFreq(uint32_t clk_id, uint32_t ref_id, uint32_t *freq);

#if defined(CORE_CA35)
void HAL_RCCEx_EnableBootCore(uint32_t RCC_BootCx);
void HAL_RCCEx_DisableBootCore(uint32_t RCC_BootCx);
void HAL_RCCEx_HoldBootMCU(void);
void HAL_RCCEx_BootMCU(void);
#endif /* CORE_CA35 */
/**
  * @}
  */

/** @addtogroup RCCEx_Exported_Functions_Group2
  * @{
  */

HAL_StatusTypeDef HAL_RCCEx_EnableLSECSS(void);
HAL_StatusTypeDef HAL_RCCEx_DisableLSECSS(void);
void              HAL_RCCEx_EnableHSECSS(void);
void              HAL_RCCEx_DisableHSECSS(void);
void              HAL_RCCEx_LSECSS_IRQHandler(void);
void              HAL_RCCEx_LSECSS_Callback(void);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* STM32MP2xx_HAL_RCC_EX_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
