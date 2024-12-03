/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "app.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_inputmux.h"
#include "fsl_qspi_dma.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DUMMY_MASTER (0xE)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
// AT_QUICKACCESS_SECTION_CODE(static void callback(QuadSPI_Type *base, qspi_dma_handle_t *handle, status_t status, void
// *userData));
extern void qspi_nor_flash_init(QuadSPI_Type *base);
#if defined(FLASH_ENABLE_QUAD_CMD)
extern void enable_quad_mode(void);
#endif
#if defined(FLASH_ENABLE_OCTAL_CMD)
extern void enable_octal_mode(void);
#endif
extern void erase_sector(uint32_t addr);
extern void erase_all(void);
extern void program_page(uint32_t dest_addr, uint32_t *src_addr);
extern void BOARD_SetQspiClock(QuadSPI_Type *qspi, uint32_t qspiClockSrc, uint32_t divider);
extern void DMA0_DriverIRQHandler(void);
/*******************************************************************************
 * Variables
 ******************************************************************************/

#if !defined(FSL_FEATURE_QSPI_CLOCK_CONTROL_EXTERNAL) || (!FSL_FEATURE_QSPI_CLOCK_CONTROL_EXTERNAL)
static bool isDivNeedRestore = false;
#endif
AT_NONCACHEABLE_SECTION_ALIGN(static uint32_t buff[64], 4); /* Test data */

dma_handle_t dmaHandle = {0};
/*******************************************************************************
 * Code
 ******************************************************************************/

/* Use QSPI DMA way to program serial flash */
void qspi_dma(void)
{
    uint32_t i    = 0;
    uint32_t err  = 0;
    uint32_t addr = 0;
#if !defined(QSPI_ERASE_ADDR_OFFSET)
    addr = FSL_FEATURE_QSPI_AMBA_BASE;
#else
    addr = FSL_FEATURE_QSPI_AMBA_BASE + QSPI_ERASE_ADDR_OFFSET;
#endif
    erase_sector(addr);
    PRINTF("Erase finished!\r\n");

#if !defined(FSL_FEATURE_QSPI_CLOCK_CONTROL_EXTERNAL) || (!FSL_FEATURE_QSPI_CLOCK_CONTROL_EXTERNAL)
    /* Reduce frequency while clock divder is less than 2 */
    uint8_t qspiClockDiv = ((EXAMPLE_QSPI->MCR & QuadSPI_MCR_SCLKCFG_MASK) >> QuadSPI_MCR_SCLKCFG_SHIFT) + 1U;
    if (qspiClockDiv == 1U)
    {
        /* Reduce the frequency */
        isDivNeedRestore = true;
        QSPI_Enable(EXAMPLE_QSPI, false);
        EXAMPLE_QSPI->MCR &= ~QuadSPI_MCR_SCLKCFG_MASK;
        EXAMPLE_QSPI->MCR |= QuadSPI_MCR_SCLKCFG(1U);
        QSPI_Enable(EXAMPLE_QSPI, true);
    }
#endif

#if defined(FSL_SDK_DRIVER_QUICK_ACCESS_ENABLE)
    BOARD_SetQspiClock(EXAMPLE_QSPI, CLKCTL0_OSPIFCLKSEL_SEL(0), 5);
#endif

    /* Program pages in a sector */
    for (i = 0; i < FLASH_SECTORE_SIZE / FLASH_PAGE_SIZE; i++)
    {
        program_page(addr + i * FLASH_PAGE_SIZE, buff);
    }
    PRINTF("Program data finished!\r\n");

#if !defined(FSL_FEATURE_QSPI_CLOCK_CONTROL_EXTERNAL) || (!FSL_FEATURE_QSPI_CLOCK_CONTROL_EXTERNAL)
    /* Restore the frequency if needed */
    if (isDivNeedRestore)
    {
        QSPI_Enable(EXAMPLE_QSPI, false);
        EXAMPLE_QSPI->MCR &= ~QuadSPI_MCR_SCLKCFG_MASK;
        EXAMPLE_QSPI->MCR |= QuadSPI_MCR_SCLKCFG(0U);
        QSPI_Enable(EXAMPLE_QSPI, true);
    }
#endif

#if defined(FLASH_NEED_DQS)
#if defined(FSL_SDK_DRIVER_QUICK_ACCESS_ENABLE)
    BOARD_SetQspiClock(EXAMPLE_QSPI, CLKCTL0_OSPIFCLKSEL_SEL(0), 2);
#else
    /* Re-configure QSPI Serial clock frequency in order to acheive high performance. */
    QSPI_ClockUpdate(EXAMPLE_QSPI);
#endif
#endif

    for (i = 0; i < FLASH_SECTORE_SIZE / 4; i++)
    {
        if (((uint32_t *)addr)[i] != buff[i % 64])
        {
            PRINTF("The data in %d is wrong!!\r\n", i);
            PRINTF("The flash value in %d is %d\r\n", i, ((uint32_t *)addr)[i]);
            err++;
        }
    }
    if (err == 0)
    {
        PRINTF("Program through QSPI DMA succeed!\r\n");
    }
}

int main(void)
{
    uint32_t i = 0;

    BOARD_InitHardware();

    PRINTF("QSPI example started!\r\n");

    /* Configure DMAMUX. */
    INPUTMUX_Init(INPUTMUX);
    INPUTMUX_AttachSignal(INPUTMUX, EXAMPLE_CHANNEL, kINPUTMUX_QspiTxToDma0);

    /* DMA init */
    DMA_Init(EXAMPLE_DMA);

    /* According to ERRATA QSPI.1, configure DMA channel 23 for QSPI TX DMA.*/
    DMA_EnableChannel(EXAMPLE_DMA, EXAMPLE_CHANNEL);
    DMA_CreateHandle(&dmaHandle, EXAMPLE_DMA, EXAMPLE_CHANNEL);

#if defined(ENABLE_RAM_VECTOR_TABLE)
    InstallIRQHandler(DMA0_IRQn, (uint32_t)DMA0_DriverIRQHandler);
#endif

    /* Copy the LUT table */
    memcpy(single_config.lookuptable, lut, sizeof(lut));

    qspi_nor_flash_init(EXAMPLE_QSPI);

    /*Initialize data buffer */
    for (i = 0; i < 64; i++)
    {
        buff[i] = i;
    }

#if defined(FLASH_ENABLE_QUAD_CMD)
    /* Enable Quad mode for the flash */
    enable_quad_mode();
#endif

#if defined(FLASH_ENABLE_OCTAL_CMD)
    enable_ddr_mode();
    /* Enable Octal mode for the flash */
    enable_octal_mode();
#endif

    /* Use dma to program flash. */
    qspi_dma();

    while (1)
    {
    }
}
