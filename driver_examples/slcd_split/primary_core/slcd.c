/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"
#include "fsl_common.h"
#include "app.h"
#include "fsl_slcd.h"
#include "slcd_engine.h"
#include "fsl_debug_console.h"
#include "mcmgr.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief SLCD time delay.
 * @param ms Milli-second
 */
static void SLCD_TimeDelay(uint32_t ms);

/* Demonstrate how to show number. */
static void SLCD_Show_Digital(void);

/* Demonstrate how to show icon. */
static void SLCD_Show_Icon(void);

/* Demonstrate how to show letter. */
static void SLCD_Show_Letter(void);

/* Demonstrate the blink feature. */
static void SLCD_Blink(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
tSLCD_Engine slcdEngine;

/*******************************************************************************
 * Code
 ******************************************************************************/
static void SLCD_SetLCDPin(lcd_set_type_t type, uint32_t lcd_pin, uint8_t pin_val, int32_t on)
{
    assert(lcd_pin > 0);

    uint8_t gpio_pin = 0;
    uint8_t bit_val  = 0;
    uint8_t i        = 0;

    /* lcd _pin starts from 1. */
    gpio_pin = slcd_lcd_gpio_seg_pin[lcd_pin - 1];

    if (type != SLCD_Set_Icon)
    {
        SLCD_SetFrontPlaneSegments(LCD, gpio_pin, (on ? pin_val : 0));
    }
    else
    {
        for (i = 0; i < 8; ++i)
        {
            bit_val = (uint8_t)(pin_val >> i) & 0x1U;
            if (bit_val)
            {
                SLCD_SetFrontPlaneOnePhase(LCD, gpio_pin, (slcd_phase_index_t)i, on);
            }
        }
    }
}

static void SLCD_Show_Digital(void)
{
    uint8_t digital;
    int32_t position;

    PRINTF("\r\nShow digital numbers\r\n");

    for (digital = 0; digital < 10; digital++)
    {
        for (position = 0; position < NUM_POSEND; position++)
        {
            SLCD_Engine_Show_Num(&slcdEngine, digital, position, 1);
        }

        SLCD_TimeDelay(500);

        for (position = 0; position < NUM_POSEND; position++)
        {
            SLCD_Engine_Show_Num(&slcdEngine, digital, position, 0);
        }
    }

    PRINTF("\r\nShow digital numbers finished\r\n");
}

static void SLCD_Show_Letter(void)
{
    int8_t letter;
    int8_t position;

    PRINTF("\r\nShow letter\r\n");

    for (letter = 'A'; letter <= 'z'; letter++)
    {
        for (position = 0; position < NUM_POSEND; position++)
        {
            if (SLCD_Engine_Show_Letter(&slcdEngine, letter, position, 1) == -2)
            {
                PRINTF("\r\nLetter '%c' is not supported by 7-segment display.\r\n", letter);
                continue;
            }
        }

        SLCD_TimeDelay(500);

        for (position = 0; position < NUM_POSEND; position++)
        {
            SLCD_Engine_Show_Letter(&slcdEngine, letter, position, 0);
        }

        if (letter == 'Z')
        {
            letter = 'a';
        }
    }

    PRINTF("\r\nShow letter finished\r\n");
}

static void SLCD_Show_Icon(void)
{
    int32_t icon;

    PRINTF("\r\nTurn on and off the icons one by one\r\n");

    for (icon = 0; icon < ICON_END; icon++)
    {
        SLCD_Engine_Show_Icon(&slcdEngine, icon, 1);

        SLCD_TimeDelay(500);

        SLCD_Engine_Show_Icon(&slcdEngine, icon, 0);
    }

    PRINTF("\r\nShow icons finished\r\n");
}

static void SLCD_Blink(void)
{
    int32_t icon;

    for (icon = 0; icon < ICON_END; icon++)
    {
        SLCD_Engine_Show_Icon(&slcdEngine, icon, 1);
    }

    PRINTF("\r\nSLCD Displays All Segments.\r\n");
    SLCD_TimeDelay(2000);

    PRINTF("\r\nSLCD Starts Blink Mode.\r\n");

    /* Blink mode Display. */
    SLCD_StartBlinkMode(LCD, kSLCD_BlankDisplayBlink, kSLCD_BlinkRate01);
    SLCD_TimeDelay(4000);

    PRINTF("\r\nSLCD Stops Blink Mode.\r\n");
    /* Stops SLCD blink display mode. */
    SLCD_StopBlinkMode(LCD);
    SLCD_TimeDelay(2000);
}

static void SLCD_TimeDelay(uint32_t ms)
{
    SDK_DelayAtLeastUs(1000 * ms, SystemCoreClock);
}

int main(void)
{
    slcd_config_t config;

    /* Initialize MCMGR, install generic event handlers */
    (void)MCMGR_Init();

    /* Hardware initialize. */
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    PRINTF("\r\nSLCD Example Starts from primary core.\r\n");

    /* SLCD get default configure. */
    /*
     * config.displayMode = kSLCD_NormalMode;
     * config.frameFreqIntEnable = false;
     * config.faultConfig = NULL;
     */
    SLCD_GetDefaultConfig(&config);

    /* Verify and Complete the configuration structure. */
    config.dutyCycle          = APP_SLCD_DUTY_CYCLE;
    config.slcdLowPinEnabled  = APP_SLCD_LOW_PIN_ENABLED;
    config.slcdHighPinEnabled = APP_SLCD_HIGH_PIN_ENABLED;
    config.backPlaneLowPin    = APP_SLCD_BACK_PANEL_LOW_PIN;
    config.backPlaneHighPin   = APP_SLCD_BACK_PANEL_HIGH_PIN;

    /* SLCD Initialize. */
    SLCD_Init(LCD, &config);

    BOARD_SetSlcdBackPlanePhase();

    memset(&slcdEngine, 0, sizeof(tSLCD_Engine));

    SLCD_Engine_Init(&slcdEngine, SLCD_SetLCDPin);

    /* Starts SLCD display. */
    SLCD_StartDisplay(LCD);

    SLCD_Show_Digital();

    SLCD_Show_Letter();

    SLCD_Show_Icon();

    SLCD_Blink();

    PRINTF("\r\nSLCD Stops Display.\r\n");
    /* Stops SLCD display. */
    SLCD_StopDisplay(LCD);

    SLCD_Deinit(LCD);

    PRINTF("\r\nSLCD Example Ends on primary core.\r\n");

#ifdef CORE1_IMAGE_COPY_TO_RAM
    /* This section ensures the secondary core image is copied from flash location to the target RAM memory.
       It consists of several steps: image size calculation, image copying and cache invalidation (optional for some
       platforms/cases). These steps are not required on MCUXpresso IDE which copies the secondary core image to the
       target memory during startup automatically. */
    uint32_t core1_image_size;
    core1_image_size = get_core1_image_size();
    (void)PRINTF("Copy Secondary core image to address: 0x%x, size: %d\r\n", (void *)(char *)CORE1_BOOT_ADDRESS,
                 core1_image_size);

    /* Copy Secondary core application from FLASH to the target memory. */
    (void)memcpy((void *)(char *)CORE1_BOOT_ADDRESS, (void *)CORE1_IMAGE_START, core1_image_size);

#ifdef APP_INVALIDATE_CACHE_FOR_SECONDARY_CORE_IMAGE_MEMORY
    /* Invalidate cache for memory range the secondary core image has been copied to. */
    invalidate_cache_for_core1_image_memory(CORE1_BOOT_ADDRESS, core1_image_size);
#endif /* APP_INVALIDATE_CACHE_FOR_SECONDARY_CORE_IMAGE_MEMORY */
#endif /* CORE1_IMAGE_COPY_TO_RAM */

    /* Boot Secondary core application */
    (void)PRINTF("Starting Secondary core.\r\n");
    (void)MCMGR_StartCore(kMCMGR_Core1, (void *)(char *)CORE1_BOOT_ADDRESS, 2, kMCMGR_Start_Synchronous);
    (void)PRINTF("The secondary core application has been started.\r\n");

    while (1)
    {
    }
}
