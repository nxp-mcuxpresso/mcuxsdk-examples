/*
 * vim: ai ts=4 sts=4 sw=4 cinoptions=>4 expandtab
 */
#define _GNU_SOURCE

#include "mbw.h"

/* how many runs to average by default */
#define DEFAULT_NR_LOOPS 10

/* we have 3 tests at the moment */
#define MAX_TESTS 3

/* default block size for test 2, in bytes */
#define DEFAULT_BLOCK_SIZE (256 * 1024) /* 256 KB */

/* test types */
#define TEST_MEMCPY  0
#define TEST_DUMB    1
#define TEST_MCBLOCK 2

/* version number */
#define VERSION "1.4"

/* allocate a test array and fill it with data
 * so as to force Linux to _really_ allocate it */
long *make_array(unsigned long long asize)
{
    unsigned long long t;
    unsigned int long_size = sizeof(long);
    long *a;

    a = my_calloc(asize, long_size);

    if (NULL == a)
    {
        PRINTF("Error allocating memory");
        exit(1);
    }

    /* make sure both arrays are allocated, fill with pattern */
    for (t = 0; t < asize; t++)
    {
        a[t] = 0xaa;
    }
    return a;
}

/* actual benchmark */
/* asize: number of type 'long' elements in test arrays
 * long_size: sizeof(long) cached
 * type: 0=use memcpy, 1=use dumb copy loop (whatever GCC thinks best)
 *
 * return value: elapsed time in seconds
 */
double worker(unsigned long long asize, long *a, long *b, int type, unsigned long long block_size)
{
    unsigned long long t;
    timeval_t starttime, endtime;
    double te;
    unsigned int long_size = sizeof(long);
    /* array size in bytes */
    unsigned long long array_bytes = asize * long_size;

    if (type == TEST_MEMCPY)
    { /* memcpy test */
        /* timer starts */
        gettimeofday(&starttime, NULL);
        memcpy(b, a, array_bytes);
        /* timer stops */
        gettimeofday(&endtime, NULL);
    }
    else if (type == TEST_MCBLOCK)
    { /* memcpy block test */
        char *aa = (char *)a;
        char *bb = (char *)b;
        gettimeofday(&starttime, NULL);
        for (t = array_bytes; t >= block_size; t -= block_size, aa += block_size)
        {
            bb = mempcpy(bb, aa, block_size);
        }
        if (t)
        {
            bb = mempcpy(bb, aa, t);
        }
        gettimeofday(&endtime, NULL);
    }
    else if (type == TEST_DUMB)
    { /* dumb test */
        gettimeofday(&starttime, NULL);
        for (t = 0; t < asize; t++)
        {
            b[t] = a[t];
        }
        gettimeofday(&endtime, NULL);
    }

    te = ((double)(endtime.tv_sec * 1000000 - starttime.tv_sec * 1000000 + endtime.tv_usec - starttime.tv_usec)) /
         1000000;

    return te;
}

/* ------------------------------------------------------ */

/* pretty print worker's output in human-readable terms */
/* te: elapsed time in seconds
 * kt: amount of transferred data in KiB
 * type: see 'worker' above
 *
 * return value: -
 */
void printout(double te, double kt, int type)
{
    switch (type)
    {
        case TEST_MEMCPY:
            PRINTF("Method: MEMCPY\t");
            break;
        case TEST_DUMB:
            PRINTF("Method: DUMB\t");
            break;
        case TEST_MCBLOCK:
            PRINTF("Method: MCBLOCK\t");
            break;
    }
    PRINTF("Elapsed: %.5f\t", te);
    PRINTF("MiB: %.5f\t", kt / 1024);
    PRINTF("Copy: %.3f MiB/s\n", kt / 1024 / te);
    return;
}

/* ------------------------------------------------------ */
/*******************************************************************************
* Input parameters:
* ---- testno       : Test set type: 1, 2, 3
                      1: memcpy test
                      2: dumb (b[i]=a[i] style) test
                      3: memcpy test with fixed block size
* ---- showavg      : Display average
* ---- nr_loops     : number of runs per test
* ---- block_size   : block size in bytes for - testno = 2
* ---- mem_start    : array start address
* ---- mem_size     : array_size_in_Byte
*/
int mbw_main(
    uint32_t testno, uint32_t showavg, uint32_t nr_loops, uint64_t block_size, uint32_t mem_start, uint32_t mem_size)
{
    uint32_t long_size = 0;
    double te, te_sum;  /* time elapsed */
    uint64_t asize = 0; /* array size (elements in array) */
    uint32_t i     = 0;
    long *a, *b;        /* the two arrays to be copied from/to */
    int tests[MAX_TESTS];
    double kt = 0;      /* MiBytes transferred == array size in KiB */
    kt        = mem_size / 1024.0;
    int quiet = 0;      /* suppress extra messages */

    tests[0] = 0;
    tests[1] = 0;
    tests[2] = 0;

    g_myMem.memStart = mem_start;
    g_myMem.memSize  = mem_size;
    for (uint32_t i = 0; i < MAX_MEM_REGIONS; i++)
    {
        g_myMem.regionStart[i] = 0;
        g_myMem.regionSize[i]  = 0;
    }

    if (!nr_loops)
    {
        nr_loops = DEFAULT_NR_LOOPS;
    }

    if ((testno > 0) || (testno <= MAX_TESTS))
    {
        tests[testno - 1] = 1;
    }

    if (!block_size)
    {
        block_size = DEFAULT_BLOCK_SIZE;
    }

    /* default is to run all tests if no specific tests were requested */
    if ((tests[0] + tests[1] + tests[2]) == 0)
    {
        tests[0] = 1;
        tests[1] = 1;
        tests[2] = 1;
    }

    if (0 >= kt)
    {
        PRINTF("Error: array size wrong!\n");
        exit(1);
    }

    long_size = sizeof(long);                                  /* the size of long on this platform */
    asize     = (unsigned long long)((1024 / long_size) * kt); /* 1024 / long_size = how many long in 1 KB */
    if (asize * long_size < block_size)
    {
        PRINTF("Error: array size larger than block size (%llu bytes)!\n", block_size);
        exit(1);
    }

    if (!quiet)
    {
        PRINTF("Long uses %d bytes. ", long_size);
        PRINTF("Allocating 2*%lld elements = %lld bytes of memory.\n", asize, 2 * asize * long_size);
        if (tests[2])
        {
            PRINTF("Using %lld bytes as blocks for memcpy block copy test.\n", block_size);
        }
    }

    a = make_array(asize / 2);
    b = make_array(asize / 2);

    /* ------------------------------------------------------ */
    if (!quiet)
    {
        PRINTF("Getting down to business... Doing %d runs per test.\n", nr_loops);
    }

    /* run all tests requested, the proper number of times */
    for (testno = 0; testno < MAX_TESTS; testno++)
    {
        te_sum = 0;
        if (tests[testno])
        {
            for (i = 0; i < nr_loops; i++)
            {
                te = worker(asize, a, b, testno, block_size);
                te_sum += te;
                PRINTF("%d\t", i);
                printout(te, kt, testno);
            }
            if (showavg)
            {
                PRINTF("AVG\t");
                printout(te_sum / nr_loops, kt, testno);
            }
        }
    }

    my_free(a);
    my_free(b);
    return 0;
}
