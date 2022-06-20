
# STM32PRGFW-UTIL_v1.0.0 Firmware utility to manage One-time Programmable (OTP) memories

## Project overview
**STM32PRGFW-UTIL** package provides multiples applications to manage the One-time Programmable (OTP) memories.
From user-friendly to developer-friendly applications, STM32 users can use STM32CubeProgrammer to flash a provided binary and directly have access to the OTP memories. A STM32CubeIDE code example is also provided to develop a custom program based.

The STM32PRGFW-UTIL meet these requirements:
- Multi-platform: STM32MP13xx and STM3MP15xx Boards are supported

- Board independent: 1 code-binary per product series (STM32MP13xx, STM3MP15xx) is delivered

- Flexible: Multiple applications for different usage are provided. This includes:
  - A STM32CubeProgrammer based application:
    - 2 ready-for-flash binaries (for STM32MP13xx and STM3MP15xx)
    - 2 connection interfaces UART and USB
  - A Standalone STM32CubeIDE based application:
    - STM32CubeIDE Semihosting Console
    - Provides OTP CLI, similar to STM32CubeProgrammer CLI
  - 2 others applications for development purpose:
    - A UART configuration for the Standalone application
    - A run mode for STM32CubeProgrammer application

- Open source: Source code delivered as example

- Small binary: DDR independent

The functional chart of the project:

![](_htmresc/functionnal_chart.png)

## Repository structure
The **STM32PRGFW-UTIL** repository consists of the following repositories:
- **Drivers**: It contains HAL/LL component per serie and CMSIS Device source code. You can notice there is no BSP as this package is board independent.
- **Middlewares**: It contains OpenBL and USB device library core source code modified for MPU protocols.
- **Projects**: Project directory. It contains 1 STM32CubeIDE project per serie and 1 common project containing source code common to the different projects.


The project structure is described as below:

![](_htmresc/project_structure.png)

## Documentation
More comprehensive documentation is available under the project Project/STM32MPxxx/Documentation.

## Release note
Details about the content of this release are available in the release note **Release_Notes.html**.

## Troubleshooting
**Caution**  : The issues are  **strictly limited**  to submit problems or suggestions related to the software delivered in this repository.

**For any other question**  related to the product, the hardware performance or characteristics, the tools, the environment, you can submit it to the  **ST Community**  on the STM32 MPUs related  [page](https://community.st.com/s/topic/0TO0X0000003u2AWAQ/stm32-mpus).
