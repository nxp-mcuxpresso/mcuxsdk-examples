/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "mcuxCsslExamples.h"
#include "sgi_hash.h"

#include "mcux_sgi.h"

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
        
    PRINTF("\r\nSGI hash example\r\n");
    PRINTF("\r\n============================\r\n");

    PRINTF("SHA224 one shot:");
    if (mcuxClHashModes_sha224_oneshot_example() == true)
    {
        pass++;
        PRINTF("pass \r\n");
    }
    else
    {
        fail++;
        PRINTF("fail \r\n");
    }

    PRINTF("SHA256 one shot:");
    if (mcuxClHashModes_sha256_oneshot_example() == true)
    {
        pass++;
        PRINTF("pass \r\n");
    }
    else
    {
        fail++;
        PRINTF("fail \r\n");
    }

    PRINTF("SHA256 streaming example:");
    if (mcuxClHashModes_sha256_streaming_example() == true)
    {
        pass++;
        PRINTF("pass \r\n");
    }
    else
    {
        fail++;
        PRINTF("fail \r\n");
    }

    PRINTF("SHA256 long message example:");
    if (mcuxClHashModes_sha256_longMsgOneshot_example() == true)
    {
        pass++;
        PRINTF("pass \r\n");
    }
    else
    {
        fail++;
        PRINTF("fail \r\n");
    }

    PRINTF("SHA384 one shot:");
    if (mcuxClHashModes_sha384_oneshot_example() == true)
    {
        pass++;
        PRINTF("pass \r\n");
    }
    else
    {
        fail++;
        PRINTF("fail \r\n");
    }

    PRINTF("SHA512 one shot:");
    if (mcuxClHashModes_sha512_oneshot_example() == true)
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

    PRINTF("SGI example END \r\n");
    while (1)
    {
        ch = GETCHAR();
        PUTCHAR(ch);
    }
}
