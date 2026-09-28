/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "sys_info.h"
#include "fsl_debug_console.h"

/* MCUXSDK_VERSION_FULL_STR is generated into the build directory by the build
 * system (cmake/extension/mcux_version_header.cmake) and force-included into
 * every compilation unit, so no explicit #include is needed here. */

/* Compile the Neutron version print only when the Neutron driver middleware
 * component is part of the build, which is reflected by its public header
 * being on the include path. Toolchains without __has_include support simply
 * skip the check (same pattern as flatbuffers/base.h). */
#if defined(__has_include)
#if __has_include("NeutronDriver.h")
#define SYS_INFO_HAS_NEUTRON 1
#endif
#endif

#if defined(SYS_INFO_HAS_NEUTRON)
#include "NeutronDriver.h"
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/

void SYS_INFO_PrintSdkVersion(void)
{
    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);
}

void SYS_INFO_PrintNeutronVersion(void)
{
#if defined(SYS_INFO_HAS_NEUTRON)
    NeutronSdkVersion sdkVersion = neutronGetSdkVersion();

    PRINTF("Neutron Software version: %u.%u.%u-%s\r\n", (unsigned int)sdkVersion.major,
           (unsigned int)sdkVersion.minor, (unsigned int)sdkVersion.patch, sdkVersion.hashString);
#endif
}

void SYS_INFO_PrintAll(void)
{
    SYS_INFO_PrintSdkVersion();
    SYS_INFO_PrintNeutronVersion();
}
