# Copyright 2024, 2026 NXP
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
include(${SdkRootDirPath}/arch/arm/target/ram.cmake)
include(${SdkRootDirPath}/examples/_common/project_setting/arm_common.cmake)

# Load core specific board segment (e.g. ca55 boots from DDR and uses its own
# board.c/mmu.c/common_freertos). cm33 keeps using the root board files.
if(EXISTS ${SdkRootDirPath}/examples/_boards/mcimx93evk/${core_id}/setting.cmake)
    include(${SdkRootDirPath}/arch/arm/target/ddr.cmake)
    include(${SdkRootDirPath}/examples/_boards/mcimx93evk/${core_id}/setting.cmake)
endif()


