/*
 * Copyright 2023, 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"

#include "mmu.h"
#include "mmu_armv8a.h"

static const struct ARM_MMU_flat_range mmu_os_ranges[] = {

	/* Mark text/rodata segments cacheable, read only and executable */
	{ .name  = "code",
	  .start = __text,
	  .end   = __etext,
	  .attrs = MT_NORMAL | MT_P_RX_U_NA | MT_DEFAULT_SECURE_STATE
	},

	/* Mark the execution regions (data, bss, noinit, etc.)
	 * cacheable, read-write
	 * Note: read-write region is marked execute-never internally
	 */
	{ .name  = "data",
	  .start = __data_start__,
	  .end   = __data_end__,
	  .attrs = MT_NORMAL | MT_P_RW_U_NA | MT_DEFAULT_SECURE_STATE
	},

	/* Mark the stack regions (_el0_stack, _el1_stack)
	 * cacheable, read-write
	 * Note: read-write region is marked execute-never internally
	 */
	{ .name  = "stacks",
	  .start = __stacks_limit__,
	  .end   = __stacks_top__,
	  .attrs = MT_NORMAL | MT_P_RW_U_NA | MT_DEFAULT_SECURE_STATE
	},

	/* Mark the shared regions (non-cacheable data)
	 * noncacheable, read-write
	 * Note: read-write region is marked execute-never internally
	 */
	{ .name  = "data_nc",
	  .start = __noncachedata_start__,
	  .end   = __noncachedata_end__,
	  .attrs = MT_NORMAL_NC	| MT_P_RW_U_NA | MT_DEFAULT_SECURE_STATE
	},

	/* Mark text/rodata segments cacheable, read only and executable */
	{ .name  = "ocram_code",
	  .start = __ocramtext_start__,
	  .end   = __ocramtext_end__,
	  .attrs = MT_NORMAL | MT_P_RX_U_NA | MT_DEFAULT_SECURE_STATE
	},

	/* Mark the data regions cacheable, read-write
	 * Note: read-write region is marked execute-never internally
	 */
	{ .name  = "ocram_data",
	  .start = __ocramdata_start__,
	  .end   = __ocramdata_end__,
	  .attrs = MT_NORMAL | MT_P_RW_U_NA | MT_DEFAULT_SECURE_STATE
	},

	/* Mark text/rodata segments cacheable, read only and executable */
	{ .name  = "itcm",
	  .start = __itcm_start__,
	  .end   = __itcm_end__,
	  .attrs = MT_NORMAL | MT_P_RX_U_NA | MT_DEFAULT_SECURE_STATE
	},

	/* Mark the data regions cacheable, read-write
	 * Note: read-write region is marked execute-never internally
	 */
	{ .name  = "dtcm",
	  .start = __dtcm_start__,
	  .end   = __dtcm_end__,
	  .attrs = MT_NORMAL | MT_P_RW_U_NA | MT_DEFAULT_SECURE_STATE
	},
};

static const struct ARM_MMU_region mmu_regions[] = {

	MMU_REGION_FLAT_ENTRY("ANA_PLL",
			      ANA_OSC_BASE, KB(64),
			      MT_DEVICE_nGnRE | MT_P_RW_U_RW | MT_NS),

	MMU_REGION_FLAT_ENTRY("CCM",
			      CCM_CTRL_BASE, KB(64),
			      MT_DEVICE_nGnRE | MT_P_RW_U_RW | MT_NS),

	MMU_REGION_FLAT_ENTRY("GIC",
			      GIC_DISTRIBUTOR_BASE, KB(1024),
			      MT_DEVICE_nGnRE | MT_P_RW_U_RW | MT_NS),

	MMU_REGION_FLAT_ENTRY("IOMUXC",
			      IOMUXC1_BASE, KB(64),
			      MT_DEVICE_nGnRE | MT_P_RW_U_RW | MT_NS),

	MMU_REGION_FLAT_ENTRY("LPUART1",
			      LPUART1_BASE, KB(64),
			      MT_DEVICE_nGnRE | MT_P_RW_U_RW | MT_NS),

	MMU_REGION_FLAT_ENTRY("LPUART2",
			      LPUART2_BASE, KB(64),
			      MT_DEVICE_nGnRE | MT_P_RW_U_RW | MT_NS),

#ifdef APP_MMU_ENTRIES
	APP_MMU_ENTRIES
#endif
};

/* MMU configuration.
 *
 * This struct is defined and populated by the application.
 * It holds the build-time configuration information for the fixed MMU
 * regions enabled during kernel initialization.
 */
static const struct ARM_MMU_config MMU_config = {
	.num_regions = ARRAY_SIZE(mmu_regions),
	.mmu_regions = mmu_regions,
	.num_os_ranges = ARRAY_SIZE(mmu_os_ranges),
	.mmu_os_ranges = mmu_os_ranges,
};

void MMU_init(void)
{
    return ARM_MMU_Initialize(&MMU_config, true);
}
