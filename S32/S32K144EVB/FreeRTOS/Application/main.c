/******************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2021
 *
 *    File name   : main.c
 *    Description : Define main module
 *
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 *  to develop code for NXP S32K144EVB board. It shows basic use of S32K144
 *  GPIO, SystemTick, ADC, Watchdog, CRC and NVIC.
 *
 *  The main functionality is as following:
 *  - Toggle LED green when switch 2 is pressed 
 *  - Toggle LED blue when switch 3 is pressed 
 *  - Turn on LED red when ADC value is greater than 1280
 *  - Turn off LED red when ADC value is less than 768
 *  - Calcualte code flash memeory checksum and compare with stored checksum
 *  - Refresh watchdog periodically
 *
 *    $Revision: $
 ******************************************************************************/
#include <stdio.h>
#include <stdbool.h>

/* Kernel includes.                                                           */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"
#include "event_groups.h"

/* Includenecessary module. Cpu.h contains other modules needed for compiling */
#include "Cpu.h"

#include "main.h"

/* Definitions for TaskLedRed                                                 */
static TaskHandle_t TaskLedRedHandle;

/* Definitions for TaskLedGreen                                               */
static TaskHandle_t TaskLedGreenHandle;

/* Definitions for TaskLedBlue                                                */
static TaskHandle_t TaskLedBlueHandle;

/* Definitions for TaskAdcRead                                                */
static TaskHandle_t TaskAdcReadHandle;

/* Definitions for TaskCrcCheck                                               */
static TaskHandle_t TaskCrcCheckHandle;

/* Definitions for StartTaskWdgRefresh                                        */
static TaskHandle_t TaskWdgRefreshHandle;

/* Definitions for MutexSwo                                                   */
static SemaphoreHandle_t MutexSwoHandle;

/* Definitions for BinarySemSwitch2                                           */
static SemaphoreHandle_t BinarySemSwitch2Handle;

/* Definitions for EventLedRed                                                */
static EventGroupHandle_t EventLedRedHandle;

/* Definitions for EventWdg                                                   */
static EventGroupHandle_t EventWdgHandle;


/* Define the counters used in the demo application...                        */
static uint32_t TaskCounter;
static uint32_t TaskLedRedCounter;
static uint32_t TaskLedGreenCounter;
static uint32_t TaskLedBlueCounter;
static uint32_t TaskAdcReadCounter;
static uint32_t TaskCrcCheckCounter;
static uint32_t TaskWdgRefreshCounter;


/* Linker generated symbols                                                   */
extern uint32_t const __checksum;
extern uint32_t __checksum_begin;
extern uint32_t __checksum_end;

/* Static functions declarations                                              */
void StartTaskLedRed(void *argument);
void StartTaskLedGreen(void *argument);
void StartTaskLedBlue(void *argument);
void StartTaskAdcRead(void *argument);
void StartTaskCrcCheck(void *argument);
void StartTaskWdgRefresh(void *argument);


static void IncrementTaskCounter(void);

static void __NVIC_SetPriorityGrouping(uint32_t PriorityGroup);

static void WDOG_Init(void);


int main(void)
{
  CLOCK_DRV_Init(&clockMan1_InitConfig0);

  PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);
  
  CRC_DRV_Init(INST_CRC1, &crc1_InitConfig0);
  
  ADC_DRV_ConfigConverter(INST_ADCONV1, &adConv1_ConvConfig0);
  
  /* Note: Watchdog Control and Status Register (CS) EN reset value is 1 and  */
  /*       UPDATE reset value is 0. This will cause NXP SDK WDOG_DRV_Init()   */
  /*       can't configure watchdog during standalone operation !!!           */
  /*       While this is not a issue duing debug as IAR flash loader will     */
  /*       update EN to 0 and UPDATE to 1 during downloading                  */
  /*       So it's better to write watchdog initization manually              */
  WDOG_Init();
  
  //WDOG_DRV_Init(INST_WATCHDOG1, &watchdog1_Config0);

  PINS_DRV_SetPins(PTD, (1U));
  
  PINS_DRV_SetPins(PTD, (1U << 15U));

  PINS_DRV_SetPins(PTD, (1U << 16U));
  
  SystemCoreClockUpdate();
  
  /* Create the mutex(es)                                                     */
  /* creation of MutexSwo                                                     */
  MutexSwoHandle = xSemaphoreCreateMutex();

  /* Create the semaphores(s)                                                 */
  /* creation of BinarySemSwitch2                                             */
  BinarySemSwitch2Handle = xSemaphoreCreateBinary();

  /* Create the thread(s)                                                     */
  /* creation of TaskLedRed                                                   */
  xTaskCreate( StartTaskLedRed,
               "TaskLedRed",
                512,
                NULL,
                2,
                &TaskLedRedHandle );                              

  /* creation of TaskLedGreen                                                 */
  xTaskCreate( StartTaskLedGreen,
               "TaskLedGreen",
                512,
                NULL,
                2,
                &TaskLedGreenHandle );     
                
  /* creation of TaskLedBlue                                                  */
  xTaskCreate( StartTaskLedBlue,
               "TaskLedBlue",
                512,
                NULL,
                2,
                &TaskLedBlueHandle );     

  /* creation of TaskAdcRead                                                  */
  xTaskCreate( StartTaskAdcRead,
               "TaskAdcRead",
                512,
                NULL,
                8,
                &TaskAdcReadHandle ); 

  /* creation of TaskCrcCheck                                                 */
  xTaskCreate( StartTaskCrcCheck,
               "TaskCrcCheck",
                512,
                NULL,
                1,
                &TaskCrcCheckHandle );
  
  /* creation of TaskWdgRefresh                                               */
  xTaskCreate( StartTaskWdgRefresh,
               "TaskWdgRefresh",
                512,
                NULL,
                16,
                &TaskWdgRefreshHandle );

    /* creation of EventLedRed                                                */
  EventLedRedHandle = xEventGroupCreate();
  
  /* creation of EventWdg                                                     */
  EventWdgHandle = xEventGroupCreate();
  
  /* Ensure all priority bits are assigned as preemption priority bits.       */
  /* https://www.freertos.org/RTOS-Cortex-M3-M4.html                          */
  __NVIC_SetPriorityGrouping(0x03);
      
  /* Service Call interrupt might be configured before kernel start           */
  /* and when its priority is lower or equal to BASEPRI, svc intruction       */
  /* causes a Hard Fault.                                                     */
  INT_SYS_SetPriority (SVCall_IRQn, 0U);
  
  /* Enable PORTC interrupt                                                   */
  INT_SYS_EnableIRQ(PORTC_IRQn);
 
  /* Set PORTC interrupt prioirty                                             */
  INT_SYS_SetPriority (PORTC_IRQn, 1U);
  
  /* Start the kernel scheduler                                               */
  vTaskStartScheduler();
 
  while (1)
  {

  }

}



/**
* @brief Function implementing the TaskLedRed Task.
* @param argument: Not used
* @retval None
*/
void StartTaskLedRed(void *argument)
{
  /* Infinite loop                                                            */
  for(;;)
  {
    /* Increment the task counter                                             */
    TaskLedRedCounter++;
        
    IncrementTaskCounter();

    /* Sleep for 100 ms                                                       */
    vTaskDelay(pdMS_TO_TICKS(100));
    
    /* Get event                                                              */
    /* Turn off LED red when the event is reset                               */
    /* Turn on LED red when the event is set                                  */
    if(0u == xEventGroupGetBits(EventLedRedHandle))
    {
      /* Turn off LED red: PTD15                                              */
      PINS_DRV_SetPins(PTD, (1U << 15U));
    }
    else
    {
      /* Turn on LED red: PTD15                                               */
      PINS_DRV_ClearPins(PTD, (1U << 15U));    
    }
  }
}


/**
* @brief Function implementing the TaskLedGreen Task.
* @param argument: Not used
* @retval None
*/
void StartTaskLedGreen(void *argument)
{
  /* Infinite loop                                                            */
  for(;;)
  {
    /* Increment the task counter                                             */
    TaskLedGreenCounter++;
        
    IncrementTaskCounter();

    /* Sleep for 100 ms                                                       */
    vTaskDelay(pdMS_TO_TICKS(100));

    /* Toggle LED green when switch2 is pressed                               */    
    /* Get the semaphore with suspension                                      */
    xSemaphoreTake(BinarySemSwitch2Handle, portMAX_DELAY);
    
    /* Toggle LED green: PTD16                                                */
    PINS_DRV_TogglePins(PTD, (1U << 16U)); 
  }
}

/**
* @brief Function implementing the TaskLedBlue Task.
* @param argument: Not used
* @retval None
*/
void StartTaskLedBlue(void *argument)
{
  /* Infinite loop                                                            */
  for(;;)
  {
    /* Increment the task counter                                             */
    TaskLedBlueCounter++;
        
    IncrementTaskCounter();

    /* Sleep for 100 ms                                                       */
    vTaskDelay(pdMS_TO_TICKS(100));

    /* Toggle LED blue when switch2 is pressed                                */      
    /* Block indefinitely (without a timeout, so no need to check the function's
       return value) to wait for a notification. Here the RTOS task notification
       is being used as a binary semaphore, so the notification value is cleared
       to zero on exit. NOTE! Real applications should not block indefinitely,
       but instead time out occasionally in order to handle error conditions
      that may prevent the interrupt from sending any more notifications. */ 
    ulTaskNotifyTake(pdTRUE,        /* Clear the notification value on exit. */
                     portMAX_DELAY ); /* Block indefinitely. */
    
    /* Toggle LED blue: PTD0                                                  */
    PINS_DRV_TogglePins(PTD, (1U));
  }
}


/**
* @brief Function implementing the TaskAdcRead Task.
* @param argument: Not used
* @retval None
*/
void StartTaskAdcRead(void *argument)
{
  /* ADC value                                                                */
  static uint32_t AdcValue;
  static uint32_t AdcValueArray[4];
  
  static bool IsFirstAdcRead = true;

  /* Infinite loop                                                            */
  for(;;)
  {
    /* Increment the task counter                                             */
    TaskAdcReadCounter++;
       
    IncrementTaskCounter();
    
    /* Software trigger: When software trigger is selected, a conversion is   */
    /* initiated following a write to SC1A.                                   */
    /* 001100b - External channel 12 is selected as input                     */
    /* Potentiometer: DC0_SE12                                                */
    ADC0->SC1[0] = 0x0C;
    
    /* Sleep for 20 ms                                                        */
    vTaskDelay(pdMS_TO_TICKS(20u));
    
    if(0u != (ADC0->SC1[0] & ADC_SC1_COCO_MASK))
    {
      if(true == IsFirstAdcRead)
      {
        IsFirstAdcRead = false;
        
        AdcValueArray[0] = ADC0->R[0];
        AdcValueArray[1] = AdcValueArray[0];
        AdcValueArray[2] = AdcValueArray[1];
        AdcValueArray[3] = AdcValueArray[2];
      }
      else
      {
        AdcValueArray[3] = AdcValueArray[2];
        AdcValueArray[2] = AdcValueArray[1];
        AdcValueArray[1] = AdcValueArray[0];
        AdcValueArray[0] = ADC0->R[0];      
      }
    }
    else
    {
    
    }     
        
    /* Moving average                                                         */    
    AdcValue = (AdcValueArray[0] + AdcValueArray[1]
              + AdcValueArray[2] + AdcValueArray[3])
              /4u;
    
    /* Hysteresis                                                             */
    /* Set event flag EventLedRed when ADC value is greater than 1280         */
    /* Clear event flag EventLedRe when ADC value is less than 768            */
    if(1280u < AdcValue)
    {
      /* Set event flag EventLedRedHandle */
      xEventGroupSetBits(EventLedRedHandle, 0x1);           
    }
    else if(768u > AdcValue)
    {
      /* Clear event flag EventLedRedHandle */
      xEventGroupClearBits(EventLedRedHandle, 0x1);       
    }
    else
    {
    
    }
    
    /* Get the SWO mutex with suspension                                      */
    xSemaphoreTake(MutexSwoHandle, portMAX_DELAY);
    
    /* Print via SWO                                                          */
    printf("ADC value is %d\r\n", AdcValue);	
    
    /* Release ownership of mutex                                             */
    xSemaphoreGive(MutexSwoHandle);
    
    /* Set event flag EventWdgHandle                                          */
    xEventGroupSetBits(EventWdgHandle, 0x1);
  }
}


/**
* @brief Function implementing the TaskCrcCheck Task.
* @param argument: Not used
* @retval None
*/
void StartTaskCrcCheck(void *argument)
{
  uint32_t * DataPointer;

  /* Infinite loop                                                            */
  for(;;)
  {
    /* Increment the task counter                                             */
    TaskCrcCheckCounter ++;
    
    IncrementTaskCounter();
    
    /* Calculate the code flash checksum using CRC calculation unit           */
        
    /* After a completed CRC calculation, the module can be reinitialized     */
    /* for a new CRC computation by reasserting CRC_CTRL[WAS]                 */
    /* 4. Set CRC_CTRL[WAS] to program the seed value                         */    
    CRC->CTRL = (CRC->CTRL | CRC_CTRL_WAS_MASK);
    
    /* 5. Write a 32-bit seed to CRC_DATA[HU:HL:LU:LL]                        */ 
    CRC->DATAu.DATA = 0xFFFFFFFFU;
    
    /* 6. Clear CRC_CTRL[WAS] to start writing data values                    */ 
    CRC->CTRL = (CRC->CTRL & (~CRC_CTRL_WAS_MASK));
    
    /* Write data values into CRC_DATA[HU:HL:LU:LL]. A CRC is computed on     */
    /* every data value write, and the intermediate CRC result is stored back */
    /* into CRC_DATA[HU:HL:LU:LL]                                             */   
    for (DataPointer = &__checksum_begin; DataPointer < &__checksum_end;
         DataPointer = DataPointer + 1U)
    {
      CRC->DATAu.DATA = * DataPointer;
    }
    
    /* 8. When all values have been written, read the final CRC result from   */
    /* CRC_DATA[HU:HL:LU:LL]. The CRC is calculated bytewise, and two clocks  */
    /* are required to complete one CRC calculation                           */
    /* Compare the calculated CRC with the previously stored CRC              */
    if(__checksum == CRC->DATAu.DATA)
    {  
      /* Get the SWO mutex with suspension                                    */
      xSemaphoreTake(MutexSwoHandle, portMAX_DELAY);
    
      /* Print via SWO                                                        */
      printf("Code Flash CRC is OK\r\n");	
    
      /* Release ownership of mutex                                           */
      xSemaphoreGive(MutexSwoHandle);
    }
    else
    {
      /* Get the SWO mutex with suspension                                    */
      xSemaphoreTake(MutexSwoHandle, portMAX_DELAY);
    
      /* Print via SWO                                                        */
      printf("Code Flash CRC is NOK\r\n");	
    
      /* Release ownership of mutex                                           */
      xSemaphoreGive(MutexSwoHandle);
    }      
          
  }
}


/**
* @brief Function implementing the StartTaskWdgRefresh Task.
* @param argument: Not used
* @retval None
*/
void StartTaskWdgRefresh(void *argument)
{
  /* Infinite loop                                                            */
  for(;;)
  {
    /* Increment the task counter                                             */
    TaskWdgRefreshCounter ++;
        
    IncrementTaskCounter();
    
    /* Window watchdog refresh mechanism:                                     */
    /* The tiemout is about 64ms and the window is about 48ms                 */
    /* Refresh watchdog at the middle of window and timeout: 56ms             */
    /* Refresh watchdog at the highest priority task to avoid blocking        */
    /* Need check the event flag to make sure low prioirty task also executes */
    
    
    /* Sleep for 56 ms                                                        */
    vTaskDelay(pdMS_TO_TICKS(56));
    
    /* Refresh watchdog                                                       */
    WDOG_DRV_Trigger(INST_WATCHDOG1);
        
    /* Wait for event flag EventWdgHandle                                     */
    xEventGroupWaitBits(EventWdgHandle, 0x1, pdTRUE, pdFALSE, portMAX_DELAY);
  }
}



static void IncrementTaskCounter(void)
{
  TaskCounter ++;
}


void __NVIC_SetPriorityGrouping(uint32_t PriorityGroup)
{
  uint32_t reg_value;
  uint32_t PriorityGroupTmp;             /* only values 0..7 are used         */

  PriorityGroupTmp = (PriorityGroup & (uint32_t)0x07UL);  
  
  /* Read old register configuration. Note: VECTKEY is read as 0xFA05         */
  reg_value  =  S32_SCB->AIRCR & (!S32_SCB_AIRCR_VECTKEY_MASK);   
  reg_value &= ~((uint32_t)(S32_SCB_AIRCR_VECTRESET_MASK | S32_SCB_AIRCR_PRIGROUP_MASK)); /* clear bits to change               */
  reg_value  =  (reg_value                                   |
                ((uint32_t)0x5FAUL << S32_SCB_AIRCR_VECTKEY_SHIFT) |
                (PriorityGroupTmp << S32_SCB_AIRCR_PRIGROUP_SHIFT)  );              /* Insert write key and priority group */
    
  S32_SCB->AIRCR =  reg_value;
}


void PORTC_IRQHandler(void)
{
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  
  /* SW2: PTC12                                                               */
  if(0 != (PORTC->ISFR & (1U << 12)))
  {
      /* Clear the flag first                                                 */
      /* The flag remains set until a logic 1 is written to the flag          */
      PORTC->ISFR = (PORTC->ISFR | (1U << 12));

      /* Gives (or releases) binary semaphore SW2                             */
      xSemaphoreGiveFromISR(BinarySemSwitch2Handle, &xHigherPriorityTaskWoken);
      
      /* Now the task has been unblocked a context switch should be performed if
      xHigherPriorityTaskWoken is equal to pdTRUE. NOTE: The syntax required to perform
      a context switch from an ISR varies from port to port, and from compiler to
      compiler. Check the web documentation and examples for the port being used to
      find the syntax required for your application. */
      portYIELD_FROM_ISR( xHigherPriorityTaskWoken );      
  }
  /* SW3: PTC13                                                               */
  else if(0u != (PORTC->ISFR & (1U << 13)))
  {
      /* Clear the flag first                                                 */
      /* The flag remains set until a logic 1 is written to the flag          */
      PORTC->ISFR = (PORTC->ISFR | (1U << 13));

      /* Unblock the handling task so the task can perform any processing necessitated
         by the interrupt. xHandlingTask is the task's handle, which was obtained
         when the task was created. */
      vTaskNotifyGiveFromISR( TaskLedBlueHandle, &xHigherPriorityTaskWoken );      
      
      /* Now the task has been unblocked a context switch should be performed if
      xHigherPriorityTaskWoken is equal to pdTRUE. NOTE: The syntax required to perform
      a context switch from an ISR varies from port to port, and from compiler to
      compiler. Check the web documentation and examples for the port being used to
      find the syntax required for your application. */
      portYIELD_FROM_ISR( xHigherPriorityTaskWoken );      
  }
  else
  {
  
  }
}

static void WDOG_Init(void)
{
  /* Unlock watchdog 		                                              */
  WDOG->CNT = 0xD928C520U;     
  
  /* Timeout value = 8224U (64ms with LPO128K_CLK)	                      */
  WDOG->TOVAL = 8224U;   
  
  /* Window value = 6168U (48ms with LPO128K_CLK)	                      */
  WDOG->WIN = 6168U;     

  /* Window value = 6168U	                                              */
  WDOG->CS = (WDOG_CS_WIN(1)    | WDOG_CS_CMD32EN(1) | WDOG_CS_PRES(0) |
              WDOG_CS_CLK(1)    | WDOG_CS_EN(1)      | WDOG_CS_INT(0)  | 
              WDOG_CS_UPDATE(1) | WDOG_CS_TST(1)     | WDOG_CS_DBG(0)  |
              WDOG_CS_WAIT(0)   | WDOG_CS_STOP(0));     
}
