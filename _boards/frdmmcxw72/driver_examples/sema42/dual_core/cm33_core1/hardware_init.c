/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

static const uint32_t froPostDivFreq[] = {
    16000000U, 24000000U, 32000000U, 48000000U, 64000000U, 0U, 0U, 0U
};
/*${function:start}*/
void APP_InitCore1Domain(void)
{
}

uint8_t APP_GetCore1DomainID(void)
{
    return 5U;
}
void APP_InitInterCoreNotifications(void)
{
    /* Init IMU */
    IMU_Init(kIMU_LinkCpu2Cpu1);
}
uint32_t APP_GetInterCoreNotificationsData(void)
{
    uint32_t msg;
    IMU_ReceiveMsgsBlocking(kIMU_LinkCpu2Cpu1, &msg, 1UL, NULL);
    return msg;
}
void APP_SetInterCoreNotificationsData(uint32_t data)
{
    IMU_SendMsgsBlocking(kIMU_LinkCpu2Cpu1, &data, 1UL, false);
}

void BOARD_InitHardware(void)
{
    BOARD_InitPins();

    uint32_t froPostDivSel = (FRO192M0->FROCCSR & FRO192M_FROCCSR_POSTDIV_SEL_MASK) >> FRO192M_FROCCSR_POSTDIV_SEL_SHIFT;
    SystemCoreClock = froPostDivFreq[froPostDivSel];
}
/*${function:end}*/
