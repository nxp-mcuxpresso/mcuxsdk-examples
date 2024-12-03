/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void APP_InitCore1Domain(void)
{
}

uint8_t APP_GetCore1DomainID(void)
{
    return 4U;
}
void APP_InitInterCoreNotifications(void)
{
    /* Init Mailbox */
    MAILBOX_Init(MAILBOX);
}
uint32_t APP_GetInterCoreNotificationsData(void)
{
    return MAILBOX_GetValue(MAILBOX, SECONDARY_CORE_MAILBOX_CPU_ID);
}
void APP_SetInterCoreNotificationsData(uint32_t data)
{
    MAILBOX_SetValue(MAILBOX, PRIMARY_CORE_MAILBOX_CPU_ID, data);
}

void BOARD_InitHardware(void)
{
    /* enable clock for GPIO */
    CLOCK_EnableClock(kCLOCK_Gpio0);
    BOARD_InitBootPins();
}
/*${function:end}*/
