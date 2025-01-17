/* -------------------------------------------------------------------------- */
/*                           Copyright 2021-2022 NXP                          */
/*                            All rights reserved.                            */
/*                    SPDX-License-Identifier: BSD-3-Clause                   */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */

#include "board_extflash.h"
#include "pin_mux.h"
#include "fsl_clock.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "board_platform.h"
#include "fsl_lpspi_nor_flash.h"

/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */

void BOARD_InitExternalFlash(void)
{
    /* Init the LPSPI instance for the external NOR flash */
    CLOCK_EnableClock(BOARD_LPSPI_MRCC_ADDRESS);
    CLOCK_SetIpSrc(BOARD_LPSPI_MRCC_ADDRESS, BOARD_LPSPI_CLKSRC);
    CLOCK_SetIpSrcDiv(BOARD_LPSPI_MRCC_ADDRESS, BOARD_LPSPI_MRCC_CLK_DIV);

    /* Init Write Protect for External Flash */
    BOARD_InitExternalFlashWriteProtect();
    /* Disable Write Protect for External Flash. If enabled cannot write in External flash */
    BOARD_DisableExternalFlashWriteProtect();
}

void BOARD_UninitExternalFlash(void)
{
    BOARD_UninitExternalFlashWriteProtect();
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
    GPIO_PinWrite(BOARD_EXTFLASH_CS_GPIO, BOARD_EXTFLASH_CS_PIN,
                  isSelected ? 0U : 1U);
}

void BOARD_LpspiIomuxConfig(spi_pin_mode_t pinMode)
{
    if (pinMode == kSpiIomux_SpiMode)
    {
        /* Warning: On EVK boards, this functions must configure a 5th pin PTB4 as a workaround.
         * PTB4 is connected to HOLDRESET pin of the flash, so it has to stay high to make the flash work.
         * In case of OTA from external flash, the bootloader won't be able to read from the external flash as it
         * doesn't use this pin, because this is not standard but board specific. A workaround to solve this is to
         * connect PTB4 to +3.3V output so the bootloader can access the external flash See KFOURWONE-2793 */
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

void BOARD_InitExternalFlashWriteProtect(void)
{
    PORT_Type * port;
    gpio_pin_config_t config;
    config.pinDirection = kGPIO_DigitalOutput;
    config.outputLogic  = 0;

    if (BOARD_EXTFLASH_WP_GPIO == GPIOB) /* KW45EVK - KW47EVK - MCXW72EVK case*/
    {
        CLOCK_EnableClock(kCLOCK_GpioB);
        CLOCK_EnableClock(kCLOCK_PortB);
        port = PORTB;

    }
    else if (BOARD_EXTFLASH_WP_GPIO == GPIOC) /* FRDM-MCXW7x case */
    {
        CLOCK_EnableClock(kCLOCK_GpioC);
        CLOCK_EnableClock(kCLOCK_PortC);
        port = PORTC;

    }

    PORT_SetPinMux(port, BOARD_EXTFLASH_WP_PIN, kPORT_MuxAsGpio);
    GPIO_PinInit(BOARD_EXTFLASH_WP_GPIO, BOARD_EXTFLASH_WP_PIN, &config);
}

void BOARD_UninitExternalFlashWriteProtect(void)
{
    PORT_Type * port;
    if (BOARD_EXTFLASH_WP_GPIO == GPIOB) /* KW45EVK - KW47EVK - MCXW72EVK case*/
    {
        CLOCK_EnableClock(kCLOCK_GpioB);
        CLOCK_EnableClock(kCLOCK_PortB);
        port = PORTB;
    }
    else if (BOARD_EXTFLASH_WP_GPIO == GPIOC) /* FRDM-MCXW7x case */
    {
        CLOCK_EnableClock(kCLOCK_GpioC);
        CLOCK_EnableClock(kCLOCK_PortC);
        port = PORTC;
    }

    PORT_SetPinMux(port, BOARD_EXTFLASH_WP_PIN, kPORT_PinDisabledOrAnalog);
}

void BOARD_DisableExternalFlashWriteProtect(void)
{
    GPIO_PinWrite(BOARD_EXTFLASH_WP_GPIO, BOARD_EXTFLASH_WP_PIN, 1U);
}

void BOARD_EnableExternalFlashWriteProtect(void)
{
    GPIO_PinWrite(BOARD_EXTFLASH_WP_GPIO, BOARD_EXTFLASH_WP_PIN, 0U);
}


#if defined BOARD_EXFLASH_IS_MX25R6435FM2IL0 && (BOARD_EXFLASH_IS_MX25R6435FM2IL0 >0)
const lpspi_memory_config_t MX25R6435FM2IL0Config = {
    .bytesInPageSize   = 256UL,    /* 256Byte */
    .bytesInSectorSize = 4096UL,   /* 4KByte */
    .bytesInMemorySize = 0x800000, /*8MByte, 64 MBit*/
};

const nor_config_t norConfig = {
    .memControlConfig = (void*)&MX25R6435FM2IL0Config,
    .driverBaseAddr = (void*)BOARD_EEPROM_LPSPI_BASEADDR,
};
#elif defined BOARD_EXFLASH_IS_AT25XE161D && (BOARD_EXFLASH_IS_AT25XE161D > 0)

const lpspi_memory_config_t AT25XE161DConfig = {
    .bytesInPageSize   = 256UL,    /* 256Byte */
    .bytesInSectorSize = 4096UL,   /* 4KByte */
    .bytesInMemorySize = 0x200000, /*2MByte, 16 MBit*/
};

const nor_config_t norConfig = {
    .memControlConfig = (void*)&AT25XE161DConfig,
    .driverBaseAddr = (void*)BOARD_EEPROM_LPSPI_BASEADDR,
};
#endif

const nor_config_t * BOARD_GetExtFlashConfig(void)
{
#if ((defined BOARD_EXFLASH_IS_MX25R6435FM2IL0 && (BOARD_EXFLASH_IS_MX25R6435FM2IL0 > 0)) || \
    (defined BOARD_EXFLASH_IS_AT25XE161D && (BOARD_EXFLASH_IS_AT25XE161D > 0)))
    return &norConfig;
#else
    return NULL;
#endif

}
