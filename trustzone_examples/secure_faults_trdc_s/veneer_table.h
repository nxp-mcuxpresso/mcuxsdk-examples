/*
 * Copyright 2021, 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _VENEER_TABLE_
#define _VENEER_TABLE_

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* NOTE: These defines are not related to veneer table. But since they are needed
 *       in both secure and non-secure project, they are placed here */
#define FAULT_NONE                0
#define FAULT_INV_S_TO_NS_TRANS   1
#define FAULT_INV_S_ENTRY         2
#define FAULT_INV_NS_DATA_ACCESS  3
#define FAULT_INV_INPUT_PARAMS    4
#define FAULT_INV_NS_DATA2_ACCESS 5

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
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
 * @brief Entry function for GetTestCaseNumber(void)
 *
 * This function retuns number of actual fault testcase
 * This function is called from normal world only
 *
 * @return             number of actual fault testcase
 */
uint32_t GetTestCaseNumber_NSE(void);
#endif /* _VENEER_TABLE_ */
