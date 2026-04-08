/*******************************************************************************
                                Copyright 2020 IAR Systems. All rights reserved.
Function: Cortex-A53(AARCH64) Performance Monitor Unit (PMU)

Note:

History:
 +-------------- No(000 .. 999)
 |    +--------- Version
 |    |   +----- New,Change,Add,delete
 v    v   v
 No  Ver  Category    Date      Name        Description
---+-----+--------+----------+-------------+------------------------------------
000 00.00 New      2020/07/15 S.Tonoshita   New development
*******************************************************************************/
#ifndef __PMU_H__
#define __PMU_H__

/* Constants declaration ***************************************************************/
#define PMU_CLOCK_MODE1     0                               // PMU_CLOCK_MODE 1/1
#define PMU_CLOCK_MODE64    1                               // PMU_CLOCK_MODE 1/64

/* Function prototypes declaration *****************************************************/
#ifndef __PMU_C__
    extern void pmu_start(void);                            // Start of PMU
    extern void pmu_stop(void);                             // Stop of PMU
    extern void pmu_enable_cycle_counter(void);             // Start of cycle counter
    extern void pmu_enable_event0(void);                    // Start of event counter0
    extern void pmu_enable_event1(void);                    // Start of event counter1
    extern void pmu_enable_event2(void);                    // Start of event counter2
    extern void pmu_enable_event3(void);                    // Start of event counter3
    extern void pmu_enable_event4(void);                    // Start of event counter4
    extern void pmu_enable_event5(void);                    // Start of event counter5
    extern void pmu_disable_cycle_counter(void);            // Stop of cycle counter
    extern void pmu_disable_event0(void);                   // Stop of event counter0
    extern void pmu_disable_event1(void);                   // Stop of event counter1
    extern void pmu_disable_event2(void);                   // Stop of event counter2
    extern void pmu_disable_event3(void);                   // Stop of event counter3
    extern void pmu_disable_event4(void);                   // Stop of event counter4
    extern void pmu_disable_event5(void);                   // Stop of event counter5
    extern uint64_t pmu_read_cycle_counter(void);           // Read of cycle counter
    extern uint32_t pmu_read_event0(void);                  // Read of event counter0
    extern uint32_t pmu_read_event1(void);                  // Read of event counter1
    extern uint32_t pmu_read_event2(void);                  // Read of event counter2
    extern uint32_t pmu_read_event3(void);                  // Read of event counter3
    extern uint32_t pmu_read_event4(void);                  // Read of event counter4
    extern uint32_t pmu_read_event5(void);                  // Read of event counter5
    extern void pmu_set_event0(int);                        // Set the event to counter0
    extern void pmu_set_event1(int);                        // Set the event to counter1
    extern void pmu_set_event2(int);                        // Set the event to counter2
    extern void pmu_set_event3(int);                        // Set the event to counter3
    extern void pmu_set_event4(int);                        // Set the event to counter4
    extern void pmu_set_event5(int);                        // Set the event to counter5
    extern void pmu_reset_cycle_counter(void);              // Reset the cycle counter value
    extern void pmu_reset_evnt_counter(void);               // Reset the event counter value
    extern void pmu_set_cleck_devider(int64_t mode);        // Set the clock frequency divider
#endif  // __PMU_C__

#endif  // __PMU_H__
