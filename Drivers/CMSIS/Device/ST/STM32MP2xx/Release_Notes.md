---
pagetitle: Release Notes for STM32MP2xx CMSIS
lang: en
header-includes: <link rel="icon" type="image/x-icon" href="_htmresc/favicon.png" />
---

::: {.row}
::: {.col-sm-12 .col-lg-4}

<center>
# Release Notes for
# <mark>STM32MP2xx CMSIS</mark>
Copyright &copy; 2022 STMicroelectronics\

[![ST logo](./_htmresc/st_logo_2020.png)](https://www.st.com){.logo}
</center>

# License

This software component is licensed by ST under Apache-2.0 license, the \"License\"; You may
not use this component except in compliance with the License. You may
obtain a copy of the License at:

[Apache License v2.0](https://opensource.org/licenses/Apache-2.0)

# Purpose

This driver provides the CMSIS device for the STM32MP2 products. This covers STM32MP25xx devices.

This driver is composed of the descriptions of the registers under “Include” directory.

Various template file are provided to easily build an application. They can be adapted to fit applications requirements.
- Include/Templates_TZ/partition_stm32mp2xx.h contain a default Secure Attribute Unit (SAU) set up from SystemInit() for secure application when the TrustZone-M security is enabled (TZEN=1).
- Source/Templates/system_stm32mp2xx_a35.c contains the initialization code referred as SystemInit() for CA35 core.
- Source/Templates/system_stm32mp2xx_m33_s.c contains the initialization code referred as SystemInit() for CM33 core secure code and security extension enabled (secure code and TZEN=1).
- Source/Templates/system_stm32mp2xx_m33_ns.c contains the initialization code referred as SystemInit() for CM33 core with security extension not enabled or non secure code with security extension enables (TZEN=0).
- Source/Templates/system_stm32mp2xx_m33.c contains the initialization code referred as SystemInit() for CM33 core with security extension enabled or not enabled (both of above cases, configured by __ARM_FEATURE_CMSE compilation flag).
- Source/Templates/system_stm32mp2xx_m0plus.c contains the initialization code referred as SystemInit() for CM0+ core.
- Startup files are provided as example for STM32CubeIDE.
- Linker files are provided as example for STM32CubeIDE.
:::

::: {.col-sm-12 .col-lg-8}
# Update History

::: {.collapse}
<input type="checkbox" id="collapse-section3" checked aria-hidden="true">
<label for="collapse-section3" aria-hidden="true">__V0.0.8 / 09-June-2022__</label>
<div>

## Main Changes

**Forth Internal release**

### **Base addresses**
  - DDRDGB base address added

### **Register definitions**
  - DDR Controller Registers DBG def added
  - ADC, DDR Controller, RCC, SPDIFRX Registers def updated
  
### **Other changes**
  - M33 TZ Partiton template updated
  - startup source code for GCC (STM32CubeIDE) templates updated
  - system source code templates updated
  - Updated GCC (STM32CubeIDE) Linker scripts templates for Secure M33 in RETRAM, Secure and non secure M33 in MCUSRAM, A35_0 in SYSRAM
  - MISRA-C Compatibility improved

## Contents

## Notes

</div>
:::
::: {.collapse}
<input type="checkbox" id="collapse-section2" checked aria-hidden="true">
<label for="collapse-section2" aria-hidden="true">__V0.0.7 / 02-February-2022__</label>
<div>

## Main Changes

**Third Internal release**

### **Base addresses**
  - ADF1 base address added
  - ETH Switch base addreses updated
  
### **Register definitions**
  - DDR Controller Registers def added
  - DSI Controller, ETH, IWDG, PKA, RIFSC, RISC, SAES Registers def updated
  
### **Other changes**
  - CSI Oscillator renamed to MSI to be in line with Reference Manual
  - startup source code for GCC (STM32CubeIDE) templates updated
  - system source code templates updated
  - Added GCC (STM32CubeIDE) Linker scripts templates for Secure M33 in RETRAM, Secure and non secure M33 in MCUSRAM, A35_0 in SYSRAM
  - MISRA-C Compatibility improved

## Contents

## Notes

</div>
:::
::: {.collapse}
<input type="checkbox" id="collapse-section1" aria-hidden="true">
<label for="collapse-section1" aria-hidden="true">__V0.0.6 / 01-July-2021__</label>
<div>

## Main Changes

**Second Internal release**

## Contents

## Notes

</div>
:::
:::
:::

<footer class="sticky">
For complete documentation on STM32MP2xx, visit: [[www.st.com/stm32l5](http://www.st.com/stm32l5)]

*This release note uses up to date web standards and, for this reason, should not be opened with Internet Explorer but preferably with popular browsers such as Google Chrome, Mozilla Firefox, Opera or Microsoft Edge.*
</footer>
