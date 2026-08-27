/*
 * Copyright 2023, 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#if __has_include("app_mmu.h")
#include "app_mmu.h"
#endif
#include "fsl_common.h"

#include "mmu.h"
#include "mmu_armv8a.h"

static const struct ARM_MMU_flat_range mmu_os_ranges[] = {

	/* Text/rodata segments: cacheable, read only and executable */
	{ .name  = "code",
	  .start = __text,
	  .end   = __etext,
	  .attrs = MT_NORMAL | MT_P_RX_U_NA | MT_DEFAULT_SECURE_STATE
	},

	/* The execution regions (data, bss, noinit, etc.): cacheable, read-write */
	{ .name  = "data",
	  .start = __data_start__,
	  .end   = __data_end__,
	  .attrs = MT_NORMAL | MT_P_RW_U_NA | MT_DEFAULT_SECURE_STATE
	},

	/* The stack regions: cacheable, read-write */
	{ .name  = "stacks",
	  .start = __stacks_limit__,
	  .end   = __stacks_top__,
	  .attrs = MT_NORMAL | MT_P_RW_U_NA | MT_DEFAULT_SECURE_STATE
	},

	/* Non-cacheable data: noncacheable, read-write */
	{ .name  = "data_nc",
	  .start = __noncachedata_start__,
	  .end   = __noncachedata_end__,
	  .attrs = MT_NORMAL_NC	| MT_P_RW_U_NA | MT_DEFAULT_SECURE_STATE
	},

	/* ocram_code: cacheable, read only and executable */
	{ .name  = "ocram_code",
	  .start = __ocramtext_start__,
	  .end   = __ocramtext_end__,
	  .attrs = MT_NORMAL | MT_P_RX_U_NA | MT_DEFAULT_SECURE_STATE
	},

	/* ocram_data: cacheable, read-write */
	{ .name  = "ocram_data",
	  .start = __ocramdata_start__,
	  .end   = __ocramdata_end__,
	  .attrs = MT_NORMAL | MT_P_RW_U_NA | MT_DEFAULT_SECURE_STATE
	},

	/* itcm: cacheable, read only and executable */
	{ .name  = "itcm",
	  .start = __itcm_start__,
	  .end   = __itcm_end__,
	  .attrs = MT_NORMAL | MT_P_RX_U_NA | MT_DEFAULT_SECURE_STATE
	},

	/* dtcm: cacheable, read-write */
	{ .name  = "dtcm",
	  .start = __dtcm_start__,
	  .end   = __dtcm_end__,
	  .attrs = MT_NORMAL | MT_P_RW_U_NA | MT_DEFAULT_SECURE_STATE
	},
};

static const struct ARM_MMU_region mmu_regions[] = {

	MMU_REGION_FLAT_ENTRY("ANA_PLL",
			      CCM_ANALOG_BASE, KB(64),
			      MT_DEVICE_nGnRE | MT_P_RW_U_RW | MT_NS),

	MMU_REGION_FLAT_ENTRY("CCM",
			      CCM_BASE, KB(64),
			      MT_DEVICE_nGnRE | MT_P_RW_U_RW | MT_NS),

	MMU_REGION_FLAT_ENTRY("GIC",
			      GIC_DISTRIBUTOR_BASE, KB(1024),
			      MT_DEVICE_nGnRE | MT_P_RW_U_RW | MT_NS),

	MMU_REGION_FLAT_ENTRY("GPT1",
			      GPT1_BASE, KB(64),
			      MT_DEVICE_nGnRE | MT_P_RW_U_RW | MT_NS),

	MMU_REGION_FLAT_ENTRY("GPT2",
			      GPT2_BASE, KB(64),
			      MT_DEVICE_nGnRE | MT_P_RW_U_RW | MT_NS),

	MMU_REGION_FLAT_ENTRY("IOMUXC",
			      IOMUXC_BASE, KB(64),
			      MT_DEVICE_nGnRE | MT_P_RW_U_RW | MT_NS),

	MMU_REGION_FLAT_ENTRY("RDC",
			      RDC_BASE, KB(64),
			      MT_DEVICE_nGnRE | MT_P_RW_U_RW | MT_NS),

	MMU_REGION_FLAT_ENTRY("UART2",
			      UART2_BASE, KB(64),
			      MT_DEVICE_nGnRE | MT_P_RW_U_RW | MT_NS),

	MMU_REGION_FLAT_ENTRY("UART4",
			      UART4_BASE, KB(64),
			      MT_DEVICE_nGnRE | MT_P_RW_U_RW | MT_NS),

#ifdef APP_MMU_ENTRIES
	APP_MMU_ENTRIES
#endif
};

/* MMU configuration. */
static const struct ARM_MMU_config mmu_config = {
	.num_regions = ARRAY_SIZE(mmu_regions),
	.mmu_regions = mmu_regions,
	.num_os_ranges = ARRAY_SIZE(mmu_os_ranges),
	.mmu_os_ranges = mmu_os_ranges,
};

void MMU_init(void)
{
    return ARM_MMU_Initialize(&mmu_config, true);
}
