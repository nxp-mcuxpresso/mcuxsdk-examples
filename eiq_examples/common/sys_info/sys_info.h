/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SYS_INFO_H_
#define _SYS_INFO_H_

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*! @brief Print the MCUX SDK version string to the debug console. */
void SYS_INFO_PrintSdkVersion(void);

/*! @brief Print the Neutron software version to the debug console.
 *! @note Does nothing when the Neutron driver middleware is not part of the
 *! build (detected via its public header being on the include path). */
void SYS_INFO_PrintNeutronVersion(void);

/*! @brief Print all system information at once. */
void SYS_INFO_PrintAll(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _SYS_INFO_H_ */
