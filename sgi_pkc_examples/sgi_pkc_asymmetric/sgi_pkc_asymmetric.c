/*
 * Copyright 2025-2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "mcuxCsslExamples.h"
#include "sgi_pkc_asymmetric.h"

#include "mcux_sgi.h"
#include "mcux_pkc.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    char ch;
    uint8_t pass = 0;
    uint8_t fail = 0;

    /* Init board hardware. */
    BOARD_InitHardware();

    /* Enable SGI and related HW */
    if (SGI_PowerDownWakeupInit(SGI0) != kStatus_Success)
    {
        PRINTF("\r\nSGI init failed\r\n");
    }

    /* Enable PKC and related HW */
    if (PKC_PowerDownWakeupInit(PKC0) != kStatus_Success)
    {
        PRINTF("\r\nPKC init failed\r\n");
    }

    PRINTF("\r\nSGI PKC asymmetric cipher example\r\n");
    PRINTF("\r\n============================\r\n");

    PRINTF("ECC ECDH key agreement NIST P256:");
    if (mcuxClEcc_ECDH_KeyAgreement_NIST_P256_example() == true)
    {
        pass++;
        PRINTF("pass \r\n");
    }
    else
    {
        fail++;
        PRINTF("fail \r\n");
    }

    PRINTF("ECC ECDSA NIST P256:");
    if (mcuxClEcc_ECDSA_FixedKeys_NIST_P256_example() == true)
    {
        pass++;
        PRINTF("pass \r\n");
    }
    else
    {
        fail++;
        PRINTF("fail \r\n");
    }

    PRINTF("RSA key generation:");
    if (mcuxClRsa_KeyGeneration_example() == true)
    {
        pass++;
        PRINTF("pass \r\n");
    }
    else
    {
        fail++;
        PRINTF("fail \r\n");
    }

    PRINTF("ECC MontDH Curve25519:");
    if (mcuxClEcc_MontDH_Curve25519_example() == true)
    {
        pass++;
        PRINTF("pass \r\n");
    }
    else
    {
        fail++;
        PRINTF("fail \r\n");
    }

#if defined(MCUXCL_FEATURE_ECC_CURVE448)
    PRINTF("ECC MontDH Curve448:");
    if (mcuxClEcc_MontDH_Curve448_example() == true)
    {
        pass++;
        PRINTF("pass \r\n");
    }
    else
    {
        fail++;
        PRINTF("fail \r\n");
    }
#endif /* defined(MCUXCL_FEATURE_ECC_CURVE448) */

    PRINTF("ECC EdDSA Ed25519:");
    if (mcuxClEcc_EdDSA_Ed25519ph_example() == true)
    {
        pass++;
        PRINTF("pass \r\n");
    }
    else
    {
        fail++;
        PRINTF("fail \r\n");
    }

    PRINTF("\r\n============================\r\n");
    PRINTF("RESULT: ");
    if (fail == 0)
    {
        PRINTF("All %d test PASS!!\r\n", pass);
    }
    else
    {
        PRINTF("%d / %d test PASSED, %d FAILED!!\r\n", pass, pass + fail, fail);
    }

    PRINTF("SGI PKC example END \r\n");
    while (1)
    {
        ch = GETCHAR();
        PUTCHAR(ch);
    }
}
