/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "ecat_def.h"
#include "ecat_hw.h"
#include <string.h>
#include "ipc_shm.h"

/*
 * The workaround for ECAT bus occupancy issue (EtherCAT Stack side).
 *
 * Prior to each ESC register access, query the lock status and wait
 * until the motor control side releases the lock.
 */
void Ecat_Read(ECAT_Type *ecat, uint8_t *pData, uint32_t addr, uint32_t len)
{
    union {
        volatile uint32_t u32;
        volatile uint8_t  u8[4];
    } u32v;
    uint32_t unaligned_addr = addr & 0x03;
    uint32_t unaligned_len = sizeof(int) - unaligned_addr;
    volatile uint32_t *src = &((uint32_t *)(ecat))[(addr) >> 2];
    uint32_t *dst;

    if (len && unaligned_addr) {
        Ecat_WaitForUnlock();
        u32v.u32 = *src++;
        unaligned_len = len > unaligned_len ? unaligned_len : len;
        memcpy(pData, (void *)&u32v.u8[unaligned_addr], unaligned_len);
        len -= unaligned_len;
        pData += unaligned_len;
    }

    dst = (uint32_t *)pData;
    while (len >= 4U)
    {
        Ecat_WaitForUnlock();
        *dst++ = *src++;
        len -= 4U;
    }

    if (len > 0U)
    {
        Ecat_WaitForUnlock();
        u32v.u32 = *src;
        memcpy((uint8_t *)dst, (void *)&u32v.u8[0], len);
    }
}

void Ecat_Write(ECAT_Type *ecat, uint8_t *pData, uint32_t addr, uint32_t len)
{
    union {
        volatile uint32_t u32;
        volatile uint8_t  u8[4];
    } u32v;
    uint32_t unaligned_addr = addr & 0x03;
    uint32_t unaligned_len = sizeof(int) - unaligned_addr;
    volatile uint32_t *dst = &((uint32_t *)(ecat))[(addr) >> 2];
    uint32_t *src;

    if (len && unaligned_addr) {
        Ecat_WaitForUnlock();
        u32v.u32 = *dst;
        unaligned_len = len > unaligned_len ? unaligned_len : len;
        memcpy((void *)&u32v.u8[unaligned_addr], pData, unaligned_len);
        Ecat_WaitForUnlock();
        *dst++ = u32v.u32;
        len -= unaligned_len;
        pData += unaligned_len;
    }

    src = (uint32_t *)pData;
    while (len >= 4U)
    {
        Ecat_WaitForUnlock();
        *dst++ = *src++;
        len -= 4U;
    }

    if (len > 0U)
    {
        Ecat_WaitForUnlock();
        u32v.u32 = *dst;
        memcpy((void *)&u32v.u8[0], (uint8_t *)src, len);
        Ecat_WaitForUnlock();
        *dst = u32v.u32;
    }
}
