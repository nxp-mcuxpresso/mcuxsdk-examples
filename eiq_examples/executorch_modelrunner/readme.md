Overview - ExecuTorch ModelRunner example project
========

This example turns the board into a generic ExecuTorch model runner / benchmark
tool: instead of hard-coding one model in firmware, the PC uploads any `.pte`
model over UART at runtime, loads it with the ExecuTorch runtime, and runs
inference on uploaded input tensors. Both pure-CPU models (portable kernels)
and Neutron NPU models (delegate) are supported.

After each inference the board returns the raw output tensor bytes together
with the inference time measured with the DWT cycle counter (pure inference
time — UART transfer is not included), so the same firmware serves as a
latency benchmark for any compatible model.

Serial protocol (binary, 115200 baud)
--------

Each frame is `[0x55][0xAA][CMD][LEN:4B LE][PAYLOAD][CRC16:2B LE]`
(CRC-16/CCITT-FALSE over CMD+LEN+PAYLOAD). Commands:

| Command   | Payload                | Response |
|-----------|------------------------|----------|
| LOAD_MODEL| raw `.pte` bytes       | OK / ERROR |
| GET_INFO  | -                      | input/output tensor specs (dtype/shape/dim_order/nbytes) |
| LOAD_INPUT| raw input tensor bytes | OK / ERROR |
| RUN       | -                      | RESULT: `timing_us` (u64 LE) + raw output tensor bytes |

The layout is implemented in `protocol.h` (frame and error codes) and
`executorch_engine.cpp` (INFO / RESULT payload serialization); the Python
implementation is `scripts/run.py`. Error frames carry a 1-byte code
(see `protocol.h`), e.g. `0x03` model too big, `0x14` missing operator
registration.

PC-side tool: scripts/run.py
--------

`run.py` implements the frame codec and provides both an interactive shell and
a one-shot mode. It needs `pyserial`:

```
pip install -r scripts/requirements.txt
```

### Interactive shell

```
python scripts/run.py --port /dev/ttyACM0 --baud 115200
```

```
(modelrunner|no model)> model cifarnet_int8.pte
  >> uploading model cifarnet_int8.pte (111240 bytes)
  [OK] model upload success (111240 bytes)

  model spec:
    inputs : 1
      [0] dtype=Char shape=[1, 3, 32, 32] dim_order=[0, 2, 3, 1] nbytes=3072
    outputs: 1
      [0] dtype=Char shape=[1, 10] nbytes=10

(modelrunner|model loaded)> input img0.bin
  >> uploading input img0.bin (3072 bytes)
  [OK] input upload success (3072 bytes)

(modelrunner|input ready)> run
  >> running inference...
  [OK] inference time: 848 us

  output[0]: dtype=Char shape=[1, 10]
    values: [-128, -128, -128, 127, ...]
```

Commands: `model <file>` load a `.pte`, `input <file>` load an input tensor,
`info` re-query the model spec, `run` run inference, `status`, `reconnect
[<port>]`, `help`, `quit`.

### One-shot mode (script friendly)

With the model (and optionally an input) given as positional arguments the
tool runs once and exits; the exit code reflects success (0) or failure (1).
It prints the inference time plus the raw output tensor values:

```
python scripts/run.py --port /dev/ttyACM0 model.pte input.bin
```

Input tensor files
--------

`.bin` inputs are header-less raw dumps of the model's input tensor: the byte
count must equal the input spec reported by GET_INFO (`nbytes`), and the data
must already match the model's dtype, shape, memory layout (dim_order) and
quantization — the board copies the bytes straight into the input tensor with
no preprocessing. For quantized models the quantization parameters can be
read from the GET_INFO spec.

Kernel registry
--------

`RegisterKernels.cpp` is a hand-written registration table (the generated
"Everything" table does not link against the shipped `libexecutorch.a`).
It covers the op set of common vision and keyword-spotting models; when a new
`.pte` reports load error 0x14 (`load_method`), diff its operators against
this table (`strings -n 2 <model>.pte` shows the operator names) and add the
missing entries, following the unboxing pattern of the existing entries.

Build
--------

Currently supported board: MIMXRT700-EVK (CM33 core0). From the SDK root
(with west and the GNU Arm toolchain in PATH):

```
west build -p always -b mimxrt700evk examples/eiq_examples/executorch_modelrunner --toolchain armgcc -Dcore_id=cm33_core0 --config flash_release -d build_dir
```

The resulting firmware is `build_dir/executorch_modelrunner_cm33_core0.elf`.
