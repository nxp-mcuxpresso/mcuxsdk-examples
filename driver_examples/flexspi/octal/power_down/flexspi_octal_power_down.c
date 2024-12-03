/*
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_flexspi.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "fsl_power.h"
#include "fsl_irtc.h"
#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern status_t flexspi_nor_flash_erase_sector(FLEXSPI_Type *base, uint32_t address);
status_t flexspi_nor_flash_page_program(FLEXSPI_Type *base, uint32_t dstAddr, const uint32_t *src);
extern status_t flexspi_nor_get_vendor_id(FLEXSPI_Type *base, uint8_t *vendorId);
extern status_t flexspi_nor_enable_octal_mode(FLEXSPI_Type *base);
extern void flexspi_nor_flash_init(FLEXSPI_Type *base);
void DEMO_PrePowerDown(void);
void DEMO_PostPowerDown(void);
/*******************************************************************************
 * Variables
 ******************************************************************************/

static uint8_t s_nor_program_buffer[256];
static uint8_t s_nor_read_buffer[256];

/*******************************************************************************
 * Code
 ******************************************************************************/

int main(void)
{
    uint32_t i = 0;
    status_t status;
    uint8_t vendorID = 0;
    uint32_t excludeFromPD[1];
    uint32_t wakeupFromPD[4];

    BOARD_InitHardware();

    excludeFromPD[0] = kPDRUNCFG_PD_LDOMEM | kPDRUNCFG_PD_FRO32K;
    wakeupFromPD[0]  = WAKEUP_RTC_ALARM_WAKEUP;
    wakeupFromPD[1]  = 0;
    wakeupFromPD[2]  = 0;
    wakeupFromPD[3]  = 0;

    PRINTF("\r\nFLEXSPI example enter power down!\r\n");

    DEMO_PrePowerDown();
    POWER_EnterPowerDown(excludeFromPD, kPOWER_SRAM_PDWN_MASK, wakeupFromPD, 0x20000000);

    LED_BLUE_INIT(1);
    DEMO_PostPowerDown();

    PRINTF("\r\nFLEXSPI example wakeup from power down!\r\n");

    DEMO_PostPowerDown();

    flexspi_nor_flash_init(EXAMPLE_FLEXSPI);

    /* Enter quad mode. */
    status = flexspi_nor_enable_octal_mode(EXAMPLE_FLEXSPI);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* Get vendor ID. */
    status = flexspi_nor_get_vendor_id(EXAMPLE_FLEXSPI, &vendorID);
    if (status != kStatus_Success)
    {
        return status;
    }
    PRINTF("Vendor ID: 0x%x\r\n", vendorID);

    /* Erase sectors. */
    PRINTF("Erasing Serial NOR over FlexSPI...\r\n");
    status = flexspi_nor_flash_erase_sector(EXAMPLE_FLEXSPI, EXAMPLE_SECTOR * SECTOR_SIZE);
    if (status != kStatus_Success)
    {
        PRINTF("Erase sector failure !\r\n");
        return -1;
    }

#if defined(CACHE_MAINTAIN) && CACHE_MAINTAIN
    DCACHE_InvalidateByRange(EXAMPLE_FLEXSPI_AMBA_BASE + EXAMPLE_SECTOR * SECTOR_SIZE, FLASH_PAGE_SIZE);
#endif

    memset(s_nor_program_buffer, 0xFFU, sizeof(s_nor_program_buffer));
    memcpy(s_nor_read_buffer, (void *)(EXAMPLE_FLEXSPI_AMBA_BASE + EXAMPLE_SECTOR * SECTOR_SIZE),
           sizeof(s_nor_read_buffer));

    if (memcmp(s_nor_program_buffer, s_nor_read_buffer, sizeof(s_nor_program_buffer)))
    {
        PRINTF("Erase data -  read out data value incorrect !\r\n ");
        return -1;
    }
    else
    {
        PRINTF("Erase data - successfully. \r\n");
    }

    for (i = 0; i < 0xFFU; i++)
    {
        s_nor_program_buffer[i] = i;
    }

    status =
        flexspi_nor_flash_page_program(EXAMPLE_FLEXSPI, EXAMPLE_SECTOR * SECTOR_SIZE, (void *)s_nor_program_buffer);
    if (status != kStatus_Success)
    {
        PRINTF("Page program failure !\r\n");
        return -1;
    }

#if defined(CACHE_MAINTAIN) && CACHE_MAINTAIN
    DCACHE_InvalidateByRange(EXAMPLE_FLEXSPI_AMBA_BASE + EXAMPLE_SECTOR * SECTOR_SIZE, FLASH_PAGE_SIZE);
#endif

    memcpy(s_nor_read_buffer, (void *)(EXAMPLE_FLEXSPI_AMBA_BASE + EXAMPLE_SECTOR * SECTOR_SIZE),
           sizeof(s_nor_read_buffer));

    if (memcmp(s_nor_read_buffer, s_nor_program_buffer, sizeof(s_nor_program_buffer)) != 0)
    {
        PRINTF("Program data -  read out data value incorrect !\r\n ");
        return -1;
    }
    else
    {
        PRINTF("Program data - successfully. \r\n");
    }

    while (1)
    {
    }
}

#define IOCON_PIO_DIGITAL_EN    0x0100u /*!<@brief Enables digital function */
#define IOCON_PIO_FUNC0         0x00u   /*!<@brief Selects pin function 1 */
#define IOCON_PIO_FUNC1         0x01u   /*!<@brief Selects pin function 1 */
#define IOCON_PIO_INV_DI        0x00u   /*!<@brief Input function is not inverted */
#define IOCON_PIO_MODE_INACT    0x00u   /*!<@brief No addition pin function */
#define IOCON_PIO_OPENDRAIN_DI  0x00u   /*!<@brief Open drain is disabled */
#define IOCON_PIO_SLEW_STANDARD 0x00u   /*!<@brief Standard mode, output slew rate control is enabled */

void DEMO_PrePowerDown(void)
{
    irtc_datetime_t date;
    irtc_config_t irtcConfig;

    /* Set a start date time and start RT */
    date.year    = 2021U;
    date.month   = 12U;
    date.day     = 25U;
    date.weekDay = 6U;
    date.hour    = 19U;
    date.minute  = 0;
    date.second  = 0;

    PRINTF("System will wakeup at 5s later\r\n");

    POWER_EnablePD(kPDRUNCFG_PD_XTAL32K); /*!< Powered down the XTAL 32 kHz RTC oscillator */
    POWER_DisablePD(kPDRUNCFG_PD_FRO32K); /*!< Powered the FRO 32 kHz RTC oscillator */
    CLOCK_AttachClk(kFRO32K_to_OSC32K);   /*!< Switch OSC32K to FRO32K */

    IRTC_GetDefaultConfig(&irtcConfig);

    /* Init RTC */
    IRTC_Init(RTC, &irtcConfig);

    /* Enable the RTC 32KHz oscillator at CFG0 by writing a 0 */
    IRTC_Enable32kClkDuringRegisterWrite(RTC, true);

    /* Clear all Tamper events by writing a 1 to the bits */
    IRTC_ClearTamperStatusFlag(RTC);

    IRTC_SetDatetime(RTC, &date);
    date.second += 5;
    IRTC_SetAlarm(RTC, &date);

    /* Enable RTC alarm interrupt */
    IRTC_EnableInterrupts(RTC, kIRTC_AlarmInterruptEnable);

    /* Enable at the NVIC */
    EnableIRQ(RTC_IRQn);
}
void DEMO_PostPowerDown(void)
{
    BOARD_InitHardware();
}

void RTC_IRQHandler(void)
{
    IRTC_ClearStatusFlags(RTC, RTC_ISR_ALM_IS_MASK);
}
