/*
 * Copyright 2021 NXP
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

TZM_IS_NOSECURE_ENTRY void DbgConsole_Printf_NSE(char const *s)
{
    size_t string_length;
    /* Access to non-secure memory from secure world has to be properly validated */
    /* Check whether string is properly terminated */
    string_length = strnlen(s, MAX_STRING_LENGTH);
    if ((string_length == MAX_STRING_LENGTH) && (s[string_length] != '\0'))
    {
        PRINTF("String too long or invalid string termination!\r\n");
        while (1)
            ;
    }

    /* Check whether string is located in non-secure memory */
    /* Due to the bug in GCC 10 cmse_check_address_range() always fail, do not call it, see GCC Bugzilla - Bug 99157 */
#if (__GNUC__ != 10)
    if (cmse_check_address_range((void *)s, string_length, CMSE_NONSECURE | CMSE_MPU_READ) == NULL)
    {
        PRINTF("String is not located in normal world!\r\n");
        while (1)
            ;
    }
#endif
    PRINTF(s);
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
