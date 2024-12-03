/*
 *  Benchmark demonstration program
 *
 *  Copyright The Mbed TLS Contributors
 *  SPDX-License-Identifier: Apache-2.0
 *  Copyright 2024 NXP
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#if defined(MBEDTLS_THREADING_C)
#include "mbedtls/threading.h"
#endif

/* Include psa-crypto-driver interface file*/
#if defined(PSA_CRYPTO_DRIVER_ELS_PKC)
#include "els_pkc_driver.h"
#endif /* PSA_CRYPTO_DRIVER_ELS_PKC */

#include "app.h"

#if defined(MBEDTLS_PLATFORM_C)
#if defined(FREESCALE_KSDK_BM)

#include "fsl_debug_console.h"
#endif
#include "mbedtls/version.h"
#define return_SUCCESS 0
#define return_FAILURE 1
#include "mbedtls/platform.h"
#else /* defined(FREESCALE_KSDK_BM)*/

#include <stdio.h>
#include <stdlib.h>
#define mbedtls_printf     printf
#define mbedtls_snprintf   snprintf
#define mbedtls_exit       exit
#define return_SUCCESS EXIT_SUCCESS
#define return_FAILURE EXIT_FAILURE
#endif

#if defined(MBEDTLS_MEMORY_BUFFER_ALLOC_C)
#include "mbedtls/memory_buffer_alloc.h"
#endif

#if ( !defined(MBEDTLS_THREADING_C) || !defined(MBEDTLS_THREADING_ALT) )
#error "SDK example aims to test mbedTLS threading support. So MBEDTLS_THREADING_C and MBEDTLS_THREADING_ALT macros must be defined together."
#endif

#if ( !defined(MBEDTLS_PLATFORM_MEMORY) )
#error "SDK example aims to test mbedTLS threading support. So MBEDTLS_PLATFORM_MEMORY macro must be defined to enable thread safe implementations of memory allocation functions"
#endif

#if ( !defined(PSA_CRYPTO_DRIVER_THREAD_EN) )
#error "SDK example aims to test mbedTLS threading support with psa-crypto-driver. So PSA_CRYPTO_DRIVER_THREAD_EN macro must be defined to enable thread safe implementations in psa-crypto-driver"
#endif
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define TASK_PRIORITY (configMAX_PRIORITIES - 1)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void thread_one_func(void *param);
static void thread_two_func(void *param);
mbedtls_threading_mutex_t alloc_mutex;
/*******************************************************************************
 * Variables
 ******************************************************************************/
const int TASK_MAIN_STACK_SIZE = 1500;
portSTACK_TYPE *task_main_stack = NULL;
TaskHandle_t task_main_task_handler;
/*******************************************************************************
 * Code
 ******************************************************************************/

static void *buffer_alloc_calloc_mutexed_custom( size_t n, size_t size )
{
    void *buf = NULL;
    if( mbedtls_mutex_lock( &alloc_mutex ) != 0 )
        return( NULL );
    buf = calloc( n, size );
    if( mbedtls_mutex_unlock( &alloc_mutex ) != 0 )
    {
        mbedtls_free(buf);
        return( NULL );
    }
    return( buf );
}

int free_count = 0;
static void buffer_alloc_free_mutexed_custom( void *ptr )
{
    if( mbedtls_mutex_lock( &alloc_mutex ) != 0 )
        return;
    free( ptr );
    if( mbedtls_mutex_unlock( &alloc_mutex ) != 0 )
        return;

}

/*******************************************************************************
 * MACROS
 ******************************************************************************/
#define ASSERT(predicate)                                                                          \
	do {                                                                                       \
		if (!(predicate)) {                                                                \
			PRINTF("\tassertion failed at %s:%d - '%s'\r\n", __FILE__, __LINE__,       \
			       #predicate);                                                        \
			goto exit;                                                                 \
		}                                                                                  \
	} while (0)

#define ASSERT_STATUS(actual, expected)                                                            \
	do {                                                                                       \
		if ((actual) != (expected)) {                                                      \
			PRINTF("\tassertion failed at %s:%d - "                                    \
			       "actual:%d expected:%d\r\n",                                        \
			       __FILE__, __LINE__, (psa_status_t)actual, (psa_status_t)expected);  \
			goto exit;                                                                 \
		}                                                                                  \
	} while (0)


/*******************************************************************************
 * Definitions
 ******************************************************************************/

static psa_status_t cipher_operation(psa_cipher_operation_t *operation, const uint8_t *input,
				     size_t input_size, size_t part_size, uint8_t *output,
				     size_t output_size, size_t *output_len)
{
	psa_status_t status;
	size_t bytes_to_write = 0, bytes_written = 0, len = 0;

	*output_len = 0;
	while (bytes_written != input_size) {
		bytes_to_write =
			(input_size - bytes_written > part_size ? part_size
								: input_size - bytes_written);

		status = psa_cipher_update(operation, input + bytes_written, bytes_to_write,
					   output + *output_len, output_size - *output_len, &len);
		ASSERT_STATUS(status, PSA_SUCCESS);

		bytes_written += bytes_to_write;
		*output_len += len;
	}

	status =
		psa_cipher_finish(operation, output + *output_len, output_size - *output_len, &len);
	ASSERT_STATUS(status, PSA_SUCCESS);
	*output_len += len;

exit:
	return status;
}

static psa_status_t cipher_encrypt(psa_key_id_t key, psa_algorithm_t alg, uint8_t *iv,
				   size_t iv_size, const uint8_t *input, size_t input_size,
				   size_t part_size, uint8_t *output, size_t output_size,
				   size_t *output_len)
{
	psa_status_t status;
	psa_cipher_operation_t operation = PSA_CIPHER_OPERATION_INIT;
	size_t iv_len = 0;

	memset(&operation, 0, sizeof(operation));
	status = psa_cipher_encrypt_setup(&operation, key, alg);
	ASSERT_STATUS(status, PSA_SUCCESS);

	status = psa_cipher_generate_iv(&operation, iv, iv_size, &iv_len);
	ASSERT_STATUS(status, PSA_SUCCESS);

	status = cipher_operation(&operation, input, input_size, part_size, output, output_size,
				  output_len);
	ASSERT_STATUS(status, PSA_SUCCESS);

exit:
	psa_cipher_abort(&operation);
	return status;
}

static psa_status_t cipher_decrypt(psa_key_id_t key, psa_algorithm_t alg, const uint8_t *iv,
				   size_t iv_size, const uint8_t *input, size_t input_size,
				   size_t part_size, uint8_t *output, size_t output_size,
				   size_t *output_len)
{
	psa_status_t status;
	psa_cipher_operation_t operation = PSA_CIPHER_OPERATION_INIT;

	memset(&operation, 0, sizeof(operation));
	status = psa_cipher_decrypt_setup(&operation, key, alg);
	ASSERT_STATUS(status, PSA_SUCCESS);

	status = psa_cipher_set_iv(&operation, iv, iv_size);
	ASSERT_STATUS(status, PSA_SUCCESS);

	status = cipher_operation(&operation, input, input_size, part_size, output, output_size,
				  output_len);
	ASSERT_STATUS(status, PSA_SUCCESS);

exit:
	psa_cipher_abort(&operation);
	return status;
}

static psa_status_t cipher_example_encrypt_decrypt_aes_cbc_nopad_1_block(void)
{
	enum {
		block_size = PSA_BLOCK_CIPHER_BLOCK_LENGTH(PSA_KEY_TYPE_AES),
		key_bits = 256,
		part_size = block_size,
	};
	const psa_algorithm_t alg = PSA_ALG_CBC_NO_PADDING;

	psa_status_t status;
	psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
	psa_key_id_t key = 0;
	size_t output_len = 0;
	uint8_t iv[block_size];
	uint8_t input[block_size];
	uint8_t encrypt[block_size];
	uint8_t decrypt[block_size];

	status = psa_generate_random(input, sizeof(input));
	ASSERT_STATUS(status, PSA_SUCCESS);

	psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT);
	psa_set_key_algorithm(&attributes, alg);
	psa_set_key_type(&attributes, PSA_KEY_TYPE_AES);
	psa_set_key_bits(&attributes, key_bits);

	status = psa_generate_key(&attributes, &key);
	ASSERT_STATUS(status, PSA_SUCCESS);

	status = cipher_encrypt(key, alg, iv, sizeof(iv), input, sizeof(input), part_size, encrypt,
				sizeof(encrypt), &output_len);
	ASSERT_STATUS(status, PSA_SUCCESS);

	status = cipher_decrypt(key, alg, iv, sizeof(iv), encrypt, output_len, part_size, decrypt,
				sizeof(decrypt), &output_len);
	ASSERT_STATUS(status, PSA_SUCCESS);

	status = memcmp(input, decrypt, sizeof(input));
	ASSERT_STATUS(status, PSA_SUCCESS);

exit:
	psa_destroy_key(key);
	return status;
}

static psa_status_t cipher_example_encrypt_decrypt_aes_cbc_pkcs7_multi(void)
{
	enum {
		block_size = PSA_BLOCK_CIPHER_BLOCK_LENGTH(PSA_KEY_TYPE_AES),
		key_bits = 256,
		input_size = 100,
		part_size = 10,
	};

	const psa_algorithm_t alg = PSA_ALG_CBC_PKCS7;

	psa_status_t status;
	psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
	psa_key_id_t key = 0;
	size_t output_len = 0;
	uint8_t iv[block_size], input[input_size], encrypt[input_size + block_size],
		decrypt[input_size + block_size];

	status = psa_generate_random(input, sizeof(input));
	ASSERT_STATUS(status, PSA_SUCCESS);

	psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT);
	psa_set_key_algorithm(&attributes, alg);
	psa_set_key_type(&attributes, PSA_KEY_TYPE_AES);
	psa_set_key_bits(&attributes, key_bits);

	status = psa_generate_key(&attributes, &key);
	ASSERT_STATUS(status, PSA_SUCCESS);

	status = cipher_encrypt(key, alg, iv, sizeof(iv), input, sizeof(input), part_size, encrypt,
				sizeof(encrypt), &output_len);
	ASSERT_STATUS(status, PSA_SUCCESS);

	status = cipher_decrypt(key, alg, iv, sizeof(iv), encrypt, output_len, part_size, decrypt,
				sizeof(decrypt), &output_len);
	ASSERT_STATUS(status, PSA_SUCCESS);

	status = memcmp(input, decrypt, sizeof(input));
	ASSERT_STATUS(status, PSA_SUCCESS);

exit:
	psa_destroy_key(key);
	return status;
}

static psa_status_t cipher_example_encrypt_decrypt_aes_ctr_multi(void)
{
	enum {
		block_size = PSA_BLOCK_CIPHER_BLOCK_LENGTH(PSA_KEY_TYPE_AES),
		key_bits = 256,
		input_size = 100,
		part_size = 10,
	};
	const psa_algorithm_t alg = PSA_ALG_CTR;

	psa_status_t status = PSA_SUCCESS;
	psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;
	psa_key_id_t key = 0;
	size_t output_len = 0;
	uint8_t iv[block_size], input[input_size], encrypt[input_size], decrypt[input_size];

	status = psa_generate_random(input, sizeof(input));
	ASSERT_STATUS(status, PSA_SUCCESS);

	psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT);
	psa_set_key_algorithm(&attributes, alg);
	psa_set_key_type(&attributes, PSA_KEY_TYPE_AES);
	psa_set_key_bits(&attributes, key_bits);

	status = psa_generate_key(&attributes, &key);
	ASSERT_STATUS(status, PSA_SUCCESS);

	status = cipher_encrypt(key, alg, iv, sizeof(iv), input, sizeof(input), part_size, encrypt,
				sizeof(encrypt), &output_len);
	ASSERT_STATUS(status, PSA_SUCCESS);

	status = cipher_decrypt(key, alg, iv, sizeof(iv), encrypt, output_len, part_size, decrypt,
				sizeof(decrypt), &output_len);
	ASSERT_STATUS(status, PSA_SUCCESS);

	status = memcmp(input, decrypt, sizeof(input));
	ASSERT_STATUS(status, PSA_SUCCESS);

exit:
	psa_destroy_key(key);
	return status;
}

static psa_status_t cipher_examples(void)
{
	psa_status_t status;

	status = cipher_example_encrypt_decrypt_aes_cbc_nopad_1_block();
	if (status != PSA_SUCCESS) {
		return status;
	}

	status = cipher_example_encrypt_decrypt_aes_cbc_pkcs7_multi();
	if (status != PSA_SUCCESS) {
		return status;
	}

	status = cipher_example_encrypt_decrypt_aes_ctr_multi();
	if (status != PSA_SUCCESS) {
		return status;
	}
        return status;
}


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
int main(void)
{
    TaskHandle_t task_one;
    TaskHandle_t task_two;
    BaseType_t result = 0;

#if defined(FREESCALE_KSDK_BM)

    /* HW init */
    BOARD_InitHardware();

    /* Initialize the mbedtls mutex and call mbedtls_threading_set_alt(...) 
       as required by mbedTLS3x threading documentation*/
#if defined(MBEDTLS_THREADING_C) && defined(MBEDTLS_THREADING_ALT)
    config_mbedtls_threading_alt();
#endif /* (MBEDTLS_THREADING_C) && defined(MBEDTLS_THREADING_ALT) */
    
#endif

    PRINTF("\r\nMbedtls - PSA - Crypto - examples - Multithread\r\n");
    PRINTF("\r\n============================\r\n");
        
    mbedtls_mutex_init( &alloc_mutex );
    mbedtls_platform_set_calloc_free( buffer_alloc_calloc_mutexed_custom, buffer_alloc_free_mutexed_custom );
    
    result = xTaskCreate(thread_one_func, "thread_one_func", TASK_MAIN_STACK_SIZE, task_main_stack, TASK_PRIORITY, &task_one);
    if (pdPASS == result)
        PRINTF("Create thread one successfully\r\n");

    result = xTaskCreate(thread_two_func, "thread_two_func", TASK_MAIN_STACK_SIZE, task_main_stack, TASK_PRIORITY, &task_two);
    if (pdPASS == result)
        PRINTF("Create thread two successfully\r\n");
    vTaskStartScheduler();

    while (1)
    {
        //mbedtls_psa_crypto_free();
        //mbedtls_threading_free_alt();
        
        char ch = GETCHAR();
        PUTCHAR(ch);
    }
}

void thread_one_func(void *param)
{
	psa_status_t status;
	int count = 0;

	while (1) {
		status = psa_crypto_init();
		if (status != PSA_SUCCESS) {
			PRINTF("Thread 1 failed psa_crypto_init.\r\n");
			return;
		}

		status = cipher_examples();
		if (status != PSA_SUCCESS) {
			PRINTF("Thread 1 failed cipher_examples.\r\n");
			return;
		}

		count++;
		if (count % 400 == 0) {
                  PRINTF("Thread 1 is running...\r\n");
		}
	}
}

void thread_two_func(void *param)
{
	psa_status_t status;
	int count = 0;

	while (1)  {
		status = psa_crypto_init();
		if (status != PSA_SUCCESS) {
			PRINTF("Thread 2 failed psa_crypto_init.\r\n");
			return;
		}

		status = cipher_examples();
		if (status != PSA_SUCCESS) {
			PRINTF("Thread 2 failed cipher_examples.\r\n");
			return;
		}

		count++;
		if (count % 1000 == 0) {
			PRINTF("Thread 2 is running...\r\n");
		}
	}
}