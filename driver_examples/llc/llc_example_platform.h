/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _LLC_EXAMPLE_PLATFORM_H_
#define _LLC_EXAMPLE_PLATFORM_H_

#include "fsl_common.h"
#include "fsl_llc.h"

/*
 * Portable LLC example / board-port contract.
 *
 * The common LLC example (llc.c) is memory-type agnostic: it never references
 * PSRAM, SDRAM, HyperRAM or any specific external-memory technology, and it
 * never hard-codes an LLC instance or a memory address. All platform-specific
 * facts are supplied at run time by the board port through this contract.
 *
 * A new board enables the example simply by providing a board port that
 * implements LLC_ExamplePlatformInit() and LLC_ExampleGetPlatform(); the
 * common source is never modified.
 */

/*!
 * @brief Platform description handed to the portable LLC example by the board
 * port.
 */
typedef struct _llc_example_platform
{
    LLC_Type *instance;   /*!< LLC instance that owns the test region. */
    uint32_t regionBase;  /*!< Base address of a valid LLC-cacheable test
                               region (the LLC-routed view of that memory). */
    uint32_t regionSize;  /*!< Size, in bytes, of the LLC-cacheable region. */
    bool bootInitedLlc;   /*!< true if boot code already initialized and enabled
                               the LLC (the example must not re-init it). */
} llc_example_platform_t;

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Returns the board LLC platform description.
 *
 * Valid only after LLC_ExamplePlatformInit() has returned.
 *
 * @return Pointer to the board's constant platform description.
 */
const llc_example_platform_t *LLC_ExampleGetPlatform(void);

/*!
 * @brief Brings up any board-specific memory that backs the LLC test region.
 *
 * Called by the common example after BOARD_InitHardware() and before the
 * platform description is used. The board port performs any device-specific
 * memory init here (for example, software bring-up of an external RAM) that is
 * not safe to do in the common example. Idempotent.
 */
void LLC_ExamplePlatformInit(void);

#if defined(__cplusplus)
}
#endif

#endif /* _LLC_EXAMPLE_PLATFORM_H_ */
