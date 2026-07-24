/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _ISI_CONFIG_H_
#define _ISI_CONFIG_H_

/*${macro:start}*/
/* Camera interface: MIPI CSI-2. */
#ifndef ISI_EXAMPLE_CI
#define ISI_EXAMPLE_CI ISI_MIPI_CSI2
#endif

#define APP_CAMERA_WIDTH    320U
#define APP_CAMERA_HEIGHT   240U
#define APP_CAMERA_FRAME_RATE 30U

/*${macro:end}*/

#endif /* _ISI_CONFIG_H_ */
