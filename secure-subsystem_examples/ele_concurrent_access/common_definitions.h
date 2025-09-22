/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_sema42.h"

const uint8_t input[] =
    "Be that word our sign of parting, bird or fiend! I shrieked upstarting"
    "Get thee back into the tempest and the Nights Plutonian shore!"
    "Leave no black plume as a token of that lie thy soul hath spoken!"
    "Leave my loneliness unbroken! quit the bust above my door!"
    "Take thy beak from out my heart, and take thy form from off my door!"
    "Quoth the raven, Nevermore.  ";
const size_t input_length = sizeof(input) - 1;

const uint8_t sha2_256_expected[] = {0x63, 0x76, 0xea, 0xcc, 0xc9, 0xa2, 0xc0, 0x43, 0xf4, 0xfb, 0x01,
                                     0x34, 0x69, 0xb3, 0x0c, 0xf5, 0x28, 0x63, 0x5c, 0xfa, 0xa5, 0x65,
                                     0x60, 0xef, 0x59, 0x7b, 0xd9, 0x1c, 0xac, 0xaa, 0x31, 0xf7};

/**
 * Must be non-zero for repeated initialization calls to NOT fail.
 */
#define ELE_SHARED_SESSION_ID  (1u)
#define ELE_MAX_SUBSYSTEM_WAIT (0xFFFFFFFFu)
#define ELE_SUBSYSTEM          (kType_SSS_Ele200)

static sscp_context_t sscpContext    = {0};
static sss_sscp_session_t sssSession = {0};
static sss_sscp_key_store_t keyStore = {0};

#define NUMBER_OF_TASKS (1 + 24) /* 1 init task + rest generic crypto tasks */

/**
 * @def DO_TASK(task_call, task_retval_var, task_completed_var)
 *
 * Wraps a function call passed as @ref task_call in a mutex and saves the
 * return value and a boolean task completion flag into @ref task_retval_var
 * and @ref task_completed_var respectively.
 */
#define DO_TASK(task_call, task_retval_var, task_completed_var) \
    do                                                          \
    {                                                           \
        (void)mutex_lock();                                     \
        (task_retval_var)    = (uint32_t)(task_call);           \
        (task_completed_var) = true;                            \
        (void)mutex_unlock();                                   \
    } while (0)

/**
 * @def DOMAIN_ID
 * Select domain ID based on current core.
 * Defined COREx_DOMAIN_ID value is present in app.h of the given core.
 */
#if (defined(IS_RADIO_CORE) && (IS_RADIO_CORE > 0))
#define DOMAIN_ID CORE1_DOMAIN_ID
#else
#define DOMAIN_ID CORE0_DOMAIN_ID
#endif

typedef struct _task_info_t
{
    bool task_completed;
    uint32_t task_retval;
} task_info_t;

typedef struct
{
    bool init_done;
    uint8_t init_done_by_core;
    task_info_t core0_tasks[NUMBER_OF_TASKS];
    task_info_t core1_tasks[NUMBER_OF_TASKS];
} ele_multicore_ctx_t;

ele_multicore_ctx_t *g_ctx_ptr = NULL;

static inline int mutex_lock(void)
{
    if (SEMA42_Lock(APP_SEMA42, APP_SEMA42_GATE, DOMAIN_ID) != kStatus_Success)
    {
        return -1;
    }

    return 0;
}

static inline int mutex_unlock(void)
{
    do
    {
        SEMA42_Unlock(APP_SEMA42, APP_SEMA42_GATE);
    } while (SEMA42_GetGateStatus(APP_SEMA42, APP_SEMA42_GATE) != kSEMA42_Unlocked);

    return 0;
}

/*******************************************************************************
 * Code
 ******************************************************************************/
/**
 * The below functions are common for both cores, but in some of them we may
 * need to do some additional steps on the NBU side (e.g. get the data buffers
 * to S200-accessible memory (variables noted by the `_sacc` suffix).
 */

/**
 * This is a common init routine. We don't really care who initializes first,
 * we just require the party to first reach the initialization to let the
 * other one know that the S200 HW init has already happened. In that case
 * the second party only needs to set up their local SW contexts.
 */
static status_t init_s200()
{
    status_t status = kStatus_Fail;
    do
    {
        status = ELEMU_mu_wait_for_ready(ELEMUA, ELE_MAX_SUBSYSTEM_WAIT);
        if (status != kStatus_Success)
        {
            break;
        }

        status = sscp_mu_init(&sscpContext, (ELEMU_Type *)(uintptr_t)ELEMUA);
        if (status != kStatus_SSCP_Success)
        {
            break;
        }

        status = sss_sscp_open_session(&sssSession, ELE_SHARED_SESSION_ID, ELE_SUBSYSTEM, &sscpContext);
        if (status != kStatus_SSS_Success)
        {
            break;
        }

        status = sss_sscp_key_store_init(&keyStore, &sssSession);
        if (status != kStatus_SSS_Success)
        {
            break;
        }

        status = kStatus_Success;
    } while (0);

    if (kStatus_Success == status)
    {
        /* For the purpose of this example, we'll also store which core
         * managed to reach HW initialization first. We'll use the SEMA42
         * domain IDs to differentiate the given core.
         */
        if (false == g_ctx_ptr->init_done)
        {
            g_ctx_ptr->init_done_by_core = DOMAIN_ID;
        }

        g_ctx_ptr->init_done = true;
        status               = kStatus_Success;
    }
    else
    {
        status = kStatus_Fail;
    }

    return status;
}

/**
 * Hash @ref input with SHA2_256 and compare with expected digest.
 */
status_t sha2_256(void)
{
    status_t status       = kStatus_Fail;
    sss_sscp_digest_t ctx = {0};
    uint8_t digest[32]    = {0u};
    size_t digest_length  = sizeof(digest);

#if (defined(IS_RADIO_CORE) && (IS_RADIO_CORE > 0))
    extern uint8_t nbu_s200_workarea[];
    uint8_t *input_sacc  = (uint8_t *)nbu_s200_workarea;
    uint8_t *digest_sacc = (uint8_t *)input_sacc + input_length + 1u;
    (void)memcpy(input_sacc, input, input_length);
#else
    uint8_t *input_sacc  = (uint8_t *)input;
    uint8_t *digest_sacc = digest;
#endif

    do
    {
        /* Init the digest context */
        status = sss_sscp_digest_context_init(&ctx, &sssSession, kAlgorithm_SSS_SHA256, kMode_SSS_Digest);
        if (status != kStatus_SSS_Success)
        {
            break;
        }

        /* Generate SHA256 digest with one-go operation */
        status = sss_sscp_digest_one_go(&ctx, input_sacc, input_length, digest_sacc, &digest_length);
        if (status != kStatus_SSS_Success)
        {
            break;
        }
#if (defined(IS_RADIO_CORE) && (IS_RADIO_CORE > 0))
        /* If we are NBU, we want to get the result from our S200-accessible
         * memory into the actual output buffer.
         */
        (void)memcpy((void *)digest, digest_sacc, digest_length);
#endif

        /* Check if digest matches expected value */
        if (memcmp(sha2_256_expected, digest, digest_length) != 0)
        {
            status = kStatus_Fail;
            break;
        }

        /* Free the digest context */
        status = sss_sscp_digest_context_free(&ctx);
        if (status != kStatus_SSS_Success)
        {
            break;
        }

        status = kStatus_Success;
    } while (0);

    return status;
}

/**
 * Encrypt @ref input with AES-128 CBC, then decrypt and compare the plaintexts.
 */
status_t aes_128(void)
{
    status_t status                   = kStatus_Fail;
    sss_sscp_symmetric_t ctx          = {0};
    sss_sscp_object_t sssKey          = {0};
    uint8_t ciphertext[sizeof(input)] = {0u};
    size_t ciphertext_length          = input_length;
    uint8_t plaintext[sizeof(input)]  = {0u};
    uint8_t key[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
    size_t key_length = sizeof(key);
    uint8_t iv[16]   = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
    size_t iv_length = sizeof(iv);

#if (defined(IS_RADIO_CORE) && (IS_RADIO_CORE > 0))
    extern uint8_t nbu_s200_workarea[];
    uint8_t *input_sacc      = (uint8_t *)nbu_s200_workarea;
    uint8_t *ciphertext_sacc = (uint8_t *)input_sacc + input_length + 1u;
    uint8_t *plaintext_sacc  = (uint8_t *)ciphertext_sacc + ciphertext_length + 1u;
    uint8_t *key_sacc        = (uint8_t *)plaintext_sacc + input_length + 1u;
    uint8_t *iv_sacc         = (uint8_t *)key_sacc + key_length + 1u;
    (void)memcpy(input_sacc, input, input_length);
    (void)memcpy(key_sacc, key, key_length);
    (void)memcpy(iv_sacc, iv, iv_length);
#else
    uint8_t *input_sacc      = (uint8_t *)input;
    uint8_t *ciphertext_sacc = ciphertext;
    uint8_t *plaintext_sacc  = plaintext;
    uint8_t *key_sacc        = key;
    uint8_t *iv_sacc         = iv;
#endif

    do
    {
        /* Init key object  */
        status = sss_sscp_key_object_init(&sssKey, &keyStore);
        if (status != kStatus_SSS_Success)
        {
            break;
        }

        /* Allocate keystore handle */
        status = sss_sscp_key_object_allocate_handle(&sssKey, 0u, kSSS_KeyPart_Default, kSSS_CipherType_AES, 16u,
                                                     kSSS_KeyProp_CryptoAlgo_AES);
        if (status != kStatus_SSS_Success)
        {
            (void)sss_sscp_key_object_free(&sssKey, kSSS_keyObjFree_KeysStoreDefragment);
            break;
        }

        /* Set key into key object*/
        status = sss_sscp_key_store_set_key(&keyStore, &sssKey, key_sacc, key_length, (key_length * 8U),
                                            kSSS_KeyPart_Default);
        if (status != kStatus_SSS_Success)
        {
            (void)sss_sscp_key_object_free(&sssKey, kSSS_keyObjFree_KeysStoreDefragment);
            break;
        }

        /* Init symmetric context */
        status = sss_sscp_symmetric_context_init(&ctx, &sssSession, &sssKey, kAlgorithm_SSS_AES_CBC, kMode_SSS_Encrypt);
        if (status != kStatus_SSS_Success)
        {
            (void)sss_sscp_key_object_free(&sssKey, kSSS_keyObjFree_KeysStoreDefragment);
            break;
        }

        /* Encrypt */
        status = sss_sscp_cipher_one_go(&ctx, iv_sacc, iv_length, input_sacc, ciphertext_sacc, input_length);
        if (status != kStatus_SSS_Success)
        {
            (void)sss_sscp_symmetric_context_free(&ctx);
            (void)sss_sscp_key_object_free(&sssKey, kSSS_keyObjFree_KeysStoreDefragment);
            break;
        }

        /* Close AES context*/
        status = sss_sscp_symmetric_context_free(&ctx);

#if (defined(IS_RADIO_CORE) && (IS_RADIO_CORE > 0))
        /* If we are NBU, we want to get the result from our S200-accessible
         * memory into the actual output buffer.
         */
        (void)memcpy((void *)ciphertext, ciphertext_sacc, ciphertext_length);
#endif

        /* Init symmetric context */
        status = sss_sscp_symmetric_context_init(&ctx, &sssSession, &sssKey, kAlgorithm_SSS_AES_CBC, kMode_SSS_Decrypt);
        if (status != kStatus_SSS_Success)
        {
            (void)sss_sscp_key_object_free(&sssKey, kSSS_keyObjFree_KeysStoreDefragment);
            break;
        }

        /* Decrypt */
        status = sss_sscp_cipher_one_go(&ctx, iv_sacc, iv_length, ciphertext_sacc, plaintext_sacc, ciphertext_length);
        if (status != kStatus_SSS_Success)
        {
            (void)sss_sscp_symmetric_context_free(&ctx);
            (void)sss_sscp_key_object_free(&sssKey, kSSS_keyObjFree_KeysStoreDefragment);
            break;
        }

#if (defined(IS_RADIO_CORE) && (IS_RADIO_CORE > 0))
        /* If we are NBU, we want to get the result from our S200-accessible
         * memory into the actual output buffer.
         */
        (void)memcpy((void *)plaintext, plaintext_sacc, ciphertext_length);
#endif

        /* Cleanup Close all context, objects and sessions which were opened before */
        /* Close AES context*/
        (void)sss_sscp_symmetric_context_free(&ctx);
        status = sss_sscp_key_object_free(&sssKey, kSSS_keyObjFree_KeysStoreDefragment);
    } while (0);

    if (status == kStatus_SSS_Success)
    {
        if (memcmp(input, plaintext, input_length) == 0)
        {
            status = kStatus_Success;
        }
        else
        {
            status = kStatus_Fail;
        }
    }

    return status;
}
