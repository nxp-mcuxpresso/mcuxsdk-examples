/*
 * Copyright 2022-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "tensorflow/lite/micro/kernels/micro_ops.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/kernels/neutron/neutron.h"
#include "app.h"

tflite::MicroOpResolver &MODEL_GetOpsResolver()
{
  static tflite::MicroMutableOpResolver<10> s_microOpResolver;
#if RUN_CPU
    s_microOpResolver.AddAveragePool2D();
    s_microOpResolver.AddConv2D();
    s_microOpResolver.AddDepthwiseConv2D();
    s_microOpResolver.AddDequantize();
    s_microOpResolver.AddReshape();
    s_microOpResolver.AddSoftmax();
    s_microOpResolver.AddFullyConnected();
    s_microOpResolver.AddBatchMatMul();
    s_microOpResolver.AddAdd();
    s_microOpResolver.AddSlice();

#else
s_microOpResolver.AddQuantize();
s_microOpResolver.AddReshape();
s_microOpResolver.AddGather();
s_microOpResolver.AddMul();
s_microOpResolver.AddDequantize();
s_microOpResolver.AddPad();
s_microOpResolver.AddSoftmax();
s_microOpResolver.AddSlice();
s_microOpResolver.AddCustom(tflite::GetString_NEUTRON_GRAPH(),
        tflite::Register_NEUTRON_GRAPH());
#endif


    return s_microOpResolver;
}
