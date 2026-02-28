/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _OUTPUT_POSTPROCESS_H_
#define _OUTPUT_POSTPROCESS_H_

#include "model.h"
#include "app.h"


#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#if TIMING
status_t MODEL_ProcessOutput(const uint8_t* data, const tensor_dims_t* dims,
                             tensor_type_t type, int inferenceTime);
#else
status_t MODEL_ProcessOutput(const uint8_t* data, const tensor_dims_t* dims,
                             tensor_type_t type);
#endif

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* _OUTPUT_POSTPROCESS_H_ */
