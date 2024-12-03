/*
 * Copyright 2020-2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_flexram.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static status_t OCRAM_Reallocate(void);
static void OCRAM_Access(void);
static void DTCM_Access(void);
static void ITCM_Access(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Boundary flag */
static volatile bool s_flexram_ocram_access_error_match = false;
static volatile bool s_flexram_dtcm_access_error_match  = false;
static volatile bool s_flexram_itcm_access_error_match  = false;
/* ECC error flag */
static volatile bool s_flexram_ocram_ecc_single_error = false;
static volatile bool s_flexram_ocram_ecc_multi_error  = false;
static volatile bool s_flexram_dtcm_ecc_single_error  = false;
static volatile bool s_flexram_dtcm_ecc_multi_error   = false;
static volatile bool s_flexram_itcm_ecc_single_error  = false;
static volatile bool s_flexram_itcm_ecc_multi_error   = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
void APP_FLEXRAM_IRQ_HANDLER(void)
{
    if (FLEXRAM_GetInterruptStatus(APP_FLEXRAM) & kFLEXRAM_OCRAMAccessError)
    {
        FLEXRAM_ClearInterruptStatus(APP_FLEXRAM, kFLEXRAM_OCRAMAccessError);
        s_flexram_ocram_access_error_match = true;
    }

    if (FLEXRAM_GetInterruptStatus(APP_FLEXRAM) & kFLEXRAM_OCRAMECCSingleError)
    {
        FLEXRAM_ClearInterruptStatus(APP_FLEXRAM, kFLEXRAM_OCRAMECCSingleError);
        s_flexram_ocram_ecc_single_error = true;
    }

    if (FLEXRAM_GetInterruptStatus(APP_FLEXRAM) & kFLEXRAM_DTCMAccessError)
    {
        FLEXRAM_ClearInterruptStatus(APP_FLEXRAM, kFLEXRAM_DTCMAccessError);
        s_flexram_dtcm_access_error_match = true;
    }

    if (FLEXRAM_GetInterruptStatus(APP_FLEXRAM) & kFLEXRAM_D0TCMECCSingleError)
    {
        FLEXRAM_ClearInterruptStatus(APP_FLEXRAM, kFLEXRAM_D0TCMECCSingleError);
        s_flexram_dtcm_ecc_single_error = true;
    }

    if (FLEXRAM_GetInterruptStatus(APP_FLEXRAM) & kFLEXRAM_ITCMAccessError)
    {
        FLEXRAM_ClearInterruptStatus(APP_FLEXRAM, kFLEXRAM_ITCMAccessError);
        s_flexram_itcm_access_error_match = true;
    }

    if (FLEXRAM_GetInterruptStatus(APP_FLEXRAM) & kFLEXRAM_ITCMECCSingleError)
    {
        FLEXRAM_ClearInterruptStatus(APP_FLEXRAM, kFLEXRAM_ITCMECCSingleError);
        s_flexram_itcm_ecc_single_error = true;
    }

    SDK_ISR_EXIT_BARRIER;
}

void APP_FLEXRAM_ECC_IRQ_HANDLER(void)
{
    if (FLEXRAM_GetInterruptStatus(APP_FLEXRAM) & kFLEXRAM_OCRAMECCMultiError)
    {
        FLEXRAM_ClearInterruptStatus(APP_FLEXRAM, kFLEXRAM_OCRAMECCMultiError);
        s_flexram_ocram_ecc_multi_error = true;
    }

    if (FLEXRAM_GetInterruptStatus(APP_FLEXRAM) & kFLEXRAM_D0TCMECCMultiError)
    {
        FLEXRAM_ClearInterruptStatus(APP_FLEXRAM, kFLEXRAM_D0TCMECCMultiError);
        s_flexram_dtcm_ecc_multi_error = true;
    }

    if (FLEXRAM_GetInterruptStatus(APP_FLEXRAM) & kFLEXRAM_ITCMECCMultiError)
    {
        FLEXRAM_ClearInterruptStatus(APP_FLEXRAM, kFLEXRAM_ITCMECCMultiError);
        s_flexram_itcm_ecc_multi_error = true;
    }

    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
#if APP_USING_CACHE
#include "fsl_cache.h"
#endif

int main(void)
{
    /* Board pin, clock, debug console init */
    BOARD_InitHardware();

    PRINTF("\r\nFLEXRAM ECC example.\r\n");

    /* enable IRQ */
    EnableIRQ(APP_FLEXRAM_IRQ);
    EnableIRQ(APP_FLEXRAM_ECC_IRQ);
    /* reallocate ram */
    OCRAM_Reallocate();
    /* init flexram */
    FLEXRAM_Init(APP_FLEXRAM);
    /* test OCRAM access*/
    OCRAM_Access();
    /* test DTCM access*/
    DTCM_Access();
    /* test ITCM access*/
    ITCM_Access();

    PRINTF("\r\nFLEXRAM ECC example finished.\r\n");

    while (1)
    {
    }
}

static status_t OCRAM_Reallocate(void)
{
    flexram_allocate_ram_t ramAllocate = {
        .ocramBankNum = APP_OCRAM_ALLOCATE_BANK_NUM,
        .dtcmBankNum  = APP_DTCM_ALLOCATE_BANK_NUM,
        .itcmBankNum  = APP_ITCM_ALLOCATE_BANK_NUM,
    };

    PRINTF("\r\nAllocate on-chip ram:\r\n");
    PRINTF("\r\n   OCRAM bank numbers %d\r\n", ramAllocate.ocramBankNum);
    PRINTF("\r\n   DTCM  bank numbers %d\r\n", ramAllocate.dtcmBankNum);
    PRINTF("\r\n   ITCM  bank numbers %d\r\n", ramAllocate.itcmBankNum);

    if (FLEXRAM_AllocateRam(&ramAllocate) != kStatus_Success)
    {
        PRINTF("\r\nAllocate on-chip ram fail\r\n");
        return kStatus_Fail;
    }
    else
    {
        PRINTF("\r\nAllocate on-chip ram success\r\n");
    }

    return kStatus_Success;
}

static void OCRAM_Access(void)
{
    volatile uint32_t *ocramAddr = (volatile uint32_t *)APP_FLEXRAM_OCRAM_START_ADDR;
    uint32_t ocramReadBuffer     = 0x00U;
    flexram_ocram_ecc_single_error_info_t info;
    flexram_ecc_error_type_t ocramECCErrorInfo = {0x01, 0x00, true, false, false, false};

    /* Enable OCRAM ECC function. */
    FLEXRAM_EnableECC(APP_FLEXRAM, true, false);
    /* Enable FLEXRAM OCRAM access error interrupt and ECC single error interrupt. */
    FLEXRAM_EnableInterruptSignal(APP_FLEXRAM, kFLEXRAM_OCRAMAccessError | kFLEXRAM_OCRAMECCSingleError);

    /* FEXRAM ECC error injection. */
    FLEXRAM_ErrorInjection(APP_FLEXRAM, kFLEXRAM_OCRAM, &ocramECCErrorInfo);

    for (;;)
    {
        *ocramAddr = 0xCCU;
        /* Synchronizes the execution stream with memory accesses */
        __DSB();
        __ISB();

#if APP_USING_CACHE
        DCACHE_CleanByRange((uint32_t)ocramAddr, sizeof(uint32_t));
#endif

        /* check ocram access error event */
        if (s_flexram_ocram_access_error_match)
        {
            s_flexram_ocram_access_error_match = false;
            PRINTF("\r\nOCRAM access to 0x%x boundary.\r\n", ocramAddr);
            break;
        }

        /* If there has ECC error, it will be generate ECC error interrupt. */
        ocramReadBuffer = *ocramAddr;

        if (s_flexram_ocram_ecc_single_error)
        {
            s_flexram_ocram_ecc_single_error = false;
            FLEXRAM_EnableECC(APP_FLEXRAM, false, false);

            FLEXRAM_GetOcramSingleErroInfo(APP_FLEXRAM, &info);
#if defined(FLEXRAM_ECC_ERROR_DETAILED_INFO) && FLEXRAM_ECC_ERROR_DETAILED_INFO
            PRINTF("\r\nOCRAM single-bit ECC error corresponding ECC cipher of OCRAM single-bit ECC error: 0x%x.\r\n",
                   info.OcramSingleErrorECCCipher);
            PRINTF("\r\nOCRAM single-bit ECC error corresponding ECC syndrome of OCRAM single-bit ECC error: 0x%x.\r\n",
                   info.OcramSingleErrorECCSyndrome);
#else
            PRINTF("\r\nOCRAM single error information: 0x%x.\r\n", info.OcramSingleErrorInfo);
#endif /*FLEXRAM_ECC_ERROR_DETAILED_INFO*/
            PRINTF("\r\nOCRAM single-bit ECC error address: 0x%x.\r\n", info.OcramSingleErrorAddr);
            PRINTF("\r\nOCRAM single-bit ECC error data LSB: 0x%x.\r\n", info.OcramSingleErrorDataLSB);
            PRINTF("\r\nOCRAM single-bit ECC error data MSB: 0x%x.\r\n", info.OcramSingleErrorDataMSB);

            PRINTF("\r\nOCRAM expected data LSB: 0x%x.\r\n", ocramReadBuffer);
        }

        ocramAddr++;
    }
}

static void DTCM_Access(void)
{
    volatile uint32_t *dtcmAddr = (volatile uint32_t *)APP_FLEXRAM_DTCM_START_ADDR;
    uint32_t dtcmReadBuffer     = 0x00U;
    uint32_t dtcmEndAddr        = APP_FLEXRAM_DTCM_START_ADDR + 0x8000U * APP_DTCM_ALLOCATE_BANK_NUM - 0x400U;
    flexram_dtcm_ecc_single_error_info_t info;
    flexram_ecc_error_type_t dtcmECCErrorInfo = {0x01, 0x00, true, false, false, false};

    /* Enable OCRAM/TCM ECC function. */
    FLEXRAM_EnableECC(APP_FLEXRAM, true, true);
    /* Enable FLEXRAM DTCM ECC access error interrupt and single error interrupt */
    FLEXRAM_EnableInterruptSignal(APP_FLEXRAM, kFLEXRAM_DTCMAccessError | kFLEXRAM_D0TCMECCSingleError);

    /* FEXRAM ECC error injection. */
    FLEXRAM_ErrorInjection(APP_FLEXRAM, kFLEXRAM_D0TCM, &dtcmECCErrorInfo);

    for (;;)
    {
        *dtcmAddr = 0x00U;
        /* Synchronizes the execution stream with memory accesses */
        __DSB();
        __ISB();

        /* If there has ECC error, it will be generate ECC error interrupt. */
        dtcmReadBuffer = *(volatile uint32_t *)dtcmAddr;

        if (s_flexram_dtcm_ecc_single_error)
        {
            s_flexram_dtcm_ecc_single_error = false;
            FLEXRAM_EnableECC(APP_FLEXRAM, false, false);

            FLEXRAM_GetDtcmSingleErroInfo(APP_FLEXRAM, &info, 0x00);
#if defined(FLEXRAM_ECC_ERROR_DETAILED_INFO) && FLEXRAM_ECC_ERROR_DETAILED_INFO
            PRINTF("\r\nDTCM single-bit ECC error bit position: 0x%x.\r\n", info.DtcmSingleErrorBitPostion);
            PRINTF("\r\nDTCM single-bit ECC error TCM privilege: 0x%x.\r\n", info.DtcmSingleErrorTCMPrivilege);
            PRINTF("\r\nDTCM single-bit ECC error TCM master: 0x%x.\r\n", info.DtcmSingleErrorTCMMaster);
            PRINTF("\r\nDTCM single-bit ECC error TCM access size: 0x%x.\r\n", info.DtcmSingleErrorTCMAccessSize);
            PRINTF("\r\nDTCM single-bit ECC error TCM write or read: 0x%x.\r\n", info.DtcmSingleErrorTCMWriteRead);
#else
            PRINTF("\r\nDTCM single error information: 0x%x.\r\n", info.DtcmSingleErrorInfo);
#endif /*FLEXRAM_ECC_ERROR_DETAILED_INFO*/
            PRINTF("\r\nDTCM single-bit ECC error address: 0x%x.\r\n", info.DtcmSingleErrorAddr);
            PRINTF("\r\nDTCM single-bit ECC error data: 0x%x.\r\n", info.DtcmSingleErrorData);

            PRINTF("\r\nDTCM real read data: 0x%x.\r\n", dtcmReadBuffer);
        }

        dtcmAddr++;

        /* Break to avoid hardfault when access to DTCM boundary;
           Otherwise, it will be hardfault and assert DTCM access error IRQ synchronously. */
        if (dtcmEndAddr == (uint32_t)dtcmAddr)
        {
            PRINTF("\r\nDTCM access to 0x%x boundary.\r\n", dtcmAddr);
            break;
        }
    }
}

static void ITCM_Access(void)
{
    volatile uint32_t *itcmAddr = (volatile uint32_t *)APP_FLEXRAM_ITCM_START_ADDR;
    uint32_t itcmReadBuffer     = 0x00U;
    uint32_t itcmEndAddr        = APP_FLEXRAM_ITCM_START_ADDR + 0x8000U * APP_ITCM_ALLOCATE_BANK_NUM;
    flexram_itcm_ecc_single_error_info_t info;
    flexram_ecc_error_type_t itcmECCErrorInfo = {0x01, 0x00, true, false, false, false};

    /* Enable OCRAM/TCM ECC function. */
    FLEXRAM_EnableECC(APP_FLEXRAM, true, true);
    /* Enable FLEXRAM DTCM ECC access error interrupt and single error interrupt */
    FLEXRAM_EnableInterruptSignal(APP_FLEXRAM, kFLEXRAM_ITCMAccessError | kFLEXRAM_ITCMECCSingleError);

    /* FEXRAM ECC error injection. */
    FLEXRAM_ErrorInjection(APP_FLEXRAM, kFLEXRAM_ITCM, &itcmECCErrorInfo);

    for (;;)
    {
        *itcmAddr = 0x00U;
        /* Synchronizes the execution stream with memory accesses */
        __DSB();
        __ISB();

        /* If there has ECC error, it will be generate ECC error interrupt. */
        itcmReadBuffer = *(volatile uint32_t *)itcmAddr;

        if (s_flexram_itcm_ecc_single_error)
        {
            s_flexram_itcm_ecc_single_error = false;
            FLEXRAM_EnableECC(APP_FLEXRAM, false, false);

            FLEXRAM_GetItcmSingleErroInfo(APP_FLEXRAM, &info);
#if defined(FLEXRAM_ECC_ERROR_DETAILED_INFO) && FLEXRAM_ECC_ERROR_DETAILED_INFO
            PRINTF("\r\nITCM single-bit ECC error bit position: 0x%x.\r\n", info.ItcmSingleErrorBitPostion);
            PRINTF("\r\nITCM single-bit ECC error TCM privilege: 0x%x.\r\n", info.ItcmSingleErrorTCMPrivilege);
            PRINTF("\r\nITCM single-bit ECC error TCM master: 0x%x.\r\n", info.ItcmSingleErrorTCMMaster);
            PRINTF("\r\nITCM single-bit ECC error TCM access size: 0x%x.\r\n", info.ItcmSingleErrorTCMAccessSize);
            PRINTF("\r\nITCM single-bit ECC error TCM write or read: 0x%x.\r\n", info.ItcmSingleErrorTCMWriteRead);
#else
            PRINTF("\r\nITCM single error information: 0x%x.\r\n", info.ItcmSingleErrorInfo);
#endif /*FLEXRAM_ECC_ERROR_DETAILED_INFO*/
            PRINTF("\r\nITCM single-bit ECC error address: 0x%x.\r\n", info.ItcmSingleErrorAddr);
            PRINTF("\r\nITCM single-bit ECC error data LSB: 0x%x.\r\n", info.ItcmSingleErrorDataLSB);
            PRINTF("\r\nITCM single-bit ECC error data MSB: 0x%x.\r\n", info.ItcmSingleErrorDataMSB);

            PRINTF("\r\nITCM real read data: 0x%x.\r\n", itcmReadBuffer);
        }

        itcmAddr++;

        /* Break to avoid hardfault when access to ITCM boundary;
           Otherwise, it will be hardfault and assert DTCM access error IRQ synchronously. */
        if (itcmEndAddr == (uint32_t)itcmAddr)
        {
            PRINTF("\r\nITCM access to 0x%x boundary.\r\n", itcmAddr);
            break;
        }
    }
}
