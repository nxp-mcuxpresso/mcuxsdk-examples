/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * EdgeLock V2X/FCE performance benchmark (MCUX-90943).
 *
 * Measures the throughput, per-request latency, fixed PRIME/FCE setup latency,
 * operations-per-second (Ops), and jitter of the V2X/FCE Fast Crypto Engine on
 * i.MX943 (Cortex-M7) for four crypto primitives:
 *   - AES-256-GCM authenticated encrypt
 *   - AES-256-CBC encrypt
 *   - HMAC-SHA-256 generate
 *   - HASH  SHA-256 generate
 *
 * Two request-completion strategies are timed independently, matching the two
 * modes described in the V2X/FCE user guide:
 *   - SIGNAL mode: the driver waits on the MU RX-full flag via the interrupt
 *     path (MU RX interrupt enabled); the caller blocks until the completion
 *     signal arrives.
 *   - POLL mode:   the driver spins on the MU RX-full flag with interrupts
 *     masked; the caller busy-polls until the response is ready.
 *
 * The public v2x_fce API is synchronous, so a per-request latency is captured
 * with the Cortex-M DWT cycle counter around each call. For each (algo, mode,
 * payload) point N iterations are run; min/max/avg/stddev(jitter) and the
 * derived throughput (MB/s), Ops (1/avg) and the extrapolated fixed setup cost
 * are reported. The fixed setup latency is obtained from a linear fit of the
 * average latency against payload size (ExecTime = Setup + N_bytes * T_per_byte),
 * matching the FCE user-guide execution-time model.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdio.h>

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"

#include "v2x_fce.h"          /* EdgeLock V2X / FCE fast crypto engine service */
#include "fsl_ele_base_api.h" /* EdgeLock enclave base API: V2X FW bring-up */
#include "v2x_base.h"         /* EdgeLock V2X base (debug) service: FW version query */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#ifndef V2X_FCE_MU
#define V2X_FCE_MU ((MU_Type *)EXAMPLE_V2X_FCE_MU_BASE)
#endif

#ifndef V2X_MU
#define V2X_MU ((MU_Type *)EXAMPLE_V2X_MU_BASE)
#endif

#ifndef V2X_FW_STATE_RETRIES
#define V2X_FW_STATE_RETRIES (1000u)
#endif

/* AES/HMAC key slot used by the benchmark. */
#define BENCH_AES_KEY_SLOT  (0u)
#define BENCH_HMAC_KEY_SLOT (0u)

/* Iterations per measurement point. Kept modest so the whole sweep finishes in
 * a few seconds; increase for tighter jitter statistics. */
#ifndef BENCH_ITERATIONS
#define BENCH_ITERATIONS (256u)
#endif

/* Warmup iterations discarded before timing (prime caches / DMA path). */
#ifndef BENCH_WARMUP
#define BENCH_WARMUP (16u)
#endif

/* Largest payload exercised (bytes). Buffers are sized to this. */
#define BENCH_MAX_PAYLOAD (1024u)

/* SHA-256 digest size. */
#define BENCH_SHA256_DIGEST (32u)

/* Largest SHA digest exercised (SHA-512 = 64 bytes). Buffer is sized to this. */
#define BENCH_MAX_DIGEST (64u)

/* Core clock (Hz) used to convert DWT cycles to microseconds. Overridden from
 * app.h if the board layer provides BOARD_CORE_CLOCK_HZ. */
#ifndef BENCH_CORE_CLOCK_HZ
#ifdef BOARD_CORE_CLOCK_HZ
#define BENCH_CORE_CLOCK_HZ BOARD_CORE_CLOCK_HZ
#else
#define BENCH_CORE_CLOCK_HZ (800000000u)
#endif
#endif

/* Runtime core-clock (Hz) used to convert DWT cycles to nanoseconds. The
 * DWT cycle counter runs at the core clock, which differs per core
 * (cm33_core1, cm7_core0, cm7_core1) and per clock configuration. It is
 * seeded with the BENCH_CORE_CLOCK_HZ fallback and then overridden at
 * runtime in main() from SystemCoreClock (see SystemCoreClockUpdate()), so
 * the timing math tracks the actual frequency the board layer programmed
 * rather than a hard-coded constant. */
static uint32_t s_benchCoreClockHz = BENCH_CORE_CLOCK_HZ;

/*! @brief Completion strategy under test. */
typedef enum _bench_mode
{
    kBenchModeSignal = 0, /*!< MU RX interrupt driven completion. */
    kBenchModePoll   = 1, /*!< MU RX-full busy-poll completion. */
} bench_mode_t;

/*! @brief Crypto primitive under test. */
typedef enum _bench_algo
{
    kBenchAlgoGcm  = 0,
    kBenchAlgoCbc  = 1,
    kBenchAlgoHmac = 2,
    kBenchAlgoHash = 3,
} bench_algo_t;

/*! @brief Operation direction under test.
 *
 * FORWARD is the protect/send side (AES-GCM/CBC encrypt, HMAC/HASH generate);
 * REVERSE is the receive/verify side (AES-GCM/CBC decrypt, HMAC verify). The
 * FCE per-request MU/descriptor cost is direction-symmetric, but the receive
 * side is what a IEC 62351-6 authenticator runs on every inbound packet, so it
 * is measured explicitly here (MCUX-91696 direction coverage). HASH has no
 * reverse form and is skipped in REVERSE. */
typedef enum _bench_dir
{
    kBenchDirForward = 0, /*!< encrypt / generate. */
    kBenchDirReverse = 1, /*!< decrypt / verify. */
} bench_dir_t;

/*! @brief Active direction consulted by BENCH_RunOne(). */
static bench_dir_t s_benchDir = kBenchDirForward;

/*! @brief One measurement result (single algo/mode/payload point). */
typedef struct _bench_result
{
    uint32_t payload; /*!< Payload size in bytes. */
    uint32_t minCyc;  /*!< Minimum per-op latency in DWT cycles. */
    uint32_t maxCyc;  /*!< Maximum per-op latency in DWT cycles. */
    uint32_t avgCyc;  /*!< Mean per-op latency in DWT cycles. */
    uint32_t stdCyc;  /*!< Standard deviation (jitter) in DWT cycles. */
    bool ok;          /*!< true if every iteration returned success. */
} bench_result_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* AES-256 / HMAC key material (arbitrary, benchmark only). */
static const uint8_t s_key[32] = {
    0x92, 0xe1, 0x1d, 0xcd, 0xaa, 0x86, 0x6f, 0x5c, 0xe7, 0x90, 0xfd, 0x24, 0x50, 0x1f, 0x92, 0x50,
    0x9a, 0xac, 0xf4, 0xcb, 0x8b, 0x13, 0x39, 0xd5, 0x0c, 0x9c, 0x12, 0x40, 0x93, 0x5d, 0xd0, 0x8b,
};

SDK_ALIGN(static const uint8_t s_iv12[12], 8u) = {
    0xac, 0x93, 0xa1, 0xa6, 0x14, 0x52, 0x99, 0xbd, 0xe9, 0x02, 0xf2, 0x1a,
};

SDK_ALIGN(static const uint8_t s_iv16[16], 8u) = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
};

/* Input / output buffers sized for the largest payload; GCM tag / digest out. */
SDK_ALIGN(static uint8_t s_in[BENCH_MAX_PAYLOAD], 8u);
SDK_ALIGN(static uint8_t s_out[BENCH_MAX_PAYLOAD], 8u);
SDK_ALIGN(static uint8_t s_tag[16], 8u);
/* AAD is mandatory for the FCE AES-GCM Oneshot request: the FCE prefetcher
 * fetches the AAD address unconditionally, so a NULL/empty AAD makes the
 * request fail with status=0x29 rating=0x1229 (FCE_REQ_ERROR_RATING /
 * Prefetcher Read AXI error). Provide a small fixed AAD buffer for every GCM
 * benchmark op. */
SDK_ALIGN(static const uint8_t s_aad[16], 8u) = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
};

SDK_ALIGN(static uint8_t s_digest[BENCH_MAX_DIGEST], 8u);

/* Active algorithm variant selectors, consulted by BENCH_RunOne().
 * - s_shaAlgo / s_shaDigestSize select the SHA variant for HASH and HMAC.
 * - s_aesKeyBytes records the AES key size currently loaded in slot 0 (for the
 *   GCM/CBC labels only; the key length is bound at load time). */
static v2x_fce_sha_algo_t s_shaAlgo = kV2X_FCE_Sha256;
static uint32_t s_shaDigestSize     = BENCH_SHA256_DIGEST;
static uint32_t s_aesKeyBytes       = 32u;

/* Digest size (bytes) for a given SHA algorithm. */
static uint32_t BENCH_ShaDigestSize(v2x_fce_sha_algo_t algo)
{
    switch (algo)
    {
        case kV2X_FCE_Sha384:
            return 48u;
        case kV2X_FCE_Sha512:
            return 64u;
        case kV2X_FCE_Sha256:
        default:
            return 32u;
    }
}

/* Short SHA label for table headers. */
static const char *BENCH_ShaName(v2x_fce_sha_algo_t algo)
{
    switch (algo)
    {
        case kV2X_FCE_Sha384:
            return "SHA-384";
        case kV2X_FCE_Sha512:
            return "SHA-512";
        case kV2X_FCE_Sha256:
        default:
            return "SHA-256";
    }
}

/* Per-payload matrices. GCM/HMAC/HASH use arbitrary sizes; CBC needs multiples
 * of the 16-byte AES block. */
static const uint32_t s_payloadsGeneric[] = {16u, 32u, 64u, 128u, 137u, 256u, 275u, 512u, 699u, 768u, 896u, 1024u};
static const uint32_t s_payloadsCbc[]     = {16u, 32u, 64u, 128u, 144u, 256u, 288u, 512u, 704u, 768u, 896u, 1024u};

#define BENCH_NUM_POINTS (sizeof(s_payloadsGeneric) / sizeof(s_payloadsGeneric[0]))

/* Latency capture (DWT cycles) for the current measurement point. */
static uint32_t s_samples[BENCH_ITERATIONS];

/*******************************************************************************

 * Code
 ******************************************************************************/

/* FCE FW per-open-service request quota: the V2X/FCE firmware wedges
 * permanently after a fixed number of cumulative requests per open service
 * session. Empirically the wedge is at 4352 = 17 * the 256-entry GET_INFO
 * "Number of FIFO entries", after which every request returns a fail status.
 * A full 12-point sweep issues 12 * (BENCH_WARMUP + BENCH_ITERATIONS) requests
 * (= 3264 at the default 16+256), which stays under the quota, so resetting the
 * FCE service (Close + Open + reload keys) at the start of each sweep keeps
 * every table inside a fresh quota window. */
static void BENCH_SessionReset(v2x_fce_handle_t *handle)
{
    MU_Type *mu = handle->mu;

    (void)V2X_FCE_Close(handle);
    (void)V2X_FCE_Open(mu, handle);

    /* Re-bind the keys the current sweep needs. The AES key length and the
     * HMAC SHA variant are tracked by the s_aesKeyBytes / s_shaAlgo globals. */
    (void)V2X_FCE_LoadAesPlainKey(handle, BENCH_AES_KEY_SLOT, s_key, s_aesKeyBytes);
    (void)V2X_FCE_LoadHmacPlainKey(handle, BENCH_HMAC_KEY_SLOT, s_shaAlgo, s_key, sizeof(s_key));
}

/* Enable the DWT cycle counter (CYCCNT) for per-request latency capture. */
static void BENCH_DwtInit(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0u;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

static inline uint32_t BENCH_CycNow(void)
{
    return DWT->CYCCNT;
}

/* Convert a DWT cycle count to microseconds (x1000 fixed point avoided; use
 * 64-bit intermediate to keep precision at the core clock). */
static uint32_t BENCH_CycToNs(uint32_t cyc)
{
    return (uint32_t)(((uint64_t)cyc * 1000000000ull) / (uint64_t)s_benchCoreClockHz);
}

/* Compute min/max/avg/stddev over the captured samples[0..count-1]. */
static void BENCH_Stats(const uint32_t *samples, uint32_t count, bench_result_t *res)
{
    uint32_t i;
    uint64_t sum = 0u;
    uint32_t mn  = 0xFFFFFFFFu;
    uint32_t mx  = 0u;

    for (i = 0u; i < count; i++)
    {
        uint32_t v = samples[i];
        sum += v;
        if (v < mn)
        {
            mn = v;
        }
        if (v > mx)
        {
            mx = v;
        }
    }

    res->minCyc = mn;
    res->maxCyc = mx;
    res->avgCyc = (count != 0u) ? (uint32_t)(sum / count) : 0u;

    /* Population standard deviation via integer sqrt of the variance. */
    uint64_t var = 0u;
    for (i = 0u; i < count; i++)
    {
        int64_t d = (int64_t)samples[i] - (int64_t)res->avgCyc;
        var += (uint64_t)(d * d);
    }
    if (count != 0u)
    {
        var /= count;
    }

    /* Integer square root (Newton). */
    uint64_t x = var;
    uint64_t r = 0u;
    if (x != 0u)
    {
        r = x;
        uint64_t last;
        do
        {
            last = r;
            r    = (r + x / r) / 2u;
        } while (r < last);
    }
    res->stdCyc = (uint32_t)r;
}

/* Fill s_in with a deterministic pattern up to len bytes. */
static void BENCH_FillInput(uint32_t len)
{
    for (uint32_t i = 0u; i < len; i++)
    {
        s_in[i] = (uint8_t)(i * 7u + 0x11u);
    }
}
/* PLACEHOLDER_HELPERS */

static status_t BENCH_RunOne(v2x_fce_handle_t *handle, bench_algo_t algo, uint32_t payload);

/* For REVERSE direction, run one FORWARD op (untimed) to populate the buffers
 * the decrypt/verify consumes: GCM/CBC leave ciphertext in s_out (+ tag in
 * s_tag); HMAC leaves the MAC in s_digest. Called once per payload point
 * before the timed reverse loop. Returns kStatus_Success if the seed op ran. */
static status_t BENCH_PrepareReverse(v2x_fce_handle_t *handle, bench_algo_t algo, uint32_t payload)
{
    bench_dir_t saved = s_benchDir;
    status_t st;
    s_benchDir = kBenchDirForward;
    st         = BENCH_RunOne(handle, algo, payload);
    /* GCM/CBC decrypt take the ciphertext as input: move s_out -> s_in so the
     * timed reverse call decrypts real ciphertext. */
    if ((algo == kBenchAlgoGcm) || (algo == kBenchAlgoCbc))
    {
        for (uint32_t i = 0u; i < payload; i++)
        {
            s_in[i] = s_out[i];
        }
    }
    s_benchDir = saved;
    return st;
}

/* Run a single crypto op for the given algo/payload. Returns kStatus_Success on
 * success. The FCE completion strategy (signal vs poll) is a build-time
 * property of the v2x_fce driver MU wait loop; this benchmark exercises both by
 * selecting the interrupt-enable state before the call. */
static status_t BENCH_RunOne(v2x_fce_handle_t *handle, bench_algo_t algo, uint32_t payload)
{
    status_t st;

    switch (algo)
    {
        case kBenchAlgoGcm:
        {
            v2x_fce_gcm_op_t op = {0};
            op.keySlot          = BENCH_AES_KEY_SLOT;
            op.iv               = s_iv12;
            op.ivSize           = sizeof(s_iv12);
            op.aad              = s_aad;
            op.aadSize          = sizeof(s_aad);

            op.input     = s_in;
            op.inputSize = payload;
            op.output    = s_out;
            op.tag       = s_tag;
            op.tagSize   = sizeof(s_tag);
            /* REVERSE swaps ciphertext back to plaintext: input is the stored
             * ciphertext, tag is verified, output receives recovered text. */
            st = (s_benchDir == kBenchDirReverse) ? V2X_FCE_AesGcmDecrypt(handle, &op, 1u) :
                                                    V2X_FCE_AesGcmEncrypt(handle, &op, 1u);
            break;
        }
        case kBenchAlgoCbc:
        {
            v2x_fce_cbc_op_t op = {0};
            op.keySlot          = BENCH_AES_KEY_SLOT;
            op.iv               = s_iv16;
            op.ivSize           = sizeof(s_iv16);
            op.input            = s_in;
            op.inputSize        = payload;
            op.output           = s_out;
            st                  = (s_benchDir == kBenchDirReverse) ? V2X_FCE_AesCbcDecrypt(handle, &op, 1u) :
                                                                     V2X_FCE_AesCbcEncrypt(handle, &op, 1u);
            break;
        }
        case kBenchAlgoHmac:
        {
            v2x_fce_hmac_op_t op = {0};
            op.keySlot           = BENCH_HMAC_KEY_SLOT;
            op.shaAlgo           = s_shaAlgo;
            op.input             = s_in;
            op.inputSize         = payload;
            op.digest            = s_digest;
            op.digestSize        = s_shaDigestSize;
            /* REVERSE verifies the stored MAC against a fresh recompute. */
            st = (s_benchDir == kBenchDirReverse) ? V2X_FCE_HmacVerify(handle, &op, 1u) :
                                                    V2X_FCE_HmacGenerate(handle, &op, 1u);
            break;
        }
        case kBenchAlgoHash:
        default:
        {
            v2x_fce_hash_op_t op = {(v2x_fce_sha_algo_t)0};
            op.shaAlgo           = s_shaAlgo;
            op.input             = s_in;
            op.inputSize         = payload;
            op.digest            = s_digest;
            op.digestSize        = s_shaDigestSize;
            st                   = V2X_FCE_HashGenerate(handle, &op, 1u);
            break;
        }
    }

    return st;
}

/* MU RX interrupt enable/disable to select the completion strategy of the
 * underlying driver wait loop. For SIGNAL mode the MU RX-full interrupt is
 * enabled (the completion is delivered as a signal); for POLL mode it is
 * disabled and the driver spins on the flag. */
static void BENCH_SelectMode(v2x_fce_handle_t *handle, bench_mode_t mode)
{
    /* MCUX-90943: SIGNAL arms the real interrupt-driven completion path in the
     * v2x_fce driver (V2X_FH MU RX-full interrupt routed to CM7 via IRQSTEER as
     * V2X_FH_APCH0_IRQn; the driver blocks in __WFI() until the ISR flags
     * completion). POLL keeps the busy-poll path so the two modes genuinely
     * differ instead of both busy-waiting the RX-full flag. */
    /* The V2X-FH MU RX IRQ number is core/SoC specific, so the board
     * overlay app.h supplies it as EXAMPLE_V2X_FCE_IRQN (default -1 = no
     * interrupt line, i.e. busy-poll only). The application ISR that owns
     * that vector calls V2X_FCE_HandleIRQ(). */
#ifndef EXAMPLE_V2X_FCE_IRQN
#define EXAMPLE_V2X_FCE_IRQN (-1)
#endif
    (void)V2X_FCE_SetWaitMode(handle, (mode == kBenchModeSignal), (int32_t)EXAMPLE_V2X_FCE_IRQN);
}

/* Measure one (algo, mode, payload) point: warmup + N timed iterations. */
static void BENCH_Point(
    v2x_fce_handle_t *handle, bench_algo_t algo, bench_mode_t mode, uint32_t payload, bench_result_t *res)
{
    uint32_t i;
    status_t status;

    res->payload = payload;
    res->ok      = true;

    BENCH_FillInput(payload);
    BENCH_SelectMode(handle, mode);

    /* REVERSE: seed valid ciphertext/tag/MAC (untimed) before the timed loop. */
    if (s_benchDir == kBenchDirReverse)
    {
        (void)BENCH_PrepareReverse(handle, algo, payload);
    }

    for (i = 0u; i < BENCH_WARMUP; i++)
    {
        (void)BENCH_RunOne(handle, algo, payload);
    }

    for (i = 0u; i < BENCH_ITERATIONS; i++)
    {
        uint32_t t0 = BENCH_CycNow();
        status      = BENCH_RunOne(handle, algo, payload);
        uint32_t t1 = BENCH_CycNow();

        s_samples[i] = t1 - t0;
        if (status != kStatus_Success)
        {
            res->ok = false;
        }
    }

    BENCH_Stats(s_samples, BENCH_ITERATIONS, res);
}

/* Human-readable label for the current (algo, active-variant) combination.
 * Reflects the AES key size (GCM/CBC) or SHA variant (HMAC/HASH) currently
 * selected via the s_aesKeyBytes / s_shaAlgo globals. Returned in a small
 * static buffer; not reentrant, fine for this single-threaded benchmark. */
static const char *BENCH_AlgoName(bench_algo_t algo)
{
    static char label[20];

    switch (algo)
    {
        case kBenchAlgoGcm:
            (void)snprintf(label, sizeof(label), "AES-%u-GCM", (unsigned)(s_aesKeyBytes * 8u));
            break;
        case kBenchAlgoCbc:
            (void)snprintf(label, sizeof(label), "AES-%u-CBC", (unsigned)(s_aesKeyBytes * 8u));
            break;
        case kBenchAlgoHmac:
            (void)snprintf(label, sizeof(label), "HMAC-%s", BENCH_ShaName(s_shaAlgo));
            break;
        case kBenchAlgoHash:
        default:
            (void)snprintf(label, sizeof(label), "HASH-%s", BENCH_ShaName(s_shaAlgo));
            break;
    }
    if (s_benchDir == kBenchDirReverse)
    {
        /* Tag the direction so the report table header is unambiguous. */
        size_t n        = strlen(label);
        const char *suf = (algo == kBenchAlgoHmac) ? "-VERIFY" : (algo == kBenchAlgoHash) ? "-GEN" : "-DECRYPT";
        (void)snprintf(label + n, sizeof(label) - n, "%s", suf);
    }
    return label;
}

/* Report a full payload sweep for one (algo, mode). Prints a table with
 * min/avg/max latency (us), jitter (stddev, us), throughput (MB/s) and Ops
 * (ops/s), then extrapolates the fixed setup latency from a two-point fit
 * (smallest vs largest payload). */
static void BENCH_Sweep(v2x_fce_handle_t *handle, bench_algo_t algo, bench_mode_t mode)
{
    const uint32_t *payloads = (algo == kBenchAlgoCbc) ? s_payloadsCbc : s_payloadsGeneric;
    bench_result_t results[BENCH_NUM_POINTS];
    uint32_t p;

    PRINTF("\r\n#### %s  method=one-shot  submit=single-request  mode=%s\r\n\r\n", BENCH_AlgoName(algo),
           (mode == kBenchModeSignal) ? "SIGNAL" : "POLL");
    PRINTF("| bytes | min_us | avg_us | max_us | jitter_us | MB/s | Ops/s | kpps | ok |\r\n");
    PRINTF("|------:|-------:|-------:|-------:|----------:|-----:|------:|-----:|:--:|\r\n");

    for (p = 0u; p < BENCH_NUM_POINTS; p++)
    {
        uint32_t payload  = payloads[p];
        bench_result_t *r = &results[p];

        /* Reset the FCE session before each point to stay under the FW request
         * quota (see BENCH_SessionReset). One point issues at most
         * BENCH_WARMUP + BENCH_ITERATIONS requests, well below the 4352 wedge. */
        BENCH_SessionReset(handle);
        BENCH_Point(handle, algo, mode, payload, r);

        uint32_t minNs = BENCH_CycToNs(r->minCyc);
        uint32_t avgNs = BENCH_CycToNs(r->avgCyc);
        uint32_t maxNs = BENCH_CycToNs(r->maxCyc);
        uint32_t jitNs = BENCH_CycToNs(r->stdCyc);

        /* Throughput MB/s = payload / avg_seconds / 1e6
         *                 = payload * 1e9 / avgNs / 1e6 = payload * 1000 / avgNs. */
        uint32_t mbps = (avgNs != 0u) ? (uint32_t)(((uint64_t)payload * 1000u) / avgNs) : 0u;
        /* Ops/s = 1e9 / avgNs. kpps = Ops/s / 1000 (thousand ops per second). */
        uint32_t ops  = (avgNs != 0u) ? (uint32_t)(1000000000ull / avgNs) : 0u;
        uint32_t kpps = (avgNs != 0u) ? (uint32_t)(1000000ull / avgNs) : 0u;

        PRINTF("| %u | %u.%03u | %u.%03u | %u.%03u | %u.%03u | %u | %u | %u | %s |\r\n", (unsigned)payload,
               (unsigned)(minNs / 1000u), (unsigned)(minNs % 1000u), (unsigned)(avgNs / 1000u),
               (unsigned)(avgNs % 1000u), (unsigned)(maxNs / 1000u), (unsigned)(maxNs % 1000u),
               (unsigned)(jitNs / 1000u), (unsigned)(jitNs % 1000u), (unsigned)mbps, (unsigned)ops, (unsigned)kpps,
               r->ok ? "OK" : "FAIL");
    }

    /* Fixed setup latency via a two-point linear fit over avg latency:
     *   avg(p) = setup + slope * p
     *   slope  = (avg[max] - avg[min]) / (p[max] - p[min])
     *   setup  = avg[min] - slope * p[min]
     * Report setup in microseconds and per-byte cost in nanoseconds. */
    {
        uint32_t pMin = results[0].payload;
        uint32_t pMax = results[BENCH_NUM_POINTS - 1u].payload;
        uint32_t aMin = BENCH_CycToNs(results[0].avgCyc);
        uint32_t aMax = BENCH_CycToNs(results[BENCH_NUM_POINTS - 1u].avgCyc);

        if (pMax > pMin && aMax >= aMin)
        {
            uint32_t slopeNsPerByte = (aMax - aMin) / (pMax - pMin);
            int64_t setupNs         = (int64_t)aMin - (int64_t)slopeNsPerByte * (int64_t)pMin;
            if (setupNs < 0)
            {
                setupNs = 0;
            }
            PRINTF("\r\n_Setup latency ~ %u.%03u us, per-byte ~ %u ns (ExecTime = Setup + N*Tpb)_\r\n",
                   (unsigned)((uint32_t)setupNs / 1000u), (unsigned)((uint32_t)setupNs % 1000u),
                   (unsigned)slopeNsPerByte);
        }
    }
}
/* Batched (back-to-back burst) sweep for one (algo, mode). Instead of timing
 * each request individually, this times a burst of BENCH_ITERATIONS requests as
 * a single block and reports the amortized per-op latency and the sustained
 * aggregate throughput (total bytes / total time). This models the batch /
 * pipelined submission case the FCE user guide describes, where the fixed
 * per-request setup is amortized across a stream of requests. */
static void BENCH_SweepBatch(v2x_fce_handle_t *handle, bench_algo_t algo, bench_mode_t mode)
{
    const uint32_t *payloads = (algo == kBenchAlgoCbc) ? s_payloadsCbc : s_payloadsGeneric;
    uint32_t p;

    PRINTF("\r\n#### %s  method=batch-stream  submit=single-request-back-to-back  mode=%s  (x%u)\r\n\r\n",
           BENCH_AlgoName(algo), (mode == kBenchModeSignal) ? "SIGNAL" : "POLL", (unsigned)BENCH_ITERATIONS);
    PRINTF("| bytes | batch_us | amortized_us/op | MB/s | Ops/s | kpps | ok |\r\n");
    PRINTF("|------:|---------:|----------------:|-----:|------:|-----:|:--:|\r\n");

    for (p = 0u; p < BENCH_NUM_POINTS; p++)
    {
        uint32_t payload = payloads[p];
        uint32_t i;
        bool ok = true;

        /* Reset the FCE session per point to stay under the FW request quota. */
        BENCH_SessionReset(handle);

        BENCH_FillInput(payload);
        BENCH_SelectMode(handle, mode);

        for (i = 0u; i < BENCH_WARMUP; i++)
        {
            (void)BENCH_RunOne(handle, algo, payload);
        }

        uint32_t t0 = BENCH_CycNow();
        for (i = 0u; i < BENCH_ITERATIONS; i++)
        {
            if (BENCH_RunOne(handle, algo, payload) != kStatus_Success)
            {
                ok = false;
            }
        }
        uint32_t t1 = BENCH_CycNow();

        uint32_t batchNs = BENCH_CycToNs(t1 - t0);

        uint32_t perNs = (BENCH_ITERATIONS != 0u) ? (batchNs / BENCH_ITERATIONS) : 0u;

        /* Aggregate throughput = total bytes / total time.
         * total bytes = payload * N; MB/s = totalBytes * 1000 / batchNs. */
        uint64_t totalBytes = (uint64_t)payload * BENCH_ITERATIONS;
        uint32_t mbps       = (batchNs != 0u) ? (uint32_t)((totalBytes * 1000u) / batchNs) : 0u;
        uint32_t ops        = (perNs != 0u) ? (uint32_t)(1000000000ull / perNs) : 0u;
        uint32_t kpps       = (perNs != 0u) ? (uint32_t)(1000000ull / perNs) : 0u;

        PRINTF("| %u | %u.%03u | %u.%03u | %u | %u | %u | %s |\r\n", (unsigned)payload, (unsigned)(batchNs / 1000u),
               (unsigned)(batchNs % 1000u), (unsigned)(perNs / 1000u), (unsigned)(perNs % 1000u), (unsigned)mbps,
               (unsigned)ops, (unsigned)kpps, ok ? "OK" : "FAIL");
    }
}

/* Build a slotCount-entry ops[] array for the given algo (all slots identical:
 * same payload/keys, just replicated across MU slots A..D) and issue ONE
 * multi-slot PUSH_REQUEST through the now-multi base API. Returns the base API
 * status. Used by the multi-slot and peak-sustained sweeps for every primitive
 * (GCM/CBC/HMAC/HASH) so the FCE slot batching is exercised uniformly. */
static status_t BENCH_RunMulti(v2x_fce_handle_t *handle, bench_algo_t algo, uint32_t payload, uint32_t slotCount)
{
    uint32_t s;

    switch (algo)
    {
        case kBenchAlgoGcm:
        {
            v2x_fce_gcm_op_t ops[4];
            for (s = 0u; s < slotCount; s++)
            {
                ops[s].keySlot   = BENCH_AES_KEY_SLOT;
                ops[s].iv        = s_iv12;
                ops[s].ivSize    = sizeof(s_iv12);
                ops[s].aad       = s_aad;
                ops[s].aadSize   = sizeof(s_aad);
                ops[s].input     = s_in;
                ops[s].inputSize = payload;
                ops[s].output    = s_out;
                ops[s].tag       = s_tag;
                ops[s].tagSize   = sizeof(s_tag);
            }
            return V2X_FCE_AesGcmEncrypt(handle, ops, slotCount);
        }
        case kBenchAlgoCbc:
        {
            v2x_fce_cbc_op_t ops[4];
            for (s = 0u; s < slotCount; s++)
            {
                ops[s].keySlot   = BENCH_AES_KEY_SLOT;
                ops[s].iv        = s_iv16;
                ops[s].ivSize    = sizeof(s_iv16);
                ops[s].input     = s_in;
                ops[s].inputSize = payload;
                ops[s].output    = s_out;
            }
            return V2X_FCE_AesCbcEncrypt(handle, ops, slotCount);
        }
        case kBenchAlgoHmac:
        {
            v2x_fce_hmac_op_t ops[4];
            for (s = 0u; s < slotCount; s++)
            {
                ops[s].keySlot    = BENCH_HMAC_KEY_SLOT;
                ops[s].shaAlgo    = s_shaAlgo;
                ops[s].input      = s_in;
                ops[s].inputSize  = payload;
                ops[s].digest     = s_digest;
                ops[s].digestSize = s_shaDigestSize;
            }
            return V2X_FCE_HmacGenerate(handle, ops, slotCount);
        }
        case kBenchAlgoHash:
        default:
        {
            v2x_fce_hash_op_t ops[4];
            for (s = 0u; s < slotCount; s++)
            {
                ops[s].shaAlgo    = s_shaAlgo;
                ops[s].input      = s_in;
                ops[s].inputSize  = payload;
                ops[s].digest     = s_digest;
                ops[s].digestSize = s_shaDigestSize;
            }
            return V2X_FCE_HashGenerate(handle, ops, slotCount);
        }
    }
}

/* Multi-slot-per-PUSH sweep for one (algo, mode). slotCount (1..4) selects how
 * many of the four FCE MU slots (A/B/C/D) each PUSH_REQUEST fills: 1 slot = one
 * op per push (equivalent to the single-op path), up to 4 slots = four ops
 * triggered by a single header-only push so the fixed MU round-trip and FCE
 * request setup is shared across all slotCount ops. A burst of BENCH_ITERATIONS
 * pushes is timed as a single block; the amortized per-op latency
 * (block / (N*slotCount)) and the sustained aggregate throughput
 * (slotCount*payload*N total bytes / block time) are reported. Calling this
 * with slotCount = 1,2,3,4 shows how the FCE's slot batching amortizes the
 * per-push overhead. Works for every primitive via the (ops,count) base API. */
static void BENCH_SweepSlots(v2x_fce_handle_t *handle, bench_algo_t algo, bench_mode_t mode, uint32_t slotCount)
{
    const uint32_t *payloads = (algo == kBenchAlgoCbc) ? s_payloadsCbc : s_payloadsGeneric;
    uint32_t p;

    if ((slotCount == 0u) || (slotCount > 4u))
    {
        slotCount = 1u;
    }

    PRINTF("\r\n#### %s  method=multi-slot  submit=%u-slot-push  mode=%s  (x%u pushes)\r\n\r\n", BENCH_AlgoName(algo),
           (unsigned)slotCount, (mode == kBenchModeSignal) ? "SIGNAL" : "POLL", (unsigned)BENCH_ITERATIONS);
    PRINTF("| bytes | block_us | amortized_us/op | ops/push | MB/s | Ops/s | kpps | ok |\r\n");
    PRINTF("|------:|---------:|----------------:|---------:|-----:|------:|-----:|:--:|\r\n");

    for (p = 0u; p < BENCH_NUM_POINTS; p++)
    {
        uint32_t payload = payloads[p];
        uint32_t i;
        bool ok = true;

        /* Reset the FCE session per point to stay under the FW request quota. */
        BENCH_SessionReset(handle);

        BENCH_FillInput(payload);
        BENCH_SelectMode(handle, mode);

        for (i = 0u; i < BENCH_WARMUP; i++)
        {
            (void)BENCH_RunMulti(handle, algo, payload, slotCount);
        }

        uint32_t t0 = BENCH_CycNow();
        for (i = 0u; i < BENCH_ITERATIONS; i++)
        {
            if (BENCH_RunMulti(handle, algo, payload, slotCount) != kStatus_Success)
            {
                ok = false;
            }
        }
        uint32_t t1 = BENCH_CycNow();

        uint32_t blockNs  = BENCH_CycToNs(t1 - t0);
        uint32_t totalOps = BENCH_ITERATIONS * slotCount;
        uint32_t perNs    = (totalOps != 0u) ? (blockNs / totalOps) : 0u;

        uint64_t totalBytes = (uint64_t)payload * totalOps;
        uint32_t mbps       = (blockNs != 0u) ? (uint32_t)((totalBytes * 1000u) / blockNs) : 0u;
        uint32_t opsPerSec  = (perNs != 0u) ? (uint32_t)(1000000000ull / perNs) : 0u;
        uint32_t kpps       = (perNs != 0u) ? (uint32_t)(1000000ull / perNs) : 0u;

        PRINTF("| %u | %u.%03u | %u.%03u | %u | %u | %u | %u | %s |\r\n", (unsigned)payload,
               (unsigned)(blockNs / 1000u), (unsigned)(blockNs % 1000u), (unsigned)(perNs / 1000u),
               (unsigned)(perNs % 1000u), (unsigned)slotCount, (unsigned)mbps, (unsigned)opsPerSec, (unsigned)kpps,
               ok ? "OK" : "FAIL");
    }
}

/* Combined batch + multi-slot peak-sustained sweep for one (algo, mode): the
 * peak sustained throughput scenario. Each PUSH_REQUEST carries the maximum
 * slotCount=4 ops (hardware-level slot batching: one MU round-trip + one FCE
 * request setup shared across 4 ops) AND BENCH_ITERATIONS such pushes are
 * issued back-to-back (software-level pipelining), stacking both amortization
 * effects. This now applies to every primitive (GCM/CBC/HMAC/HASH) through the
 * (ops,count) base API. The amortized per-op latency and MB/s here represent
 * the practical peak the FCE + MU path can deliver on a saturated stream (the
 * closest number to the pure per-op compute-cost lower bound). Compare against:
 *   - per-request sweep    : single-op single-request latency (no amortization)
 *   - batch sweep          : single-op back-to-back (software amortization only)
 *   - 1-slot slots sweep   : one op per push (hardware amortization baseline) */
#define BENCH_SLOTS_BATCH_SLOTS 4u
static void BENCH_SweepSlotsBatch(v2x_fce_handle_t *handle, bench_algo_t algo, bench_mode_t mode)
{
    const uint32_t *payloads = (algo == kBenchAlgoCbc) ? s_payloadsCbc : s_payloadsGeneric;
    const uint32_t slotCount = BENCH_SLOTS_BATCH_SLOTS;
    uint32_t p;

    PRINTF(
        "\r\n#### %s  method=peak-sustained  submit=batch+%u-slot  mode=%s  (%u pushes x %u slots "
        "back-to-back)\r\n\r\n",
        BENCH_AlgoName(algo), (unsigned)slotCount, (mode == kBenchModeSignal) ? "SIGNAL" : "POLL",
        (unsigned)BENCH_ITERATIONS, (unsigned)slotCount);
    PRINTF("| bytes | block_us | amortized_us/op | ops/block | MB/s | Ops/s | kpps | ok |\r\n");
    PRINTF("|------:|---------:|----------------:|----------:|-----:|------:|-----:|:--:|\r\n");

    for (p = 0u; p < BENCH_NUM_POINTS; p++)
    {
        uint32_t payload = payloads[p];
        uint32_t i;
        bool ok = true;

        /* Reset the FCE session per point to stay under the FW request quota. */
        BENCH_SessionReset(handle);

        BENCH_FillInput(payload);
        BENCH_SelectMode(handle, mode);

        for (i = 0u; i < BENCH_WARMUP; i++)
        {
            (void)BENCH_RunMulti(handle, algo, payload, slotCount);
        }

        uint32_t t0 = BENCH_CycNow();
        for (i = 0u; i < BENCH_ITERATIONS; i++)
        {
            if (BENCH_RunMulti(handle, algo, payload, slotCount) != kStatus_Success)
            {
                ok = false;
            }
        }
        uint32_t t1 = BENCH_CycNow();

        uint32_t blockNs  = BENCH_CycToNs(t1 - t0);
        uint32_t totalOps = BENCH_ITERATIONS * slotCount;
        uint32_t perNs    = (totalOps != 0u) ? (blockNs / totalOps) : 0u;

        uint64_t totalBytes = (uint64_t)payload * totalOps;
        uint32_t mbps       = (blockNs != 0u) ? (uint32_t)((totalBytes * 1000u) / blockNs) : 0u;
        uint32_t opsPerSec  = (perNs != 0u) ? (uint32_t)(1000000000ull / perNs) : 0u;
        uint32_t kpps       = (perNs != 0u) ? (uint32_t)(1000000ull / perNs) : 0u;

        PRINTF("| %u | %u.%03u | %u.%03u | %u | %u | %u | %u | %s |\r\n", (unsigned)payload,
               (unsigned)(blockNs / 1000u), (unsigned)(blockNs % 1000u), (unsigned)(perNs / 1000u),
               (unsigned)(perNs % 1000u), (unsigned)totalOps, (unsigned)mbps, (unsigned)opsPerSec, (unsigned)kpps,
               ok ? "OK" : "FAIL");
    }
}

/* Collect the throughput (MB/s) of one (algo, mode) across all generic payload
 * points into mbpsOut[0..BENCH_NUM_POINTS-1]. Used to build the mermaid chart. */
static void BENCH_MermaidCollect(v2x_fce_handle_t *handle, bench_algo_t algo, bench_mode_t mode, uint32_t *mbpsOut)
{
    const uint32_t *payloads = (algo == kBenchAlgoCbc) ? s_payloadsCbc : s_payloadsGeneric;
    bench_result_t r;
    uint32_t p;

    for (p = 0u; p < BENCH_NUM_POINTS; p++)
    {
        /* Reset the FCE session per point to stay under the FW request quota. */
        BENCH_SessionReset(handle);
        BENCH_Point(handle, algo, mode, payloads[p], &r);
        uint32_t avgNs = BENCH_CycToNs(r.avgCyc);

        mbpsOut[p] = (avgNs != 0u) ? (uint32_t)(((uint64_t)payloads[p] * 1000u) / avgNs) : 0u;
    }
}

/* Emit a Mermaid xychart-beta code block plotting throughput (MB/s) versus
 * payload for four representative algorithms (POLL mode). Markdown itself has no
 * native chart, but a fenced ```mermaid block renders as a real line chart in
 * GitHub / GitLab / VS Code Mermaid preview / mermaid.live - so the board emits
 * a copy-paste-ready chart directly over the UART. */
static void BENCH_MermaidChart(v2x_fce_handle_t *handle)
{
    static uint32_t gcm[BENCH_NUM_POINTS];
    static uint32_t cbc[BENCH_NUM_POINTS];
    static uint32_t hmac[BENCH_NUM_POINTS];
    static uint32_t hash[BENCH_NUM_POINTS];
    uint32_t p;
    uint32_t yMax = 1u;

    /* Ensure representative variants: AES-256 (already reloaded by caller) and
     * SHA-256 for HMAC/HASH. */
    s_shaAlgo       = kV2X_FCE_Sha256;
    s_shaDigestSize = BENCH_SHA256_DIGEST;
    (void)V2X_FCE_LoadHmacPlainKey(handle, BENCH_HMAC_KEY_SLOT, kV2X_FCE_Sha256, s_key, sizeof(s_key));

    BENCH_MermaidCollect(handle, kBenchAlgoGcm, kBenchModePoll, gcm);
    BENCH_MermaidCollect(handle, kBenchAlgoCbc, kBenchModePoll, cbc);
    BENCH_MermaidCollect(handle, kBenchAlgoHmac, kBenchModePoll, hmac);
    BENCH_MermaidCollect(handle, kBenchAlgoHash, kBenchModePoll, hash);

    for (p = 0u; p < BENCH_NUM_POINTS; p++)
    {
        if (gcm[p] > yMax)
            yMax = gcm[p];
        if (cbc[p] > yMax)
            yMax = cbc[p];
        if (hmac[p] > yMax)
            yMax = hmac[p];
        if (hash[p] > yMax)
            yMax = hash[p];
    }
    yMax = ((yMax / 10u) + 1u) * 10u; /* round up to nearest 10 for a clean axis */

    PRINTF("\r\n### Throughput chart (Mermaid, POLL mode)\r\n");
    PRINTF(
        "_Copy the block below into a Mermaid-aware viewer "
        "(GitHub/GitLab/VS Code/mermaid.live) to render._\r\n\r\n");
    PRINTF("Line order: 1=AES-256-GCM 2=AES-256-CBC 3=HMAC-SHA-256 4=HASH-SHA-256\r\n\r\n");
    PRINTF("```mermaid\r\n");
    PRINTF("xychart-beta\r\n");
    PRINTF("    title \"FCE throughput (POLL) - MB/s vs payload bytes\"\r\n");

    PRINTF("    x-axis [");
    for (p = 0u; p < BENCH_NUM_POINTS; p++)
    {
        PRINTF("%u%s", (unsigned)s_payloadsGeneric[p], (p + 1u < BENCH_NUM_POINTS) ? ", " : "");
    }
    PRINTF("]\r\n");

    PRINTF("    y-axis \"MB/s\" 0 --> %u\r\n", (unsigned)yMax);

/* One line per algorithm; mermaid xychart-beta draws them in series order. */
#define BENCH_EMIT_LINE(arr)                                                             \
    do                                                                                   \
    {                                                                                    \
        PRINTF("    line [");                                                            \
        for (p = 0u; p < BENCH_NUM_POINTS; p++)                                          \
        {                                                                                \
            PRINTF("%u%s", (unsigned)(arr)[p], (p + 1u < BENCH_NUM_POINTS) ? ", " : ""); \
        }                                                                                \
        PRINTF("]\r\n");                                                                 \
    } while (0)

    BENCH_EMIT_LINE(gcm);
    BENCH_EMIT_LINE(cbc);
    BENCH_EMIT_LINE(hmac);
    BENCH_EMIT_LINE(hash);
#undef BENCH_EMIT_LINE

    PRINTF("```\r\n");
}

/*!
 * @brief Main function.
 */
int main(void)

{
    v2x_fce_handle_t handle = {0};
    status_t status;

    BOARD_InitHardware();
    BENCH_DwtInit();

    /* Read the real per-core clock (cm33_core1 / cm7_core0 / cm7_core1)
     * instead of trusting a compile-time constant. SystemCoreClockUpdate()
     * recomputes SystemCoreClock from the live clock tree the board layer
     * programmed. Fall back to BENCH_CORE_CLOCK_HZ only if it reads back 0. */
    SystemCoreClockUpdate();
    if (SystemCoreClock != 0u)
    {
        s_benchCoreClockHz = SystemCoreClock;
    }

    PRINTF("\r\n*************** EdgeLock V2X/FCE performance benchmark (MCUX-90943) ****\r\n\r\n");
    PRINTF("Runtime environment: BARE-METAL (no RTOS/scheduler; DWT-timed micro-benchmark)\r\n");
    PRINTF("Core clock (from SystemCoreClock): %u Hz\r\n", (unsigned)s_benchCoreClockHz);

    PRINTF("Iterations/point: %u  (warmup %u)\r\n", (unsigned)BENCH_ITERATIONS, (unsigned)BENCH_WARMUP);
    PRINTF("Payload: swept 16..1024 bytes (%u points) per algo (CBC uses 16-byte multiples).\r\n",
           (unsigned)(sizeof(s_payloadsGeneric) / sizeof(s_payloadsGeneric[0])));
    PRINTF("Payload content: filled at runtime with a deterministic pattern\r\n");
    PRINTF("(s_in[i] = i*7 + 0x11), regenerated per payload size. It is reproducible,\r\n");
    PRINTF("not truly random. AES/SHA are data-oblivious (execution time depends only on\r\n");
    PRINTF("byte count, not content), so the pattern choice does not affect the timings.\r\n\r\n");

    PRINTF("Each result table is tagged with method= and submit= so the submission\r\n");
    PRINTF("model is unambiguous:\r\n");
    PRINTF("  method=one-shot       : one op per synchronous request, timed individually\r\n");
    PRINTF("                          (single-request latency, no amortization)\r\n");
    PRINTF("  method=batch-stream   : one op per request, N requests back-to-back, block-timed\r\n");
    PRINTF("                          (software pipelining amortization)\r\n");
    PRINTF("  method=multi-slot     : 1..4 ops per PUSH_REQUEST (hardware MU slot batching)\r\n");
    PRINTF("  method=peak-sustained : batch + max slots stacked (practical peak)\r\n");
    PRINTF("Throughput metrics (unified across all tables):\r\n");
    PRINTF("  MB/s  = payload bytes / second (data rate)\r\n");
    PRINTF("  Ops/s = ops / second           (per-op rate)\r\n");
    PRINTF("  kpps  = thousand ops / second  (= Ops/s / 1000; kilo-packets-per-second)\r\n\r\n");

    /****************** Bring up V2X firmware *****************/
    PRINTF("****************** Bring up V2X firmware *****************\r\n");
    {
        uint32_t v2xState = 0u;
        uint32_t poll;

        status = ELE_BaseAPI_GetV2xFwState(ELE_MU, &v2xState);
        if ((status == kStatus_Success) && ELE_BaseAPI_IsV2xFwRunning(v2xState))
        {
            /* Already running. */
        }
        else
        {
            status = ELE_BaseAPI_V2xFwAuthenticate(ELE_MU, EXAMPLE_V2X_FW_IMG_DDR_ADDR);
            if (status != kStatus_Success)
            {
                PRINTF("Failed to authenticate V2X firmware (0x%x).\r\n", (unsigned int)status);
                goto error;
            }
            for (poll = 0u; poll < V2X_FW_STATE_RETRIES; poll++)
            {
                status = ELE_BaseAPI_GetV2xFwState(ELE_MU, &v2xState);
                if (status != kStatus_Success)
                {
                    PRINTF("Failed to read V2X firmware state (0x%x).\r\n", (unsigned int)status);
                    goto error;
                }
                if (ELE_BaseAPI_IsV2xFwRunning(v2xState))
                {
                    break;
                }
            }
            if (!ELE_BaseAPI_IsV2xFwRunning(v2xState))
            {
                status = kStatus_Timeout;
                PRINTF("Timed out waiting for V2X firmware to start.\r\n");
                goto error;
            }
        }
    }
    PRINTF("V2X firmware is running.\r\n\r\n");

    /****************** Open the V2X/FCE service ********************/
    status = V2X_FCE_Ping(V2X_FCE_MU);
    if (status != kStatus_Success)
    {
        PRINTF("Ping V2X/FCE service failed (status=%d).\r\n", (int)status);
    }

    status = V2X_FCE_Open(V2X_FCE_MU, &handle);
    if (status != kStatus_Success)
    {
        PRINTF("Open V2X/FCE service failed.\r\n");
        goto error;
    }
    PRINTF("V2X/FCE service open.\r\n");

    /****************** Load AES-256 / HMAC-SHA-256 keys ***********/
    status = V2X_FCE_LoadAesPlainKey(&handle, BENCH_AES_KEY_SLOT, s_key, sizeof(s_key));
    if (status != kStatus_Success)
    {
        PRINTF("Load AES plain key failed.\r\n");
        goto close;
    }
    status = V2X_FCE_LoadHmacPlainKey(&handle, BENCH_HMAC_KEY_SLOT, kV2X_FCE_Sha256, s_key, sizeof(s_key));
    if (status != kStatus_Success)
    {
        PRINTF("Load HMAC plain key failed.\r\n");
        goto close;
    }
    PRINTF("AES-256 and HMAC-SHA-256 keys loaded into slot %u.\r\n", (unsigned)BENCH_AES_KEY_SLOT);

    /****************** Run the benchmark sweeps ******************/

    {
        static const v2x_fce_sha_algo_t shaVariants[] = {kV2X_FCE_Sha256, kV2X_FCE_Sha384, kV2X_FCE_Sha512};
        static const uint32_t aesKeySizes[]           = {16u, 24u, 32u};
        uint32_t v;

        /* AES-GCM / AES-CBC over AES-128 / AES-192 / AES-256 (the FCE AES key
         * length is bound at load time, so reload slot 0 per key size). */
        for (v = 0u; v < 3u; v++)
        {
            uint32_t keyBytes = aesKeySizes[v];

            status = V2X_FCE_LoadAesPlainKey(&handle, BENCH_AES_KEY_SLOT, s_key, keyBytes);
            if (status != kStatus_Success)
            {
                PRINTF("Load AES-%u key failed.\r\n", (unsigned)(keyBytes * 8u));
                goto close;
            }
            s_aesKeyBytes = keyBytes;

            PRINTF("\r\n### AES-%u-GCM / AES-%u-CBC\r\n", (unsigned)(keyBytes * 8u), (unsigned)(keyBytes * 8u));
            for (int m = 0; m < 2; m++)
            {
                bench_mode_t mode = (m == 0) ? kBenchModeSignal : kBenchModePoll;
                BENCH_Sweep(&handle, kBenchAlgoGcm, mode);
                BENCH_Sweep(&handle, kBenchAlgoCbc, mode);
            }
        }

        /* HMAC / HASH over SHA-256 / SHA-384 / SHA-512 (reload the HMAC key per
         * SHA variant so the key is bound to the same algorithm). */
        for (v = 0u; v < 3u; v++)
        {
            s_shaAlgo       = shaVariants[v];
            s_shaDigestSize = BENCH_ShaDigestSize(s_shaAlgo);

            status = V2X_FCE_LoadHmacPlainKey(&handle, BENCH_HMAC_KEY_SLOT, s_shaAlgo, s_key, sizeof(s_key));
            if (status != kStatus_Success)
            {
                PRINTF("Load HMAC-%s key failed.\r\n", BENCH_ShaName(s_shaAlgo));
                goto close;
            }

            PRINTF("\r\n### HMAC-%s / HASH-%s\r\n", BENCH_ShaName(s_shaAlgo), BENCH_ShaName(s_shaAlgo));
            for (int m = 0; m < 2; m++)
            {
                bench_mode_t mode = (m == 0) ? kBenchModeSignal : kBenchModePoll;
                BENCH_Sweep(&handle, kBenchAlgoHmac, mode);
                BENCH_Sweep(&handle, kBenchAlgoHash, mode);
            }
        }

        /* Restore AES-256 for the subsequent batch / multi-slot GCM sweeps. */
        (void)V2X_FCE_LoadAesPlainKey(&handle, BENCH_AES_KEY_SLOT, s_key, 32u);
        s_aesKeyBytes = 32u;
    }

    /****************** Batched (back-to-back burst) sweeps ******************/
    PRINTF("\r\n### Batch (sustained back-to-back) throughput\r\n");
    for (int m = 0; m < 2; m++)
    {
        bench_mode_t mode = (m == 0) ? kBenchModeSignal : kBenchModePoll;

        BENCH_SweepBatch(&handle, kBenchAlgoGcm, mode);
        BENCH_SweepBatch(&handle, kBenchAlgoCbc, mode);
        BENCH_SweepBatch(&handle, kBenchAlgoHmac, mode);
        BENCH_SweepBatch(&handle, kBenchAlgoHash, mode);
    }

    /****************** Multi-slot-per-PUSH sweeps (all algos, 1/2/3/4 slots) */
    PRINTF("\r\n### Multi-slot push (1..4 MU slots per PUSH_REQUEST)\r\n");
    {
        static const bench_algo_t multiAlgos[] = {kBenchAlgoGcm, kBenchAlgoCbc, kBenchAlgoHmac, kBenchAlgoHash};
        for (int m = 0; m < 2; m++)
        {
            bench_mode_t mode = (m == 0) ? kBenchModeSignal : kBenchModePoll;

            for (uint32_t a = 0u; a < 4u; a++)
            {
                for (uint32_t slots = 1u; slots <= 4u; slots++)
                {
                    BENCH_SweepSlots(&handle, multiAlgos[a], mode, slots);
                }
            }
        }
    }

    /****************** Combined batch + multi-slot peak sweeps **************/
    PRINTF("\r\n### Peak sustained throughput (batch + 4-slot: back-to-back full-slot pushes)\r\n");
    for (int m = 0; m < 2; m++)
    {
        bench_mode_t mode = (m == 0) ? kBenchModeSignal : kBenchModePoll;

        BENCH_SweepSlotsBatch(&handle, kBenchAlgoGcm, mode);
        BENCH_SweepSlotsBatch(&handle, kBenchAlgoCbc, mode);
        BENCH_SweepSlotsBatch(&handle, kBenchAlgoHmac, mode);
        BENCH_SweepSlotsBatch(&handle, kBenchAlgoHash, mode);
    }

    /****************** Reverse direction (decrypt / verify) sweeps *********
     * Receive/verify side (IEC 62351-6 authenticator path): AES-GCM/CBC decrypt
     * and HMAC verify. HASH has no reverse form and is omitted. AES-256 and the
     * SHA-256 HMAC key are already loaded from the sweeps above. */
    PRINTF("\r\n### Reverse direction (decrypt / verify)\r\n");
    s_benchDir = kBenchDirReverse;
    for (int m = 0; m < 2; m++)
    {
        bench_mode_t mode = (m == 0) ? kBenchModeSignal : kBenchModePoll;
        BENCH_Sweep(&handle, kBenchAlgoGcm, mode);
        BENCH_Sweep(&handle, kBenchAlgoCbc, mode);
        BENCH_Sweep(&handle, kBenchAlgoHmac, mode);
    }
    s_benchDir = kBenchDirForward;

    /****************** Board-emitted Mermaid throughput chart ***************/
    BENCH_MermaidChart(&handle);

    PRINTF("\r\nNOTE (bottleneck): at small payloads the per-request latency is dominated\r\n");

    PRINTF("by the fixed MU round-trip + FCE request setup (see 'setup latency' rows);\r\n");
    PRINTF("at large payloads the DMA + LTC-AES engine throughput dominates. SIGNAL vs\r\n");
    PRINTF("POLL differ mainly in the small-payload setup component (IRQ entry/exit vs\r\n");
    PRINTF("busy-poll). Compare the two mode blocks per algorithm to identify the cross-\r\n");
    PRINTF("over point.\r\n");
    PRINTF("\r\n_All figures above are BARE-METAL results (no RTOS/scheduler, no tick IRQ);\r\n");
    PRINTF("they represent the FCE micro-benchmark lower bound. RTOS/system-level numbers\r\n");
    PRINTF("would add scheduler/tick overhead and are reported separately._\r\n");

    status = kStatus_Success;

close:
    if (V2X_FCE_Close(&handle) != kStatus_Success)
    {
        PRINTF("Close V2X/FCE service failed.\r\n");
    }

error:
    if (status == kStatus_Success)
    {
        PRINTF("\r\n*************** V2X/FCE benchmark END **************\r\n");
    }
    else
    {
        PRINTF("\r\n*************** V2X/FCE benchmark FAILED ***********\r\n");
    }

    while (1)
    {
    }
}
