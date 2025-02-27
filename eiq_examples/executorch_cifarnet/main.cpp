/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <iostream>
#include <memory>
#include <stdio.h>
#include <vector>

#include <executorch/backends/nxp/runtime/NeutronDriver.h>
#include <executorch/extension/data_loader/buffer_data_loader.h>
#include <executorch/extension/evalue_util/print_evalue.h>
#include <executorch/extension/runner_util/inputs.h>
#include <executorch/runtime/executor/method.h>
#include <executorch/runtime/executor/program.h>
#include <executorch/runtime/platform/log.h>
#include <executorch/runtime/platform/platform.h>
#include <executorch/runtime/platform/runtime.h>

#include "board_init.h"
#include "fsl_debug_console.h"
#include "image_data.h"
#include "model_pte.h"
#include "timer.h"

using namespace exec_aten;
using namespace std;
using torch::executor::Error;
using torch::executor::Result;

static uint8_t __attribute__((aligned(16))) method_allocator_pool[512 * 1024U];
static uint8_t __attribute__((aligned(16))) tmp_allocator_pool[512 * 1024U];

void et_pal_init(void) {}

int main(void)
{
    BOARD_Init();
    TIMER_Init();

    neutronInit();

    torch::executor::runtime_init();

    auto loader = torch::executor::util::BufferDataLoader(model_pte, sizeof(model_pte));
    PRINTF("Model PTE file loaded. Size: %d bytes.\r\n", sizeof(model_pte));

    Result<torch::executor::Program> program = torch::executor::Program::load(&loader);
    if (!program.ok()) {
        PRINTF("Program loading failed\r\n");
    }
    PRINTF("Model buffer loaded, has %d methods\r\n", program->num_methods());

    const char* method_name = nullptr;
    {
        const auto method_name_result = program->get_method_name(0);
        method_name = *method_name_result;
    }
    PRINTF("Running method %s\r\n", method_name);

    Result<torch::executor::MethodMeta> method_meta = program->method_meta(method_name);
    if (!method_meta.ok()) {
        PRINTF("Failed to get method_meta for %s: 0x%x\r\n",
	    method_name, (unsigned int)method_meta.error());
    }

    torch::executor::MemoryAllocator method_allocator{
        torch::executor::MemoryAllocator(sizeof(method_allocator_pool), method_allocator_pool)};
    torch::executor::MemoryAllocator tmp_allocator{
        torch::executor::MemoryAllocator(sizeof(tmp_allocator_pool), tmp_allocator_pool)};

    std::vector<std::unique_ptr<uint8_t[]>> planned_buffers; // Owns the memory
    std::vector<torch::executor::Span<uint8_t>> planned_spans; // Passed to the allocator
    size_t num_memory_planned_buffers = method_meta->num_memory_planned_buffers();

    for (size_t id = 0; id < num_memory_planned_buffers; ++id) {
        size_t buffer_size = static_cast<size_t>(method_meta->memory_planned_buffer_size(id).get());
        PRINTF("Setting up planned buffer %zu, size %zu.\r\n", id, buffer_size);

        planned_buffers.push_back(std::make_unique<uint8_t[]>(buffer_size));
        planned_spans.push_back({planned_buffers.back().get(), buffer_size});
    }

    torch::executor::HierarchicalAllocator planned_memory({planned_spans.data(), planned_spans.size()});

    torch::executor::MemoryManager memory_manager(&method_allocator, &planned_memory, &tmp_allocator);

    Result<torch::executor::Method> method = program->load_method(method_name, &memory_manager);
    if (!method.ok()) {
        PRINTF("Loading of method %s failed with status 0x%\r\n" PRIx32,
	    method_name, method.error());
    }
    PRINTF("Method loaded.\r\n");

    PRINTF("Preparing inputs...\r\n");
    torch::executor::Tensor::SizesType sizes[] = {1, 3, 32, 32};
    torch::executor::Tensor::DimOrderType dim_order[] = {0, 1, 2, 3};

    torch::executor::TensorImpl impl(torch::executor::ScalarType::Float, 4, sizes, image_data, dim_order);
    torch::executor::Tensor tensor(&impl);
    Error status = method->set_input(tensor, 0);
    if (status != Error::Ok) {
        PRINTF("Preparing inputs tensors for method %s failed with status 0x%...\r\n",
	       method_name, status);
    }
    PRINTF("Input prepared. \r\n");

    PRINTF("Starting the model execution...\r\n");

    auto startTime = TIMER_GetTimeInUS();
    status = method->execute();
    if (status != Error::Ok) {
	PRINTF("Execution of method %s failed with status 0x%\r\n" PRIx32,
               method_name, status);
    } else {
        PRINTF("Model executed successfully.\r\n");
    }
    auto endTime = TIMER_GetTimeInUS();

    PRINTF("----------------------------------------\r\n");
    PRINTF("     Inference time: %d us\r\n", endTime - startTime);
    PRINTF("----------------------------------------\r\n");

    std::vector<torch::executor::EValue> outputs(method->outputs_size());
    PRINTF("%zu outputs: \r\n", outputs.size());
    status = method->get_outputs(outputs.data(), outputs.size());
    ET_CHECK(status == Error::Ok);
    for (int i = 0; i < outputs.size(); ++i) {
        Tensor t = outputs[i].toTensor();
        for (int j = 0; j < outputs[i].toTensor().numel(); ++j) {
            if (t.scalar_type() == ScalarType::Int) {
                PRINTF("Output[%d][%d]: %d\r\n", i, j,
                    outputs[i].toTensor().const_data_ptr<int>()[j]);
            } else {
                PRINTF("Output[%d][%d]: %f\r\n", i, j,
                    outputs[i].toTensor().const_data_ptr<float>()[j]);
            }
        }
    }

    neutronDeinit();
    PRINTF("Program complete, exiting.\r\n");
    return 0;
}
