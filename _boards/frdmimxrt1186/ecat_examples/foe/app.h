/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "board.h"
#include "fsl_rgpio.h"

#ifdef EEPROM_I2C_EMULATOR
#include "fsl_lpi2c.h"
#include "fsl_romapi.h"
#include "fsl_cache.h"
#endif
/*${header:end}*/

#ifdef EEPROM_I2C_EMULATOR
#define I2C_SLAVE_DEV ((LPI2C_Type *)LPI2C3_BASE)
#define I2C_SLAVE_CLOCK_FREQUENCY (CLOCK_GetRootClockFreq(kCLOCK_Root_Lpi2c0304))
#define I2C_TARGET_ADDR 0x00
#define ESC_EMULATOR_EEPROM_SIZE 0x1000
#define FLASH_NS   FMU0
#define EMULATOR_EEPROM_UPDATE_TIME_MS   8000

#define FLASH_DUMMY_CYCLES 0x06
#define FlexSpiInstance 2U
#define FLEXSPI_AMBA_BASE FlexSPI2_AMBA_BASE

#define FLEXSPI_LUT_SEQ(cmd0, pad0, op0, cmd1, pad1, op1)                                                              \
    (FLEXSPI_LUT_OPERAND0(op0) | FLEXSPI_LUT_NUM_PADS0(pad0) | FLEXSPI_LUT_OPCODE0(cmd0) | FLEXSPI_LUT_OPERAND1(op1) | \
     FLEXSPI_LUT_NUM_PADS1(pad1) | FLEXSPI_LUT_OPCODE1(cmd1))

#endif

#define    FOE_PWD                      0x12369874

#define    MAX_FIRMWARE_NAME_SIZE       32

// MAX_FIREWARE_SIZE: Maximum file size
#define MAX_FIREWARE_SIZE               0x40000

#define MCUBOOT_IMAGE_NUMBER            1

#define BOOT_FIRST_FLASH                0x4040000

#define FIRMWARE_INFO_FLASH             0x4440000

#define FIRMWARE_INFO_OFFSET            0x440000

#define GPIO_LED     BOARD_USER_LED_GPIO

#define GPIO_LED_PIN BOARD_USER_LED_GPIO_PIN

#endif /* _APP_H_ */