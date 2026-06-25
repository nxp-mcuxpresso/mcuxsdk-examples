/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_clock.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_XSPI           XSPI0
#define EXAMPLE_XSPI_AMBA_BASE 0x28000000U
#define EXAMPLE_XSPI_CLOCK     kCLOCK_Xspi0

/* The W25Q512NW on the reworked MIMXRT700-EVK has no QPI (4-4-4) mode, so this
 * board uses plain Quad I/O (1-4-4): the read command stays on 1 pad and only the
 * address/data use 4 pads. EXAMPLE_FLASH_QPI_MODE 0 makes the shared flash-ops skip
 * Enter-QPI and keep the read command on 1 pad. */
#define EXAMPLE_FLASH_QPI_MODE 0

#define EXAMPLE_XSPI_CLOCK_SOURCE     1U         /* Audio PLL PFD0 */
#define EXAMPLE_XSPI_CLOCK_DIVIDER    2U         /* root = PFD0 / 2 */
#define EXAMPLE_XSPI_ROOT_CLOCK_FREQ  266240000U /* PFD0 532.48 MHz / 2 = 266.24 MHz; SCK ~133 MHz */
#define EXAMPLE_XSPI0_APD_MASK        (1UL << 18U)
#define EXAMPLE_XSPI0_PPD_MASK        (1UL << 18U)

#define FLASH_SIZE      0x10000U /* W25Q512: 64MB in KB */
#define SECTOR_SIZE     0x1000U  /* 4KB */
#define FLASH_PAGE_SIZE 256U
#define EXAMPLE_SECTOR  256U     /* Test sector at 1MB offset, safely past boot image */

#define FLASH_BUSY_STATUS_POL    1U
#define FLASH_BUSY_STATUS_OFFSET 0U

/*
 * LUT sequence indices for the W25Q512NW Quad-I/O NOR flash.
 */
#define NOR_CMD_LUT_SEQ_IDX_READ_FAST_QPI      0U
#define NOR_CMD_LUT_SEQ_IDX_QPI_READ_STATUS    1U
#define NOR_CMD_LUT_SEQ_IDX_QPI_RESET_ENABLE   2U
#define NOR_CMD_LUT_SEQ_IDX_QPI_RESET_MEMORY   3U
#define NOR_CMD_LUT_SEQ_IDX_QPI_WRITE_ENABLE   4U
#define NOR_CMD_LUT_SEQ_IDX_QPI_ERASE_SECTOR   5U
#define NOR_CMD_LUT_SEQ_IDX_QPI_PAGE_PROGRAM   6U
#define NOR_CMD_LUT_SEQ_IDX_SPI_RESET_ENABLE   7U
#define NOR_CMD_LUT_SEQ_IDX_SPI_RESET_MEMORY   8U
#define NOR_CMD_LUT_SEQ_IDX_SPI_READ_SR2       9U
#define NOR_CMD_LUT_SEQ_IDX_SPI_WRITE_ENABLE   10U
#define NOR_CMD_LUT_SEQ_IDX_SPI_WRITE_SR2      11U
#define NOR_CMD_LUT_SEQ_IDX_ENTER_4BYTE        12U
#define NOR_CMD_LUT_SEQ_IDX_ENTER_QPI          13U
#define NOR_CMD_LUT_SEQ_IDX_QPI_SET_READ_PARAM 14U
#define NOR_CMD_LUT_SEQ_IDX_QPI_READ_ID        15U

#define CUSTOM_LUT_LENGTH 80U

#define FSL_FEATURE_XSPI_AHB_BUFFER_COUNT 4

#define DEMO_INVALIDATE_CACHES xspi_quad_invalidate_caches()

#define EXAMPLE_XSPI_CLOCK_INIT()                                                                            \
    do                                                                                                       \
    {                                                                                                        \
        PMC0->PDRUNCFG4 &= ~EXAMPLE_XSPI0_APD_MASK;                                                          \
        PMC0->PDRUNCFG5 &= ~EXAMPLE_XSPI0_PPD_MASK;                                                          \
        while ((PMC0->STATUS & PMC_STATUS_BUSY_MASK) != 0U)                                                  \
        {                                                                                                    \
        }                                                                                                    \
        PMC0->CTRL |= PMC_CTRL_APPLYCFG_MASK;                                                                \
        while ((PMC0->STATUS & PMC_STATUS_BUSY_MASK) != 0U)                                                  \
        {                                                                                                    \
        }                                                                                                    \
        CLOCK_InitAudioPfd(kCLOCK_Pfd0, 18U); /* PFD0 = 532.48 MHz (VCO*18/18) */                           \
        if (((CLKCTL0->XSPI0FCLKSEL & (CLKCTL0_XSPI0FCLKSEL_SEL_MASK | CLKCTL0_XSPI0FCLKSEL_SEL_EN_MASK)) != \
             (CLKCTL0_XSPI0FCLKSEL_SEL(EXAMPLE_XSPI_CLOCK_SOURCE) | CLKCTL0_XSPI0FCLKSEL_SEL_EN_MASK)) ||    \
            ((CLKCTL0->XSPI0FCLKDIV & CLKCTL0_XSPI0FCLKDIV_DIV_MASK) != (EXAMPLE_XSPI_CLOCK_DIVIDER - 1U)))  \
        {                                                                                                    \
            CLKCTL0->PSCCTL1_CLR = CLKCTL0_PSCCTL1_CLR_XSPI0_MASK;                                           \
            CLKCTL0->XSPI0FCLKSEL =                                                                          \
                CLKCTL0_XSPI0FCLKSEL_SEL(EXAMPLE_XSPI_CLOCK_SOURCE) | CLKCTL0_XSPI0FCLKSEL_SEL_EN_MASK;      \
            CLKCTL0->XSPI0FCLKDIV = CLKCTL0_XSPI0FCLKDIV_DIV(EXAMPLE_XSPI_CLOCK_DIVIDER - 1U);               \
            while ((CLKCTL0->XSPI0FCLKDIV & CLKCTL0_XSPI0FCLKDIV_REQFLAG_MASK) != 0U)                        \
            {                                                                                                \
            }                                                                                                \
            CLKCTL0->PSCCTL1_SET = CLKCTL0_PSCCTL1_SET_XSPI0_MASK;                                           \
        }                                                                                                    \
        __DSB();                                                                                             \
        __ISB();                                                                                             \
    } while (0)
/*${macro:end}*/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*${variable:start}*/
/*${variable:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void xspi_quad_invalidate_caches(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
