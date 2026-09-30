# v2x_fce_benchmark

## Overview

The V2X/FCE benchmark exercises the EdgeLock V2X Fast Crypto Engine (FCE) on the
i.MX943 and measures its performance from the Cortex-M side for four crypto
primitives:

- AES-256-GCM authenticated encrypt
- AES-256-CBC encrypt
- HMAC-SHA-256 generate
- HASH  SHA-256 generate

For each primitive the benchmark sweeps a set of payload sizes and reports, per
payload point:

- per-request latency: min / avg / max (microseconds)
- jitter: standard deviation of the per-request latency (microseconds)
- throughput (MB/s)
- Ops (operations per second)
- the extrapolated fixed request setup latency and per-byte cost, from a linear
  fit of avg latency versus payload (ExecTime = Setup + N_bytes * T_per_byte)

The sweep is run twice, once in each request-completion mode:

- SIGNAL mode: the MU RX-full interrupt is enabled so the completion is
  delivered as a signal.
- POLL   mode: the MU RX-full interrupt is masked and the response is polled.

Comparing the two mode blocks per algorithm shows where the interrupt-driven
path wins or loses against busy-polling. Typically at small payloads the fixed
MU round-trip plus FCE request setup dominates the total latency; at large
payloads the DMA plus LTC-AES engine throughput dominates instead.

Per-request latency is captured with the Cortex-M DWT cycle counter. The core
clock is assumed to be 800 MHz (override with BENCH_CORE_CLOCK_HZ if different).

## Payload

Each primitive is swept over 12 payload sizes spanning 16..1024 bytes. GCM, HMAC
and HASH use arbitrary lengths; AES-CBC uses 16-byte multiples because CBC
requires whole AES blocks. The extra small-size points (16/32/64/128) sharpen
the fixed request-setup estimate at the low end, while the larger points
(256/512/768/896/1024) characterize the per-byte throughput regime. Sweeping
several sizes is what lets the benchmark separate the fixed request-setup cost
from the per-byte processing cost.

The payload content is not a static compile-time constant. It is filled at
runtime, per payload size, with a deterministic pattern (s_in[i] = i*7 + 0x11).
This is reproducible (identical across runs, boards and builds) but not truly
random. A deterministic pattern is preferred over random data so that every
(algorithm, mode, key/SHA variant) point is measured against the same input and
results stay comparable; no RNG overhead is folded into the timed section.

The choice of content does not affect the timings: AES and SHA on the FCE are
data-oblivious (constant-time) - their execution time depends only on the byte
count, not on the byte values. Payload content would only matter for a
functional known-answer test (which is a separate concern from this performance
benchmark).

Per measurement point the benchmark runs BENCH_WARMUP warmup iterations
(discarded, to reach steady state and remove first-call cache/branch-predictor
and MU/FCE first-request setup effects) followed by BENCH_ITERATIONS timed
iterations. Both are compile-time overridable (e.g. -DBENCH_ITERATIONS=256
-DBENCH_WARMUP=16).

## Throughput modes

Beyond the per-request sweep above, the benchmark reports three additional
throughput views so a report can separate the different amortization effects.
All are AES-256-GCM (except the batch sweep, which covers all four primitives):

- Batch (back-to-back): N single-op requests issued back-to-back and timed as
  one block. Each op is still its own MU push/round-trip; the fixed per-request
  overhead is amortized only by pipelined submission. Shows the sustained
  throughput of the normal single-op API.
- Multi-slot push (1..4 slots): V2X_FCE_AesGcmEncryptMulti packs 1..4 ops into
  a single PUSH_REQUEST (the FCE MU slots A/B/C/D), so one header-only push
  triggers slotCount ops and the fixed MU round-trip + FCE request setup is
  shared across them at the hardware level. Sweeping 1->4 slots shows how slot
  batching drives down the per-push overhead.
- Peak sustained (batch + 4-slot): reported for all four primitives. For
  AES-GCM the two effects are stacked - the maximum slotCount=4 ops per push AND
  N such pushes back-to-back - which is the closest number to the pure per-op
  compute-cost lower bound and the practical peak the FCE + MU path can deliver
  on a saturated stream. AES-CBC / HMAC / HASH have no multi-slot MU API, so
  their peak row is single-op back-to-back (slotCount=1) at maximum submission
  rate - the best sustained rate that primitive's API can reach. For AES-GCM
  the amortized per-op latency should be the lowest and its MB/s the highest of
  all the modes.

In short: batch = many pushes issued rapidly (software-level back-to-back);
multi-slot = fewer pushes each carrying multiple ops (hardware-level slot
batching); peak = both combined.

## Supported toolchains

- GCC ARM Embedded

## Supported boards

- imx943evk (cm33_core1, cm7_core0, cm7_core1)

## Running the demo

Flash the image, open the debug console (115200 8N1) and reset. The console
prints a per-algorithm, per-mode table followed by the derived setup-latency and
bottleneck notes.
