/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "usb_pd_config.h"
#include "usb_pd.h"
#include "usb_pd_i2c.h"
#include "fsl_adapter_gpio.h"
#include "pd_app_misc.h"
#include "usb_pd_alt_mode.h"
#include "usb_pd_alt_mode_dp.h"
#include "pd_dp_hpd_detect.h"
#include "pd_dp_board_chip.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define TIMR_TICK_USEC_TO_TICKS(n) (n)
#define TIMR_TICK_MSEC_TO_TICKS(n) ((n)*1000)
#define mSec(n)                    (n)

// IRQ_HPD Indicates an IRQ_HPD (i.e., a high-to-low transition on HPD
// followed by a low-to-high transition was detected between 250us and
// 2ms later, as specified in DP v1.3).

#define HPD_MAX_GLITCH_TICKS (TIMR_TICK_USEC_TO_TICKS(250) - 1) // Under this time is a glitch
#define HPD_MIN_IRQ_TICKS    TIMR_TICK_USEC_TO_TICKS(250)       // Must be at least this long for IRQ_HPD,
#define HPD_MAX_IRQ_TICKS    (TIMR_TICK_MSEC_TO_TICKS(2))       // but can't be greater then this value.

#define HPD_RISE_MIN_DEBOUNCE_TICKS TIMR_TICK_MSEC_TO_TICKS(100) // Level must be stable for at least this time

// The logical state of HPD transitions from high to low when a low
// level on the HPD link has been detected for 2ms (i.e., longer than
// the maximum IRQ_HPD pulse detection time). The logical state of HPD
// is unchanged during glitches (as specified in DP v1.3) on the HPD
// link
#define HPD_FALL_MIN_DEBOUNCE_TICKS TIMR_TICK_MSEC_TO_TICKS(2)
#define T_HPD_FALL_MIN_DEBOUNCE     mSec(2 + 1)   // add 1 as this timer is less accurate
#define T_HPD_RISE_MIN_DEBOUNCE     mSec(100 + 1) // Add 1 ms, as this timer is less accurate than the tick

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

uint8_t PD_DemoTimrTickIsTimeAfter(uint32_t firstIntervalVal, uint32_t endIntervalVal);
uint32_t PD_DemoGetTickValue(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

void PD_DpHpdTimrProgressChangeCallback(pd_hpd_detect_t *hpdDetect);
void PD_DpHpdDetect1msISR(pd_hpd_detect_t *hpdDetect)
{
    if (hpdDetect->debounceTime > 0)
    {
        hpdDetect->debounceTime--;
        if (hpdDetect->debounceTime == 0)
        {
            PD_DpHpdTimrProgressChangeCallback(hpdDetect);
        }
    }
}

void PD_DpCtrlHpdDetectQueueUpdate(pd_hpd_detect_t *hpdDetect, pd_hpd_detect_type_t detectVal)
{
    pd_alt_mode_control_t dpControl;
    dpControl.controlCode        = kDPControl_HPDDetectEvent;
    dpControl.controlParam       = &detectVal;
    dpControl.altModeModuleIndex = 1;
    PD_Control(hpdDetect->pdHandle, PD_CONTROL_ALT_MODE, &dpControl);
}

static void PD_DpHpdDetectSetHPDDebounceTimer(pd_hpd_detect_t *hpdDetect, uint32_t timeVal)
{
    APP_CRITICAL_ALLOC();

    APP_ENTER_CRITICAL();
    hpdDetect->debounceTime = timeVal;
    APP_EXIT_CRITICAL();
}

static void PD_DpHpdDetectCancelHPDDebounceTimer(pd_hpd_detect_t *hpdDetect)
{
    APP_CRITICAL_ALLOC();

    APP_ENTER_CRITICAL();
    hpdDetect->debounceTime = 0;
    APP_EXIT_CRITICAL();
}

void PD_DpHpdDetectStart(pd_hpd_detect_t *hpdDetect)
{
    hpdDetect->hpdDetectQueueEnable = 0;

    /* Setup for the first detection (even if not in alt-mode) */
    HAL_GpioGetInput((hal_gpio_handle_t)(&hpdDetect->gpioHpdHandle[0]), (uint8_t *)&hpdDetect->hpdDetectHigh);
    hpdDetect->hpdDetectChangeTick = PD_DemoGetTickValue();

    if (hpdDetect->hpdDetectHigh)
    {
        PD_DpHpdDetectSetHPDDebounceTimer(hpdDetect, T_HPD_RISE_MIN_DEBOUNCE);
    }
    else
    {
        PD_DpHpdDetectSetHPDDebounceTimer(hpdDetect, T_HPD_FALL_MIN_DEBOUNCE);
    }
}

void PD_DpHpdDetectStop(pd_hpd_detect_t *hpdDetect)
{
    PD_DpHpdDetectCancelHPDDebounceTimer(hpdDetect);
}

void PD_DpHpdModeEnterEnableDetectQueue(pd_hpd_detect_t *hpdDetect)
{
    APP_CRITICAL_ALLOC();

    APP_ENTER_CRITICAL();
    hpdDetect->hpdDetectQueueEnable = 1;
    APP_EXIT_CRITICAL();
}

void PD_DpHpdModeExitClearDetectQueue(pd_hpd_detect_t *hpdDetect)
{
    APP_CRITICAL_ALLOC();

    APP_ENTER_CRITICAL();
    hpdDetect->hpdDetectQueueEnable = 0;
    APP_EXIT_CRITICAL();
}

uint8_t PD_DpHpdDetectGetPinValue(pd_hpd_detect_t *hpdDetect)
{
    if (hpdDetect->hpdHigh)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

static void PD_DpHpdDetectQueueAddLevel(pd_hpd_detect_t *hpdDetect, uint8_t highLevel)
{
    if (!(hpdDetect->hpdDetectQueueEnable))
    {
        return;
    }

    if (!highLevel)
    {
        /* Low detected clears the queue */
        PD_DpCtrlHpdDetectQueueUpdate(hpdDetect, kDPHPDDetect_Low);
    }
    else
    {
        /* Just add the high in (shouldn't go beyond the end of the queue, but better to be safe) */
        PD_DpCtrlHpdDetectQueueUpdate(hpdDetect, kDPHPDDetect_High);
    }
}

static void PD_DpHpdDetectQueueAddIrq(pd_hpd_detect_t *hpdDetect)
{
    /* Discard if there are already two irqs in the queue */
    if (!(hpdDetect->hpdDetectQueueEnable))
    {
        return;
    }

    PD_DpCtrlHpdDetectQueueUpdate(hpdDetect, kDPHPDDetect_IRQ);
}

static void PD_DpHpdDetectChange(pd_hpd_detect_t *hpdDetect, uint8_t hpdPinVal)
{
    uint32_t detectTick = PD_DemoGetTickValue();

    /* Filter out glitches, detect IRQ, and change hpdDetectHigh */
    if (hpdDetect->hpdDetectHigh != hpdPinVal)
    {
        if (hpdDetect->hpdDetectChangeTick &&
            (!PD_DemoTimrTickIsTimeAfter(detectTick, hpdDetect->hpdDetectChangeTick + HPD_MAX_GLITCH_TICKS)))
        {
            /* Glitch detection -- if it's a glitch then filter the change out */
            /* Glitch is rising or falling */
            hpdDetect->hpdDetectChangeTick = 0;
            PD_DpHpdDetectCancelHPDDebounceTimer(hpdDetect);

            /* Restore the present value of HPD */
            hpdDetect->hpdDetectHigh = hpdPinVal;
        }
        else
        {
            /* is it an IRQ? -- An HPD IRQ will have started high, go low
             * (timer starts), then go back high between 250us and 2000us.
             * Here we are looking at the time from going low to coming back high */
            if (hpdDetect->hpdDetectChangeTick && hpdPinVal &&
                PD_DemoTimrTickIsTimeAfter(detectTick, hpdDetect->hpdDetectChangeTick + HPD_MIN_IRQ_TICKS) &&
                (!PD_DemoTimrTickIsTimeAfter(detectTick, hpdDetect->hpdDetectChangeTick + HPD_MAX_IRQ_TICKS)))
            {
                /* Make sure that the IRQ doesn't also detect as a low level */
                hpdDetect->hpdDetectChangeTick = 0;
                PD_DpHpdDetectCancelHPDDebounceTimer(hpdDetect);

                /* This is a valid HPD IRQ! -- leave the state as high
                 * but add the IRQ to the detect queue */
                PD_DpHpdDetectQueueAddIrq(hpdDetect);

                /* Restore the present value of HPD */
                hpdDetect->hpdDetectHigh = hpdPinVal;
                /* NOTE: IRQ will not change the hpdHigh value */
            }
            else if (hpdDetect->hpdDetectChangeTick && hpdPinVal &&
                     PD_DemoTimrTickIsTimeAfter(detectTick,
                                                hpdDetect->hpdDetectChangeTick + HPD_FALL_MIN_DEBOUNCE_TICKS))
            {
                /* The HPD has gone low, and then high again, but was low for more than 2ms
                 * This should be registered as a LOW pulse, and then the high edge should be debounced */

                /* Which will be low, the previous low before this high */
                hpdDetect->hpdHigh             = hpdDetect->hpdDetectHigh;
                hpdDetect->hpdDetectChangeTick = 0;
                PD_DpHpdDetectCancelHPDDebounceTimer(hpdDetect);
                PD_DpHpdDetectQueueAddLevel(hpdDetect, hpdDetect->hpdHigh);

                /* Now debounce the rising edge */
                hpdDetect->hpdDetectHigh       = hpdPinVal;
                hpdDetect->hpdDetectChangeTick = detectTick;
                PD_DpHpdDetectSetHPDDebounceTimer(hpdDetect, T_HPD_RISE_MIN_DEBOUNCE);
            }
            else
            {
                /* Not an HPD IRQ, or LOW,
                 * new value. This value then has to go through debounce
                 * check which is done in HpdDetectProgress(). */
                hpdDetect->hpdDetectHigh = hpdPinVal;
                if (hpdDetect->hpdHigh != hpdDetect->hpdDetectHigh)
                {
                    /* Not a glitch, the detected value is different to the current debounced state
                     * Change was detected */
                    hpdDetect->hpdDetectChangeTick = detectTick;

                    if (hpdPinVal)
                    {
                        PD_DpHpdDetectSetHPDDebounceTimer(hpdDetect, T_HPD_RISE_MIN_DEBOUNCE);
                    }
                    else
                    {
                        PD_DpHpdDetectSetHPDDebounceTimer(hpdDetect, T_HPD_FALL_MIN_DEBOUNCE);
                    }
                }
                else
                {
                    /* Change was restored, but not longer than debounce time (rise or fall) */
                }
            }
        }
    }
}

void PD_DpHpdDetectChangeCallback(void *callbackParam)
{
    pd_hpd_detect_t *hpdDetect = (pd_hpd_detect_t *)callbackParam;
    uint8_t pinState;

    HAL_GpioGetInput((hal_gpio_handle_t)(&hpdDetect->gpioHpdHandle[0]), &pinState);
    PD_DpHpdDetectChange(hpdDetect, pinState);
}

void PD_DpHpdTimrProgressChangeCallback(pd_hpd_detect_t *hpdDetect)
{
    APP_CRITICAL_ALLOC();
    /* is there a timer going? That is the only thing we have to progress.
     * We only need to look for a non-irq high or low */
    if (hpdDetect->hpdDetectChangeTick)
    {
        /* the value could change in the IRQ, so if we have a timer then do
         * the rest in a critical section */
        APP_ENTER_CRITICAL();
        hpdDetect->hpdHigh             = hpdDetect->hpdDetectHigh;
        hpdDetect->hpdDetectChangeTick = 0;
        PD_DpHpdDetectCancelHPDDebounceTimer(hpdDetect);
        PD_DpHpdDetectQueueAddLevel(hpdDetect, hpdDetect->hpdHigh);
        APP_EXIT_CRITICAL();
    }
}

uint8_t PD_DpHpdDetectInit(pd_hpd_detect_t *hpdDetect, void *pdHandle, void *hpdConfig)
{
    hal_gpio_pin_config_t pinConfig;

    hpdDetect->pdHandle             = pdHandle;
    hpdDetect->hpdDetectChangeTick  = 0;
    hpdDetect->hpdDetectHigh        = 0;
    hpdDetect->hpdDetectQueueEnable = 0;
    hpdDetect->hpdDetectConfig      = (pd_hpd_detect_config_t *)hpdConfig;

    pinConfig.direction = kHAL_GpioDirectionIn;
    pinConfig.port      = hpdDetect->hpdDetectConfig->hpdDetectPort;
    pinConfig.pin       = hpdDetect->hpdDetectConfig->hpdDetectPin;
    pinConfig.level     = 0;
    HAL_GpioInit((hal_gpio_handle_t *)(&hpdDetect->gpioHpdHandle[0]), &pinConfig);

    HAL_GpioInstallCallback((hal_gpio_handle_t *)(&hpdDetect->gpioHpdHandle[0]), PD_DpHpdDetectChangeCallback,
                            (void *)hpdDetect);
    HAL_GpioSetTriggerMode((hal_gpio_handle_t *)(&hpdDetect->gpioHpdHandle[0]), kHAL_GpioInterruptEitherEdge);

    return 0;
}

uint8_t PD_DpHpdDetectDeinit(pd_hpd_detect_t *hpdDetect)
{
    HAL_GpioDeinit((hal_gpio_handle_t *)(&hpdDetect->gpioHpdHandle[0]));
    return 0;
}
