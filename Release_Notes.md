# Release Notes for STM32PRGFW-UTIL

Copyright © 2023 STMicroelectronics

[![ST logo](_htmresc/st_logo_2020.png)](https://www.st.com)
# Purpose

The **STM32PRGFW-UTIL** package provides a firmware utility application
used to manage the One-Time Programmable (OTP) and PMIC NVM.

The project contains two sub-applications:

-A STM32CubeProgrammer based application

-A Console based application as an alternative for the first application

Both sub-applications do the same operations to manage the OTP and PMIC
NVM.

This firmware contains a minimal set of drivers based on STM32CubeMP1xx and STM32CubeMP2xx Firmware packages:

-   HAL drivers
-   CMSIS Device
-   Cortex-A CMSIS Core
-   Imageheader
-   OpenBootloader
-   STM32_USB_Device_Library

## **Update History**

**V1.2.1 / 24-September-2026**

-  Linux environment build fixes. 

**V1.2.0 / 9-June-2025**

-  Support for MP21xx, Support for STPMIC1L and STPMIC2L added. 

**V1.1.2 / 19-November-2025**

-   STPMIC automatic detection supported on ST Boards. MP23x supported through MP2x Binary.

## Supported devices and boards by applications

-   STM32MP21xx Boards
-   STM32MP23xx Boards
-   STM32MP25xx Boards
-   STM32MP15xx Boards
-   STM32MP13xx Boards

#### Note: STM32MP23xx Board support through MP25 binaries.

## Dependencies

This software release is compatible with:

-   STM32CubeIDE_1.19.0
-   STM32CubeProgrammer_v2.20.0

## Known limitations

-   MP15 boards fail sometimes to connect to STM32CubeProgrammer via
    UART (reset the board and try again)

## Backward compatibility
-   No

For detailed Release History refer [Release Notes.html](https://github.com/PRG-MPU-ALPHA/STM32PRGFW-UTIL/blob/main/Release_Notes.html)

