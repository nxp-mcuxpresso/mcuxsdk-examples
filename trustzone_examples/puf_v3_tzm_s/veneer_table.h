/*
 * Copyright 2021, 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _VENEER_TABLE_H_
#define _VENEER_TABLE_H_

#include "fsl_puf_v3.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef int (*callbackptr)(char const *s1, char const *s2);

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Entry function for PUFv3 to claim lock from non-secure world
 *
 * This function provides interface between secure and normal worlds
 *
 * @param securityLevel   Security level to be set by secure world
 *
 */
void NSC_PUF_ClaimLock(puf_sec_level_t securityLevel);

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
 * @brief Entry function for debug PUTCHAR (DbgConsole_Putchar)
 *
 * This function provides interface between secure and normal worlds
 * This function is called from normal world only
 *
 * @param c     Char to be printed
 *
 */
void DbgConsole_Putchar_NSE(int ch);

#endif /* _VENEER_TABLE_H_ */
