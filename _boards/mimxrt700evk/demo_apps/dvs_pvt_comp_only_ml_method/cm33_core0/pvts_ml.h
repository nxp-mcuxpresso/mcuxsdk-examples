/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef PVTS_ML_H_
#define PVTS_ML_H_

#include <fsl_pvts.h>
#include "model.h"
#include <math.h>

/*******************************************************************************
 * Defines
 ******************************************************************************/
typedef enum pvts_core
{
    kPVTS_CPU0  = 1U, /*!< PVTS for CPU0 */
    kPVTS_HIFI4 = 2U, /*!< PVTS for HIFI4 */
    kPVTS_CPU1  = 3U, /*!< PVTS for CPU1 */
    kPVTS_HIFI1 = 4U, /*!< PVTS for HIFI1 */
} pvts_core_t;

/* standard deviation and means values are coming from the model creation.
 * Needed to properly normalized the model's inputs.
 */
AT_QUICKACCESS_SECTION_DATA(float mean_values[5]) = {2.50007130, 0.00136128578, 0.606377193, 177.49753, 0.950390922};
AT_QUICKACCESS_SECTION_DATA(float std_values[5])  = {1.11804042, 0.000206560172, 0.0170352355, 86.5895243, 0.27118233};


#if defined(__cplusplus)
extern "C" {
#endif
static inline void standardize_data(float data[5])
{
    for (int i = 0; i < 5; i++)
    {
        data[i] = (data[i] - mean_values[i]) / std_values[i];
    }
}

/*!
 * @brief Retrieves the PVT delay for a specific frequency depending on the A (slope) and B (intercept) parameters
 * previously read in the OTPs.
 *
 * @param model_initialized Specifies whether the ML model is already initialized or not.
 * @param core Specify with core is targeted for the PVTS.
 * @param core_freq_mhz Core clock frequency in megahertz.
 * @param parameters A and B parameters from OTPs to deduce the Vmin.
 * @param delay Pointer to a uint32_t where the PVT delay resulting from the ML model inference will be returned.
 * @retval kStatus_Success Successfully executes the ML model and get the PVTS delay.
 * @retval kStatus_Fail Failed to execute the ML model and get the PVTS delay.
 */
static inline status_t PVTS0DeduceDelayUsingML(
    bool model_initialized, pvts_core_t core, uint32_t core_freq_mhz, uint32_t parameters, uint32_t *delay)
{
    uint32_t slope_raw, intercept_raw;
    float slope, intercept;
    float vmin;
    float input_model_raw[5];

    /* Enables the NPU */
    POWER_DisablePD(kPDRUNCFG_APD_NPU);
    POWER_DisablePD(kPDRUNCFG_PPD_NPU);
    POWER_ApplyPD();

    /* Retrieves encoded slope and intercept */
    slope_raw     = parameters & 0xFFFF;
    intercept_raw = (parameters >> 16) & 0xFFFF;

    /* Decodes slope and intercept.
     * Slope = OTP[15:0] / 1000000;
     * Intercept = OTP[31:16] / 10000;
     */
    slope     = slope_raw / 1000000.0f;
    intercept = intercept_raw / 10000.0f;
    vmin      = (slope * core_freq_mhz) + intercept;

    input_model_raw[0] = core;
    input_model_raw[1] = slope;
    input_model_raw[2] = intercept;
    input_model_raw[3] = core_freq_mhz;
    input_model_raw[4] = vmin;

    if (!model_initialized)
    {
        if (MODEL_Init() != kStatus_Success)
        {
            return kStatus_Fail;
        }
    }

    tensor_dims_t inputDims;
    tensor_type_t inputType;
    uint8_t *inputData = MODEL_GetInputTensorData(&inputDims, &inputType);

    standardize_data(input_model_raw);
    (void)memcpy(inputData, input_model_raw, sizeof(input_model_raw) * 5);

    tensor_dims_t outputDims;
    tensor_type_t outputType;
    uint8_t *outputData = MODEL_GetOutputTensorData(&outputDims, &outputType);

    (void)MODEL_RunInference();

    const float *delay_float = (const float *)outputData;
    int delay_int            = (int)(round((double)*delay_float));

    if ((delay_int < PVTS_DELAY_MIN) || (delay_int > PVTS_DELAY_MAX))
    {
        return kStatus_Fail;
    }

    /* Disables the NPU */
    POWER_EnablePD(kPDRUNCFG_APD_NPU);
    POWER_EnablePD(kPDRUNCFG_PPD_NPU);
    POWER_ApplyPD();

    *delay = delay_int;

    return kStatus_Success;
}
/*@}*/

#if defined(__cplusplus)
}
#endif

#endif /* PVTS_ML_H_ */
