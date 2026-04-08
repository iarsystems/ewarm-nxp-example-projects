/**HEADER**********************************************************************
*
* Copyright (c) Freescale Semiconductor
* All rights reserved
*
*******************************************************************************
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
*******************************************************************************
*
* $FileName: usb_batt_chg_app.c$
* $Version : 1.0.0.0$
* $Date    : Apr-22-2011$
*END**************************************************************************/

#include "types.h"
#include "usb_devapi.h"
#include "derivative.h"   /* Include peripheral declarations */
/* Specific file header */
#include "usb_batt_chg.h"
/* Specific MAX3353 files header */
#include "usb_otg_max3353_kinetis.h"

#include <stdio.h>

/* Private definitions *******************************************************/
#define T_UNIT_CON                        (1000)
#define T_CHECK_DM                        (3)

/* Interrupts enabled pins on the external VBUS Detection IC */
#define IRQ_H_ENABLED                     (TRUE)
#define IRQ_F_ENABLED                     (TRUE)
#define IRQ_D_ENABLED                     (TRUE)
#define IRQ_B_ENABLED                     (TRUE)
/* Interrupt type for external VBUS detect circuit */
#define IRQC_FALLING_EDGE                 (0xa)

/* Private variables *********************************************************/
USB_BATT_CHG_INIT_STRUCT batt_chg_app_init;
//USB_BATT_CHG_STATUS* batt_chg_status_app_ptr;

/* Private functions prototyping *********************************************/
void TestApp_Init(void);
void TestApp_Task(void);
/* Application callback function */
void BatteryCharging_AppCallback(USB_BATT_CHG_EVENT event, const USB_BATT_CHG_STATUS* app_bat_chg_status_ptr);
void BatteryCharging_VbusDetPinsInit(void);
/* Function to enable/disable the battery management circuit */
void BatteryCharging_EnableDisableCircuit(boolean enable_ic);
/* Function to update the VBUS status */
boolean BatteryCharging_VbusStatusUpdate(void);
/* VBUS detection ISR */
void VbusDetect_ISR(void);

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  :  TestApp_Init
* Returned Value :  none
* Comments       :  Battery Charging test application init function
*
*
*END*--------------------------------------------------------------------*/
void TestApp_Init(void)
{
  uint_32 status = USB_OK;

  /* Uart initialization */
  sci_init();
  /* IIC module initialization; it is used to communicate with the MAX3353 */
  IIC_ModuleInit();

  /* USBDCD module clock enable */
  SIM_SCGC6 |= SIM_SCGC6_USBDCD_MASK;

  /* USBDCD interrupt enable */
  NVICICPR2 = 1 << ((74)%32);
  NVICISER2 = 1 << ((74)%32);

  printf_kinetis("\nUSB Battery Charging Application Demo.\n");
  fflush(stdout);

  /* Initialize the pins used to detect the VBUS via external IC */
  BatteryCharging_VbusDetPinsInit();

  /* Prepare the application init variable */
  /* External VBUS detect circuit initialization */
  batt_chg_app_init.ext_vbus_detect_circuit_use               = TRUE;
  batt_chg_app_init.ext_vbus_detect_enable_disable_func       = _otg_max3353_enable_disable;
  batt_chg_app_init.ext_vbus_detect_update_vbus_status_func   = BatteryCharging_VbusStatusUpdate;
  /* External Battery Charging circuit initialization */
  batt_chg_app_init.ext_batt_chg_circuit_use                  = TRUE;
  batt_chg_app_init.ext_batt_chg_circuit_enable_disable_func  = BatteryCharging_EnableDisableCircuit;
  /* Timing initialization */
  batt_chg_app_init.usb_batt_chg_timings_config.time_dcd_dbnc      = T_DCD_DBNC;
  batt_chg_app_init.usb_batt_chg_timings_config.time_vdpsrc_on     = T_VDPSRC_ON;
  batt_chg_app_init.usb_batt_chg_timings_config.time_vdpsrc_con    = T_VDPSRC_CON;
  batt_chg_app_init.usb_batt_chg_timings_config.time_seq_init      = T_UNIT_CON;
  batt_chg_app_init.usb_batt_chg_timings_config.time_check_d_minus = T_CHECK_DM;

  /* Initialize the USB Battery Charger module */
  status = _usb_batt_chg_init(&batt_chg_app_init);
  if(status == USB_OK)
  {
    status = _usb_batt_chg_register_callback(BatteryCharging_AppCallback);
  }

  if(status == USB_OK)
  {
     printf_kinetis("\nInitialization passed. Plug in the USB portable device to the USB charger port.");
     fflush(stdout);
  }
  else
  {
     printf_kinetis("\nERROR!!! Initialization failed!");
     fflush(stdout);
     return;
  }
}


/*FUNCTION*----------------------------------------------------------------
*
* Function Name  :  TestApp_Task
* Returned Value :  none
* Comments       :  Battery charging application task function
*
*
*END*--------------------------------------------------------------------*/
void TestApp_Task(void)
{
  _usb_batt_chg_task();
}


/*FUNCTION*----------------------------------------------------------------
*
* Function Name  :  BatteryCharging_AppCallback
* Returned Value :  none
* Comments       :  Battery charging application callback function; informs
*                   the application about the status of charging port type
*                   detection
*
*END*--------------------------------------------------------------------*/
void BatteryCharging_AppCallback(USB_BATT_CHG_EVENT event, const USB_BATT_CHG_STATUS* app_bat_chg_status_ptr)
{
  /* Treats the battery charging detection error event */
  if(event & USB_BATT_CHG_EVENT_ERROR)
  {
    /* Information message about the error occurrence */
    printf_kinetis("\nERROR: The charging port detection has failed!!!");
    fflush(stdout);

    /* check and inform the application about the error type */
    if(app_bat_chg_status_ptr->error_type == error_seq_timeout)
    {
      printf_kinetis("\nERROR: The sequence timeout has occured!!!");
      fflush(stdout);
    }
    else
    {
      if(app_bat_chg_status_ptr->error_type == error_unknown_port_type)
      {
        printf_kinetis("\nERROR: Unknown port type!!!");
        fflush(stdout);
      }
    }
  }

  /* Treats the battery charging detection completion event */
  if(event & USB_BATT_CHG_EVENT_PHASE_COMPLETE)
  {
    /* Information message */
    printf_kinetis("\nThe charging port detection has been successfully done.");
    fflush(stdout);

    if(app_bat_chg_status_ptr->dev_state == USB_BATT_CHG_STATE_COMPLETE)
    {
      /* Charging port type message */
      switch(app_bat_chg_status_ptr->charger_type)
      {
        case USB_BATT_CHG_PORT_TYPE_SDP:
                printf_kinetis("\nThe Portable Device (PD) is attached to a Standard Downstream Port (SDP).");
                printf_kinetis("\nThe maximum current allowed to be drawn is 500mA, after configuration step.");
                break;

        case USB_BATT_CHG_PORT_TYPE_CDP:
                printf_kinetis("\nThe Portable Device (PD) is attached to a Charging Downstream Port (CDP).");
                printf_kinetis("\nThe maximum current allowed to be drawn is 1.5A.");
                break;

        case USB_BATT_CHG_PORT_TYPE_DCP:
                printf_kinetis("\nThe Portable Device (PD) is attached to a Dedicated Charging Port (DCP).");
                printf_kinetis("\nThe maximum current allowed to be drawn is 1.8A.");
                break;
      }
      fflush(stdout);
    }
  }
}


/*FUNCTION*----------------------------------------------------------------
*
* Function Name  :  BatteryCharging_EnableDisableCircuit
* Returned Value :  none
* Comments       :  Enable the Battery Charging IC
*
*END*--------------------------------------------------------------------*/
void BatteryCharging_EnableDisableCircuit(boolean enable_ic)
{
  if(enable_ic == TRUE)
  {
    /* Enable the Battery Charging IC */
    printf_kinetis("\nThe Battery Charger IC is enabled.");
    fflush(stdout);
  }
  else
  {
    /* Disable the Battery Charging IC */
    printf_kinetis("\nThe Battery Charger IC is disabled.");
    fflush(stdout);
  }
}



/*FUNCTION*----------------------------------------------------------------
*
* Function Name  :  BatteryCharging_ExtVbusDetPinsInit
* Returned Value :  none
* Comments       :  Initialize the GPIO pins used to detect the VBUS voltage
*                   via the external IC (MAX3353)
*
*END*--------------------------------------------------------------------*/
void BatteryCharging_VbusDetPinsInit(void)
{
  #if(defined MCU_MK40N512VMD) || (defined MCU_MK53N512CMD100)
	NVICICER2 =(1<<24);                     /* Clear any pending interrupts */
	NVICISER2  =(1<<24);                    /* Enable interrupts PORTB */
	
	GPIOB_PDDR &= ~((uint_32)1 << 7);		/* set input PTB 7*/
  #endif
  #ifdef MCU_MK60N512VMD100
	NVICICER2=(1<<23);                     /* Clear any pending interrupts */
	NVICISER2=(1<<23);                     /* Enable interrupts PORTA*/
	
	GPIOA_PDDR &= ~((uint_32)1 << 26);		/* set input PTA 26*/
  #endif
	
  if(TRUE == IRQ_H_ENABLED)
  {
	#if(defined MCU_MK40N512VMD) || (defined MCU_MK53N512CMD100)
		/* PTB5 */
		/* Enable the corresponding pin as GPIO */
		PORTB_PCR5 = PORT_PCR_MUX(1);
		/* Assure that the Interrupt Status Flag (ISF) is cleared before enabling interrupt */
		PORTB_PCR5 |= PORT_PCR_ISF_MASK;
		/* Configure the interrupt on falling edge */
		PORTB_PCR5 |= PORT_PCR_IRQC(IRQC_FALLING_EDGE);
	#endif
	#ifdef MCU_MK60N512VMD100
		/* PTA24 */
		/* Enable the corresponding pin as GPIO */
		PORTA_PCR24 = PORT_PCR_MUX(1);
		/* Assure that the Interrupt Status Flag (ISF) is cleared before enabling interrupt */
		PORTA_PCR24 |= PORT_PCR_ISF_MASK;
		/* Configure the interrupt on falling edge */
		PORTA_PCR24 |= PORT_PCR_IRQC(IRQC_FALLING_EDGE);
	#endif		
  }

  if(TRUE == IRQ_F_ENABLED)
  {
	#if(defined MCU_MK40N512VMD) || (defined MCU_MK53N512CMD100)
		/* PTB6 */
		/* Enable the corresponding pin as GPIO */
		PORTB_PCR6 = PORT_PCR_MUX(1);
		/* Assure that the Interrupt Status Flag (ISF) is cleared before enabling interrupt */
		PORTB_PCR6 |= PORT_PCR_ISF_MASK;
		/* Configure the interrupt on falling edge */
		PORTB_PCR6 |= PORT_PCR_IRQC(IRQC_FALLING_EDGE);
	#endif
	#ifdef MCU_MK60N512VMD100
		/* PTA25 */
		/* Enable the corresponding pin as GPIO */
		PORTA_PCR25 = PORT_PCR_MUX(1);
		/* Assure that the Interrupt Status Flag (ISF) is cleared before enabling interrupt */
		PORTA_PCR25 |= PORT_PCR_ISF_MASK;
		/* Configure the interrupt on falling edge */
		PORTA_PCR25 |= PORT_PCR_IRQC(IRQC_FALLING_EDGE);
	#endif		
  }

  if(TRUE == IRQ_D_ENABLED)
  {
	#if(defined MCU_MK40N512VMD) || (defined MCU_MK53N512CMD100)
	  /* PTB7 */
	  /* Enable the corresponding pin as GPIO */
	  PORTB_PCR7 = PORT_PCR_MUX(1);
	  /* Assure that the Interrupt Status Flag (ISF) is cleared before enabling interrupt */
	  PORTB_PCR7 |= PORT_PCR_ISF_MASK;
	  /* Configure the interrupt on falling edge */
	  PORTB_PCR7 |= PORT_PCR_IRQC(IRQC_FALLING_EDGE);
	#endif
	#ifdef MCU_MK60N512VMD100
		/* PTA26 */
		/* Enable the corresponding pin as GPIO */
		PORTA_PCR26 = PORT_PCR_MUX(1);
		/* Assure that the Interrupt Status Flag (ISF) is cleared before enabling interrupt */
		PORTA_PCR26 |= PORT_PCR_ISF_MASK;
		/* Configure the interrupt on falling edge */
		PORTA_PCR26 |= PORT_PCR_IRQC(IRQC_FALLING_EDGE);
	#endif	
  }

  if(TRUE == IRQ_B_ENABLED)
  {
	#if(defined MCU_MK40N512VMD) || (defined MCU_MK53N512CMD100)
	  /* PTB8 */
	  /* Enable the corresponding pin as GPIO */
	  PORTB_PCR8 = PORT_PCR_MUX(1);
	  /* Assure that the Interrupt Status Flag (ISF) is cleared before enabling interrupt */
	  PORTB_PCR8 |= PORT_PCR_ISF_MASK;
	  /* Configure the interrupt on falling edge */
	  PORTB_PCR8 |= PORT_PCR_IRQC(IRQC_FALLING_EDGE);
	#endif
	#ifdef MCU_MK60N512VMD100
		/* PTA27 */
		/* Enable the corresponding pin as GPIO */
		PORTA_PCR27 = PORT_PCR_MUX(1);
		/* Assure that the Interrupt Status Flag (ISF) is cleared before enabling interrupt */
		PORTA_PCR27 |= PORT_PCR_ISF_MASK;
		/* Configure the interrupt on falling edge */
		PORTA_PCR27 |= PORT_PCR_IRQC(IRQC_FALLING_EDGE);
	#endif	
  }
}


/*FUNCTION*----------------------------------------------------------------
*
* Function Name  :  BatteryCharging_VbusStatusUpdate
* Returned Value :  none
* Comments       :  Function to update VBUS status
*
*END*--------------------------------------------------------------------*/
boolean BatteryCharging_VbusStatusUpdate(void)
{
  uint_8    otg_status_reg;
  boolean   vbus_valid_status;

  /* Update the status from the MAX3353 circuit */
  otg_status_reg = _otg_max3353_get_status();

  /* Update the VBUS status */
  vbus_valid_status = (otg_status_reg & OTG_STAT_VBUS_VALID) ? TRUE : FALSE;

  return vbus_valid_status;
}


/*FUNCTION*----------------------------------------------------------------
*
* Function Name  :  VbusDetect_ISR
* Returned Value :  none
* Comments       :  VBUS Detection ISR function
*
* Note: The VbusDetect_ISR() function should be also assigned in the vector
*       table from the 'vectors.c' file (Vector: 104, IRQ: 88)
*END*--------------------------------------------------------------------*/
void VbusDetect_ISR(void)
{
  static volatile int new_state, old_state;

  /* Acknowledge the interrupt */
  if(TRUE == IRQ_H_ENABLED)
  {
	#if(defined MCU_MK40N512VMD) || (defined MCU_MK53N512CMD100)
	  /* If PTB5 is used */
	  if(PORTB_PCR5 & PORT_PCR_ISF_MASK)
	  {
		  PORTB_PCR5 |= PORT_PCR_ISF_MASK;
	  }
	#endif
	#ifdef MCU_MK60N512VMD100
	  /* If PTA24 is used */
	  if(PORTA_PCR24 & PORT_PCR_ISF_MASK)
	  {
    	PORTA_PCR24 |= PORT_PCR_ISF_MASK;
	  }
	#endif
  }

  if(TRUE == IRQ_F_ENABLED)
  {
	#if(defined MCU_MK40N512VMD) || (defined MCU_MK53N512CMD100)
	  /* If PTB6 is used */
	  if(PORTB_PCR6 & PORT_PCR_ISF_MASK)
	  {
    	PORTB_PCR6 |= PORT_PCR_ISF_MASK;
	  }
	#endif
	#ifdef MCU_MK60N512VMD100
	  /* If PTA25 is used */
	  if(PORTA_PCR25 & PORT_PCR_ISF_MASK)
	  {
    	PORTA_PCR25 |= PORT_PCR_ISF_MASK;
	  }
	#endif	
  }

  if(TRUE == IRQ_D_ENABLED)
  {
	#if(defined MCU_MK40N512VMD) || (defined MCU_MK53N512CMD100)
	  /* If PTB7 is used */
	  if(PORTB_PCR7 & PORT_PCR_ISF_MASK)
	  {
		  PORTB_PCR7 |= PORT_PCR_ISF_MASK;
	  }
	#endif
	#ifdef MCU_MK60N512VMD100
	  /* If PTA26 is used */
	  if(PORTA_PCR26 & PORT_PCR_ISF_MASK)
	  {
    	PORTA_PCR26 |= PORT_PCR_ISF_MASK;
	  }
	#endif
  }

  if(TRUE == IRQ_D_ENABLED)
  {
	#if(defined MCU_MK40N512VMD) || (defined MCU_MK53N512CMD100)
	  /* If PTB8 is used */
	  if(PORTB_PCR8 & PORT_PCR_ISF_MASK)
	  {
		  PORTB_PCR8 |= PORT_PCR_ISF_MASK;
	  }
	#endif
	#ifdef MCU_MK60N512VMD100
	  /* If PTA27 is used */
	  if(PORTA_PCR27 & PORT_PCR_ISF_MASK)
	  {
    	PORTA_PCR27 |= PORT_PCR_ISF_MASK;
	  }
	#endif
  }

  /* Enable the pending interrupt if and only if the VBUS status
     is changed since the last value */
  new_state = _otg_max3353_get_status() & OTG_STAT_VBUS_VALID;
  if(new_state != old_state)
  {
    /* Update the old status */
    old_state = new_state;

    /* Enable the pending vbus detect external interrupt */
    _usb_batt_chg_ext_isr();
  }
}

