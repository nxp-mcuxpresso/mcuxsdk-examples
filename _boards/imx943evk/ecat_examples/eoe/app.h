/*
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "board.h"
#include "fsl_rgpio.h"

#ifdef EEPROM_I2C_EMULATOR
#include "fsl_clock.h"
#include "fsl_lpi2c.h"
#include "fsl_xspi.h"
#include "fsl_cache.h"
#endif
/*${header:end}*/

#define GPIO_LED        GPIO2

#define GPIO_LED_PIN    31U

#ifdef EEPROM_I2C_EMULATOR
/*
 * I2C EEPROM emulator.
 *
 * The EtherCAT subdevice controller (ESC) drives its EEPROM interface
 * (ECAT_SDA/ECAT_SCL on GPIO_IO14/GPIO_IO15) as an I2C host. LPI2C4
 * (GPIO_IO18/GPIO_IO19) acts as the I2C target and emulates a 16-bit
 * addressable EEPROM at slave address 0x50. The EEPROM image is kept in RAM
 * and is persisted into the octal NOR flash attached to XSPI1.
 *
 * External wiring is required:
 *   J43-8  (GPIO_IO14, ECAT_SDA) <--> J12-3 (GPIO_IO18, LPI2C4_SDA)
 *   J43-12 (GPIO_IO15, ECAT_SCL) <--> J12-5 (GPIO_IO19, LPI2C4_SCL)
 */
#define I2C_SLAVE_DEV             ((LPI2C_Type *)LPI2C4_BASE)
#define I2C_SLAVE_IRQn            LPI2C4_IRQn
#define I2C_SLAVE_IRQHandler      LPI2C4_IRQHandler
#define I2C_SLAVE_CLOCK_ROOT      kCLOCK_Lpi2c4
#define I2C_SLAVE_CLOCK_FREQUENCY CLOCK_GetRate(I2C_SLAVE_CLOCK_ROOT)
#define I2C_SLAVE_ADDR            0x50U

/* ESC EEPROM size, matches the 16-bit address mode selected by CFG_ECAT[PROM_SIZE]. */
#define ESC_EMULATOR_EEPROM_SIZE       0x1000U
/* Quiet period after the last EEPROM write before the image is flushed to flash. */
#define EMULATOR_EEPROM_UPDATE_TIME_MS 300U

/* XSPI NOR flash backend. */
#define EXAMPLE_XSPI                 XSPI1
#define EXAMPLE_XSPI_AMBA_BASE       XSPI1_AMBA_BASE
#define EXAMPLE_XSPI_CLOCK           kCLOCK_Xspi1
#define FLASH_SIZE                   0x20000U /* In KBytes, 1Gb flash. */
#define FLASH_PAGE_SIZE              256U
#define SECTOR_SIZE                  0x1000U /* 4K */
#define FLASH_ENABLE_OCTAL_CMD       0xE7U
#define FLASH_BUSY_STATUS_POL        1
#define FLASH_BUSY_STATUS_OFFSET     0
/*
 * Sector used to store the EEPROM image. It is placed well above the boot
 * image area, keep it in sync with the flash layout used to program the board.
 */
#define ESC_EEPROM_FLASH_OFFSET      0x00100000U

#define NOR_CMD_LUT_SEQ_IDX_READ              0
#define NOR_CMD_LUT_SEQ_IDX_READ_STATUS       1
#define NOR_CMD_LUT_SEQ_IDX_WRITE_ENABLE      2
#define NOR_CMD_LUT_SEQ_IDX_READ_ID_OPI       3
#define NOR_CMD_LUT_SEQ_IDX_WRITE_ENABLE_OPI  4
#define NOR_CMD_LUT_SEQ_IDX_ERASE_SECTOR      5
#define NOR_CMD_LUT_SEQ_IDX_ERASE_CHIP        6
#define NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_OCTAL 7
#define NOR_CMD_LUT_SEQ_IDX_ENTER_OPI         8
/* Sequence 9 must stay a STOP command, see the xspi octal example for details. */
#define NOR_CMD_LUT_SEQ_IDX_WRITE             9
#define NOR_CMD_LUT_SEQ_IDX_READ_STATUS_OPI   10

#define CUSTOM_LUT_LENGTH        60
#define ENABLE_SFP_CONFIG        1

/*
 * Invalidate the data cache so that the image read back through the XSPI AHB
 * window comes from the flash and not from a stale cache line. The CM7 uses the
 * ARMv7-M cache maintenance operations, the CM33 uses the SoC XCACHE controller.
 */
#if (defined(__CORTEX_M) && (__CORTEX_M == 7))
#define DEMO_INVALIDATE_CACHES do { \
       SCB_InvalidateDCache(); \
} while (0)
#else
#define DEMO_INVALIDATE_CACHES do { \
       XCACHE_InvalidateCache(SOC_CORE_DCACHE_CTRL); \
} while (0)
#endif

static inline void xspi_clock_init(void)
{
    /* Clock setting for xspi */
    clk_t clk = {
        .clkId       = EXAMPLE_XSPI_CLOCK,
        .pclkId      = kCLOCK_Syspll1dfs1,
        .rate        = 400000000UL, /* 400MHz */
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };
    CLOCK_SetParent(&clk);
    CLOCK_SetRate(&clk);
    CLOCK_EnableClock(clk.clkId);
}
#endif /* EEPROM_I2C_EMULATOR */

#endif /* _APP_H_ */
