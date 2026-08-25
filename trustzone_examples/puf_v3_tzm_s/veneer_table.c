/*
 * Copyright 2021, 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#if (__ARM_FEATURE_CMSE & 1) == 0
#error "Need ARMv8-M security extensions"
#elif (__ARM_FEATURE_CMSE & 2) == 0
#error "Compile with --cmse"
#endif

#include "stdint.h"
#include "arm_cmse.h"
#include "tzm_api.h"
#include "veneer_table.h"
#include "fsl_debug_console.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define MAX_STRING_LENGTH 0x400
/* Enforce minimum GCC version for CMSE security checks.
 * GCC 10.0-10.2 has a known bug where cmse_check_address_range() and
 * cmse_check_pointed_object() always return NULL (GCC Bugzilla #99157).
 * Security checks in this file depend on these functions being correct.
 * Solved in GCC 10.3. */
#if defined(__GNUC__) && (__GNUC__ == 10) && (__GNUC_MINOR__ < 3)
#error "GCC 10.0-10.2 is not supported for this file due to GCC Bugzilla #99157 \
(cmse_check_address_range() always fails). Please upgrade to GCC 10.3 or later."
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*! @brief This function provide Non-secure (normal) world to claim PUFv3 lock */
TZM_IS_NOSECURE_ENTRY void NSC_PUF_ClaimLock(puf_sec_level_t securityLevel)
{
    PUF_SetLock(PUF, securityLevel);
}

/* strnlen function implementation for arm compiler */
#if defined(__arm__)
size_t strnlen(const char *s, size_t maxLength)
{
    size_t length = 0;
    while ((length <= maxLength) && (*s))
    {
        s++;
        length++;
    }
    return length;
}
#endif

TZM_IS_NOSECURE_ENTRY void DbgConsole_Printf_NSE(char const *s, size_t length)
{
    char secure_buf[MAX_STRING_LENGTH + 1] = {'\0'};
    size_t verified_length = 0U;

    /* Validate length before any pointer dereference */
    if (length == 0U || length > MAX_STRING_LENGTH)
    {
        PRINTF("Input data error: Invalid string length!\r\n");
        while (1);
    }

    /* Verify that the entire string, including the null terminator, is readable
     * Non-Secure memory. */
    if (cmse_check_address_range((void *)s, length + 1U, CMSE_NONSECURE | CMSE_MPU_READ) == NULL)
    {
        PRINTF("Input data error: String is not located in normal world!\r\n");
        while (1);
    }

    /* Copy into secure buffer IMMEDIATELY after cmse_check.
     * NS world cannot tamper with content after this point.
     * All subsequent ops use only secure_buf. */
    memcpy(secure_buf, s, length);
    secure_buf[length] = '\0';
 
    /* Verify the secure copy is properly null-terminated
     * and length matches what caller claimed. */
    verified_length = strnlen(secure_buf, length);
    if (verified_length != length)
    {
        /* Caller-supplied length does not match actual string length.
         * Could indicate truncated string or malicious length value. */
        PRINTF("Input data error: String too long or invalid string termination!\r\n");
        while (1);
    }

    /* Safe to print — secure copy only, no format-string risk */
    PRINTF("%s", secure_buf);
}

TZM_IS_NOSECURE_ENTRY void DbgConsole_Putchar_NSE(int c)
{
    /* Check whether string is located in non-secure memory */
    /* Due to the bug in GCC 10 cmse_check_address_range() always fail, do not call it, see GCC Bugzilla - Bug 99157 */
#if (__GNUC__ != 10)
    if (cmse_check_address_range((void *)c, 1u, CMSE_NONSECURE | CMSE_MPU_READ) == NULL)
    {
        PRINTF("Char is not located in normal world!\r\n");
        while (1)
            ;
    }
#endif
    PUTCHAR(c);
}
