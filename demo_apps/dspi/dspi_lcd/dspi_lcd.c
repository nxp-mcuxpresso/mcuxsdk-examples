/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "lcd.h"
#include "fsl_dspi.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
uint8_t g_msg[] =
    "DSPI_LCD demo\r\n"
    "Show freescale logo in LCD.\r\n";

extern const unsigned char Freescale_logo[];

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void DSPI_Init(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
void DSPI_Init(void)
{
    uint32_t sourceClock;

    dspi_master_config_t masterConfig;
    memset((void *)&masterConfig, 0, sizeof(masterConfig));

    /*Master config*/
    masterConfig.whichCtar                                = kDSPI_Ctar0;
    masterConfig.ctarConfig.baudRate                      = DSPI_TRANSFER_BAUDRATE;
    masterConfig.ctarConfig.bitsPerFrame                  = 8;
    masterConfig.ctarConfig.cpol                          = kDSPI_ClockPolarityActiveHigh;
    masterConfig.ctarConfig.cpha                          = kDSPI_ClockPhaseFirstEdge;
    masterConfig.ctarConfig.direction                     = kDSPI_MsbFirst;
    masterConfig.ctarConfig.pcsToSckDelayInNanoSec        = 0;
    masterConfig.ctarConfig.lastSckToPcsDelayInNanoSec    = 0;
    masterConfig.ctarConfig.betweenTransferDelayInNanoSec = 0;
    masterConfig.whichPcs                                 = kDSPI_Pcs2;
    masterConfig.pcsActiveHighOrLow                       = kDSPI_PcsActiveLow;
    masterConfig.enableContinuousSCK                      = false;
    masterConfig.samplePoint                              = kDSPI_SckToSin0Clock;

    sourceClock = DSPI_MASTER_CLK_FREQ;
    DSPI_MasterInit(EXAMPLE_DSPI_MASTER_BASE, &masterConfig, sourceClock);
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Init board hardware. */
    BOARD_InitHardware();

    PRINTF("%s", g_msg);

    DSPI_Init();
    LCD_Initialize();

    LCD_FillAll((unsigned char *)Freescale_logo);

    while (1)
    {
    }
}
