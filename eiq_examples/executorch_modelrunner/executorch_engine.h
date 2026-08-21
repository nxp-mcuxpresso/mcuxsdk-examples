#ifndef EXECUTORCH_ENGINE_H_
#define EXECUTORCH_ENGINE_H_

#include <cstddef>
#include <cstdint>
#include <memory>

#include "protocol.h"

// Opaque engine state (Program, Method, allocators) - defined in .cpp.
class Engine {
 public:
    // Special members defined out-of-line in the .cpp, where EngineState is a
    // complete type (this header only forward-declares it).
    Engine();
    ~Engine();
    Engine(Engine &&) noexcept;
    Engine &operator=(Engine &&) noexcept;
    Engine(const Engine &) = delete;
    Engine &operator=(const Engine &) = delete;

    // Load .pte bytes (already received into RAM by caller). Returns kErrOk or
    // kErrModel*. Records needs_neutron via method_meta->uses_backend("NeutronBackend").
    uint8_t LoadModel(const uint8_t *pte_bytes, size_t len);

    // Serialize input/output tensor specs into the INFO payload buffer.
    // Returns bytes written.
    size_t GetInfo(uint8_t *out, size_t out_max);

    // Load .bin into input tensor 0. Auto-adapts dtype from TensorInfo.scalar_type().
    // Validates len == input nbytes. Returns kErrOk / kErrNoModel / kErrInputSize.
    uint8_t LoadInput(const uint8_t *bin_bytes, size_t len);

    // Run inference. If needs_neutron, neutronInit before / neutronDeinit after.
    // Serialize timing + outputs into the RESULT payload. Returns bytes
    // written. *err = kErrOk / kErrNpuInit / kErrExecute / kErrNoModel.
    size_t Run(uint8_t *out, size_t out_max, uint8_t *err);

    // Free current Program/Method before loading a new model.
    void Reset();

 private:
    bool needs_neutron_ = false;
    // Whole engine state (allocators + planned buffers + MemoryManager + Method
    // + Program) is heap-allocated and rebuilt fresh on every LoadModel so that
    // the bump-allocator offsets reset to 0 on model swaps. Forward-declared
    // here; defined in the .cpp.
    struct EngineState;
    std::unique_ptr<EngineState> state_;
};

#endif  // EXECUTORCH_ENGINE_H_
