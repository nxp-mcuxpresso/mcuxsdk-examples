/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _DMIC_CONFIG_H_
#define _DMIC_CONFIG_H_
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*${macro:start}*/
#define DEMO_DMA                 DMA0
#define DEMO_DMIC_RX_CHANNEL     17
#define DEMO_DMIC_CHANNEL        kDMIC_Channel1
#define DEMO_DMIC_CHANNEL_ENABLE DMIC_CHANEN_EN_CH1(1)
#define DEMO_DMIC_OSR            (25U)
/*${macro:end}*/

#endif /* _DMIC_CONFIG_H_ */
