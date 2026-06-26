/**
 *   @file    ele_hseb_demo.c
 *
 *   @brief   Example of HSEB services
 *   @details Example of ELE HSEB services - get FW version, key and crypto operations
 *
 *   @{
 */
/*--------------------------------------------------------------------------*/
/* Copyright 2025 NXP                                                       */
/*                                                                          */
/* NXP Proprietary. This software is owned or controlled by NXP and may     */
/* only be used strictly in accordance with the applicable license terms.   */
/* By expressly accepting such terms or by downloading, installing,         */
/* activating and/or otherwise using the software, you are agreeing that    */
/* you have read, and that you agree to comply with and are bound by, such  */
/* license terms. If you do not agree to be bound by the applicable license */
/* terms, then you may not retain, install, activate or otherwise use the   */
/* software.                                                                */
/*--------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C"
{
#endif

/*=============================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
=============================================================================*/

#include "hse_host_attrs.h"
#include "hse_demo_app_services.h"
#include "hse_global_variables.h"
#include "hse_common_types.h"
#include "hse_interface.h"
#include "string.h"
#include "fsl_debug_console.h"
#include "app.h"

/**
 * The following symbols are weakly defined to help some compilers deal with
 * undefined symbols that are never referenced.
 */
__attribute__((weak)) void ProgramADKPService(void) { /* Dummy definition */ }
__attribute__((weak)) void Advance_LifeCycle_Service(void) { /* Dummy definition */ }
__attribute__((weak)) void Debug_Auth_Service(void) { /* Dummy definition */ }
__attribute__((weak)) void ExtendCustomerSecurityPolicyService(void) { /* Dummy definition */ }
__attribute__((weak)) void IVT_Auth_Service(void) { /* Dummy definition */ }
__attribute__((weak)) void MU_EnablementService(void) { /* Dummy definition */ }
__attribute__((weak)) void HSE_GetCapabilities_Example(uint8_t *phseCapabilites) { /* Dummy definition */ }
__attribute__((weak)) eHSEFWAttributes gProgramAttributes = { NO_ATTRIBUTE_PROGRAMMED };
__attribute__((weak)) eHSEFWAttributes gEnableIVTAuthBit = { NO_ATTRIBUTE_PROGRAMMED };
__attribute__((weak)) uint32_t HSE_HOST_RAM_DST_START_ADDR[0] = {};
__attribute__((weak)) uint32_t HSE_HOST_FLASH_SRC_START_ADDR[0] = {};
__attribute__((weak)) uint32_t HSE_HOST_FLASH_SRC_END_ADDR[0] = {};

/*=============================================================================
*                 LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
=============================================================================*/
extern void HSE_ReceiveInterruptHandler(uint8_t u8MuInstance);
extern void HSE_GeneralPurposeInterruptHandler(uint8_t u8MuInstance);
/*=============================================================================
*                         LOCAL MACROS
=============================================================================*/

/*=============================================================================
*                         LOCAL CONSTANTS
=============================================================================*/

/*=============================================================================
*                         LOCAL VARIABLES
=============================================================================*/

/*=============================================================================
*                         GLOBAL CONSTANTS
=============================================================================*/

/*=============================================================================
*                         GLOBAL VARIABLES
=============================================================================*/
hseAttrFwVersion_t gHseFwVersion = {0U};
volatile hseSrvResponse_t gsrvResponse = HSE_SRV_RSP_GENERAL_ERROR;
extern volatile fwteststatus_t gInstallHSEFwTest;
/*=============================================================================
*                        GLOBAL FUNCTIONS
* ==========================================================================*/

/******************************************************************************
 * Function:    HSE_GetVersion_Example
 * Description: Example of HSE service - get FW version
 ******************************************************************************/
hseSrvResponse_t HSE_GetVersion_Example(hseAttrFwVersion_t *pHseFwVersion)
{
    hseSrvResponse_t srvResponse;
    srvResponse = Get_Attr(
        HSE_FW_VERSION_ATTR_ID,
        sizeof(hseAttrFwVersion_t),
        pHseFwVersion);

    ASSERT(HSE_SRV_RSP_OK == srvResponse);
    return srvResponse;
}

/******************************************************************************
 * Function:    HSE_SaveVersion
 * Description: saves the firmware version
 ******************************************************************************/
void HSE_SaveVersion(hseAttrFwVersion_t *pOldHseFwVersion, hseAttrFwVersion_t *pHseFwVersion)
{
    pOldHseFwVersion->fwTypeId = pHseFwVersion->fwTypeId;
    pOldHseFwVersion->socTypeId = pHseFwVersion->socTypeId;
    pOldHseFwVersion->majorVersion = pHseFwVersion->majorVersion;
    pOldHseFwVersion->minorVersion = pHseFwVersion->minorVersion;
    pOldHseFwVersion->patchVersion = pHseFwVersion->patchVersion;
}

/******************************************************************************
 * Function:    HSE_CompareVersion
 * Description: compares the two firmware versions
 ******************************************************************************/
bool_t HSE_CompareVersion(hseAttrFwVersion_t *pOldHseFwVersion, hseAttrFwVersion_t *pHseFwVersion)
{
    bool_t flag_versionupdated = FALSE;
    if (
        (pOldHseFwVersion->majorVersion <= pHseFwVersion->majorVersion) ||
        (pOldHseFwVersion->minorVersion <= pHseFwVersion->minorVersion) ||
        (pOldHseFwVersion->patchVersion <= pHseFwVersion->patchVersion) ||
        (pOldHseFwVersion->fwTypeId <= pHseFwVersion->fwTypeId) ||
        (pOldHseFwVersion->socTypeId <= pHseFwVersion->socTypeId))
    {
        flag_versionupdated = TRUE;
    }
    return flag_versionupdated;
}

void getFwVersionService(void)
{
    hseSrvResponse_t gSresponse = HSE_SRV_RSP_GENERAL_ERROR;
    gSresponse = HSE_GetVersion_Example(&gHseFwVersion);

    if (HSE_SRV_RSP_OK == gSresponse)
        testStatus |= GET_VERSION_DONE;
}

/*
 * MU0 Receive Interrupt
 */
void MU0_B_RX_DriverIRQHandler(void)
{
    HSE_ReceiveInterruptHandler(0);
}

/*
 * MU1 Receive Interrupt
 */
void MU1_B_RX_DriverIRQHandler(void)
{
    HSE_ReceiveInterruptHandler(1);
}

/*
 * MU0 General Purpose Interrupt
 */
void MU0_B_DriverIRQHandler(void)
{
    HSE_GeneralPurposeInterruptHandler(0);
}

/*
 * MU1 General Purpose Interrupt
 */
void MU1_B_DriverIRQHandler(void)
{
    HSE_GeneralPurposeInterruptHandler(1);
}

int main()
{
    testStatus = NO_TEST_EXECUTED;

    BOARD_InitHardware();

    PRINTF("* ELE_HSEB Demo started *\r\n\r\n");

    fwversion[0].reserved = gHseFwVersion.reserved;

    /* Check if HSE FW usage flag is already enabled,
     * if not enabled then do not proceed */
    // TODO: need FLASH driver UTEST improvement
    //WaitandSetFWEnablefeatureflag();

    /* Check Fw Install Status */
    WaitForHSEFWInitToFinish();
    PRINTF("Check if HSEB FW is installed...");

    if(gInstallHSEFwTest == FW_INSTALLED)
    {
        PRINTF("HSEB FW is installed\r\n");
    }

    /* Get HSE_FW Version */
    PRINTF("Get FW version...");
    ASSERT(HSE_SRV_RSP_OK == HSE_GetVersion_Example(&gHseFwVersion));
    PRINTF("Sucess\r\n");

    /* Check if NVM and RAM keys already formatted */
    /* format NVM and RAM key catalog */
    /* import keys for cryptographic operation and secure boot */
    HSE_DemoAppConfigKeys();

    /* Erase keys */
    PRINTF("Try erase keys...");
    gsrvResponse = HSE_EraseKeys();
    if (HSE_SRV_RSP_OK == gsrvResponse)
    {
        PRINTF("Success\r\n");
        testStatus |= NVM_DATA_ERASED;
        HSE_DemoAppConfigKeys();
    }

    /*
    * Execute all cryptographic services.
    * Keys shall be formatted prior to executing any cryptographic services.
    */
    /*HSE crypto examples: sym/asym services; sync/async operation mode*/
    PRINTF("\r\nExercise crypto operations:\r\n");
    gsrvResponse = HSE_Crypto();


    if (gsrvResponse == HSE_SRV_RSP_OK)
    {
        PRINTF("\r\nAll tests passed!!\r\n");
    }
    else
    {
        PRINTF("FAIL!!\r\n");
    }

    PRINTF("Demo end\r\n");

    while(1)
    {
        GETCHAR();
    }
}
#ifdef __cplusplus
}
#endif

/** @} */
