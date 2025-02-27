/*
 * Copyright 2023-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_clock.h"
#include "fsl_power.h"
#include "fsl_ezhv.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EZHV_M_EXT_INT    (11U)

#define EZHV_ARM2EZHV_PARA_ADDR    (0x2410FF00U)

   /*******************************************************************************
 * Variables
 ******************************************************************************/
static ezhv_callback_t s_ezhvCallback;
static void *s_ezhvCallbackParam;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
void EZHV_Init(ezhv_copy_image_t *ezhvCopyImage)
{
    EZHV_InitWithoutFirmware();
    EZHV_InstallFirmware(ezhvCopyImage);
}

void EZHV_InitWithoutFirmware(void)
{
    CLOCK_EnableClock(kCLOCK_Ezhv);
    CLOCK_EnableClock(kCLOCK_AxbsEzh);

    POWER_DisablePD(kPDRUNCFG_APD_EZHV_TCM);
    POWER_DisablePD(kPDRUNCFG_PPD_EZHV_TCM);
    POWER_ApplyPD();    
}

void EZHV_InstallFirmware(ezhv_copy_image_t *ezhvCopyImage)
{
    uint32_t dstAddr;
    uint32_t srcAddr;
    uint32_t size;
    
    assert(ezhvCopyImage != NULL);
    assert(ezhvCopyImage->srcAddr != NULL);
    assert(ezhvCopyImage->destAddr != NULL);

    dstAddr = ezhvCopyImage->destAddr;
    srcAddr = ezhvCopyImage->srcAddr;
    size = ezhvCopyImage->size;

    memcpy((void*)(uint32_t*)dstAddr, (const void*)(uint32_t*)srcAddr, size);
}

void EZHV_Boot(uint32_t bootAddr, void *para)
{
    SYSCON4->EZHV_RSTBASE = bootAddr >> 2;
    SYSCON4->EZHV_RSTMTVEC = bootAddr;
    SYSCON4->EZHV_VOFFSET_T = EZHV_M_EXT_INT;
    /* reset to start ezhv */
    RESET_PeripheralReset(kEZHV_RST_SHIFT_RSTn);
}

void EZHV_InitPara(void*para, uint32_t size)
{
    if (para == NULL)
    {
        return;
    }
    memcpy((void*)EZHV_ARM2EZHV_PARA_ADDR, para, size);
}

uint32_t *EZHV_GetParaAddr(void)
{
    return (uint32_t *)EZHV_ARM2EZHV_PARA_ADDR;
}

void EZHV_Deinit(void)
{
    CLOCK_DisableClock(kCLOCK_Ezhv);
    CLOCK_DisableClock(kCLOCK_AxbsEzh);

    POWER_EnablePD(kPDRUNCFG_APD_EZHV_TCM);
    POWER_EnablePD(kPDRUNCFG_PPD_EZHV_TCM);
    POWER_ApplyPD();     
}

void EZHV_InstallCallback(ezhv_callback_t callback, void *param)
{
    s_ezhvCallback      = callback;
    s_ezhvCallbackParam      = param;
}

void EZHV_HandleIRQ(void)
{
    if (NULL != s_ezhvCallback)
    {
        s_ezhvCallback(s_ezhvCallbackParam);
    }
}

void EZHV_DriverIRQHandler(void)
{
    EZHV_HandleIRQ();
}
