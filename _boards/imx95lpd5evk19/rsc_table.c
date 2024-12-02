/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* This file populates resource table for BM remote
 * for use by the Linux Master */

#include <string.h>
#include "board.h"
#include "rsc_table.h"
#include "rpmsg_lite.h"

#define NUM_VRINGS 0x02

#if defined(__ICCARM__)
extern void __RscTblStart;
extern void __RscTblInDramStart;
#elif defined(__GNUC__)
extern const void __RscTblStart;
extern const void __RscTblInDramStart;
#else
#error Not support the compiler.
#endif

/* Place resource table in special ELF section */
#if defined(__ARMCC_VERSION) || defined(__GNUC__)
__attribute__((section(".resource_table")))
#elif defined(__ICCARM__)
#pragma location = ".resource_table"
#else
#error Compiler not supported!
#endif
const struct remote_resource_table resources = {
    /* Version */
    1,

    /* NUmber of table entries */
    NO_RESOURCE_ENTRIES,

    /* reserved fields */
    {
        0,
        0,
    },

    /* Offsets of rsc entries */
    {
        offsetof(struct remote_resource_table, srtm_vdev),
        offsetof(struct remote_resource_table, user_vdev),
    },

    /* SRTM virtio device entry */
    {
        RSC_VDEV,
        7,
        0,
        RSC_VDEV_FEATURE_NS,
        0,
        0,
        0,
        NUM_VRINGS,
        {0, 0},
    },

    /* Vring rsc entry - part of vdev rsc entry */
    {VDEV0_VRING_BASE, VRING_ALIGN, RL_BUFFER_COUNT, 0, 0},
    {VDEV0_VRING_BASE + VRING_SIZE, VRING_ALIGN, RL_BUFFER_COUNT, 1, 0},

    /* SRTM virtio device entry */
    {
        RSC_VDEV,
        7,
        1,
        RSC_VDEV_FEATURE_NS,
        0,
        0,
        0,
        NUM_VRINGS,
        {0, 0},
    },

    /* Vring rsc entry - part of vdev rsc entry */
    {VDEV1_VRING_BASE, VRING_ALIGN, RL_BUFFER_COUNT, 2, 0},
    {VDEV1_VRING_BASE + VRING_SIZE, VRING_ALIGN, RL_BUFFER_COUNT, 3, 0},
};

void copyResourceTable(void)
{
    uint32_t dst_addr = 0U;

    /*
     * On startup, DDR not be enabled when M Core does resource table copy.
     * Here store the resource table to a destination address
     * (such as: 0x2001f000(NS)/0x3001F000(S) in TCM).
     */
    dst_addr = (uint32_t)&__RscTblStart;
    memset((void *)dst_addr, 0,
           0x1000); /* ECC is enabled defaultly , cortex-A55 failed to read 4KB from the region when the region is not
                       initialized.(cortex-A55 report an ECC read error) */
    memcpy((void *)dst_addr, &resources, sizeof(resources));
    /*
     * copy resource table to dram address for rpmsg resource table(backup in dram)
     * Note: Must ensure that the dram is accessable for cortex-m7(dram is initialized by ddr oei image)
     */
    dst_addr = (uint32_t)&__RscTblInDramStart;
    memcpy((void *)dst_addr, &resources, sizeof(resources));
}
