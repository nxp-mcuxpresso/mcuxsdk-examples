/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */

/**
 * \file        NCP-DUMP-functions.h
 * \brief       NCP Dumping Functions
 * \author      Luc Paugam (luc.paugam@nxp.com)
 * \version     1.0
 * \date        7-may-2024
 *
 * This library.
 *
 */

#ifndef _NCP_DUMP_FUNCTIONS_H_
#define _NCP_DUMP_FUNCTIONS_H_


#include <stdio.h>


/* -------------------------------------------------------------------------- */
/*                                 Types                                      */
/* -------------------------------------------------------------------------- */

// clang-format off

#ifdef NCP_LWIP_SOCKET
// SDK Included Files
#  include "fsl_debug_console.h"
#  define NCP_PRINTF    PRINTF
#else
#  define NCP_PRINTF    printf
#endif


/* -------------------------------------------------------------------------- */
/*                              Constants                                     */
/* -------------------------------------------------------------------------- */

/*
** String Colors
**
** REF: https://github.com/fidian/ansi
**
** ===> where N = color code:
**
**          dim text is \e[2;Nm
**          blinking text (though not supported by most terminal emulators) is \e[5;Nm
**          reversed text (highlighted text) is \e[7;Nm
*/

#define C_Reset "\033[0m"

#define C_Black "\033[0;30m"
#define C_Red "\033[0;31m"
#define C_Green "\033[0;32m"
#define C_Yellow "\033[0;33m"
#define C_Blue "\033[0;34m"
#define C_Purple "\033[0;35m"
#define C_Cyan "\033[0;36m"
#define C_White "\033[0;37m"

#define C_BoldBlack "\033[1;30m"
#define C_BoldRed "\033[1;31m"
#define C_BoldGreen "\033[1;32m"
#define C_BoldYellow "\033[1;33m"
#define C_BoldBlue "\033[1;34m"
#define C_BoldPurple "\033[1;35m"
#define C_BoldCyan "\033[1;36m"
#define C_BoldWhite "\033[1;37m"

#define C_UnderlineBlack "\033[4;30m"
#define C_UnderlineRed "\033[4;31m"
#define C_UnderlineGreen "\033[4;32m"
#define C_UnderlineYellow "\033[4;33m"
#define C_UnderlineBlue "\033[4;34m"
#define C_UnderlinePurple "\033[4;35m"
#define C_UnderlineCyan "\033[4;36m"
#define C_UnderlineWhite "\033[4;37m"

#define C_BackgroundBlack "\033[40m"
#define C_BackgroundRed "\033[41m"
#define C_BackgroundGreen "\033[42m"
#define C_BackgroundYellow "\033[43m"
#define C_BackgroundBlue "\033[44m"
#define C_BackgroundPurple "\033[45m"
#define C_BackgroundCyan "\033[46m"
#define C_BackgroundWhite "\033[47m"

#define C_LightBlack "\033[0;90m"
#define C_LightRed "\033[0;91m"
#define C_LightGreen "\033[0;92m"
#define C_LightYellow "\033[0;93m"
#define C_LightBlue "\033[0;94m"
#define C_LightPurple "\033[0;95m"
#define C_LightCyan "\033[0;96m"
#define C_LightWhite "\033[0;97m"

#define C_BoldLightBlack "\033[1;90m"
#define C_BoldLightRed "\033[1;91m"
#define C_BoldLightGreen "\033[1;92m"
#define C_BoldLightYellow "\033[1;93m"
#define C_BoldLightBlue "\033[1;94m"
#define C_BoldLightPurple "\033[1;95m"
#define C_BoldLightCyan "\033[1;96m"
#define C_BoldLightWhite "\033[1;97m"

#define C_BackgroundLightBlack "\033[0;100m"
#define C_BackgroundLightRed "\033[0;101m"
#define C_BackgroundLightGreen "\033[0;102m"
#define C_BackgroundLightYellow "\033[0;103m"
#define C_BackgroundLightBlue "\033[0;104m"
#define C_BackgroundLightPurple "\033[0;105m"
#define C_BackgroundLightCyan "\033[0;106m"
#define C_BackgroundLightWhite "\033[0;107m"

/* -------------------------------------------------------------------------- */
/*                                 Macros                                     */
/* -------------------------------------------------------------------------- */

// MACRO: Display of Debugging Information, Warning or Error...

#ifdef CONFIG_NCP_MSG
#  ifdef NCP_LWIP_SOCKET
#    define LOG_NCP_INFO(format, ...) PRINTF(                  format C_Reset "\r\n", ##__VA_ARGS__)
#    define LOG_NCP_WARN(format, ...) PRINTF(C_BoldYellow "%s" format C_Reset "\r\n", "WARNING: ", ##__VA_ARGS__)
#  else
#    define LOG_NCP_INFO(format, ...) fprintf(stdout,                   format C_Reset "\r\n", ##__VA_ARGS__)
#    define LOG_NCP_WARN(format, ...) fprintf(stdout, C_BoldYellow "%s" format C_Reset "\r\n", "WARNING: ", ##__VA_ARGS__)
#  endif
#else
#    define LOG_NCP_INFO(format, ...)
#    define LOG_NCP_WARN(format, ...)
#endif

#ifdef NCP_LWIP_SOCKET
#  define LOG_NCP_ERR(format, ...)  PRINTF(C_LightRed   "%s" format C_Reset "\r\n",   "ERROR:   ", ##__VA_ARGS__)
#else
#  define LOG_NCP_ERR(format, ...)  fprintf(stderr, C_LightRed "%s" format C_Reset "\r\n",   "ERROR:   ", ##__VA_ARGS__)
#endif

// clang-format on

/* -------------------------------------------------------------------------- */
/*                               Public Functions                             */
/* -------------------------------------------------------------------------- */

// DEFINE: NCP dumping functions

/**
 * \fn void NCP_dump_buf(int start_pos, int end_pos, unsigned char* buf, int len)
 * \brief ...
 *
 * \param start_pos ...
 * \param end_pos   ...
 * \param buf       ...
 * \param len       ...
 *
 * \return ...
 */
extern void NCP_dump_buf(int start_pos, int end_pos, unsigned char *buf, int len);

#endif // _NCP_DUMP_FUNCTIONS_H_
