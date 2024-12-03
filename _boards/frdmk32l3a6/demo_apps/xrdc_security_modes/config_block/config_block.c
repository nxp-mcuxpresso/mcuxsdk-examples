/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2017 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "config_block.h"
#include <stdlib.h>

const config_domain_t domainAssignments[] = {{.busId = 0, .domains = {0}, .numDomains = 1},
                                             {.busId = 1, .domains = {0}, .numDomains = 1},
                                             {.busId = 32, .domains = {1}, .numDomains = 1}};

const uint8_t periph_disabled[] = {0xb1, 0xb2, 0xa8, 0x20, 0xa0, 0x2f, 0x4c, 0x08, 0x09, 0x88, 0x89, 0x21, 0xa1, 0x38,
                                   0x22, 0x8f, 0x0c, 0x39, 0x23, 0xb3, 0xb4, 0xa2, 0x24, 0xa3, 0x4a, 0x4b, 0xd6, 0x3a,
                                   0x3b, 0x3c, 0xae, 0x30, 0xaa, 0x3f, 0x40, 0x41, 0xb5, 0x32, 0x33, 0xab, 0x43, 0x44,
                                   0xb6, 0x01, 0x25, 0xa4, 0x2b, 0xa7, 0x46, 0x47, 0x48, 0x49, 0xb7, 0xaf, 0x31, 0x3d,
                                   0x2c, 0x3e, 0x1b, 0x9b, 0x26, 0x27, 0x28, 0x2d, 0x35, 0x36, 0x37, 0xad, 0x29, 0xa5,
                                   0xa9, 0x34, 0xac, 0x45, 0x2e, 0x4d, 0x2a, 0xa6, 0xb0};

const uint8_t periph_core0[] = {0x42, 0x14, 0x17, 0x16, 0x15};

const config_peripheral_t peripherals = {
    .disabled = periph_disabled, .core0 = periph_core0, .core1 = NULL, .shared = NULL, .sizes = {79, 5, 0, 0}};

const config_memory_t memoryRegions[] = {
    {.memId = 0, .startAddr = 0x0, .size = 0x100000, .lock = false, .accset = {0xb6d, 0x942}},
    {.memId = 16, .startAddr = 0x1000000, .size = 0x40000, .lock = false, .accset = {0x942, 0xb6d}},
    {.memId = 1, .startAddr = 0x8000000, .size = 0x1000, .lock = false, .accset = {0xdb6, 0xdb6}},
    {.memId = 17, .startAddr = 0x9000000, .size = 0x20000, .lock = false, .accset = {0xdb6, 0xdb6}},
    {.memId = 2, .startAddr = 0x20000000, .size = 0x30000, .lock = false, .accset = {0xdb6, 0xdb6}},
    {.memId = 3, .startAddr = 0x48010000, .size = 0x800, .lock = false, .accset = {0xdb6, 0xdb6}}};

const config_block_info_t configBlockInfo = {
    .version               = 1,
    .device                = "K32L3A6",
    .sizeDomainAssignments = sizeof(domainAssignments) / sizeof(config_domain_t),
    .sizeMemoryRegions     = sizeof(memoryRegions) / sizeof(config_memory_t),
    .globalPeripheralLock  = false};

const config_block_t configBlock = {.configBlockInfo   = &configBlockInfo,
                                    .domainAssignments = domainAssignments,
                                    .peripherals       = &peripherals,
                                    .memoryRegions     = memoryRegions};
