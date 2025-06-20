/*
 * Copyright 2022, 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "fwk_platform.h"
#include "fwk_platform_ble.h"
#include "fwk_platform_ics.h" /* to get NbuInfo */
#include "assert.h"
#include "stdbool.h"

/* -------------------------------------------------------------------------- */
/*                              Private functions                             */
/* -------------------------------------------------------------------------- */

static void APP_RecoverLinkLayerFimrware(int reason)
{
    /* Customer shall add any code that could help for Software/Hardware recovery here */
    (void)reason;
    while (true)
    {
    }
}

static bool App_CheckNbuCompatibility(NbuInfo_t *nbu_info_p)
{
    /* Do whatever compatibility check with NBU version and return false for requesting recovery */
    (void)nbu_info_p;
    return true;
}

/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */

int APP_InitBle(void)
{
    int              status = 0;
    static NbuInfo_t nbu_info;

    do
    {
        status = PLATFORM_InitTimerManager();
        if (status < 0)
        {
            break;
        }

        /* Start link layer firmware and set up HCI link */
        status = PLATFORM_InitBle();
        if (status < 0)
        {
            /* Platform Ble can not start, start recovery */
            APP_RecoverLinkLayerFimrware(1);
            break;
        }

        status = PLATFORM_GetNbuInfo(&nbu_info);
        if ((status < 0) || (App_CheckNbuCompatibility(&nbu_info) == false))
        {
            /* Link Layer version is not compatible, start recovery */
            APP_RecoverLinkLayerFimrware(2);
            break;
        }

    } while (false);

#if defined(gAppDisableControllerLowPower_d) && (gAppDisableControllerLowPower_d > 0)
    if (status == 0)
    {
        /* Disallow Controller low power entry
         * Depending on the platform, this can concern multiple controllers
         * Controller low power is always enabled by default, so this should be
         * called mainly for debug purpose
         */
        PLATFORM_DisableControllerLowPower();
    }
#endif
#if defined(gAppHighNBUClockFrequency_d) && (gAppHighNBUClockFrequency_d > 0)
    /* some Link layer use cases require radio core (NBU) to run at higher frequency :
     * Increase LDO core voltage to 1.1v - make sure the DCDC output voltage is at least 1.35mV
     * Request Radio core to switch to higher frequency
     * @warning : make sure LDO core is not decreased to lower voltage after this step (when going to low power for instance)
     **/
    PLATFORM_SetLdoCoreNormalDriveVoltage();
    PLATFORM_SetNbuConstraintFrequency(PLATFORM_NBU_MIN_FREQ_64MHZ);
#endif
    return status;
}