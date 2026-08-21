/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Hand-written kernel registration table for executorch_modelrunner.
 *
 * Registers a "common models" op subset (cifarnet / kws / mobilenet /
 * anomaly_detection / small transformers) as a static Kernel[] table.
 * Registration happens at static-init time via register_kernels().
 *
 * Why hand-written instead of the generated
 * RegisterCodegenUnboxedKernelsEverything:
 *   - The generated "Everything" table references ops that are not compiled
 *     into the shipped libexecutorch.a, and it pulls ~2.9 MB of .text —
 *     larger than the internal flash region. Hand-listing only the ops that
 *     exist in NativeFunctions.h + libexecutorch.a keeps the build valid and
 *     the .text bounded.
 *
 * Every op below must exist in NativeFunctions.h (its *_out signature) and in
 * libexecutorch.a (its symbol). When adding an op, copy its unboxing pattern
 * from an existing entry: each lambda pulls EValues off the stack, casts them
 * to the C++ types the native *_out function expects (Tensor, Scalar, int64_t,
 * bool, double, optional<...>, ArrayRef via toIntList(), TensorList via
 * toTensorList(), string_view via toStringRef()), then calls
 * torch::executor::native::<op>_out(...).
 */

#include <executorch/runtime/core/evalue.h>
#include <executorch/runtime/core/exec_aten/exec_aten.h>
#include <executorch/runtime/core/exec_aten/util/tensor_util.h>
#include <executorch/runtime/core/span.h>
#include <executorch/runtime/kernel/operator_registry.h>
#include <executorch/runtime/platform/profiler.h>
#include "NativeFunctions.h" // Generated Function import headers

using KernelSpan = ::executorch::runtime::Span<
    const ::executorch::ET_RUNTIME_NAMESPACE::Kernel>;
namespace torch {
namespace executor {
namespace function {
namespace {

static Kernel kernels_to_register[] = {

// ===========================================================================
// Basic op set shared by the CIFAR-style models: add, addmm, _softmax,
// constant_pad_nd, convolution, max_pool2d_with_indices, permute_copy,
// quantize_per_tensor, dequantize_per_tensor.
// ===========================================================================

Kernel(
    "aten::add.out",
    [](torch::executor::KernelRuntimeContext & context, Span<EValue *> stack) {
        ET_KERNEL_CHECK_MSG(
            context, stack.size() == 5, InvalidProgram, /*void*/,
            "Expected %" ET_PRIsize_t "args received %" ET_PRIsize_t, (size_t)5,
            stack.size());
        EValue &self = *stack[0];
        EValue &other = *stack[1];
        EValue &alpha = *stack[2];
        EValue &out = *stack[3];
        const torch::executor::Tensor &self_base = self.to<torch::executor::Tensor>();
        const torch::executor::Tensor &other_base = other.to<torch::executor::Tensor>();
        const torch::executor::Scalar &alpha_base = alpha.to<torch::executor::Scalar>();
        torch::executor::Tensor &out_base = out.to<torch::executor::Tensor>();

        internal::EventTracerProfileOpScope event_tracer_op_scope(
            context.internal_event_tracer(), "native_call_add.out");
        EXECUTORCH_SCOPE_PROF("native_call_add.out");
        torch::executor::native::add_out(
            context, self_base, other_base, alpha_base, out_base);

        internal::event_tracer_log_evalue(context.internal_event_tracer(), *stack[3]);
    }),

Kernel(
    "aten::addmm.out",
    [](torch::executor::KernelRuntimeContext & context, Span<EValue *> stack) {
        ET_KERNEL_CHECK_MSG(
            context, stack.size() == 7, InvalidProgram, /*void*/,
            "Expected %" ET_PRIsize_t "args received %" ET_PRIsize_t, (size_t)7,
            stack.size());
        EValue &self = *stack[0];
        EValue &mat1 = *stack[1];
        EValue &mat2 = *stack[2];
        EValue &beta = *stack[3];
        EValue &alpha = *stack[4];
        EValue &out = *stack[5];
        const torch::executor::Tensor &self_base = self.to<torch::executor::Tensor>();
        const torch::executor::Tensor &mat1_base = mat1.to<torch::executor::Tensor>();
        const torch::executor::Tensor &mat2_base = mat2.to<torch::executor::Tensor>();
        const torch::executor::Scalar &beta_base = beta.to<torch::executor::Scalar>();
        const torch::executor::Scalar &alpha_base = alpha.to<torch::executor::Scalar>();
        torch::executor::Tensor &out_base = out.to<torch::executor::Tensor>();

        internal::EventTracerProfileOpScope event_tracer_op_scope(
            context.internal_event_tracer(), "native_call_addmm.out");
        EXECUTORCH_SCOPE_PROF("native_call_addmm.out");
        torch::executor::native::addmm_out(
            context, self_base, mat1_base, mat2_base, beta_base, alpha_base,
            out_base);

        internal::event_tracer_log_evalue(context.internal_event_tracer(), *stack[5]);
    }),

Kernel(
    "aten::_softmax.out",
    [](torch::executor::KernelRuntimeContext & context, Span<EValue *> stack) {
        ET_KERNEL_CHECK_MSG(
            context, stack.size() == 5, InvalidProgram, /*void*/,
            "Expected %" ET_PRIsize_t "args received %" ET_PRIsize_t, (size_t)5,
            stack.size());
        EValue &self = *stack[0];
        EValue &dim = *stack[1];
        EValue &half_to_float = *stack[2];
        EValue &out = *stack[3];
        const torch::executor::Tensor &self_base = self.to<torch::executor::Tensor>();
        int64_t dim_base = dim.to<int64_t>();
        bool half_to_float_base = half_to_float.to<bool>();
        torch::executor::Tensor &out_base = out.to<torch::executor::Tensor>();

        internal::EventTracerProfileOpScope event_tracer_op_scope(
            context.internal_event_tracer(), "native_call__softmax.out");
        EXECUTORCH_SCOPE_PROF("native_call__softmax.out");
        torch::executor::native::softmax_out(
            context, self_base, dim_base, half_to_float_base, out_base);

        internal::event_tracer_log_evalue(context.internal_event_tracer(), *stack[3]);
    }),

Kernel(
    "aten::constant_pad_nd.out",
    [](torch::executor::KernelRuntimeContext & context, Span<EValue *> stack) {
        ET_KERNEL_CHECK_MSG(
            context, stack.size() == 5, InvalidProgram, /*void*/,
            "Expected %" ET_PRIsize_t "args received %" ET_PRIsize_t, (size_t)5,
            stack.size());
        EValue &self = *stack[0];
        EValue &pad = *stack[1];
        EValue &value = *stack[2];
        EValue &out = *stack[3];
        const torch::executor::Tensor &self_base = self.to<torch::executor::Tensor>();
        auto pad_list_out = pad.toIntList();
        const torch::executor::Scalar &value_base = value.to<torch::executor::Scalar>();
        torch::executor::Tensor &out_base = out.to<torch::executor::Tensor>();

        internal::EventTracerProfileOpScope event_tracer_op_scope(
            context.internal_event_tracer(), "native_call_constant_pad_nd.out");
        EXECUTORCH_SCOPE_PROF("native_call_constant_pad_nd.out");
        torch::executor::native::constant_pad_nd_out(
            context, self_base, pad_list_out, value_base, out_base);

        internal::event_tracer_log_evalue(context.internal_event_tracer(), *stack[3]);
    }),

Kernel(
    "aten::convolution.out",
    [](torch::executor::KernelRuntimeContext & context, Span<EValue *> stack) {
        ET_KERNEL_CHECK_MSG(
            context, stack.size() == 11, InvalidProgram, /*void*/,
            "Expected %" ET_PRIsize_t "args received %" ET_PRIsize_t, (size_t)11,
            stack.size());
        EValue &input = *stack[0];
        EValue &weight = *stack[1];
        EValue &bias = *stack[2];
        EValue &stride = *stack[3];
        EValue &padding = *stack[4];
        EValue &dilation = *stack[5];
        EValue &transposed = *stack[6];
        EValue &output_padding = *stack[7];
        EValue &groups = *stack[8];
        EValue &out = *stack[9];
        const torch::executor::Tensor &input_base = input.to<torch::executor::Tensor>();
        const torch::executor::Tensor &weight_base = weight.to<torch::executor::Tensor>();
        auto bias_opt_out = bias.toOptional<torch::executor::Tensor>();
        auto stride_list_out = stride.toIntList();
        auto padding_list_out = padding.toIntList();
        auto dilation_list_out = dilation.toIntList();
        bool transposed_base = transposed.to<bool>();
        auto output_padding_list_out = output_padding.toIntList();
        int64_t groups_base = groups.to<int64_t>();
        torch::executor::Tensor &out_base = out.to<torch::executor::Tensor>();

        internal::EventTracerProfileOpScope event_tracer_op_scope(
            context.internal_event_tracer(), "native_call_convolution.out");
        EXECUTORCH_SCOPE_PROF("native_call_convolution.out");
        torch::executor::native::convolution_out(
            context, input_base, weight_base, bias_opt_out, stride_list_out,
            padding_list_out, dilation_list_out, transposed_base,
            output_padding_list_out, groups_base, out_base);

        internal::event_tracer_log_evalue(context.internal_event_tracer(), *stack[9]);
    }),

Kernel(
    "aten::max_pool2d_with_indices.out",
    [](torch::executor::KernelRuntimeContext & context, Span<EValue *> stack) {
        ET_KERNEL_CHECK_MSG(
            context, stack.size() == 9, InvalidProgram, /*void*/,
            "Expected %" ET_PRIsize_t "args received %" ET_PRIsize_t, (size_t)9,
            stack.size());
        EValue &self = *stack[0];
        EValue &kernel_size = *stack[1];
        EValue &stride = *stack[2];
        EValue &padding = *stack[3];
        EValue &dilation = *stack[4];
        EValue &ceil_mode = *stack[5];
        EValue &out = *stack[6];
        EValue &indices = *stack[7];
        const torch::executor::Tensor &self_base = self.to<torch::executor::Tensor>();
        auto kernel_size_list_out = kernel_size.toIntList();
        auto stride_list_out = stride.toIntList();
        auto padding_list_out = padding.toIntList();
        auto dilation_list_out = dilation.toIntList();
        bool ceil_mode_base = ceil_mode.to<bool>();
        torch::executor::Tensor &out_base = out.to<torch::executor::Tensor>();
        torch::executor::Tensor &indices_base = indices.to<torch::executor::Tensor>();

        internal::EventTracerProfileOpScope event_tracer_op_scope(
            context.internal_event_tracer(), "native_call_max_pool2d_with_indices.out");
        EXECUTORCH_SCOPE_PROF("native_call_max_pool2d_with_indices.out");
        torch::executor::native::max_pool2d_with_indices_out(
            context, self_base, kernel_size_list_out, stride_list_out,
            padding_list_out, dilation_list_out, ceil_mode_base, out_base,
            indices_base);

        internal::event_tracer_log_evalue(context.internal_event_tracer(), *stack[6]);
        internal::event_tracer_log_evalue(context.internal_event_tracer(), *stack[7]);
    }),

// Schema (kernels/portable/functions.yaml: avg_pool2d.out):
//   (Tensor self, int[2] kernel_size, int[2] stride, int[2] padding,
//    bool ceil_mode, bool count_include_pad, int? divisor_override, *,
//    Tensor(a!) out) -> Tensor(a!)
// Used by models with average pooling (e.g. DS-CNN, MobileNetV1); without it
// load_method() fails with InvalidProgram. stack.size() = schema args + 1.
Kernel(
    "aten::avg_pool2d.out",
    [](torch::executor::KernelRuntimeContext & context, Span<EValue *> stack) {
        ET_KERNEL_CHECK_MSG(
            context, stack.size() == 9, InvalidProgram, /*void*/,
            "Expected %" ET_PRIsize_t "args received %" ET_PRIsize_t, (size_t)9,
            stack.size());
        EValue &self = *stack[0];
        EValue &kernel_size = *stack[1];
        EValue &stride = *stack[2];
        EValue &padding = *stack[3];
        EValue &ceil_mode = *stack[4];
        EValue &count_include_pad = *stack[5];
        EValue &divisor_override = *stack[6];
        EValue &out = *stack[7];
        const torch::executor::Tensor &self_base = self.to<torch::executor::Tensor>();
        auto kernel_size_list_out = kernel_size.toIntList();
        auto stride_list_out = stride.toIntList();
        auto padding_list_out = padding.toIntList();
        bool ceil_mode_base = ceil_mode.to<bool>();
        bool count_include_pad_base = count_include_pad.to<bool>();
        auto divisor_override_opt = divisor_override.toOptional<int64_t>();
        torch::executor::Tensor &out_base = out.to<torch::executor::Tensor>();

        internal::EventTracerProfileOpScope event_tracer_op_scope(
            context.internal_event_tracer(), "native_call_avg_pool2d.out");
        EXECUTORCH_SCOPE_PROF("native_call_avg_pool2d.out");
        torch::executor::native::avg_pool2d_out(
            context, self_base, kernel_size_list_out, stride_list_out,
            padding_list_out, ceil_mode_base, count_include_pad_base,
            divisor_override_opt, out_base);

        internal::event_tracer_log_evalue(context.internal_event_tracer(), *stack[7]);
    }),

// Schema (kernels/portable/functions.yaml:1068):
//   dim_order_ops::_clone_dim_order.out(Tensor self, *, bool non_blocking=False,
//     int[]? dim_order=None, Tensor(a!) out) -> Tensor(a!)
// Emitted by the AOT dim_order pass when a tensor's memory layout needs an
// explicit copy. stack.size() is schema args + 1 (4 args -> 5).
Kernel(
    "dim_order_ops::_clone_dim_order.out",
    [](torch::executor::KernelRuntimeContext & context, Span<EValue *> stack) {
        ET_KERNEL_CHECK_MSG(
            context, stack.size() == 5, InvalidProgram, /*void*/,
            "Expected %" ET_PRIsize_t "args received %" ET_PRIsize_t, (size_t)5,
            stack.size());
        EValue &self = *stack[0];
        EValue &non_blocking = *stack[1];
        EValue &dim_order = *stack[2];
        EValue &out = *stack[3];
        const torch::executor::Tensor &self_base = self.to<torch::executor::Tensor>();
        bool non_blocking_base = non_blocking.to<bool>();
        auto dim_order_opt =
            dim_order.toOptional<torch::executor::ArrayRef<int64_t>>();
        torch::executor::Tensor &out_base = out.to<torch::executor::Tensor>();

        internal::EventTracerProfileOpScope event_tracer_op_scope(
            context.internal_event_tracer(), "native_call__clone_dim_order.out");
        EXECUTORCH_SCOPE_PROF("native_call__clone_dim_order.out");
        torch::executor::native::_clone_dim_order_out(
            context, self_base, non_blocking_base, dim_order_opt, out_base);

        internal::event_tracer_log_evalue(context.internal_event_tracer(), *stack[3]);
    }),

Kernel(
    "aten::permute_copy.out",
    [](torch::executor::KernelRuntimeContext & context, Span<EValue *> stack) {
        ET_KERNEL_CHECK_MSG(
            context, stack.size() == 4, InvalidProgram, /*void*/,
            "Expected %" ET_PRIsize_t "args received %" ET_PRIsize_t, (size_t)4,
            stack.size());
        EValue &self = *stack[0];
        EValue &dims = *stack[1];
        EValue &out = *stack[2];
        const torch::executor::Tensor &self_base = self.to<torch::executor::Tensor>();
        auto dims_list_out = dims.toIntList();
        torch::executor::Tensor &out_base = out.to<torch::executor::Tensor>();

        internal::EventTracerProfileOpScope event_tracer_op_scope(
            context.internal_event_tracer(), "native_call_permute_copy.out");
        EXECUTORCH_SCOPE_PROF("native_call_permute_copy.out");
        torch::executor::native::permute_copy_out(
            context, self_base, dims_list_out, out_base);

        internal::event_tracer_log_evalue(context.internal_event_tracer(), *stack[2]);
    }),

Kernel(
    "quantized_decomposed::dequantize_per_tensor.out",
    [](torch::executor::KernelRuntimeContext & context, Span<EValue *> stack) {
        ET_KERNEL_CHECK_MSG(
            context, stack.size() == 9, InvalidProgram, /*void*/,
            "Expected %" ET_PRIsize_t "args received %" ET_PRIsize_t, (size_t)9,
            stack.size());
        EValue &input = *stack[0];
        EValue &scale = *stack[1];
        EValue &zero_point = *stack[2];
        EValue &quant_min = *stack[3];
        EValue &quant_max = *stack[4];
        EValue &dtype = *stack[5];
        EValue &out_dtype = *stack[6];
        EValue &out = *stack[7];
        const torch::executor::Tensor &input_base = input.to<torch::executor::Tensor>();
        double scale_base = scale.to<double>();
        int64_t zero_point_base = zero_point.to<int64_t>();
        int64_t quant_min_base = quant_min.to<int64_t>();
        int64_t quant_max_base = quant_max.to<int64_t>();
        torch::executor::ScalarType dtype_base = dtype.to<torch::executor::ScalarType>();
        auto out_dtype_opt_out = out_dtype.toOptional<torch::executor::ScalarType>();
        torch::executor::Tensor &out_base = out.to<torch::executor::Tensor>();

        internal::EventTracerProfileOpScope event_tracer_op_scope(
            context.internal_event_tracer(), "native_call_dequantize_per_tensor.out");
        EXECUTORCH_SCOPE_PROF("native_call_dequantize_per_tensor.out");
        torch::executor::native::dequantize_per_tensor_out(
            context, input_base, scale_base, zero_point_base, quant_min_base,
            quant_max_base, dtype_base, out_dtype_opt_out, out_base);

        internal::event_tracer_log_evalue(context.internal_event_tracer(), *stack[7]);
    }),

// Schema (kernels/quantized/quantized.yaml):
//   (Tensor input, Tensor scales, Tensor? zero_points, int axis, int
//    quant_min, int quant_max, ScalarType dtype, *, ScalarType? out_dtype=None,
//    Tensor(a!) out) -> Tensor(a!)
// Needed by delegate-free (CPU) ptes to dequantize per-channel quantized
// conv weights; without it load_method() fails with InvalidProgram.
// stack.size() is schema args + 1 (the convention of every entry here).
Kernel(
    "quantized_decomposed::dequantize_per_channel.out",
    [](torch::executor::KernelRuntimeContext & context, Span<EValue *> stack) {
        ET_KERNEL_CHECK_MSG(
            context, stack.size() == 10, InvalidProgram, /*void*/,
            "Expected %" ET_PRIsize_t "args received %" ET_PRIsize_t, (size_t)10,
            stack.size());
        EValue &input = *stack[0];
        EValue &scales = *stack[1];
        EValue &zero_points = *stack[2];
        EValue &axis = *stack[3];
        EValue &quant_min = *stack[4];
        EValue &quant_max = *stack[5];
        EValue &dtype = *stack[6];
        EValue &out_dtype = *stack[7];
        EValue &out = *stack[8];
        const torch::executor::Tensor &input_base = input.to<torch::executor::Tensor>();
        const torch::executor::Tensor &scales_base = scales.to<torch::executor::Tensor>();
        auto zero_points_opt_base = zero_points.toOptional<torch::executor::Tensor>();
        int64_t axis_base = axis.to<int64_t>();
        int64_t quant_min_base = quant_min.to<int64_t>();
        int64_t quant_max_base = quant_max.to<int64_t>();
        torch::executor::ScalarType dtype_base = dtype.to<torch::executor::ScalarType>();
        auto out_dtype_opt_out = out_dtype.toOptional<torch::executor::ScalarType>();
        torch::executor::Tensor &out_base = out.to<torch::executor::Tensor>();

        internal::EventTracerProfileOpScope event_tracer_op_scope(
            context.internal_event_tracer(), "native_call_dequantize_per_channel.out");
        EXECUTORCH_SCOPE_PROF("native_call_dequantize_per_channel.out");
        torch::executor::native::dequantize_per_channel_out(
            context, input_base, scales_base, zero_points_opt_base, axis_base,
            quant_min_base, quant_max_base, dtype_base, out_dtype_opt_out, out_base);

        internal::event_tracer_log_evalue(context.internal_event_tracer(), *stack[8]);
    }),

Kernel(
    "quantized_decomposed::quantize_per_tensor.out",
    [](torch::executor::KernelRuntimeContext & context, Span<EValue *> stack) {
        ET_KERNEL_CHECK_MSG(
            context, stack.size() == 8, InvalidProgram, /*void*/,
            "Expected %" ET_PRIsize_t "args received %" ET_PRIsize_t, (size_t)8,
            stack.size());
        EValue &input = *stack[0];
        EValue &scale = *stack[1];
        EValue &zero_point = *stack[2];
        EValue &quant_min = *stack[3];
        EValue &quant_max = *stack[4];
        EValue &dtype = *stack[5];
        EValue &out = *stack[6];
        const torch::executor::Tensor &input_base = input.to<torch::executor::Tensor>();
        double scale_base = scale.to<double>();
        int64_t zero_point_base = zero_point.to<int64_t>();
        int64_t quant_min_base = quant_min.to<int64_t>();
        int64_t quant_max_base = quant_max.to<int64_t>();
        torch::executor::ScalarType dtype_base = dtype.to<torch::executor::ScalarType>();
        torch::executor::Tensor &out_base = out.to<torch::executor::Tensor>();

        internal::EventTracerProfileOpScope event_tracer_op_scope(
            context.internal_event_tracer(), "native_call_quantize_per_tensor.out");
        EXECUTORCH_SCOPE_PROF("native_call_quantize_per_tensor.out");
        torch::executor::native::quantize_per_tensor_out(
            context, input_base, scale_base, zero_point_base, quant_min_base,
            quant_max_base, dtype_base, out_base);

        internal::event_tracer_log_evalue(context.internal_event_tracer(), *stack[6]);
    }),

// ===========================================================================
// Arithmetic: sub, mul, div
// ===========================================================================

Kernel(
    "aten::sub.out",
    [](torch::executor::KernelRuntimeContext & context, Span<EValue *> stack) {
        ET_KERNEL_CHECK_MSG(
            context, stack.size() == 5, InvalidProgram, /*void*/,
            "Expected %" ET_PRIsize_t "args received %" ET_PRIsize_t, (size_t)5,
            stack.size());
        EValue &self = *stack[0];
        EValue &other = *stack[1];
        EValue &alpha = *stack[2];
        EValue &out = *stack[3];
        const torch::executor::Tensor &self_base = self.to<torch::executor::Tensor>();
        const torch::executor::Tensor &other_base = other.to<torch::executor::Tensor>();
        const torch::executor::Scalar &alpha_base = alpha.to<torch::executor::Scalar>();
        torch::executor::Tensor &out_base = out.to<torch::executor::Tensor>();

        internal::EventTracerProfileOpScope event_tracer_op_scope(
            context.internal_event_tracer(), "native_call_sub.out");
        EXECUTORCH_SCOPE_PROF("native_call_sub.out");
        torch::executor::native::sub_out(
            context, self_base, other_base, alpha_base, out_base);

        internal::event_tracer_log_evalue(context.internal_event_tracer(), *stack[3]);
    }),

Kernel(
    "aten::mul.out",
    [](torch::executor::KernelRuntimeContext & context, Span<EValue *> stack) {
        ET_KERNEL_CHECK_MSG(
            context, stack.size() == 4, InvalidProgram, /*void*/,
            "Expected %" ET_PRIsize_t "args received %" ET_PRIsize_t, (size_t)4,
            stack.size());
        EValue &self = *stack[0];
        EValue &other = *stack[1];
        EValue &out = *stack[2];
        const torch::executor::Tensor &self_base = self.to<torch::executor::Tensor>();
        const torch::executor::Tensor &other_base = other.to<torch::executor::Tensor>();
        torch::executor::Tensor &out_base = out.to<torch::executor::Tensor>();

        internal::EventTracerProfileOpScope event_tracer_op_scope(
            context.internal_event_tracer(), "native_call_mul.out");
        EXECUTORCH_SCOPE_PROF("native_call_mul.out");
        torch::executor::native::mul_out(context, self_base, other_base, out_base);

        internal::event_tracer_log_evalue(context.internal_event_tracer(), *stack[2]);
    }),

Kernel(
    "aten::div.out",
    [](torch::executor::KernelRuntimeContext & context, Span<EValue *> stack) {
        ET_KERNEL_CHECK_MSG(
            context, stack.size() == 4, InvalidProgram, /*void*/,
            "Expected %" ET_PRIsize_t "args received %" ET_PRIsize_t, (size_t)4,
            stack.size());
        EValue &self = *stack[0];
        EValue &other = *stack[1];
        EValue &out = *stack[2];
        const torch::executor::Tensor &self_base = self.to<torch::executor::Tensor>();
        const torch::executor::Tensor &other_base = other.to<torch::executor::Tensor>();
        torch::executor::Tensor &out_base = out.to<torch::executor::Tensor>();

        internal::EventTracerProfileOpScope event_tracer_op_scope(
            context.internal_event_tracer(), "native_call_div.out");
        EXECUTORCH_SCOPE_PROF("native_call_div.out");
        torch::executor::native::div_out(context, self_base, other_base, out_base);

        internal::event_tracer_log_evalue(context.internal_event_tracer(), *stack[2]);
    }),

// ===========================================================================
// Activations: relu, sigmoid, tanh, leaky_relu, gelu
// ===========================================================================

Kernel(
    "aten::relu.out",
    [](torch::executor::KernelRuntimeContext & context, Span<EValue *> stack) {
        ET_KERNEL_CHECK_MSG(
            context, stack.size() == 3, InvalidProgram, /*void*/,
            "Expected %" ET_PRIsize_t "args received %" ET_PRIsize_t, (size_t)3,
            stack.size());
        EValue &self = *stack[0];
        EValue &out = *stack[1];
        const torch::executor::Tensor &self_base = self.to<torch::executor::Tensor>();
        torch::executor::Tensor &out_base = out.to<torch::executor::Tensor>();

        internal::EventTracerProfileOpScope event_tracer_op_scope(
            context.internal_event_tracer(), "native_call_relu.out");
        EXECUTORCH_SCOPE_PROF("native_call_relu.out");
        torch::executor::native::relu_out(context, self_base, out_base);

        internal::event_tracer_log_evalue(context.internal_event_tracer(), *stack[1]);
    }),

Kernel(
    "aten::sigmoid.out",
    [](torch::executor::KernelRuntimeContext & context, Span<EValue *> stack) {
        ET_KERNEL_CHECK_MSG(
            context, stack.size() == 3, InvalidProgram, /*void*/,
            "Expected %" ET_PRIsize_t "args received %" ET_PRIsize_t, (size_t)3,
            stack.size());
        EValue &self = *stack[0];
        EValue &out = *stack[1];
        const torch::executor::Tensor &self_base = self.to<torch::executor::Tensor>();
        torch::executor::Tensor &out_base = out.to<torch::executor::Tensor>();

        internal::EventTracerProfileOpScope event_tracer_op_scope(
            context.internal_event_tracer(), "native_call_sigmoid.out");
        EXECUTORCH_SCOPE_PROF("native_call_sigmoid.out");
        torch::executor::native::sigmoid_out(context, self_base, out_base);

        internal::event_tracer_log_evalue(context.internal_event_tracer(), *stack[1]);
    }),

Kernel(
    "aten::tanh.out",
    [](torch::executor::KernelRuntimeContext & context, Span<EValue *> stack) {
        ET_KERNEL_CHECK_MSG(
            context, stack.size() == 3, InvalidProgram, /*void*/,
            "Expected %" ET_PRIsize_t "args received %" ET_PRIsize_t, (size_t)3,
            stack.size());
        EValue &self = *stack[0];
        EValue &out = *stack[1];
        const torch::executor::Tensor &self_base = self.to<torch::executor::Tensor>();
        torch::executor::Tensor &out_base = out.to<torch::executor::Tensor>();

        internal::EventTracerProfileOpScope event_tracer_op_scope(
            context.internal_event_tracer(), "native_call_tanh.out");
        EXECUTORCH_SCOPE_PROF("native_call_tanh.out");
        torch::executor::native::tanh_out(context, self_base, out_base);

        internal::event_tracer_log_evalue(context.internal_event_tracer(), *stack[1]);
    }),

Kernel(
    "aten::leaky_relu.out",
    [](torch::executor::KernelRuntimeContext & context, Span<EValue *> stack) {
        ET_KERNEL_CHECK_MSG(
            context, stack.size() == 4, InvalidProgram, /*void*/,
            "Expected %" ET_PRIsize_t "args received %" ET_PRIsize_t, (size_t)4,
            stack.size());
        EValue &self = *stack[0];
        EValue &negative_slope = *stack[1];
        EValue &out = *stack[2];
        const torch::executor::Tensor &self_base = self.to<torch::executor::Tensor>();
        const torch::executor::Scalar &negative_slope_base =
            negative_slope.to<torch::executor::Scalar>();
        torch::executor::Tensor &out_base = out.to<torch::executor::Tensor>();

        internal::EventTracerProfileOpScope event_tracer_op_scope(
            context.internal_event_tracer(), "native_call_leaky_relu.out");
        EXECUTORCH_SCOPE_PROF("native_call_leaky_relu.out");
        torch::executor::native::leaky_relu_out(
            context, self_base, negative_slope_base, out_base);

        internal::event_tracer_log_evalue(context.internal_event_tracer(), *stack[2]);
    }),

Kernel(
    "aten::gelu.out",
    [](torch::executor::KernelRuntimeContext & context, Span<EValue *> stack) {
        ET_KERNEL_CHECK_MSG(
            context, stack.size() == 4, InvalidProgram, /*void*/,
            "Expected %" ET_PRIsize_t "args received %" ET_PRIsize_t, (size_t)4,
            stack.size());
        EValue &self = *stack[0];
        EValue &approximate = *stack[1];
        EValue &out = *stack[2];
        const torch::executor::Tensor &self_base = self.to<torch::executor::Tensor>();
        torch::executor::string_view approximate_base =
            approximate.to<torch::executor::string_view>();
        torch::executor::Tensor &out_base = out.to<torch::executor::Tensor>();

        internal::EventTracerProfileOpScope event_tracer_op_scope(
            context.internal_event_tracer(), "native_call_gelu.out");
        EXECUTORCH_SCOPE_PROF("native_call_gelu.out");
        torch::executor::native::gelu_out(
            context, self_base, approximate_base, out_base);

        internal::event_tracer_log_evalue(context.internal_event_tracer(), *stack[2]);
    }),

// ===========================================================================
// Normalization: native_layer_norm, _native_batch_norm_legit
// ===========================================================================

Kernel(
    "aten::native_layer_norm.out",
    [](torch::executor::KernelRuntimeContext & context, Span<EValue *> stack) {
        ET_KERNEL_CHECK_MSG(
            context, stack.size() == 8, InvalidProgram, /*void*/,
            "Expected %" ET_PRIsize_t "args received %" ET_PRIsize_t, (size_t)8,
            stack.size());
        EValue &input = *stack[0];
        EValue &normalized_shape = *stack[1];
        EValue &weight = *stack[2];
        EValue &bias = *stack[3];
        EValue &eps = *stack[4];
        EValue &out0 = *stack[5];
        EValue &out1 = *stack[6];
        EValue &out2 = *stack[7];
        const torch::executor::Tensor &input_base = input.to<torch::executor::Tensor>();
        auto normalized_shape_list_out = normalized_shape.toIntList();
        const torch::executor::optional<torch::executor::Tensor> &weight_base =
            weight.toOptional<torch::executor::Tensor>();
        const torch::executor::optional<torch::executor::Tensor> &bias_base =
            bias.toOptional<torch::executor::Tensor>();
        double eps_base = eps.to<double>();
        torch::executor::Tensor &out0_base = out0.to<torch::executor::Tensor>();
        torch::executor::Tensor &out1_base = out1.to<torch::executor::Tensor>();
        torch::executor::Tensor &out2_base = out2.to<torch::executor::Tensor>();

        internal::EventTracerProfileOpScope event_tracer_op_scope(
            context.internal_event_tracer(), "native_call_native_layer_norm.out");
        EXECUTORCH_SCOPE_PROF("native_call_native_layer_norm.out");
        torch::executor::native::native_layer_norm_out(
            context, input_base, normalized_shape_list_out, weight_base, bias_base,
            eps_base, out0_base, out1_base, out2_base);

        internal::event_tracer_log_evalue(context.internal_event_tracer(), *stack[5]);
    }),

Kernel(
    "aten::_native_batch_norm_legit.out",
    [](torch::executor::KernelRuntimeContext & context, Span<EValue *> stack) {
        ET_KERNEL_CHECK_MSG(
            context, stack.size() == 12, InvalidProgram, /*void*/,
            "Expected %" ET_PRIsize_t "args received %" ET_PRIsize_t, (size_t)12,
            stack.size());
        EValue &input = *stack[0];
        EValue &weight = *stack[1];
        EValue &bias = *stack[2];
        EValue &running_mean = *stack[3];
        EValue &running_var = *stack[4];
        EValue &training = *stack[5];
        EValue &momentum = *stack[6];
        EValue &eps = *stack[7];
        EValue &out = *stack[8];
        EValue &save_mean = *stack[9];
        EValue &save_invstd = *stack[10];
        const torch::executor::Tensor &input_base = input.to<torch::executor::Tensor>();
        const torch::executor::optional<torch::executor::Tensor> &weight_base =
            weight.toOptional<torch::executor::Tensor>();
        const torch::executor::optional<torch::executor::Tensor> &bias_base =
            bias.toOptional<torch::executor::Tensor>();
        torch::executor::Tensor &running_mean_base =
            running_mean.to<torch::executor::Tensor>();
        torch::executor::Tensor &running_var_base =
            running_var.to<torch::executor::Tensor>();
        bool training_base = training.to<bool>();
        double momentum_base = momentum.to<double>();
        double eps_base = eps.to<double>();
        torch::executor::Tensor &out_base = out.to<torch::executor::Tensor>();
        torch::executor::Tensor &save_mean_base = save_mean.to<torch::executor::Tensor>();
        torch::executor::Tensor &save_invstd_base =
            save_invstd.to<torch::executor::Tensor>();

        internal::EventTracerProfileOpScope event_tracer_op_scope(
            context.internal_event_tracer(), "native_call__native_batch_norm_legit.out");
        EXECUTORCH_SCOPE_PROF("native_call__native_batch_norm_legit.out");
        torch::executor::native::_native_batch_norm_legit_out(
            context, input_base, weight_base, bias_base, running_mean_base,
            running_var_base, training_base, momentum_base, eps_base, out_base,
            save_mean_base, save_invstd_base);

        internal::event_tracer_log_evalue(context.internal_event_tracer(), *stack[8]);
    }),

// ===========================================================================
// Shape ops: view_copy, squeeze_copy_dim, unsqueeze_copy, transpose_copy_int,
// cat, stack, split_copy_Tensor, slice_copy_Tensor, select_copy_int,
// expand_copy, clone, to_copy
// ===========================================================================

Kernel(
    "aten::view_copy.out",
    [](torch::executor::KernelRuntimeContext & context, Span<EValue *> stack) {
        ET_KERNEL_CHECK_MSG(
            context, stack.size() == 4, InvalidProgram, /*void*/,
            "Expected %" ET_PRIsize_t "args received %" ET_PRIsize_t, (size_t)4,
            stack.size());
        EValue &self = *stack[0];
        EValue &size = *stack[1];
        EValue &out = *stack[2];
        const torch::executor::Tensor &self_base = self.to<torch::executor::Tensor>();
        auto size_list_out = size.toIntList();
        torch::executor::Tensor &out_base = out.to<torch::executor::Tensor>();

        internal::EventTracerProfileOpScope event_tracer_op_scope(
            context.internal_event_tracer(), "native_call_view_copy.out");
        EXECUTORCH_SCOPE_PROF("native_call_view_copy.out");
        torch::executor::native::view_copy_out(
            context, self_base, size_list_out, out_base);

        internal::event_tracer_log_evalue(context.internal_event_tracer(), *stack[2]);
    }),

Kernel(
    "aten::squeeze_copy.dim.out",
    [](torch::executor::KernelRuntimeContext & context, Span<EValue *> stack) {
        ET_KERNEL_CHECK_MSG(
            context, stack.size() == 4, InvalidProgram, /*void*/,
            "Expected %" ET_PRIsize_t "args received %" ET_PRIsize_t, (size_t)4,
            stack.size());
        EValue &self = *stack[0];
        EValue &dim = *stack[1];
        EValue &out = *stack[2];
        const torch::executor::Tensor &self_base = self.to<torch::executor::Tensor>();
        int64_t dim_base = dim.to<int64_t>();
        torch::executor::Tensor &out_base = out.to<torch::executor::Tensor>();

        internal::EventTracerProfileOpScope event_tracer_op_scope(
            context.internal_event_tracer(), "native_call_squeeze_copy.dim.out");
        EXECUTORCH_SCOPE_PROF("native_call_squeeze_copy.dim.out");
        torch::executor::native::squeeze_copy_dim_out(
            context, self_base, dim_base, out_base);

        internal::event_tracer_log_evalue(context.internal_event_tracer(), *stack[2]);
    }),

Kernel(
    "aten::unsqueeze_copy.out",
    [](torch::executor::KernelRuntimeContext & context, Span<EValue *> stack) {
        ET_KERNEL_CHECK_MSG(
            context, stack.size() == 4, InvalidProgram, /*void*/,
            "Expected %" ET_PRIsize_t "args received %" ET_PRIsize_t, (size_t)4,
            stack.size());
        EValue &self = *stack[0];
        EValue &dim = *stack[1];
        EValue &out = *stack[2];
        const torch::executor::Tensor &self_base = self.to<torch::executor::Tensor>();
        int64_t dim_base = dim.to<int64_t>();
        torch::executor::Tensor &out_base = out.to<torch::executor::Tensor>();

        internal::EventTracerProfileOpScope event_tracer_op_scope(
            context.internal_event_tracer(), "native_call_unsqueeze_copy.out");
        EXECUTORCH_SCOPE_PROF("native_call_unsqueeze_copy.out");
        torch::executor::native::unsqueeze_copy_out(
            context, self_base, dim_base, out_base);

        internal::event_tracer_log_evalue(context.internal_event_tracer(), *stack[2]);
    }),

Kernel(
    "aten::transpose_copy.int.out",
    [](torch::executor::KernelRuntimeContext & context, Span<EValue *> stack) {
        ET_KERNEL_CHECK_MSG(
            context, stack.size() == 5, InvalidProgram, /*void*/,
            "Expected %" ET_PRIsize_t "args received %" ET_PRIsize_t, (size_t)5,
            stack.size());
        EValue &self = *stack[0];
        EValue &dim0 = *stack[1];
        EValue &dim1 = *stack[2];
        EValue &out = *stack[3];
        const torch::executor::Tensor &self_base = self.to<torch::executor::Tensor>();
        int64_t dim0_base = dim0.to<int64_t>();
        int64_t dim1_base = dim1.to<int64_t>();
        torch::executor::Tensor &out_base = out.to<torch::executor::Tensor>();

        internal::EventTracerProfileOpScope event_tracer_op_scope(
            context.internal_event_tracer(), "native_call_transpose_copy.int.out");
        EXECUTORCH_SCOPE_PROF("native_call_transpose_copy.int.out");
        torch::executor::native::transpose_copy_int_out(
            context, self_base, dim0_base, dim1_base, out_base);

        internal::event_tracer_log_evalue(context.internal_event_tracer(), *stack[3]);
    }),

Kernel(
    "aten::cat.out",
    [](torch::executor::KernelRuntimeContext & context, Span<EValue *> stack) {
        ET_KERNEL_CHECK_MSG(
            context, stack.size() == 4, InvalidProgram, /*void*/,
            "Expected %" ET_PRIsize_t "args received %" ET_PRIsize_t, (size_t)4,
            stack.size());
        EValue &tensors = *stack[0];
        EValue &dim = *stack[1];
        EValue &out = *stack[2];
        torch::executor::TensorList tensors_base = tensors.toTensorList();
        int64_t dim_base = dim.to<int64_t>();
        torch::executor::Tensor &out_base = out.to<torch::executor::Tensor>();

        internal::EventTracerProfileOpScope event_tracer_op_scope(
            context.internal_event_tracer(), "native_call_cat.out");
        EXECUTORCH_SCOPE_PROF("native_call_cat.out");
        torch::executor::native::cat_out(context, tensors_base, dim_base, out_base);

        internal::event_tracer_log_evalue(context.internal_event_tracer(), *stack[2]);
    }),

Kernel(
    "aten::stack.out",
    [](torch::executor::KernelRuntimeContext & context, Span<EValue *> stack) {
        ET_KERNEL_CHECK_MSG(
            context, stack.size() == 4, InvalidProgram, /*void*/,
            "Expected %" ET_PRIsize_t "args received %" ET_PRIsize_t, (size_t)4,
            stack.size());
        EValue &tensors = *stack[0];
        EValue &dim = *stack[1];
        EValue &out = *stack[2];
        torch::executor::TensorList tensors_base = tensors.toTensorList();
        int64_t dim_base = dim.to<int64_t>();
        torch::executor::Tensor &out_base = out.to<torch::executor::Tensor>();

        internal::EventTracerProfileOpScope event_tracer_op_scope(
            context.internal_event_tracer(), "native_call_stack.out");
        EXECUTORCH_SCOPE_PROF("native_call_stack.out");
        torch::executor::native::stack_out(context, tensors_base, dim_base, out_base);

        internal::event_tracer_log_evalue(context.internal_event_tracer(), *stack[2]);
    }),

Kernel(
    "aten::split_copy.Tensor_out",
    [](torch::executor::KernelRuntimeContext & context, Span<EValue *> stack) {
        ET_KERNEL_CHECK_MSG(
            context, stack.size() == 5, InvalidProgram, /*void*/,
            "Expected %" ET_PRIsize_t "args received %" ET_PRIsize_t, (size_t)5,
            stack.size());
        EValue &self = *stack[0];
        EValue &split_size = *stack[1];
        EValue &dim = *stack[2];
        EValue &out = *stack[3];
        const torch::executor::Tensor &self_base = self.to<torch::executor::Tensor>();
        int64_t split_size_base = split_size.to<int64_t>();
        int64_t dim_base = dim.to<int64_t>();
        torch::executor::TensorList out_base = out.toTensorList();

        internal::EventTracerProfileOpScope event_tracer_op_scope(
            context.internal_event_tracer(), "native_call_split_copy.Tensor_out");
        EXECUTORCH_SCOPE_PROF("native_call_split_copy.Tensor_out");
        torch::executor::native::split_copy_Tensor_out(
            context, self_base, split_size_base, dim_base, out_base);

        internal::event_tracer_log_evalue(context.internal_event_tracer(), *stack[3]);
    }),

Kernel(
    "aten::slice_copy.Tensor_out",
    [](torch::executor::KernelRuntimeContext & context, Span<EValue *> stack) {
        ET_KERNEL_CHECK_MSG(
            context, stack.size() == 7, InvalidProgram, /*void*/,
            "Expected %" ET_PRIsize_t "args received %" ET_PRIsize_t, (size_t)7,
            stack.size());
        EValue &self = *stack[0];
        EValue &dim = *stack[1];
        EValue &start = *stack[2];
        EValue &end = *stack[3];
        EValue &step = *stack[4];
        EValue &out = *stack[5];
        const torch::executor::Tensor &self_base = self.to<torch::executor::Tensor>();
        int64_t dim_base = dim.to<int64_t>();
        auto start_opt = start.toOptional<int64_t>();
        auto end_opt = end.toOptional<int64_t>();
        int64_t step_base = step.to<int64_t>();
        torch::executor::Tensor &out_base = out.to<torch::executor::Tensor>();

        internal::EventTracerProfileOpScope event_tracer_op_scope(
            context.internal_event_tracer(), "native_call_slice_copy.Tensor_out");
        EXECUTORCH_SCOPE_PROF("native_call_slice_copy.Tensor_out");
        torch::executor::native::slice_copy_Tensor_out(
            context, self_base, dim_base, start_opt, end_opt, step_base, out_base);

        internal::event_tracer_log_evalue(context.internal_event_tracer(), *stack[5]);
    }),

Kernel(
    "aten::select_copy.int_out",
    [](torch::executor::KernelRuntimeContext & context, Span<EValue *> stack) {
        ET_KERNEL_CHECK_MSG(
            context, stack.size() == 5, InvalidProgram, /*void*/,
            "Expected %" ET_PRIsize_t "args received %" ET_PRIsize_t, (size_t)5,
            stack.size());
        EValue &self = *stack[0];
        EValue &dim = *stack[1];
        EValue &index = *stack[2];
        EValue &out = *stack[3];
        const torch::executor::Tensor &self_base = self.to<torch::executor::Tensor>();
        int64_t dim_base = dim.to<int64_t>();
        int64_t index_base = index.to<int64_t>();
        torch::executor::Tensor &out_base = out.to<torch::executor::Tensor>();

        internal::EventTracerProfileOpScope event_tracer_op_scope(
            context.internal_event_tracer(), "native_call_select_copy.int_out");
        EXECUTORCH_SCOPE_PROF("native_call_select_copy.int_out");
        torch::executor::native::select_copy_int_out(
            context, self_base, dim_base, index_base, out_base);

        internal::event_tracer_log_evalue(context.internal_event_tracer(), *stack[3]);
    }),

Kernel(
    "aten::expand_copy.out",
    [](torch::executor::KernelRuntimeContext & context, Span<EValue *> stack) {
        ET_KERNEL_CHECK_MSG(
            context, stack.size() == 5, InvalidProgram, /*void*/,
            "Expected %" ET_PRIsize_t "args received %" ET_PRIsize_t, (size_t)5,
            stack.size());
        EValue &self = *stack[0];
        EValue &size = *stack[1];
        EValue &implicit = *stack[2];
        EValue &out = *stack[3];
        const torch::executor::Tensor &self_base = self.to<torch::executor::Tensor>();
        auto size_list_out = size.toIntList();
        bool implicit_base = implicit.to<bool>();
        torch::executor::Tensor &out_base = out.to<torch::executor::Tensor>();

        internal::EventTracerProfileOpScope event_tracer_op_scope(
            context.internal_event_tracer(), "native_call_expand_copy.out");
        EXECUTORCH_SCOPE_PROF("native_call_expand_copy.out");
        torch::executor::native::expand_copy_out(
            context, self_base, size_list_out, implicit_base, out_base);

        internal::event_tracer_log_evalue(context.internal_event_tracer(), *stack[3]);
    }),

Kernel(
    "aten::clone.out",
    [](torch::executor::KernelRuntimeContext & context, Span<EValue *> stack) {
        ET_KERNEL_CHECK_MSG(
            context, stack.size() == 4, InvalidProgram, /*void*/,
            "Expected %" ET_PRIsize_t "args received %" ET_PRIsize_t, (size_t)4,
            stack.size());
        EValue &self = *stack[0];
        EValue &memory_format = *stack[1];
        EValue &out = *stack[2];
        const torch::executor::Tensor &self_base = self.to<torch::executor::Tensor>();
        auto memory_format_opt = memory_format.toOptional<torch::executor::MemoryFormat>();
        torch::executor::Tensor &out_base = out.to<torch::executor::Tensor>();

        internal::EventTracerProfileOpScope event_tracer_op_scope(
            context.internal_event_tracer(), "native_call_clone.out");
        EXECUTORCH_SCOPE_PROF("native_call_clone.out");
        torch::executor::native::clone_out(
            context, self_base, memory_format_opt, out_base);

        internal::event_tracer_log_evalue(context.internal_event_tracer(), *stack[2]);
    }),

Kernel(
    "aten::to_copy.out",
    [](torch::executor::KernelRuntimeContext & context, Span<EValue *> stack) {
        ET_KERNEL_CHECK_MSG(
            context, stack.size() == 5, InvalidProgram, /*void*/,
            "Expected %" ET_PRIsize_t "args received %" ET_PRIsize_t, (size_t)5,
            stack.size());
        EValue &self = *stack[0];
        EValue &non_blocking = *stack[1];
        EValue &memory_format = *stack[2];
        EValue &out = *stack[3];
        const torch::executor::Tensor &self_base = self.to<torch::executor::Tensor>();
        bool non_blocking_base = non_blocking.to<bool>();
        auto memory_format_opt = memory_format.toOptional<torch::executor::MemoryFormat>();
        torch::executor::Tensor &out_base = out.to<torch::executor::Tensor>();

        internal::EventTracerProfileOpScope event_tracer_op_scope(
            context.internal_event_tracer(), "native_call_to_copy.out");
        EXECUTORCH_SCOPE_PROF("native_call_to_copy.out");
        torch::executor::native::to_copy_out(
            context, self_base, non_blocking_base, memory_format_opt, out_base);

        internal::event_tracer_log_evalue(context.internal_event_tracer(), *stack[3]);
    }),

// ===========================================================================
// Reduction: mean.dim, sum.dim, amax, amin, argmax
// ===========================================================================

Kernel(
    // PyTorch structured op: mean.out carries the dim/keepdim args of
    // mean.dim (native_functions.yaml maps it to mean_dim_out). MobileNetV2's
    // global-average-pooling lowering emits exactly this op.
    "aten::mean.out",
    [](torch::executor::KernelRuntimeContext & context, Span<EValue *> stack) {
        ET_KERNEL_CHECK_MSG(
            context, stack.size() == 6, InvalidProgram, /*void*/,
            "Expected %" ET_PRIsize_t "args received %" ET_PRIsize_t, (size_t)6,
            stack.size());
        EValue &self = *stack[0];
        EValue &dim = *stack[1];
        EValue &keepdim = *stack[2];
        EValue &dtype = *stack[3];
        EValue &out = *stack[4];
        const torch::executor::Tensor &self_base = self.to<torch::executor::Tensor>();
        auto dim_list_opt = dim.toOptional<torch::executor::ArrayRef<int64_t>>();
        bool keepdim_base = keepdim.to<bool>();
        auto dtype_opt = dtype.toOptional<torch::executor::ScalarType>();
        torch::executor::Tensor &out_base = out.to<torch::executor::Tensor>();

        internal::EventTracerProfileOpScope event_tracer_op_scope(
            context.internal_event_tracer(), "native_call_mean.dim.out");
        EXECUTORCH_SCOPE_PROF("native_call_mean.dim.out");
        torch::executor::native::mean_dim_out(
            context, self_base, dim_list_opt, keepdim_base, dtype_opt, out_base);

        internal::event_tracer_log_evalue(context.internal_event_tracer(), *stack[4]);
    }),

Kernel(
    "aten::sum.dim_IntList_out",
    [](torch::executor::KernelRuntimeContext & context, Span<EValue *> stack) {
        ET_KERNEL_CHECK_MSG(
            context, stack.size() == 6, InvalidProgram, /*void*/,
            "Expected %" ET_PRIsize_t "args received %" ET_PRIsize_t, (size_t)6,
            stack.size());
        EValue &self = *stack[0];
        EValue &dim = *stack[1];
        EValue &keepdim = *stack[2];
        EValue &dtype = *stack[3];
        EValue &out = *stack[4];
        const torch::executor::Tensor &self_base = self.to<torch::executor::Tensor>();
        auto dim_list_opt = dim.toOptional<torch::executor::ArrayRef<int64_t>>();
        bool keepdim_base = keepdim.to<bool>();
        auto dtype_opt = dtype.toOptional<torch::executor::ScalarType>();
        torch::executor::Tensor &out_base = out.to<torch::executor::Tensor>();

        internal::EventTracerProfileOpScope event_tracer_op_scope(
            context.internal_event_tracer(), "native_call_sum.dim_IntList_out");
        EXECUTORCH_SCOPE_PROF("native_call_sum.dim_IntList_out");
        torch::executor::native::sum_dim_out(
            context, self_base, dim_list_opt, keepdim_base, dtype_opt, out_base);

        internal::event_tracer_log_evalue(context.internal_event_tracer(), *stack[4]);
    }),

Kernel(
    "aten::amax.out",
    [](torch::executor::KernelRuntimeContext & context, Span<EValue *> stack) {
        ET_KERNEL_CHECK_MSG(
            context, stack.size() == 5, InvalidProgram, /*void*/,
            "Expected %" ET_PRIsize_t "args received %" ET_PRIsize_t, (size_t)5,
            stack.size());
        EValue &self = *stack[0];
        EValue &dim = *stack[1];
        EValue &keepdim = *stack[2];
        EValue &out = *stack[3];
        const torch::executor::Tensor &self_base = self.to<torch::executor::Tensor>();
        auto dim_list_out = dim.toIntList();
        bool keepdim_base = keepdim.to<bool>();
        torch::executor::Tensor &out_base = out.to<torch::executor::Tensor>();

        internal::EventTracerProfileOpScope event_tracer_op_scope(
            context.internal_event_tracer(), "native_call_amax.out");
        EXECUTORCH_SCOPE_PROF("native_call_amax.out");
        torch::executor::native::amax_out(
            context, self_base, dim_list_out, keepdim_base, out_base);

        internal::event_tracer_log_evalue(context.internal_event_tracer(), *stack[3]);
    }),

Kernel(
    "aten::amin.out",
    [](torch::executor::KernelRuntimeContext & context, Span<EValue *> stack) {
        ET_KERNEL_CHECK_MSG(
            context, stack.size() == 5, InvalidProgram, /*void*/,
            "Expected %" ET_PRIsize_t "args received %" ET_PRIsize_t, (size_t)5,
            stack.size());
        EValue &self = *stack[0];
        EValue &dim = *stack[1];
        EValue &keepdim = *stack[2];
        EValue &out = *stack[3];
        const torch::executor::Tensor &self_base = self.to<torch::executor::Tensor>();
        auto dim_list_out = dim.toIntList();
        bool keepdim_base = keepdim.to<bool>();
        torch::executor::Tensor &out_base = out.to<torch::executor::Tensor>();

        internal::EventTracerProfileOpScope event_tracer_op_scope(
            context.internal_event_tracer(), "native_call_amin.out");
        EXECUTORCH_SCOPE_PROF("native_call_amin.out");
        torch::executor::native::amin_out(
            context, self_base, dim_list_out, keepdim_base, out_base);

        internal::event_tracer_log_evalue(context.internal_event_tracer(), *stack[3]);
    }),

Kernel(
    "aten::argmax.out",
    [](torch::executor::KernelRuntimeContext & context, Span<EValue *> stack) {
        ET_KERNEL_CHECK_MSG(
            context, stack.size() == 5, InvalidProgram, /*void*/,
            "Expected %" ET_PRIsize_t "args received %" ET_PRIsize_t, (size_t)5,
            stack.size());
        EValue &self = *stack[0];
        EValue &dim = *stack[1];
        EValue &keepdim = *stack[2];
        EValue &out = *stack[3];
        const torch::executor::Tensor &self_base = self.to<torch::executor::Tensor>();
        auto dim_opt = dim.toOptional<int64_t>();
        bool keepdim_base = keepdim.to<bool>();
        torch::executor::Tensor &out_base = out.to<torch::executor::Tensor>();

        internal::EventTracerProfileOpScope event_tracer_op_scope(
            context.internal_event_tracer(), "native_call_argmax.out");
        EXECUTORCH_SCOPE_PROF("native_call_argmax.out");
        torch::executor::native::argmax_out(
            context, self_base, dim_opt, keepdim_base, out_base);

        internal::event_tracer_log_evalue(context.internal_event_tracer(), *stack[3]);
    }),

// ===========================================================================
// Other: clamp, embedding, arange.start, scalar_tensor
// ===========================================================================

Kernel(
    "aten::clamp.out",
    [](torch::executor::KernelRuntimeContext & context, Span<EValue *> stack) {
        ET_KERNEL_CHECK_MSG(
            context, stack.size() == 5, InvalidProgram, /*void*/,
            "Expected %" ET_PRIsize_t "args received %" ET_PRIsize_t, (size_t)5,
            stack.size());
        EValue &self = *stack[0];
        EValue &min = *stack[1];
        EValue &max = *stack[2];
        EValue &out = *stack[3];
        const torch::executor::Tensor &self_base = self.to<torch::executor::Tensor>();
        auto min_opt = min.toOptional<torch::executor::Scalar>();
        auto max_opt = max.toOptional<torch::executor::Scalar>();
        torch::executor::Tensor &out_base = out.to<torch::executor::Tensor>();

        internal::EventTracerProfileOpScope event_tracer_op_scope(
            context.internal_event_tracer(), "native_call_clamp.out");
        EXECUTORCH_SCOPE_PROF("native_call_clamp.out");
        torch::executor::native::clamp_out(
            context, self_base, min_opt, max_opt, out_base);

        internal::event_tracer_log_evalue(context.internal_event_tracer(), *stack[3]);
    }),

Kernel(
    "aten::embedding.out",
    [](torch::executor::KernelRuntimeContext & context, Span<EValue *> stack) {
        ET_KERNEL_CHECK_MSG(
            context, stack.size() == 7, InvalidProgram, /*void*/,
            "Expected %" ET_PRIsize_t "args received %" ET_PRIsize_t, (size_t)7,
            stack.size());
        EValue &weight = *stack[0];
        EValue &indices = *stack[1];
        EValue &padding_idx = *stack[2];
        EValue &scale_grad_by_freq = *stack[3];
        EValue &sparse = *stack[4];
        EValue &out = *stack[5];
        const torch::executor::Tensor &weight_base = weight.to<torch::executor::Tensor>();
        const torch::executor::Tensor &indices_base = indices.to<torch::executor::Tensor>();
        int64_t padding_idx_base = padding_idx.to<int64_t>();
        bool scale_grad_by_freq_base = scale_grad_by_freq.to<bool>();
        bool sparse_base = sparse.to<bool>();
        torch::executor::Tensor &out_base = out.to<torch::executor::Tensor>();

        internal::EventTracerProfileOpScope event_tracer_op_scope(
            context.internal_event_tracer(), "native_call_embedding.out");
        EXECUTORCH_SCOPE_PROF("native_call_embedding.out");
        torch::executor::native::embedding_out(
            context, weight_base, indices_base, padding_idx_base,
            scale_grad_by_freq_base, sparse_base, out_base);

        internal::event_tracer_log_evalue(context.internal_event_tracer(), *stack[5]);
    }),

Kernel(
    "aten::arange.start_out",
    [](torch::executor::KernelRuntimeContext & context, Span<EValue *> stack) {
        ET_KERNEL_CHECK_MSG(
            context, stack.size() == 5, InvalidProgram, /*void*/,
            "Expected %" ET_PRIsize_t "args received %" ET_PRIsize_t, (size_t)5,
            stack.size());
        EValue &start = *stack[0];
        EValue &end = *stack[1];
        EValue &step = *stack[2];
        EValue &out = *stack[3];
        const torch::executor::Scalar &start_base = start.to<torch::executor::Scalar>();
        const torch::executor::Scalar &end_base = end.to<torch::executor::Scalar>();
        const torch::executor::Scalar &step_base = step.to<torch::executor::Scalar>();
        torch::executor::Tensor &out_base = out.to<torch::executor::Tensor>();

        internal::EventTracerProfileOpScope event_tracer_op_scope(
            context.internal_event_tracer(), "native_call_arange.start_out");
        EXECUTORCH_SCOPE_PROF("native_call_arange.start_out");
        torch::executor::native::arange_start_out(
            context, start_base, end_base, step_base, out_base);

        internal::event_tracer_log_evalue(context.internal_event_tracer(), *stack[3]);
    }),

Kernel(
    "aten::scalar_tensor.out",
    [](torch::executor::KernelRuntimeContext & context, Span<EValue *> stack) {
        ET_KERNEL_CHECK_MSG(
            context, stack.size() == 3, InvalidProgram, /*void*/,
            "Expected %" ET_PRIsize_t "args received %" ET_PRIsize_t, (size_t)3,
            stack.size());
        EValue &s = *stack[0];
        EValue &out = *stack[1];
        const torch::executor::Scalar &s_base = s.to<torch::executor::Scalar>();
        torch::executor::Tensor &out_base = out.to<torch::executor::Tensor>();

        internal::EventTracerProfileOpScope event_tracer_op_scope(
            context.internal_event_tracer(), "native_call_scalar_tensor.out");
        EXECUTORCH_SCOPE_PROF("native_call_scalar_tensor.out");
        torch::executor::native::scalar_tensor_out(context, s_base, out_base);

        internal::event_tracer_log_evalue(context.internal_event_tracer(), *stack[1]);
    }),

}; // end kernels_to_register[]

// Explicitly convert to Span, so that the API can take an empty C array of
// Kernels.
static KernelSpan kernel_span(
    kernels_to_register,
    kernels_to_register + sizeof(kernels_to_register) / sizeof(Kernel));

// Return value not used. Keep the static variable assignment to register
// kernels in static initialization time.
static auto success_with_kernel_reg = register_kernels(kernel_span);

} // namespace
} // namespace function
} // namespace executor
} // namespace torch

// et_module_init_system is the example entry hook; kernel registration above is
// performed at static-init, so it is already complete by the time main runs.
extern "C" void et_module_init_system() {}
