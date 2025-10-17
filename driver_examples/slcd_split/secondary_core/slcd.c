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
}

static void SLCD_Show_Letter(void)
{
    int8_t letter;
    int8_t position;

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
}

static void SLCD_Show_Icon(void)
{
    int32_t icon;

    for (icon = 0; icon < ICON_END; icon++)
    {
        SLCD_Engine_Show_Icon(&slcdEngine, icon, 1);

        SLCD_TimeDelay(500);

        SLCD_Engine_Show_Icon(&slcdEngine, icon, 0);
    }
}

static void SLCD_Blink(void)
{
    int32_t icon;

    for (icon = 0; icon < ICON_END; icon++)
    {
        SLCD_Engine_Show_Icon(&slcdEngine, icon, 1);
    }

    SLCD_TimeDelay(2000);

    /* Blink mode Display. */
    SLCD_StartBlinkMode(LCD, kSLCD_BlankDisplayBlink, kSLCD_BlinkRate01);
    SLCD_TimeDelay(4000);

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
    uint32_t startupData, i;
    slcd_config_t config;
    mcmgr_status_t status;

    /* Hardware initialize. */
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    /* Initialize MCMGR, install generic event handlers */
    (void)MCMGR_Init();

    /* Get the startup data */
    do
    {
        status = MCMGR_GetStartupData(kMCMGR_Core0, &startupData);
    } while (status != kStatus_MCMGR_Success);

    /* Make a noticable delay after the reset */
    /* Use startup parameter from the master core... */
    for (i = 0; i < startupData; i++)
    {
        SDK_DelayAtLeastUs(1000000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    }

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

    /* Stops SLCD display. */
    SLCD_StopDisplay(LCD);

    SLCD_Deinit(LCD);

    while (1)
    {
    }
}
