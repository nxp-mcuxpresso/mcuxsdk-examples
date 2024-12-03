/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_lpspi_mem_adapter.h"
#include "fsl_nor_flash.h"
#include "fsl_lpspi_nor_flash.h"
/*${header:end}*/

/*${function:start}*/
lpspi_memory_config_t MX25R6435FM2IL0Config = 
{
    .bytesInPageSize = 256UL,      /* 256Byte */
    .bytesInSectorSize = 4096UL,   /* 4KByte */
    .bytesInMemorySize = 0x800000, /*8MByte, 64 MBit*/
};

nor_config_t norConfig = {
  .memControlConfig = &MX25R6435FM2IL0Config,
};

void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Init the LPSPI instance for the external NOR flash */
    CLOCK_EnableClock(BOARD_LPSPI_MRCC_ADDRESS);
    CLOCK_SetIpSrc(BOARD_LPSPI_MRCC_ADDRESS, BOARD_LPSPI_CLKSRC);
    CLOCK_SetIpSrcDiv(BOARD_LPSPI_MRCC_ADDRESS, BOARD_LPSPI_MRCC_CLK_DIV);
}

uint32_t BOARD_GetLpspiClock(void)
{
    return CLOCK_GetIpFreq(BOARD_LPSPI_MRCC_ADDRESS);
}

LPSPI_Type *BOARD_GetLpspiForNorFlash(void)
{
    return BOARD_EEPROM_LPSPI_BASEADDR;
}

void BOARD_LpspiPcsPinControl(bool isSelected)
{
    GPIO_PinWrite(BOARD_DEINITEXTFLASHPINS_LPSPI1_PCS0_GPIO, BOARD_DEINITEXTFLASHPINS_LPSPI1_PCS0_PIN,
                  isSelected ? 0U : 1U);
}

void BOARD_LpspiIomuxConfig(spi_pin_mode_t pinMode)
{
    if (pinMode == kSpiIomux_SpiMode)
    {
        BOARD_InitExtFlashPins();
    }
    else
    {
        BOARD_DeinitExtFlashPins();
    }
}

uint32_t BOARD_GetNorFlashBaudrate(void)
{
    return BOARD_LPSPI_NOR_BAUDRATE;
}
/*${function:end}*/
