/*
 * Copyright 2018, 2026 NXP
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
#include "veneer_table.h"
#include "tzm_api.h"
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
extern uint32_t GetTestCaseNumber(void);

/*******************************************************************************
 * Code
 ******************************************************************************/
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

/* This function validates two Non-Secure provided strings in Secure world
 * and compares them using strcmp() on validated Secure copies */
TZM_IS_NOSECURE_ENTRY uint32_t StringCompare_NSE(char const *s1, size_t s1_len,
                                                   char const *s2, size_t s2_len)
{
    char secure_buf1[MAX_STRING_LENGTH + 1] = {'\0'};
    char secure_buf2[MAX_STRING_LENGTH + 1] = {'\0'};
    size_t verified_length;
    int result;

    /* Validate lengths */
    if (s1_len == 0U || s1_len > MAX_STRING_LENGTH)
    {
        PRINTF("Input data error: Invalid length for first string!\r\n");
        while (1);
    }
    if (s2_len == 0U || s2_len > MAX_STRING_LENGTH)
    {
        PRINTF("Input data error: Invalid length for second string!\r\n");
        while (1);
    }

    /* Verify that the entire string s1, including the null terminator, is readable
     * Non-Secure memory. */
    if (cmse_check_address_range((void *)s1, s1_len + 1U, CMSE_NONSECURE | CMSE_MPU_READ) == NULL)
    {
        PRINTF("Input data error: First string is not located in normal world!\r\n");
        while (1);
    }

    /* Verify that the entire string s2, including the null terminator, is readable
     * Non-Secure memory. */
    if (cmse_check_address_range((void *)s2, s2_len + 1U, CMSE_NONSECURE | CMSE_MPU_READ) == NULL)
    {
        PRINTF("Input data error: Second string is not located in normal world!\r\n");
        while (1);
    }

    /* Copy s1 into Secure-world local buffer immediately after validation.
     * All subsequent operations use only the immutable Secure copy. 
     * NS world cannot tamper with it after this point. */  
    memcpy(secure_buf1, s1, s1_len);
    secure_buf1[s1_len] = '\0';

    /* Verify secure copies match claimed lengths */
    verified_length = strnlen(secure_buf1, s1_len);
    if (verified_length != s1_len)
    {
        PRINTF("Input data error: First string too long or invalid string termination!\r\n");
        while (1);
    }

    /* Copy s2 into Secure-world local buffer immediately after validation.
     * All subsequent operations use only the immutable Secure copy. 
     * NS world cannot tamper with it after this point. */    
    memcpy(secure_buf2, s2, s2_len);
    secure_buf2[s2_len] = '\0';
    /* Verify secure copies match claimed lengths */
    verified_length = strnlen(secure_buf2, s2_len);
    if (verified_length != s2_len)
    {
        PRINTF("Input data error: Second string too long or invalid string termination!\r\n");
        while (1);
    }

    /* format-string — PRINTF operates on validated Secure
     * copies only. Untrusted NS strings never used as format arguments. */
    PRINTF("Comparing two strings in secure world\r\n");
    PRINTF("String 1: ");
    PRINTF("%s", secure_buf1);
    PRINTF("String 2: ");
    PRINTF("%s", secure_buf2);

    /* Compare validated Secure copies entirely within Secure world. */
    result = strcmp(secure_buf1, secure_buf2);

    /* Explicit mapping: 0 = equal, 1 = not equal */
    return (result == 0) ? 0u : 1u;
}

/* Non-secure callable (entry) function */
TZM_IS_NOSECURE_ENTRY uint32_t GetTestCaseNumber_NSE(void)
{
    return GetTestCaseNumber();
}
