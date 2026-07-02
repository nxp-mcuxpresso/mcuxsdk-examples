/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * Project-local override for FreeRTOS secure port macros (ARMGCC).
 *
 * Purpose:
 * When BOARD_TZM_SG_IN_SRAM is enabled, place NSC callable function bodies into
 * the .sg_ramfunc section so they execute from secure SRAM close to the SG
 * veneer table (.gnu.sgstubs). This avoids out-of-range branch issues.
 *
 * This file is intended to be force-included (gcc -include) via reconfig.cmake
 * for the freertos_mpu_s target.
 */

#ifndef MCUX_FREERTOS_SECUREPORT_OVERRIDE_ARMGCC_H_
#define MCUX_FREERTOS_SECUREPORT_OVERRIDE_ARMGCC_H_

#if defined(__GNUC__) && !defined(__IAR_SYSTEMS_ICC__) && defined(BOARD_TZM_SG_IN_SRAM)

/*
 * Include the upstream header once so its include guard is set.
 * Then override only secureportNON_SECURE_CALLABLE.
 *
 * NOTE: Use a relative path from this file to avoid requiring additional
 * include-path changes.
 */
#if defined(__MCUXPRESSO)
#include <secure_port_macros.h>
#else
#include "../../../../../../../rtos/freertos/freertos-kernel-upstream/portable/GCC/ARM_CM33/secure/secure_port_macros.h"
#endif

#undef secureportNON_SECURE_CALLABLE

#define secureportNON_SECURE_CALLABLE \
    __attribute__( ( cmse_nonsecure_entry, section( ".sg_ramfunc" ) ) ) __attribute__( ( used ) )

#endif /* __GNUC__ && BOARD_TZM_SG_IN_SRAM */

#endif /* MCUX_FREERTOS_SECUREPORT_OVERRIDE_ARMGCC_H_ */
