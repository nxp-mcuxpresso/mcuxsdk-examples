/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "app.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_spi_adapter.h"
/*${header:end}*/

/*${function:start}*/

flash_command_table_t flashCommand = {
    .writeEnable =
        {
            .command     = {0x06u},
            .commandSize = 1,
        },
    .writeDisable =
        {
            .command     = {0x04u},
            .commandSize = 1,
        },
    .pageProgram =
        {
            .command     = {0x02u},
            .commandSize = 1,
        },
    .readData =
        {
            .command     = {0x03u},
            .commandSize = 1,
        },
    .readDataFast =
        {
            .command     = {0x0Bu},
            .commandSize = 1,
        },
    .writeStatus =
        {
            .command     = {0x01u},
            .commandSize = 1,
        },
    .readStatus =
        {
            .command     = {0x05u},
            .commandSize = 1,
        },
    .blockErase4K =
        {
            .command     = {0x20u},
            .commandSize = 1,
        },
    .blockErase32K =
        {
            .command     = {0x52u},
            .commandSize = 1,
        },
    .blockErase64K =
        {
            .command     = {0xd8u},
            .commandSize = 1,
        },
    .chipErase =
        {
            .command     = {0xc7u},
            .commandSize = 1,
        },
};

flash_config_t flashConfig = {
    .totalSize       = 0x400000u,
    .pageSize        = 256,
    .sectorSize      = 0x1000u,
    .needWriteEnable = true,
    .statusValueSize = 1,
    .statusBusyMask  = 0x01u,
    .statusBusyValue = 1,
};

void BOARD_SPI_CSActivate(void)
{
    GPIO_PinWrite(BOARD_SPI_FLASH_CS_GPIO, BOARD_SPI_FLASH_CS_GPIO_PIN, 0);
}

void BOARD_SPI_Deactivate(void)
{
    GPIO_PinWrite(BOARD_SPI_FLASH_CS_GPIO, BOARD_SPI_FLASH_CS_GPIO_PIN, 1);
}

spi_resource_t spiResource = {
    .spi              = LPSPI1,
    .whichPCSTransfer = kLPSPI_MasterPcs2,
    .assertCS         = BOARD_SPI_CSActivate,
    .deassertCS       = BOARD_SPI_Deactivate,
    .commandTable     = &flashCommand,
};

flash_handle_t handle = {
    .executeCommand = SPI_Flash_ExecuteCommand,
    .resource       = &spiResource,
};

void BOARD_InitSPIAdapter(void)
{
    lpspi_master_config_t masterConfig;

    /*Master config*/
    masterConfig.baudRate     = 20000000;
    masterConfig.bitsPerFrame = 8;
    masterConfig.cpol         = kLPSPI_ClockPolarityActiveHigh;
    masterConfig.cpha         = kLPSPI_ClockPhaseFirstEdge;
    masterConfig.direction    = kLPSPI_MsbFirst;

    masterConfig.pcsToSckDelayInNanoSec        = 1000000000 / masterConfig.baudRate;
    masterConfig.lastSckToPcsDelayInNanoSec    = 1000000000 / masterConfig.baudRate;
    masterConfig.betweenTransferDelayInNanoSec = 1000000000 / masterConfig.baudRate;

    masterConfig.whichPcs           = EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT;
    masterConfig.pcsActiveHighOrLow = kLPSPI_PcsActiveLow;

    masterConfig.pinCfg        = kLPSPI_SdiInSdoOut;
    masterConfig.dataOutConfig = kLpspiDataOutRetained;

    LPSPI_MasterInit(EXAMPLE_LPSPI_MASTER_BASEADDR, &masterConfig, EXAMPLE_LPSPI_MASTER_CLOCK_FREQ);

    /* Sets the FLASH PCS as low logic */
    gpio_pin_config_t csPinConfig = {
        kGPIO_DigitalOutput,
        1,
    };
    GPIO_PinInit(BOARD_SPI_FLASH_CS_GPIO, BOARD_SPI_FLASH_CS_GPIO_PIN, &csPinConfig);
}

void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /*Set clock source for LPSPI and get master clock source*/
    CLOCK_SetIpSrc(kCLOCK_Lpspi1, kCLOCK_IpSrcFircAsync);

    BOARD_InitSPIAdapter();
}
/*${function:end}*/
