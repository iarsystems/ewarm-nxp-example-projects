;*******************************************************************************
;                               Copyright 2020 IAR Systems. All rights reserved.
;Function: Cortex-A53(AARCH64) Performance Monitor Unit (PMU)
;
;Note:
;
;History:
; +------------- No(000 .. 999)
; |   +--------- Version
; |   |    +---- New,Change,Add,delete
; v   v    v
; No  Ver  Category    Date      Name       Description
;---+-----+--------+----------+-------------+-----------------------------------
;000 00.00 New      2020/07/15 S.Tonoshita  New development
;*******************************************************************************


        SECTION .text:CODE:ROOT(2)                      ;

        PUBLIC  pmu_start                               ;
        PUBLIC  pmu_stop                                ;
        PUBLIC  pmu_enable_cycle_counter                ;
        PUBLIC  pmu_enable_event0                       ;
        PUBLIC  pmu_enable_event1                       ;
        PUBLIC  pmu_enable_event2                       ;
        PUBLIC  pmu_enable_event3                       ;
        PUBLIC  pmu_enable_event4                       ;
        PUBLIC  pmu_enable_event5                       ;
        PUBLIC  pmu_disable_counter                     ;
        PUBLIC  pmu_disable_event0                      ;
        PUBLIC  pmu_disable_event1                      ;
        PUBLIC  pmu_disable_event2                      ;
        PUBLIC  pmu_disable_event3                      ;
        PUBLIC  pmu_disable_event4                      ;
        PUBLIC  pmu_disable_event5                      ;
        PUBLIC  pmu_read_cycle_counter                  ;
        PUBLIC  pmu_read_event0                         ;
        PUBLIC  pmu_read_event1                         ;
        PUBLIC  pmu_read_event2                         ;
        PUBLIC  pmu_read_event3                         ;
        PUBLIC  pmu_read_event4                         ;
        PUBLIC  pmu_read_event5                         ;
        PUBLIC  pmu_set_event0                          ;
        PUBLIC  pmu_set_event1                          ;
        PUBLIC  pmu_set_event2                          ;
        PUBLIC  pmu_set_event3                          ;
        PUBLIC  pmu_set_event4                          ;
        PUBLIC  pmu_set_event5                          ;
        PUBLIC  pmu_reset_cycle_counter                 ;
        PUBLIC  pmu_reset_evnt_counter                  ;
        PUBLIC  pmu_set_cleck_devider                   ;

;*******************************************************************************
;1.Function: Start of PMU
;
;2.Note: void pmu_start(void)
;
;3.I/O: 
;I/O|Name                      |Description
;---+--------------------------+------------------------------------------------
;
;4.Return:
;*******************************************************************************
pmu_start:
        MRS     x0, PMCR_EL0                                                    ; Read PMCR_EL0 register
        ORR     x0, x0, #0x01                                                   ; PMCR_EL0.E=1
        MSR     PMCR_EL0, x0                                                    ; Write PMCR_EL0 register
        ret                                                                     ;

;*******************************************************************************
;1.Function: Stop of PMU
;
;2.Note: void pmu_stop(void)
;
;3.I/O: 
;I/O|Name                      |Description
;---+--------------------------+------------------------------------------------
;
;4.Return:
;*******************************************************************************
pmu_stop:
        MRS     x0, PMCR_EL0                                                    ; Read PMCR_EL0 register
        AND     x0, x0,#0xFFFFFFFE                                              ; PMCR_EL0.E=0
        MSR     PMCR_EL0, x0                                                    ; Write PMCR_EL0 register
        ret                                                                     ;

;*******************************************************************************
;1.Function: Start of cycle counter
;
;2.Note: void pmu_enable_cycle_counter(void)
;
;3.I/O: 
;I/O|Name                      |Description
;---+--------------------------+------------------------------------------------
;
;4.Return:
;*******************************************************************************
pmu_enable_cycle_counter:
        MOVZ    x0,0x8000,lsl #16                                               ; PMEVCNTR0_EL0.P0=1
        MSR     PMCNTENSET_EL0, x0                                              ; Write PMCNTENSET_EL0 register
        ret                                                                     ;

;*******************************************************************************
;1.Function: Start of event counter0
;
;2.Note: void pmu_enable_event0(void)
;
;3.I/O: 
;I/O|Name                      |Description
;---+--------------------------+------------------------------------------------
;
;4.Return:
;*******************************************************************************
pmu_enable_event0:
        MOVZ    x0,(1 << 0)                                                     ; PMEVCNTR0_EL0.P0=1
        MSR     PMCNTENSET_EL0, x0                                              ; Write PMCNTENSET_EL0 register
        ret                                                                     ;

;*******************************************************************************
;1.Function: Start of event counter1
;
;2.Note: void pmu_enable_event1(void)
;
;3.I/O: 
;I/O|Name                      |Description
;---+--------------------------+------------------------------------------------
;
;4.Return:
;*******************************************************************************
pmu_enable_event1:
        MOVZ    x0,(1 << 1)                                                     ; PMEVCNTR0_EL0.P1=1
        MSR     PMCNTENSET_EL0, x0                                              ; Write PMCNTENSET_EL0 register
        ret                                                                     ;

;*******************************************************************************
;1.Function: Start of event counter2
;
;2.Note: void pmu_enable_event2(void)
;
;3.I/O: 
;I/O|Name                      |Description
;---+--------------------------+------------------------------------------------
;
;4.Return:
;*******************************************************************************
pmu_enable_event2:
        MOVZ    x0, (1 << 2)                                                    ; PMEVCNTR0_EL0.P2=1
        MSR     PMCNTENSET_EL0, x0                                              ; Write PMCNTENSET_EL0 register
        ret                                                                     ;

;*******************************************************************************
;1.Function: Start of event counter3
;
;2.Note: void pmu_enable_event3(void)
;
;3.I/O: 
;I/O|Name                      |Description
;---+--------------------------+------------------------------------------------
;
;4.Return:
;*******************************************************************************
pmu_enable_event3:
        MOVZ    x0, (1 << 3)                                                    ; PMEVCNTR0_EL0.P3=1
        MSR     PMCNTENSET_EL0, x0                                              ; Write PMCNTENSET_EL0 register
        ret                                                                     ;

;*******************************************************************************
;1.Function: Start of event counter4
;
;2.Note: void pmu_enable_event4(void)
;
;3.I/O: 
;I/O|Name                      |Description
;---+--------------------------+------------------------------------------------
;
;4.Return:
;*******************************************************************************
pmu_enable_event4:
        MOVZ    x0, (1 << 4)                                                    ; PMEVCNTR0_EL0.P4=1
        MSR     PMCNTENSET_EL0, x0                                              ; Write PMCNTENSET_EL0 register
        ret                                                                     ;

;*******************************************************************************
;1.Function: Start of event counter5
;
;2.Note: void pmu_enable_event5(void)
;
;3.I/O: 
;I/O|Name                      |Description
;---+--------------------------+------------------------------------------------
;
;4.Return:
;*******************************************************************************
pmu_enable_event5:
        MOVZ    x0, (1 << 5)                                                    ; PMEVCNTR0_EL0.P5=1
        MSR     PMCNTENSET_EL0, x0                                              ; Write PMCNTENSET_EL0 register
        ret                                                                     ;

;*******************************************************************************
;1.Function: Stop of cycle counter
;
;2.Note: void pmu_disable_counter(void)
;
;3.I/O: 
;I/O|Name                      |Description
;---+--------------------------+------------------------------------------------
;
;4.Return:
;*******************************************************************************
pmu_disable_counter:
        MOVZ    x0, 0x8000,lsl #16                                              ; PMEVCNTR0_EL0.C=0
        MSR     PMCNTENCLR_EL0, x0                                              ; Write PMCNTENCLR_EL0 register
        ret                                                                     ;

;*******************************************************************************
;1.Function: Stop of evente counter0
;
;2.Note: void pmu_disable_event0(void)
;
;3.I/O: 
;I/O|Name                      |Description
;---+--------------------------+------------------------------------------------
;
;4.Return:
;*******************************************************************************
pmu_disable_event0:
        MOVZ    x0, (1 << 0)                                                    ; PMEVCNTR0_EL0.P0=0
        MSR     PMCNTENSET_EL0, x0                                              ; Write PMCNTENCLR_EL0 register
        ret                                                                     ;

;*******************************************************************************
;1.Function: Stop of evente counter1
;
;2.Note: void pmu_disable_event1(void)
;
;3.I/O: 
;I/O|Name                      |Description
;---+--------------------------+------------------------------------------------
;
;4.Return:
;*******************************************************************************
pmu_disable_event1:
        MOVZ    x0, (1 << 1)                                                    ; PMEVCNTR0_EL0.P1=0
        MSR     PMCNTENSET_EL0, x0                                              ; Write PMCNTENCLR_EL0 register
        ret                                                                     ;

;*******************************************************************************
;1.Function: Stop of evente counter2
;
;2.Note: void pmu_disable_event2(void)
;
;3.I/O: 
;I/O|Name                      |Description
;---+--------------------------+------------------------------------------------
;
;4.Return:
;*******************************************************************************
pmu_disable_event2:
        MOVZ    x0, (1 << 2)                                                    ; PMEVCNTR0_EL0.P2=0
        MSR     PMCNTENSET_EL0, x0                                              ; Write PMCNTENCLR_EL0 register
        ret                                                                     ;

;*******************************************************************************
;1.Function: Stop of evente counter3
;
;2.Note: void pmu_disable_event3(void)
;
;3.I/O: 
;I/O|Name                      |Description
;---+--------------------------+------------------------------------------------
;
;4.Return:
;*******************************************************************************
pmu_disable_event3:
        MOVZ    x0, (1 << 3)                                                    ; PMEVCNTR0_EL0.P3=0
        MSR     PMCNTENSET_EL0, x0                                              ; Write PMCNTENCLR_EL0 register
        ret                                                                     ;

;*******************************************************************************
;1.Function: Stop of evente counter4
;
;2.Note: void pmu_disable_event4(void)
;
;3.I/O: 
;I/O|Name                      |Description
;---+--------------------------+------------------------------------------------
;
;4.Return:
;*******************************************************************************
pmu_disable_event4:
        MOVZ    x0, (1 << 4)                                                    ; PMEVCNTR0_EL0.P4=0
        MSR     PMCNTENSET_EL0, x0                                              ; Write PMCNTENCLR_EL0 register
        ret                                                                     ;

;*******************************************************************************
;1.Function: Stop of evente counter5
;
;2.Note: void pmu_disable_event5(void)
;
;3.I/O: 
;I/O|Name                      |Description
;---+--------------------------+------------------------------------------------
;
;4.Return:
;*******************************************************************************
pmu_disable_event5:
        MOVZ    x0, (1 << 5)                                                    ; PMEVCNTR0_EL0.P5=0
        MSR     PMCNTENSET_EL0, x0                                              ; Write PMCNTENCLR_EL0 register
        ret                                                                     ;

;*******************************************************************************
;1.Function: Read of cycle counter
;
;2.Note: uint64_t pmu_read_cycle_counter(void)
;
;3.I/O: 
;I/O|Name                      |Description
;---+--------------------------+------------------------------------------------
;
;4.Return:                      The value of the cycle counter
;*******************************************************************************
pmu_read_cycle_counter:
        MRS     x0, PMCCNTR_EL0                                                 ; Read PMCCNTR_EL0 register
        ret                                                                     ;

;*******************************************************************************
;1.Function: Read of event counter0
;
;2.Note: uint32_t pmu_read_event0(void)
;
;3.I/O: 
;I/O|Name                      |Description
;---+--------------------------+------------------------------------------------
;
;4.Return:                      The value of the event counter0
;*******************************************************************************
pmu_read_event0:
        MOV     x0,#0                                                           ; Select event counter
        MSR     PMSELR_EL0, x0                                                  ; Write PMSELR_EL0 register
        MRS     x0, PMXEVCNTR_EL0                                               ; Read PMXEVCNTR_EL0 register
        ret                                                                     ;

;*******************************************************************************
;1.Function: Read of event counter1
;
;2.Note: uint32_t pmu_read_event1(void)
;
;3.I/O: 
;I/O|Name                      |Description
;---+--------------------------+------------------------------------------------
;
;4.Return:                      The value of the event counter1
;*******************************************************************************
pmu_read_event1:
        MOV     x0,#1                                                           ;
        MSR     PMSELR_EL0, x0                                                  ;
        MRS     x0, PMXEVCNTR_EL0                                               ;
        ret                                                                     ;

;*******************************************************************************
;1.Function: Read of event counter2
;
;2.Note: uint32_t pmu_read_event2(void)
;
;3.I/O: 
;I/O|Name                      |Description
;---+--------------------------+------------------------------------------------
;
;4.Return:                      The value of the event counter2
;*******************************************************************************
pmu_read_event2:
        MOV     x0,#2                                                           ;
        MSR     PMSELR_EL0, x0                                                  ;
        MRS     x0, PMXEVCNTR_EL0                                               ;
        ret                                                                     ;

;*******************************************************************************
;1.Function: Read of event counter3
;
;2.Note: uint32_t pmu_read_event3(void)
;
;3.I/O: 
;I/O|Name                      |Description
;---+--------------------------+------------------------------------------------
;
;4.Return:                      The value of the event counter3
;*******************************************************************************
pmu_read_event3:
        MOV     x0,#3                                                           ;
        MSR     PMSELR_EL0, x0                                                  ;
        MRS     x0, PMXEVCNTR_EL0                                               ;
        ret                                                                     ;

;*******************************************************************************
;1.Function: Read of event counter4
;
;2.Note: uint32_t pmu_read_event4(void)
;
;3.I/O: 
;I/O|Name                      |Description
;---+--------------------------+------------------------------------------------
;
;4.Return:                      The value of the event counter4
;*******************************************************************************
pmu_read_event4:
        MOV     x0,#4                                                           ;
        MSR     PMSELR_EL0, x0                                                  ;
        MRS     x0, PMXEVCNTR_EL0                                               ;
        ret                                                                     ;

;*******************************************************************************
;1.Function: Read of event counter5
;
;2.Note: uint32_t pmu_read_event5(void)
;
;3.I/O: 
;I/O|Name                      |Description
;---+--------------------------+------------------------------------------------
;
;4.Return:                      The value of the event counter5
;*******************************************************************************
pmu_read_event5:
        MOV     x0,#5                                                           ;
        MSR     PMSELR_EL0, x0                                                  ;
        MRS     x0, PMXEVCNTR_EL0                                               ;
        ret                                                                     ;



;*******************************************************************************
;1.Function: Set the event to counter0.
;
;2.Note: void pmu_set_event0(uint32_t event)
;
;3.I/O: 
;I/O|Name                      |Description
;---+--------------------------+------------------------------------------------
;I/ event                       Event number setting value
;
;4.Return:
;*******************************************************************************
pmu_set_event0:
        MOV     x1,#0                                                           ;
        MSR     PMSELR_EL0, x1                                                  ;
        MSR     PMXEVTYPER_EL0, x0                                              ;
        ret                                                                     ;

;*******************************************************************************
;1.Function: Set the event to counter1.
;
;2.Note: void pmu_set_event1(uint32_t event)
;
;3.I/O: 
;I/O|Name                      |Description
;---+--------------------------+------------------------------------------------
;I/ event                       Event number setting value
;
;4.Return:
;*******************************************************************************
pmu_set_event1:
        MOV     x1,#1                                                           ;
        MSR     PMSELR_EL0, x1                                                  ;
        MSR     PMXEVTYPER_EL0, x0                                              ;
        ret                                                                     ;

;*******************************************************************************
;1.Function: Set the event to counter2.
;
;2.Note: void pmu_set_event1(uint32_t event)
;
;3.I/O: 
;I/O|Name                      |Description
;---+--------------------------+------------------------------------------------
;I/ event                       Event number setting value
;
;4.Return:
;*******************************************************************************
pmu_set_event2:
        MOV     x1,#2                                                           ;
        MSR     PMSELR_EL0, x1                                                  ;
        MSR     PMXEVTYPER_EL0, x0                                              ;
        ret                                                                     ;

;*******************************************************************************
;1.Function: Set the event to counter3.
;
;2.Note: void pmu_set_event3(uint32_t event)
;
;3.I/O: 
;I/O|Name                      |Description
;---+--------------------------+------------------------------------------------
;I/ event                       Event number setting value
;
;4.Return:
;*******************************************************************************
pmu_set_event3:
        MOV     x1,#3                                                           ;
        MSR     PMSELR_EL0, x1                                                  ;
        MSR     PMXEVTYPER_EL0, x0                                              ;
        ret                                                                     ;

;*******************************************************************************
;1.Function: Set the event to counter4.
;
;2.Note: void pmu_set_event4(uint32_t event)
;
;3.I/O: 
;I/O|Name                      |Description
;---+--------------------------+------------------------------------------------
;I/ event                       Event number setting value
;
;4.Return:
;*******************************************************************************
pmu_set_event4:
        MOV     x1,#4                                                           ;
        MSR     PMSELR_EL0, x1                                                  ;
        MSR     PMXEVTYPER_EL0, x0                                              ;
        ret                                                                     ;

;*******************************************************************************
;1.Function: Set the event to counter5.
;
;2.Note: void pmu_set_event5(uint32_t event)
;
;3.I/O: 
;I/O|Name                      |Description
;---+--------------------------+------------------------------------------------
;I/ event                       Event number setting value
;
;4.Return:
;*******************************************************************************
pmu_set_event5:
        MOV     x1,#5                                                           ;
        MSR     PMSELR_EL0, x1                                                  ;
        MSR     PMXEVTYPER_EL0, x0                                              ;
        ret                                                                     ;

;*******************************************************************************
;1.Function: Reset the cycle counter value.
;
;2.Note: void pmu_reset_counter(void)
;
;3.I/O: 
;I/O|Name                      |Description
;---+--------------------------+------------------------------------------------
;
;4.Return:
;*******************************************************************************
pmu_reset_cycle_counter:
        MRS     x0, PMCR_EL0                                                    ;
        ORR     x0, x0, #0x4                                                    ;
        MSR     PMCR_EL0, x0                                                    ;
        ret                                                                     ;

;*******************************************************************************
;1.Function: Reset the event counter value.
;
;2.Note: void pmu_reset_event_counter(void)
;
;3.I/O: 
;I/O|Name                      |Description
;---+--------------------------+------------------------------------------------
;
;4.Return:
;*******************************************************************************
pmu_reset_evnt_counter:
        MRS     x0, PMCR_EL0                                                    ;
        ORR     x0, x0, #0x2                                                    ;
        MSR     PMCR_EL0, x0                                                    ;
        ret                                                                     ;

;*******************************************************************************
;1.Function: Set the clock frequency divider.
;
;2.Note: void pmu_reset_event_counter(int64_t mode)
;
;3.I/O: 
;I/O|Name                      |Description
;---+--------------------------+------------------------------------------------
;I/ mode                        The clock frequency divider setting (0:PMU_CLOCK_MODE1/1:PMU_CLOCK_MODE64)
;
;4.Return:
;*******************************************************************************
pmu_set_cleck_devider:
        CMP     x0, #0x0                                                        ; PMU_CLOCK_MODE1?
        B.EQ    pmu_set_cleck_devider1                                          ;
        MRS     x0, PMCR_EL0                                                    ;
        AND     x0, x0, 0xFFFFFFFFFFFFFFF7                                      ;
        MSR     PMCR_EL0, x0                                                    ;
        ret                                                                     ;

pmu_set_cleck_devider1:
        MRS     x0, PMCR_EL0                                                    ;
        ORR     x0, x0, #0x8                                                    ; PMCR_EL0.D=1
        MSR     PMCR_EL0, x0                                                    ;
        ret                                                                     ;

        END
