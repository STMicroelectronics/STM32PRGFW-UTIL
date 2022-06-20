/**
  @page STM32PRGFW-UTIL

  @verbatim
  ******************************************************************************
  * @file    Documentation/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the consumer example.
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
  @endverbatim

@par Application Description
The application developed here is called STM32PRGFW-UTIL, a firmware utility
application used to manage the One-Time Programmable (OTP).

The STM32PRGFW-UTIL project contains 2 sub-applications:
- A STM32CubeProgrammer based application
- A Console based application as an alternative for the first application
Both sub-applications do the same operations to manage the OTP.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock for
STM32MP15xx Devices.

Then, these protocols and memories interfaces are configured
- USART interface
- USB interface
- RAM interface
- IWDG interface
- OTP interface

Then, the STM32PRGFW-UTIL application will wait for incoming communication on
one of the supported protocols.

Once a communication is established through one of the supported protocols, the
OpenBootloader application will wait for commands sent by the host.


@par Keywords
OpenBootloader, USART, OTP, USB


@par Directory contents
- Core/Src/main.c                                    Main program file
- Core/Src/stm32mp1xx_hal_msp.c                      MSP Initialization file
- Core/Src/stm32mp1xx_it.c                           Interrupt handlers file
- Core/Src/system_stm32mp1xx.c                       STM32MP15xx system clock config file
- Core/Inc/main.h                                    Main program header file
- Core/Inc/RTE_Components.h                          Main program header file
- Core/Inc/stm32mp1xx_hal_conf.h                     HAL Library Configuration file
- Core/Inc/stm32mp1xx_it.h                           Interrupt handlers header file
- Core/Inc/stm32_assert.h                            STM32 assert file
- Documentation/readme.txt                           Readme
- Binary/FlashLayout_STM32PRGFW_UTIL.tsv             Application flashlayout
- Binary/stm32prgfw_util_cpserialboot_signed.bin     Application signed binary
- Common/OpenBootloader/App/app_openbootloader.c     OpenBootloader app entry point
- Common/OpenBootloader/App/app_openbootloader.h     Header for OpenBootloader app
- Common/OpenBootloader/Target/common_interface.c    Common functions used by interfaces
- Common/OpenBootloader/Target/common_interface.h    Header for common functions file
- Common/OpenBootloader/Target/interfaces_conf.h     Contains Interfaces configuration
- Common/OpenBootloader/Target/iwdg_interface.c      Contains IWDG interface
- Common/OpenBootloader/Target/iwdg_interface.h      Header of IWDG interface file
- Common/OpenBootloader/Target/openbootloader_conf.h Header of OpenBL HW dependent config
- Common/OpenBootloader/Target/platform.h            Header for patterns
- Common/OpenBootloader/Target/ram_interface.c       Contains RAM interface
- Common/OpenBootloader/Target/ram_interface.h       Header of RAM interface file
- Common/OpenBootloader/Target/usart_interface.c     Contains USART interface
- Common/OpenBootloader/Target/usart_interface.h     Header of USART interface file
- Common/OTP_Console/Src/otp_interface_cli.c         Contains OTP CLI interface
- Common/OTP_Console/Src/otp_interface_cli_util.c    Contains OTP CLI utility
- Common/OTP_Console/Inc/otp_interface_cli.h         Header OTP CLI interface
- Common/OTP_Console/Inc/otp_interface_cli_util.h    Header of OTP CLI utility
- Common/Core/Src/otp_util.c                         Contains OTP utility
- Common/Core/Inc/otp_util.h                         Header of OTP utility
- Common/USB_Device/App/usb_device.c                 Implements the USB Device
- Common/USB_Device/App/usb_device.h                 Header for USB Device
- Common/USB_Device/App/usbd_desc.c                  Implements the USB device descriptors
- Common/USB_Device/App/usbd_desc.h                  Header for USB device descriptors
- Common/USB_Device/App/usbd_dfu_media.c             Implements the USB DFU media             
- Common/USB_Device/App/usbd_dfu_media.h             Header for USB DFU media 
- Common/USB_Device/App/usbd_conf.c                  Implements the BSP for the USB device library                
- Common/USB_Device/App/usbd_conf.h                  Header for USB device library 



@par Hardware and Software environment
- This example runs on STM32MP15xx devices.

- This example has been tested with STM32MP157C-DK2 board and can be
  easily tailored to any other supported device and development board.

- STLINK UART is used
- USB


@par How to use it?
STM32PRGFW-UTIL has four different debug configurations:
- CP_Dev_Boot (STM32CubeProgrammer with development boot)
- CP_Serial_Boot (STM32CubeProgrammer with serial boot)
- Console_UART (Console UART)
- Console_SH (Console Semihosting)

STM32PRGFW-UTIL has two principal usages:
- With STM32CubeProgrammer, using the following debug configurations:
  - CP_Dev_Boot
  - CP_Serial_Boot
- With Serial Console, using the following debug configurations:
  - Console_UART
  - Console_SH


To use the STM32PRGFW-UTIL application with STM32CubeProgrammer
- In case of CP_Dev_Boot
  - Run the debug configuration with STM32CubeIDE
  - Launch the STM32CubeProgrammer GUI or CLI interface (see below how to)
  - Read the STM32CubeProgrammer documentation to know how to use the GUI.
  - For CLI interface (see below how to)
- In case of CP_Serial_Boot
  - Launch the STM32CubeProgrammer GUI or CLI interface (see below how to)
  - Read the STM32CubeProgrammer documentation to know how to use the GUI.

  - For CLI interface (see below how to)
  - Launch the STM32CubeProgrammer CLI interface (see below how to)
  - \Binary directory contains the STM32PRGFW-UTIL signed binary and
    a tsv file used by STM32CubeProgrammer to load the application binary in
    embedded ram.
  - Run this command to load the STM32PRGFW-UTIL binary in the embedded ram:
  	$STM32_Programmer_CLI.exe -c port=COM<num> -w <dir_path>\Binary\FlashLayout_STM32PRGFW_UTIL.tsv
  - If flashing service completed successfully, otp operations can now be done (do not reset the board):
  	$STM32_Programmer_CLI.exe -c port=COM<num> -otp displ
  	$STM32_Programmer_CLI.exe -c port=USB<num> -otp displ
  	$STM32_Programmer_CLI.exe -c port=COM<num> -otp write word=10 value=0x1
  	$STM32_Programmer_CLI.exe -c port=USB<num> -otp write word=10 value=0x1


To Launch the STM32CubeProgrammer CLI interface:
(Note: There is no OTP support on STM32CubeProgrammer graphic interface for now.
So,STM32CubeProgrammer CLI will be used instead.)
- Go to the STM32CubeProgrammer binary directory
  (i.e. STM32CubeProgrammer-210-OTP\bin)
- Open command prompt
- Set the current directory in the command prompt to the STM32CubeProgrammer
  binary directory (i.e. cd C:\STM32CubeProgrammer\bin\)
- STM32CubeProgrammer commands can now be used to manage the OTP. Fully
  information about the OTP commands can be found in the STM32CubeProgrammer
  documentation. But, the commands can be displayed, using this command:
  $STM32_Programmer_CLI.exe -h
- Some command examples:
  $STM32_Programmer_CLI.exe -c port=COM8 -otp displ
  $STM32_Programmer_CLI.exe -c port=COM8 -otp write word=10 value=0x1


To use STM32PRGFW-UTIL with Serial Console
- In case of Console_UART
  - Open a serial console (Putty, Terra Term...)
  - Set the serial port configuration as following:
    - Speed: 115200
    - Data: 8 bits
    - Parity: even
    - Stop bits: 1 bits
    - Flow control: none
  - Open it
  - Run the debug configuration with STM32CubeIDE
  - The OTP interface CLI will be displayed in the Console and can now be used

- In case of Console_SH
  - Run the debug configuration with STM32CubeIDE
  - The OTP interface CLI will be displayed in the Console and can now be used
  - Some command examples:
    $help
    $displ
    $write word=10 value=1


@par Known issues?
- Uart connexion with STM32CubeProgrammer fail sometimes:
  - Reset the board and try again.
  - Sometimes, unplug and replug the stlink and the power may help.
- There is a USB reconnection problem on Serial boot on MP15


Note: No issue faced with Console Semihosting OTP CLI application


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
