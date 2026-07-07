/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>

#include "app.h"
#include "fsl_semihost.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* <= 23 chars: some host debuggers mishandle longer SYS_OPEN names. */
#define EXAMPLE_FILE_NAME "semihost.txt"
#define EXAMPLE_FILE_MSG  "semihost: hello from the target\n"

/*******************************************************************************
 * Code
 ******************************************************************************/

int main(void)
{
    BOARD_InitHardware();

    /* printf goes through semihost_console; the file round-trip uses the
     * direct SEMIHOST_* API. */
    printf("semihost: console + host file I/O demo\n");

    int msg_len = (int)strlen(EXAMPLE_FILE_MSG);

    /* Write a host file. */
    int wh = SEMIHOST_Open(EXAMPLE_FILE_NAME, kSemihost_OpenW);
    if (wh < 0)
    {
        printf("semihost: open(\"%s\", W) failed\n", EXAMPLE_FILE_NAME);
    }
    else
    {
        (void)SEMIHOST_Write(wh, EXAMPLE_FILE_MSG, msg_len);
        (void)SEMIHOST_Close(wh);
        printf("semihost: wrote \"%s\" (%d bytes) to the debugger host\n", EXAMPLE_FILE_NAME, msg_len);

        /* Read it back and verify. */
        int rh = SEMIHOST_Open(EXAMPLE_FILE_NAME, kSemihost_OpenR);
        if (rh < 0)
        {
            printf("semihost: open(\"%s\", R) failed\n", EXAMPLE_FILE_NAME);
        }
        else
        {
            /* SEMIHOST_Flen is available too, but SYS_FLEN is host-debugger
             * dependent (some report 0 here), so this demo doesn't rely on it. */
            char readback[64] = {0};
            int got = SEMIHOST_Read(rh, readback, (int)sizeof(readback) - 1);
            (void)SEMIHOST_Close(rh);

            if (got == msg_len && memcmp(readback, EXAMPLE_FILE_MSG, (size_t)msg_len) == 0)
            {
                printf("semihost: read back %d bytes, contents match\n", got);
            }
            else
            {
                printf("semihost: read back %d bytes, contents MISMATCH\n", got);
            }
        }
    }

    printf("semihost: done");

    /* Flush the trailing (newline-less) line before requesting exit. */
    SEMIHOST_ConsoleFlush();

    (void)SEMIHOST_Exit(0);

    while (1)
    {
    }
}
