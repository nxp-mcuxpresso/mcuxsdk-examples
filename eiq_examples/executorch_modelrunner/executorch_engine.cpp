/*
 * Copyright 2024,2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// ExecuTorch engine wrapper for the modelrunner example: a persistent Engine
// object (Program/Method live across LoadModel/Run) holding a runtime-uploaded
// model, with automatic dtype handling from TensorInfo.scalar_type() and
// binary serialization of the tensor spec (GetInfo) and results (Run).

#include "executorch_engine.h"

#include "fsl_common.h"
#include <cstring>
#include <memory>
#include <vector>

#include <executorch/backends/nxp/runtime/NeutronDriver.h>
#include <executorch/extension/data_loader/buffer_data_loader.h>
#include <executorch/runtime/executor/method.h>
#include <executorch/runtime/executor/method_meta.h>
#include <executorch/runtime/executor/program.h>
#include <executorch/runtime/core/hierarchical_allocator.h>
#include <executorch/runtime/core/memory_allocator.h>
#include <executorch/runtime/executor/memory_manager.h>
#include <executorch/runtime/platform/runtime.h>

#include "timer.h"

// SystemCoreClock is defined by the CMSIS system startup. Declared extern here
// rather than including board.h, which pulls in conflicting clock-config
// declarations alongside it.
extern uint32_t SystemCoreClock;

using executorch::aten::ScalarType;
using executorch::aten::Tensor;
using executorch::aten::TensorImpl;
using executorch::extension::BufferDataLoader;
using executorch::runtime::Error;
using executorch::runtime::EValue;
using executorch::runtime::HierarchicalAllocator;
using executorch::runtime::MemoryAllocator;
using executorch::runtime::MemoryManager;
using executorch::runtime::Method;
using executorch::runtime::MethodMeta;
using executorch::runtime::Program;
using executorch::runtime::Result;
using executorch::runtime::Span;
using executorch::runtime::TensorInfo;

// ---------------------------------------------------------------------------
// Work pools and RAM model buffer, all in the NonCacheable region so the
// NPU's DMA view of any buffer is coherent without cache maintenance.
// Total .ncache footprint: 1216K + 96K + 384K + 256K = 1952K of the 2M region.
// Sizing (adjust when supporting larger models):
//   method_pool 1216K: planned buffers (largest supported model needs ~1133K
//                    + method metadata.
//   temp_pool 96K: kernel scratch. Keep >= 96K: smaller pools silently
//                 corrupt Neutron NPU output.
//   pte_buffer 384K: uploaded .pte bytes (largest supported model ~365K).
//   input_buffer 256K: input tensor bytes (largest supported input ~251K).
// ---------------------------------------------------------------------------
constexpr size_t kPoolSize = 1216 * 1024;
static uint8_t method_pool[kPoolSize] __attribute__((aligned(16))) __attribute__((section("NonCacheable")));
constexpr size_t kTempPoolSize = 96 * 1024;
static uint8_t temp_pool[kTempPoolSize] __attribute__((aligned(16))) __attribute__((section("NonCacheable")));

// .pte bytes uploaded at runtime live here. NonCacheable so the NPU's DMA view
// of the model weights is coherent without cache maintenance: with the NS3.2.1
// delegate, a cacheable pte buffer (even with explicit DCCMVAC writeback)
// produced input-insensitive garbage outputs.
constexpr size_t kPteBufSize = 384 * 1024;
static uint8_t pte_buffer[kPteBufSize] __attribute__((aligned(32)))
    __attribute__((section("NonCacheable")));

// Persistent input storage. NonCacheable so the NPU's DMA view of the bytes is
// coherent without any cache maintenance, and persistent because the input
// tensor handed to set_input() must stay alive until execute() (see
// EngineState::input_impl below).
constexpr size_t kInputBufSize = 256 * 1024;
static uint8_t input_buffer[kInputBufSize] __attribute__((aligned(32)))
    __attribute__((section("NonCacheable")));

// ---------------------------------------------------------------------------
// Per-load engine state. The whole bundle is heap-allocated via unique_ptr
// (see Engine::state_) and rebuilt fresh on every LoadModel, so the
// bump-allocator offsets reset to 0 on model swaps (the stock MemoryAllocator
// has no reset()). Program/Method are move-only (no move-assign), so they are
// held via unique_ptr inside this struct.
//
// Member declaration order is significant for destruction: method (declared
// before the allocators/planned buffers) is destroyed first, before the
// allocators and planned buffers it references. ~EngineState runs in reverse
// declaration order: temp_allocator, method_allocator, planned_spans,
// planned_buffers, method_name, method, program - correct.
// ---------------------------------------------------------------------------
struct Engine::EngineState {
    std::unique_ptr<Program> program;
    std::unique_ptr<Method> method;
    std::string method_name;

    // Input tensor plumbing. The ExecuTorch runtime keeps a reference to the
    // tensor passed to set_input() and the NPU reads the input bytes via DMA
    // during execute(), so the impl, its sizes/dim_order arrays and the data
    // storage (input_buffer above) must all stay alive from LoadInput() until
    // execute(). input_impl references input_sizes/input_dim_order, so it must
    // be destroyed before them (reverse declaration order).
    std::vector<Tensor::SizesType> input_sizes;
    std::vector<Tensor::DimOrderType> input_dim_order;
    std::unique_ptr<TensorImpl> input_impl;

    // Owned planned buffers + spans handed to HierarchicalAllocator. Kept alive
    // for the lifetime of method.
    std::vector<uint8_t *> planned_buffers;
    std::vector<Span<uint8_t>> planned_spans;

    // Allocators back the MemoryManager; must outlive method. Reconstructed over
    // the fixed pools (offset 0) on every LoadModel.
    MemoryAllocator method_allocator;
    MemoryAllocator temp_allocator;

    EngineState()
        : method_allocator(sizeof(method_pool), method_pool),
          temp_allocator(sizeof(temp_pool), temp_pool) {}
};

namespace {

// Small unchecked writer for little-endian serialization into a flat buffer.
class Writer {
 public:
    Writer(uint8_t *out, size_t cap) : out_(out), cap_(cap), n_(0) {}

    bool put_u8(uint8_t v) {
        if (n_ + 1 > cap_) return false;
        out_[n_++] = v;
        return true;
    }
    bool put_u32(uint32_t v) {
        if (n_ + 4 > cap_) return false;
        out_[n_++] = v & 0xFF;
        out_[n_++] = (v >> 8) & 0xFF;
        out_[n_++] = (v >> 16) & 0xFF;
        out_[n_++] = (v >> 24) & 0xFF;
        return true;
    }
    bool put_u64(uint64_t v) {
        for (int i = 0; i < 8; ++i) {
            if (n_ + 1 > cap_) return false;
            out_[n_++] = (v >> (8 * i)) & 0xFF;
        }
        return true;
    }
    bool put_i32(int32_t v) { return put_u32(static_cast<uint32_t>(v)); }
    bool put_bytes(const uint8_t *src, size_t len) {
        if (n_ + len > cap_) return false;
        std::memcpy(out_ + n_, src, len);
        n_ += len;
        return true;
    }

    size_t written() const { return n_; }

 private:
    uint8_t *out_;
    size_t cap_;
    size_t n_;
};

// Serialize one tensor's spec into the writer (INFO entry layout):
// dtype:u8  ndim:u8  sizes:ndim x i32LE  dim_order:ndim x u8  nbytes:u32LE
bool WriteTensorSpec(Writer &w, const TensorInfo &info) {
    auto sizes = info.sizes();
    auto order = info.dim_order();
    if (!w.put_u8(static_cast<uint8_t>(info.scalar_type()))) return false;
    if (!w.put_u8(static_cast<uint8_t>(sizes.size()))) return false;
    for (size_t i = 0; i < sizes.size(); ++i) {
        if (!w.put_i32(sizes[i])) return false;
    }
    for (size_t i = 0; i < order.size(); ++i) {
        if (!w.put_u8(order[i])) return false;
    }
    if (!w.put_u32(static_cast<uint32_t>(info.nbytes()))) return false;
    return true;
}

}  // namespace

// ---------------------------------------------------------------------------
// Engine public API
// ---------------------------------------------------------------------------

Engine::Engine() = default;
Engine::~Engine() = default;
Engine::Engine(Engine &&) noexcept = default;
Engine &Engine::operator=(Engine &&) noexcept = default;

void Engine::Reset() {
    // Drop the whole bundle: ~EngineState destroys method, then program, then
    // planned buffers, then allocators (reverse declaration order).
    state_.reset();
    needs_neutron_ = false;
}

uint8_t Engine::LoadModel(const uint8_t *pte_bytes, size_t len) {
    if (len > kPteBufSize) {
        return kErrModelTooBig;
    }

    // Destroy any existing state first. This drops the old Method (which
    // references the old Program/allocators), then the old Program, then the
    // old MemoryManager/allocators, and crucially frees the per-load state so
    // the new EngineState starts with allocator offsets at 0.
    state_.reset();
    needs_neutron_ = false;

    // Zero the work pools: they live in the NOLOAD non-cacheable section,
    // which the startup code does not clear. Planned buffers and NPU scratch
    // must not start with random contents.
    std::memset(method_pool, 0, sizeof(method_pool));
    std::memset(temp_pool, 0, sizeof(temp_pool));

    std::memcpy(pte_buffer, pte_bytes, len);
    // pte_buffer is NonCacheable: the NPU's DMA view of the model weights is
    // coherent without any cache maintenance.

    BufferDataLoader loader(pte_buffer, len);
    Result<Program> program = Program::load(&loader);
    if (!program.ok()) {
        return kErrDbgProgramLoad;
    }

    // Build a brand-new EngineState: fresh allocators (offset 0), fresh planned
    // buffers, fresh MemoryManager.
    auto st = std::make_unique<EngineState>();
    // Move-construct into owning unique_ptr (Program has no move-assign).
    st->program = std::unique_ptr<Program>(new Program(std::move(program.get())));

    const char *method_name = nullptr;
    {
        auto name_res = st->program->get_method_name(0);
        if (!name_res.ok()) {
            return kErrDbgMethodName;
        }
        method_name = *name_res;
        st->method_name = method_name;
    }

    Result<MethodMeta> meta = st->program->method_meta(method_name);
    if (!meta.ok()) {
        return kErrDbgMethodMeta;
    }
    needs_neutron_ = meta->uses_backend("NeutronBackend");

    // Memory planning: one planned buffer per memory-planned buffer id.
    size_t num_planned = meta->num_memory_planned_buffers();
    st->planned_buffers.reserve(num_planned);
    st->planned_spans.reserve(num_planned);
    for (size_t id = 0; id < num_planned; ++id) {
        auto sz_res = meta->memory_planned_buffer_size(id);
        if (!sz_res.ok()) {
            return kErrDbgPlannedBuf;
        }
        size_t buffer_size = static_cast<size_t>(sz_res.get());
        uint8_t *buffer =
            reinterpret_cast<uint8_t *>(st->method_allocator.allocate(buffer_size));
        if (buffer == nullptr) {
            return kErrDbgPlannedBuf;
        }
        st->planned_buffers.push_back(buffer);
        st->planned_spans.push_back({st->planned_buffers.back(), buffer_size});
    }

    HierarchicalAllocator planned_memory(
        {st->planned_spans.data(), st->planned_spans.size()});
    MemoryManager memory_manager(&st->method_allocator, &planned_memory,
                                 &st->temp_allocator);

    Result<Method> method =
        st->program->load_method(method_name, &memory_manager);
    if (!method.ok()) {
        return kErrDbgLoadMethod;
    }
    st->method = std::unique_ptr<Method>(new Method(std::move(method.get())));

    // Commit the freshly-built state. No further failures possible after this.
    state_ = std::move(st);
    return kErrOk;
}

size_t Engine::GetInfo(uint8_t *out, size_t out_max) {
    if (!state_) {
        return 0;
    }
    EngineState &st = *state_;

    Result<MethodMeta> meta = st.program->method_meta(st.method_name.c_str());
    if (!meta.ok()) {
        return 0;
    }

    Writer w(out, out_max);
    if (!w.put_u8(static_cast<uint8_t>(meta->num_inputs()))) return 0;
    for (size_t i = 0; i < meta->num_inputs(); ++i) {
        auto ti = meta->input_tensor_meta(i);
        if (!ti.ok() || !WriteTensorSpec(w, ti.get())) return 0;
    }
    if (!w.put_u8(static_cast<uint8_t>(meta->num_outputs()))) return 0;
    for (size_t i = 0; i < meta->num_outputs(); ++i) {
        auto ti = meta->output_tensor_meta(i);
        if (!ti.ok() || !WriteTensorSpec(w, ti.get())) return 0;
    }
    return w.written();
}

uint8_t Engine::LoadInput(const uint8_t *bin_bytes, size_t len) {
    if (!state_) {
        return kErrNoModel;
    }
    EngineState &st = *state_;

    Result<MethodMeta> meta = st.program->method_meta(st.method_name.c_str());
    if (!meta.ok()) {
        return kErrNoModel;
    }

    auto in_res = meta->input_tensor_meta(0);
    if (!in_res.ok()) {
        return kErrNoModel;
    }
    TensorInfo in = in_res.get();
    if (len != in.nbytes()) {
        return kErrInputSize;
    }

    // Copy the uploaded bytes into the persistent NonCacheable input buffer
    // and build the input tensor from EngineState-owned storage: the runtime
    // keeps a reference to the tensor passed to set_input(), so the impl and
    // its storage must stay alive until execute().
    if (len > kInputBufSize) {
        return kErrInputSize;
    }
    memcpy(input_buffer, bin_bytes, len);
    auto sizes_span = in.sizes();
    auto order_span = in.dim_order();
    st.input_sizes.assign(sizes_span.begin(), sizes_span.end());
    st.input_dim_order.assign(order_span.begin(), order_span.end());
    st.input_impl = std::make_unique<TensorImpl>(
        in.scalar_type(), st.input_sizes.size(), st.input_sizes.data(),
        input_buffer, st.input_dim_order.data());
    Tensor tensor(st.input_impl.get());

    if (st.method->set_input(tensor, 0) != Error::Ok) {
        return kErrInputSize;
    }
    return kErrOk;
}

size_t Engine::Run(uint8_t *out, size_t out_max, uint8_t *err) {
    if (!state_) {
        *err = kErrNoModel;
        return 0;
    }
    EngineState &st = *state_;

    // Time execute with the DWT cycle counter: it is a free-running CPU cycle
    // counter unaffected by interrupt masking or SysTick wrap-around, so it
    // stays exact for sub-millisecond executes.
    uint32_t cycles_per_us = SystemCoreClock / 1000000u;
    uint32_t cyc0 = DWT->CYCCNT;
    Error status = st.method->execute();
    uint32_t cyc1 = DWT->CYCCNT;
    uint64_t inference_us = (uint64_t)(cyc1 - cyc0) / cycles_per_us;

    if (status != Error::Ok) {
        *err = kErrExecute;
        return 0;
    }

    // Gather outputs and serialize the RESULT payload:
    // timing_us:u64 LE  n_outputs:u8
    // per output: dtype:u8  ndim:u8  sizes:ndim x i32LE  nbytes:u32LE  data:nbytes
    std::vector<EValue> outputs(st.method->outputs_size());
    if (st.method->get_outputs(outputs.data(), outputs.size()) != Error::Ok) {
        *err = kErrExecute;
        return 0;
    }

    Writer w(out, out_max);
    if (!w.put_u64(inference_us)) {
        *err = kErrExecute;
        return 0;
    }
    if (!w.put_u8(static_cast<uint8_t>(outputs.size()))) {
        *err = kErrExecute;
        return 0;
    }
    for (size_t i = 0; i < outputs.size(); ++i) {
        Tensor t = outputs[i].toTensor();
        auto sizes = t.sizes();
        if (!w.put_u8(static_cast<uint8_t>(t.scalar_type()))) {
            *err = kErrExecute;
            return 0;
        }
        if (!w.put_u8(static_cast<uint8_t>(sizes.size()))) {
            *err = kErrExecute;
            return 0;
        }
        for (size_t d = 0; d < sizes.size(); ++d) {
            if (!w.put_i32(static_cast<int32_t>(sizes[d]))) {
                *err = kErrExecute;
                return 0;
            }
        }
        if (!w.put_u32(static_cast<uint32_t>(t.nbytes()))) {
            *err = kErrExecute;
            return 0;
        }
        if (!w.put_bytes(static_cast<const uint8_t *>(t.const_data_ptr()),
                         t.nbytes())) {
            *err = kErrExecute;
            return 0;
        }
    }

    *err = kErrOk;
    return w.written();
}
