/*******************************************************************************
                                Copyright 2020 IAR Systems. All rights reserved.
Function: i.MX8M Nano Applications Processor

Note: Evaluation Kit for the i.MX 8M Nano

History:
 No  Rev  Date       Name           Note
---+-----+----------+-------------+--------------------------------------------
000 00.00 2020/07/15 S.Tonoshita    New
*******************************************************************************/

/******************************************************************************/
#include <stdint.h>
#include <intrinsics.h>
#include <stdio.h>
#include "misc.h"
#include "pmu.h"

/******************************************************************************/
#define TEST_MAX        (100)
#define ICACHE_MAX      (100)
#define DCACHE_MAX      (100)

/******************************************************************************/
__noreturn int main( void );
__noreturn int core1_main( void );
__noreturn int core2_main( void );
__noreturn int core3_main( void );

/******************************************************************************/
__root uint64_t icache_cycle[TEST_MAX];
__root uint64_t dcache_cycle[TEST_MAX];
__root uint64_t data[DCACHE_MAX];

int cache_enabled = 0;        /* change this in the debugger to enable caches */

/******************************************************************************/
__noreturn int main( void )
{
    uint32_t i,j;
    uint64_t start;

    if (cache_enabled)
        enable_caches_el3();
    pmu_start();
    pmu_enable_cycle_counter();

    for(i=0;i<TEST_MAX;i++) {
        start = pmu_read_cycle_counter();
        for(j=0;j<ICACHE_MAX;j++){
            __no_operation();
            __no_operation();
            __no_operation();
            __no_operation();
        }
        icache_cycle[i] = pmu_read_cycle_counter() - start;

        start = pmu_read_cycle_counter();
        for(j=0;j<DCACHE_MAX;j++) {
            data[i] = i +1000ull;
        }
        dcache_cycle[i] = pmu_read_cycle_counter() - start;
    }
    printf("Hello world, cache %sabled\n", cache_enabled ? "en" : "dis");
    for(j = 0; j < 1000; j++) {
        printf("Loop %d\n", j);
        for(i = 0; i < 0x8000000;i++);
    }
    for(;;);
}

/******************************************************************************/
__noreturn int core1_main( void )
{
    for(;;);
}

/******************************************************************************/
__noreturn int core2_main( void )
{
    for(;;);
}

/******************************************************************************/
__noreturn int core3_main( void )
{
    for(;;);
}
