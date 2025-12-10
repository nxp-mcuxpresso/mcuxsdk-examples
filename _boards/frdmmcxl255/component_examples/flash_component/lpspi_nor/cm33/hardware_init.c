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
#include "fsl_port.h"
/*${header:end}*/

/*${function:start}*/
lpspi_memory_config_t W25Q40RLSNJQConfig = 
{
    .bytesInPageSize = 256UL,      /* 256Byte */
    .bytesInSectorSize = 4096UL,   /* 4KByte */
    .bytesInMemorySize = 0x80000,  /* 512KByte, 4 MBit */
};

nor_config_t norConfig = {
  .memControlConfig = &W25Q40RLSNJQConfig,
};

void BOARD_InitHardware(void)
{
    BOARD_InitBootClocks();
    BOARD_InitBootPins();
    BOARD_InitDebugConsole();
    BOARD_InitSPIFlashPins();

}

uint32_t BOARD_GetLpspiClock(void)
{
    /* Configure LPSPI1 CLK */
    CLOCK_AttachClk(BOARD_LPSPI_CLKSRC);
    CLOCK_SetClockDiv(kCLOCK_DivLPSPI1, BOARD_LPSPI_MRCC_CLK_DIV);
    CLOCK_EnableClock(BOARD_LPSPI_MRCC_ADDRESS);

    return CLOCK_GetLpspiClkFreq(1);
}

LPSPI_Type *BOARD_GetLpspiForNorFlash(void)
{
    return BOARD_EEPROM_LPSPI_BASEADDR;
}

void BOARD_LpspiPcsPinControl(bool isSelected)
{
    GPIO_PinWrite(GPIO1, 7, isSelected ? 0U : 1U);
}

void BOARD_LpspiIomuxConfig(spi_pin_mode_t pinMode)
{

}

uint32_t BOARD_GetNorFlashBaudrate(void)
{
    return BOARD_LPSPI_NOR_BAUDRATE;
}
/*${function:end}*/
