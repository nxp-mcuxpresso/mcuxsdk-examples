/*
 * Copyright 2018, 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _VENEER_TABLE_H_
#define _VENEER_TABLE_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Callback type for receiving scalar results from Secure world.
 * Uses size_t — consistent with length-returning NSE APIs. */
typedef void (*length_callbackptr)(size_t value);



/*******************************************************************************
 * Prototypes
 ******************************************************************************/
 /* @brief Entry function for printing an unsigned integer value
 *
 * This function provides a safe interface to print a size_t value
 * from Non-Secure world via Secure world UART. Scalar argument is
 * passed by value — no pointer/memory access concerns.
 *
 * This function is called from normal world only.
 *
 * @param value    size_t value to be printed (e.g. string length).
 */
void DbgConsole_PrintScalar_NSE(size_t value);

/*!
 * @brief Entry function for debug PRINTF (DbgConsole_Printf)
 *
 * This function provides interface between secure and normal worlds
 * This function is called from normal world only
 *
 * @param s      String to be printed
 * @param length Length of the string in bytes, excluding null terminator.
 *               Must be less than MAX_STRING_LENGTH.
 *
 */
void DbgConsole_Printf_NSE(char const *s, size_t length);

/*!
 * @brief Entry function for two string comparison
 *
 * This function validates two Non-Secure provided strings in Secure world
 * and compares them using strcmp() on validated Secure copies.
 *
 * This function is called from normal world only.
 *
 * @note  The comparison is performed entirely in Secure world
 *        using strcmp() on validated Secure copies.
 *
 * @param s1      First string to be compared. Must reside in Non-Secure memory.
 * @param s1_len  Length of s1 in bytes, excluding null terminator.
 * @param s2      Second string to be compared. Must reside in Non-Secure memory.
 * @param s2_len  Length of s2 in bytes, excluding null terminator.
 *
 *  @return            >0 for s1 > s2
 *                     =0 for s1 = s2
 *                     <0 for s1 < s2
 *                     Function does not return on error (halts with while(1)).
 */
uint32_t StringCompare_NSE(char const *s1, size_t s1_len, char const *s2, size_t s2_len);

/*!
 * @brief Entry function for string length calculation with Non-Secure callback notification
 *
 * This function validates a Non-Secure provided string in Secure world,
 * calculates its length, and notifies the Non-Secure world of the result
 * via a scalar callback argument.
 *
 * This function demonstrates the correct ARM CMSE pattern for Non-Secure
 * callbacks: only scalar values (passed by value in CPU registers) are safe
 * to pass to Non-Secure callbacks. Pointer arguments to Non-Secure callbacks
 * are unsafe because Non-Secure world cannot access Secure stack memory.
 *
 * This function is called from normal world only.
 *
 * @param s            Pointer to the Non-Secure string to be validated and measured.
 *                     Must reside entirely in Non-Secure memory.
 * @param length       Length of the string in bytes, excluding null terminator.
 *                     Must be less than MAX_STRING_LENGTH.
 * @param callback     Pointer to a Non-Secure callback function that receives
 *                     the calculated string length as a scalar uint32_t argument.
 *                     Must reside in Non-Secure memory.
 *                     Callback signature: void callback(uint32_t length)
 * @return             Length of the validated string in bytes (excluding null terminator).
 *                     Function does not return on error (halts with while(1)).
 */
uint32_t GetStringLength_NSE(char const *s, size_t length, volatile length_callbackptr callback);
#endif /* _VENEER_TABLE_H_ */
