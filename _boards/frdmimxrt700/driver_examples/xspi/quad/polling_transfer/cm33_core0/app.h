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

#define EXAMPLE_XSPI_CLOCK_SOURCE     1U         /* Audio PLL PFD0 (XSPI0FCLKSEL SEL=1) */
#define EXAMPLE_XSPI_CLOCK_DIVIDER    1U         /* Divide-by-1: root clock = PFD0 output */
#define EXAMPLE_XSPI_ROOT_CLOCK_FREQ  330500000U /* Audio PLL VCO 532.48 MHz × 18/29 ≈ 330.5 MHz */
#define EXAMPLE_XSPI_FLASH_CLOCK_FREQ (EXAMPLE_XSPI_ROOT_CLOCK_FREQ / 2U)
#define EXAMPLE_XSPI0_APD_MASK        (1UL << 18U)
#define EXAMPLE_XSPI0_PPD_MASK        (1UL << 18U)

#define FLASH_SIZE      0x8000U /* 32MB in KB */
#define SECTOR_SIZE     0x1000U /* 4KB */
#define FLASH_PAGE_SIZE 256U
#define EXAMPLE_SECTOR  256U    /* Test sector at 1MB offset, safely past boot image */

#define FLASH_BUSY_STATUS_POL    1U
#define FLASH_BUSY_STATUS_OFFSET 0U

/*
 * LUT sequence indices for W25Q25PWZEIM QPI NOR flash (4-4-4 mode).
 * Hardware limit: 80 words = 16 sequences × 5 words.
 *
 * Indices  0-6  : QPI-mode operational commands (all on 4 pads).
 * Indices  7-8  : QPI rescue reset (4-pad 0x66/0x99, separate CS per cmd).
 * Indices  9-13 : SPI-mode init commands (1-pad) used before entering QPI.
 * Index   14    : QPI Set Read Parameters (configures dummy cycle count).
 * Index   15    : QPI Read JEDEC ID (vendor verification after QPI entry).
 */
#define NOR_CMD_LUT_SEQ_IDX_READ_FAST_QPI      0U  /* Fast Read Quad I/O  (QPI 4-4-4, 0xEC, 4B addr) */
#define NOR_CMD_LUT_SEQ_IDX_QPI_READ_STATUS    1U  /* Read Status Reg-1   (QPI 4-pad, 0x05) */
#define NOR_CMD_LUT_SEQ_IDX_QPI_RESET_ENABLE   2U  /* Enable Reset        (QPI 4-pad, 0x66) */
#define NOR_CMD_LUT_SEQ_IDX_QPI_RESET_MEMORY   3U  /* Reset Memory        (QPI 4-pad, 0x99) */
#define NOR_CMD_LUT_SEQ_IDX_QPI_WRITE_ENABLE   4U  /* Write Enable        (QPI 4-pad, 0x06) */
#define NOR_CMD_LUT_SEQ_IDX_QPI_ERASE_SECTOR   5U  /* Sector Erase 4KB   (QPI 4-pad, 0x21, 4B addr) */
#define NOR_CMD_LUT_SEQ_IDX_QPI_PAGE_PROGRAM   6U  /* Page Program        (QPI 4-pad, 0x12, 4B addr) */
#define NOR_CMD_LUT_SEQ_IDX_SPI_RESET_ENABLE   7U  /* Enable Reset        (SPI 1-pad, 0x66) */
#define NOR_CMD_LUT_SEQ_IDX_SPI_RESET_MEMORY   8U  /* Reset Memory        (SPI 1-pad, 0x99) */
#define NOR_CMD_LUT_SEQ_IDX_SPI_READ_SR2       9U  /* Read Status Reg-2   (SPI 1-pad, 0x35) — QE bit check */
#define NOR_CMD_LUT_SEQ_IDX_SPI_WRITE_ENABLE   10U /* Write Enable        (SPI 1-pad, 0x06) — before SR write */
#define NOR_CMD_LUT_SEQ_IDX_SPI_WRITE_SR2      11U /* Write Status Reg-2  (SPI 1-pad, 0x31) — set QE bit */
#define NOR_CMD_LUT_SEQ_IDX_ENTER_4BYTE        12U /* Enter 4-Byte Addr   (SPI 1-pad, 0xB7) */
#define NOR_CMD_LUT_SEQ_IDX_ENTER_QPI          13U /* Enter QPI Mode      (SPI 1-pad, 0x38) */
#define NOR_CMD_LUT_SEQ_IDX_QPI_SET_READ_PARAM 14U /* Set Read Parameters (QPI 4-pad, 0xC0) */
#define NOR_CMD_LUT_SEQ_IDX_QPI_READ_ID        15U /* Read JEDEC ID       (QPI 4-pad, 0x9F) */

#define CUSTOM_LUT_LENGTH 80U                      /* 16 sequences x 5 words — hardware maximum */

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
        CLOCK_InitAudioPfd(kCLOCK_Pfd0, 29U);                                                                \
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
