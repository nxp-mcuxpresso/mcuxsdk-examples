/* @file ncp_log.h
 *
 *  Copyright 2025 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __NCP_LOG_H__
#define __NCP_LOG_H__

#include <stdio.h>
#include <stdint.h>
#include "fsl_debug_console.h"

#define CONFIG_LOG_NCP_LEVEL          NCP_LOG_LEVEL_INF
#define CONFIG_LOG_NCP_INTF_LEVEL     NCP_LOG_LEVEL_INF
#define CONFIG_LOG_NCP_PM_LEVEL       NCP_LOG_LEVEL_INF
#define CONFIG_LOG_NCP_ADAPTER_LEVEL  NCP_LOG_LEVEL_INF
#define CONFIG_LOG_NCP_CRC_LEVEL      NCP_LOG_LEVEL_INF


#define NCP_LOG_LEVEL_ERR   1
#define NCP_LOG_LEVEL_WRN   2
#define NCP_LOG_LEVEL_INF   3
#define NCP_LOG_LEVEL_DBG   4

#define NCP_LOG_MODULE_REGISTER(module_name, level) \
    static const char *NCP_LOG_MODULE_NAME = #module_name; \
    static const int NCP_LOG_MODULE_LEVEL = level;

#define NCP_LOG_MODULE_DEFINE(module_name, level) \
    const char NCP_LOG_MODULE_NAME_##module_name[] = #module_name; \
    const int NCP_LOG_MODULE_LEVEL_##module_name = level;

#define NCP_LOG_MODULE_DECLARE(module_name) \
    extern const char NCP_LOG_MODULE_NAME_##module_name[]; \
    extern const int NCP_LOG_MODULE_LEVEL_##module_name; \
    static const char *NCP_LOG_MODULE_NAME = NCP_LOG_MODULE_NAME_##module_name; \
    static int NCP_LOG_MODULE_LEVEL = NCP_LOG_LEVEL_INF; \
    __attribute__((constructor)) static void _log_init_##module_name(void) { \
        NCP_LOG_MODULE_LEVEL = NCP_LOG_MODULE_LEVEL_##module_name; \
    }

#define NCP_LOG_ERR(...) do { \
    if (NCP_LOG_LEVEL_ERR <= NCP_LOG_MODULE_LEVEL) \
        PRINTF("[%s] ERR: ", NCP_LOG_MODULE_NAME), PRINTF(__VA_ARGS__), PRINTF("\r\n"); \
} while(0)

#define NCP_LOG_WRN(...) do { \
    if (NCP_LOG_LEVEL_WRN <= NCP_LOG_MODULE_LEVEL) \
        PRINTF("[%s] WRN: ", NCP_LOG_MODULE_NAME), PRINTF(__VA_ARGS__), PRINTF("\r\n"); \
} while(0)

#define NCP_LOG_INF(...) do { \
    if (NCP_LOG_LEVEL_INF <= NCP_LOG_MODULE_LEVEL) \
        PRINTF("[%s] ", NCP_LOG_MODULE_NAME), PRINTF(__VA_ARGS__), PRINTF("\r\n"); \
} while(0)

#define NCP_LOG_DBG(...) do { \
    if (NCP_LOG_LEVEL_DBG <= NCP_LOG_MODULE_LEVEL) \
        PRINTF("[%s] DBG: ", NCP_LOG_MODULE_NAME), PRINTF(__VA_ARGS__), PRINTF("\r\n"); \
} while(0)

#define NCP_LOG_HEXDUMP_DBG(data, len) do { \
    if (NCP_LOG_LEVEL_DBG <= NCP_LOG_MODULE_LEVEL) { \
        PRINTF("[%s] HEX: ", NCP_LOG_MODULE_NAME); \
        for (int _i = 0; _i < (len); _i++) { \
            PRINTF("%02x ", ((uint8_t*)(data))[_i]); \
            if ((_i + 1) % 16 == 0) PRINTF("\n            "); \
        } \
        PRINTF("\r\n"); \
    } \
} while(0)

#endif /* __NCP_LOG_H__ */
