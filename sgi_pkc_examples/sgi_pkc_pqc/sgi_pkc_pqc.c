/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "sgi_pkc_pqc.h"

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

    PRINTF("\r\nSGI PKC PQC example\r\n");
    PRINTF("\r\n============================\r\n");

    PRINTF("KEM example using ML-KEM:");
    if (mcuxClMlKem_KeyGenEncAndDec_example() == true)
    {
        pass++;
        PRINTF("pass \r\n");
    }
    else
    {
        fail++;
        PRINTF("fail \r\n");
    }

    PRINTF("MlDsa44 keygen, sign and verify:");
    if (mcuxClSignature_MlDsa44_KeyGenSignVerify_example() == true)
    {
        pass++;
        PRINTF("pass \r\n");
    }
    else
    {
        fail++;
        PRINTF("fail \r\n");
    }

    PRINTF("MlDsa44 verify example:");
    if (mcuxClSignature_MlDsa44_Verify_example() == true)
    {
        pass++;
        PRINTF("pass \r\n");
    }
    else
    {
        fail++;
        PRINTF("fail \r\n");
    }

    PRINTF("MlDsa65 keygen, sign and verify:");
    if (mcuxClSignature_MlDsa65_KeyGenSignVerify_example() == true)
    {
        pass++;
        PRINTF("pass \r\n");
    }
    else
    {
        fail++;
        PRINTF("fail \r\n");
    }

    PRINTF("MlDsa65 verify example:");
    if (mcuxClSignature_MlDsa65_Verify_example() == true)
    {
        pass++;
        PRINTF("pass \r\n");
    }
    else
    {
        fail++;
        PRINTF("fail \r\n");
    }

    PRINTF("MlDsa87 keygen, sign and verify:");
    if (mcuxClSignature_MlDsa87_KeyGenSignVerify_example() == true)
    {
        pass++;
        PRINTF("pass \r\n");
    }
    else
    {
        fail++;
        PRINTF("fail \r\n");
    }

    PRINTF("MlDsa87 verify example:");
    if (mcuxClSignature_MlDsa87_Verify_example() == true)
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
