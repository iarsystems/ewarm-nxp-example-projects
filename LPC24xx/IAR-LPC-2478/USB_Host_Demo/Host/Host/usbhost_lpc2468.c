/*
**************************************************************************************************************
*                                                 NXP USB Host Stack
*
*                                     (c) Copyright 2008, NXP SemiConductors
*                                     (c) Copyright 2008, OnChip  Technologies LLC
*                                                 All Rights Reserved
*
*                                                  www.nxp.com
*                                               www.onchiptech.com
*
* File           : usbhost_lpc2468.c
* Programmer(s)  : Ravikanth.P
* Version        :
*
**************************************************************************************************************
*/

/*
**************************************************************************************************************
*                                            INCLUDE HEADER FILES
**************************************************************************************************************
*/

#include  "usbhost_lpc2468.h"

/*
**************************************************************************************************************
*                                              GLOBAL VARIABLES
**************************************************************************************************************
*/

volatile  USB_INT32U   HOST_RhscIntr = 0;         /* Root Hub Status Change interrupt                       */
volatile  USB_INT32U   HOST_WdhIntr  = 0;         /* Semaphore to wait until the TD is submitted            */
volatile  HCED        *EDCtrl;                    /* Control endpoint descriptor structure                  */
volatile  HCED        *EDBulkIn;                  /* BulkIn endpoint descriptor  structure                  */
volatile  HCED        *EDBulkOut;                 /* BulkOut endpoint descriptor structure                  */
volatile  HCTD        *TDHead;                    /* Head transfer descriptor structure                     */
volatile  HCTD        *TDTail;                    /* Tail transfer descriptor structure                     */
volatile  HCCA        *Hcca;                      /* Host Controller Communications Area structure          */
volatile  USB_INT08U  *TDBuffer;                  /* Current Buffer Pointer of transfer descriptor          */
volatile  USB_INT08U  *FATBuffer;                 /* Buffer used by FAT file system                         */
volatile  USB_INT08U  *UserBuffer;                /* Buffer used by application                             */

/*
**************************************************************************************************************
*                                         DELAY IN MILLI SECONDS
*
* Description: This function provides a delay in milli seconds
*
* Arguments  : delay    The delay required
*
* Returns    : None
*
**************************************************************************************************************
*/

void  Host_DelayMS (USB_INT32U  delay)
{
    volatile  USB_INT32U  i;


    for (i = 0; i < delay; i++) {
        Host_DelayUS(1000);
    }
}

/*
**************************************************************************************************************
*                                         DELAY IN MICRO SECONDS
*
* Description: This function provides a delay in micro seconds
*
* Arguments  : delay    The delay required
*
* Returns    : None
*
**************************************************************************************************************
*/

void  Host_DelayUS (USB_INT32U  delay)
{
    volatile  USB_INT32U  i;


    for (i = 0; i < (4 * delay); i++) {    /* This logic was tested. It gives app. 1 micro sec delay        */
        ;
    }
}

/*
**************************************************************************************************************
*                                         INITIALIZE THE HOST CONTROLLER
*
* Description: This function initializes lpc2468 host controller
*
* Arguments  : None
*
* Returns    :
*
**************************************************************************************************************
*/

void  Host_Init (void)
{
    USB_INT32U  pinsel;
    USB_INT32U  pinmode;


    PCONP         |= 0x80000000;
    VICINTENCLEAR  = (1 << 22);                               /* Disable the USB interrupt source            */
    OTGCLKCTRL     = 0x00000001;                              /* Enable USB host clock                      */

    while ((OTGCLKST & 0x00000001) == 0) {
    }
                                                              /* P0[12] = USB_PPWR2          01             */
    pinsel    = PINSEL0;
    pinsel   &= 0xFCFFFFFF;
    pinsel   |= 0x01000000;
    PINSEL0   = pinsel;
                                                              /* P0[31] = USB_D+2                           */
    pinsel    = PINSEL1;
    pinsel   &= 0x3FFFFFFF;
    pinsel   |= 0x40000000;
    PINSEL1   = pinsel;
                                                              /* P1[30] = USB_PWRD2          01             */
                                                              /* P1[31] = USB_OVRCR2         01             */
    pinsel    = PINSEL3;
    pinsel   &= 0x0FFFFFFF;
    pinsel   |= 0x50000000;
    PINSEL3   = pinsel;


    PRINT_Log("Initializing Host Stack\n\r");
    Hcca       = (volatile  HCCA       *)0x7FD00000;
    TDHead     = (volatile  HCTD       *)0x7FD00100;
    TDTail     = (volatile  HCTD       *)0x7FD00110;
    EDCtrl     = (volatile  HCED       *)0x7FD00120;
    EDBulkIn   = (volatile  HCED       *)0x7FD00130;
    EDBulkOut  = (volatile  HCED       *)0x7FD00140;
    TDBuffer   = (volatile  USB_INT08U *)0x7FD00150;
    FATBuffer  = (volatile  USB_INT08U *)0x7FD001D0;
    UserBuffer = (volatile  USB_INT08U *)0x7FD01000;

                                                              /* Initialize all the TDs, EDs and HCCA to 0  */
    Host_EDInit(EDCtrl);
    Host_EDInit(EDBulkIn);
    Host_EDInit(EDBulkOut);
    Host_TDInit(TDHead);
    Host_TDInit(TDTail);
    Host_HCCAInit(Hcca);


    Host_DelayMS(50);                                         /* Wait 50 ms before apply reset              */
    HcControl       = 0;                                      /* HARDWARE RESET                             */
    HcControlHeadED = 0;                                      /* Initialize Control list head to Zero       */
    HcBulkHeadED    = 0;                                      /* Initialize Bulk list head to Zero          */

                                                              /* SOFTWARE RESET                             */
    HcCommandStatus = OR_CMD_STATUS_HCR;
    HcFmInterval    = DEFAULT_FMINTERVAL;              /* Write Fm Interval and Largest Data Packet Counter */

                                                              /* Put HC in operational state                */
    HcControl  = (HcControl & (~OR_CONTROL_HCFS)) | OR_CONTROL_HC_OPER;
    HcRhStatus = OR_RH_STATUS_LPSC;                           /* Set Global Power                           */

    HcHCCA = (USB_INT32U)Hcca;
    HcInterruptStatus |= HcInterruptStatus;                   /* Clear Interrrupt Status                    */
                                                              /* Enable  interrupts                         */
    HcInterruptEnable  = OR_INTR_ENABLE_MIE |
                         OR_INTR_ENABLE_WDH |
                         OR_INTR_ENABLE_RHSC;


    VICINTSELECT &= ~(1 << 22);                               /* Configure the ISR handler                  */
    VICVECTADDR22 =  (USB_INT32U)Host_Isr;                    /* Set the vector address                     */
    VICINTENABLE  = (1 << 22);                                /* Enable the USB interrupt source            */
    PRINT_Log("Host Initialized\n\r");
}

/*
**************************************************************************************************************
*                                         INTERRUPT SERVICE ROUTINE
*
* Description: This function services the interrupt caused by host controller
*
* Arguments  : None
*
* Returns    : None
*
**************************************************************************************************************
*/

void  Host_Isr (void)
{
    USB_INT32U   int_status;
    USB_INT32U   ie_status;


    if ((USBINTS & 0x00000008) > 0) {

        int_status    = HcInterruptStatus;                          /* Read Interrupt Status                */
        ie_status     = HcInterruptEnable;                          /* Read Interrupt enable status         */

        if (!(int_status & ie_status)) {
            return;
        } else {

            int_status = int_status & ie_status;

            if (int_status & OR_INTR_STATUS_RHSC) {                 /* Root hub status change interrupt     */
                if (HcRhPortStatus & OR_RH_PORT_CSC) {
                    if (HcRhPortStatus & OR_RH_PORT_CCS) {
                        HOST_RhscIntr = 1;
                    } else {
                        HOST_RhscIntr = 0;
                        PRINT_Log("Device Disconnected\n\r");
                    }
                HcRhPortStatus = OR_RH_PORT_CSC;
                }
                if (HcRhPortStatus & OR_RH_PORT_PRSC) {
                    HcRhPortStatus = OR_RH_PORT_PRSC;
                }
            }
            if (int_status & OR_INTR_STATUS_WDH) {                  /* Writeback Done Head interrupt        */
                HOST_WdhIntr = 1;
            }
            HcInterruptStatus = int_status;                         /* Clear interrupt status register      */
        }
        VICADDRESS = 0;
    }
}

/*
**************************************************************************************************************
*                                     PROCESS TRANSFER DESCRIPTOR
*
* Description: This function processes the transfer descriptor
*
* Arguments  : ed            Endpoint descriptor that contains this transfer descriptor
*              token         SETUP, IN, OUT
*              buffer        Current Buffer Pointer of the transfer descriptor
*              buffer_len    Length of the buffer
*
* Returns    : OK       if TD submission is successful
*              ERROR    if TD submission fails
*
**************************************************************************************************************
*/

USB_INT32S  Host_ProcessTD (volatile  HCED       *ed,
                            volatile  USB_INT32U  token,
                            volatile  USB_INT08U *buffer,
                                      USB_INT32U  buffer_len)
{
    volatile  USB_INT32U   td_toggle;


    if (ed == EDCtrl) {
        if (token == TD_SETUP) {
            td_toggle = TD_TOGGLE_0;
        } else {
            td_toggle = TD_TOGGLE_1;
        }
    } else {
        td_toggle = 0;
    }
    TDHead->Control = (TD_ROUNDING    |
                      token           |
                      TD_DELAY_INT(0) |
                      td_toggle       |
                      TD_CC);
    TDTail->Control = 0;
    TDHead->CurrBufPtr   = (USB_INT32U) buffer;
    TDTail->CurrBufPtr   = 0;
    TDHead->Next         = (USB_INT32U) TDTail;
    TDTail->Next         = 0;
    TDHead->BufEnd       = (USB_INT32U)(buffer + (buffer_len - 1));
    TDTail->BufEnd       = 0;

    ed->HeadTd  = (USB_INT32U)TDHead | ((ed->HeadTd) & 0x00000002);
    ed->TailTd  = (USB_INT32U)TDTail;
    ed->Next    = 0;

    if (ed == EDCtrl) {
        HcControlHeadED = (USB_INT32U)ed;
        HcCommandStatus = HcCommandStatus | OR_CMD_STATUS_CLF;
        HcControl       = HcControl       | OR_CONTROL_CLE;
    } else {
        HcBulkHeadED    = (USB_INT32U)ed;
        HcCommandStatus = HcCommandStatus | OR_CMD_STATUS_BLF;
        HcControl       = HcControl       | OR_CONTROL_BLE;
    }

    Host_WDHWait();

    if (!(TDHead->Control & 0xF0000000)) {
        return (OK);
    } else {
        return (ERR_TD_FAIL);
    }
}

/*
**************************************************************************************************************
*                                       ENUMERATE THE DEVICE
*
* Description: This function is used to enumerate the device connected
*
* Arguments  : None
*
* Returns    : None
*
**************************************************************************************************************
*/

USB_INT32S  Host_EnumDev (void)
{
    USB_INT32S  rc,
                lowSpeed = 0;

	#define EPD0_LowSpeed		0x00002000

    PRINT_Log("Connect a Mass Storage device\n\r");
    while (!HOST_RhscIntr);
    Host_DelayMS(100);                             /* USB 2.0 spec says atleast 50ms delay beore port reset */
    HcRhPortStatus |= OR_RH_PORT_PRS;                                  /* Apply port reset                  */
    Host_DelayMS(100);                                                 /* Wait for 100 MS after port reset  */
    if (HcRhPortStatus & OR_RH_PORT_LSDA)
        lowSpeed = EPD0_LowSpeed;

    EDCtrl->Control = (8 << 16) | lowSpeed;                            /* Put max pkt size = 8              */
                                                                       /* Read first 8 bytes of device desc */
    rc = HOST_GET_DESCRIPTOR(USB_DESCRIPTOR_TYPE_DEVICE, 0, TDBuffer, 8);
    if (rc != OK) {
        PRINT_Err(rc);
        return (rc);
    }
    EDCtrl->Control = (TDBuffer[7] << 16) | lowSpeed;                  /* Get max pkt size of endpoint 0    */
    rc = HOST_SET_ADDRESS(1);                                          /* Set the device address to 1       */
    if (rc != OK) {
        PRINT_Err(rc);
        return (rc);
    }
    Host_DelayMS(2);
    EDCtrl->Control = (EDCtrl->Control) | 1;                          /* Modify control pipe with address 1 */
                                                                      /* Get the configuration descriptor   */
    rc = HOST_GET_DESCRIPTOR(USB_DESCRIPTOR_TYPE_CONFIGURATION, 0, TDBuffer, 9);
    if (rc != OK) {
        PRINT_Err(rc);
        return (rc);
    }
                                                                       /* Get the first configuration data  */
    rc = HOST_GET_DESCRIPTOR(USB_DESCRIPTOR_TYPE_CONFIGURATION, 0, TDBuffer, ReadLE16U(&TDBuffer[2]));
    if (rc != OK) {
        PRINT_Err(rc);
        return (rc);
    }
    rc = MS_ParseConfiguration();                                      /* Parse the configuration           */
    if (rc != OK) {
        PRINT_Err(rc);
        return (rc);
    }
    rc = USBH_SET_CONFIGURATION(1);                                    /* Select device configuration 1     */
    if (rc != OK) {
        PRINT_Err(rc);
    }
    Host_DelayMS(100);                                               /* Some devices may require this delay */
    return (rc);
}

/*
**************************************************************************************************************
*                                        RECEIVE THE CONTROL INFORMATION
*
* Description: This function is used to receive the control information
*
* Arguments  : bm_request_type
*              b_request
*              w_value
*              w_index
*              w_length
*              buffer
*
* Returns    : OK       if Success
*              ERROR    if Failed
*
**************************************************************************************************************
*/

USB_INT32S  Host_CtrlRecv (         USB_INT08U   bm_request_type,
                                    USB_INT08U   b_request,
                                    USB_INT16U   w_value,
                                    USB_INT16U   w_index,
                                    USB_INT16U   w_length,
                          volatile  USB_INT08U  *buffer)
{
    USB_INT32S  rc;


    Host_FillSetup(bm_request_type, b_request, w_value, w_index, w_length);
    rc = Host_ProcessTD(EDCtrl, TD_SETUP, TDBuffer, 8);
    if (rc == OK) {
        if (w_length) {
            rc = Host_ProcessTD(EDCtrl, TD_IN, buffer, w_length);
        }
        if (rc == OK) {
            rc = Host_ProcessTD(EDCtrl, TD_OUT, NULL, 0);
        }
    }
    return (rc);
}

/*
**************************************************************************************************************
*                                         SEND THE CONTROL INFORMATION
*
* Description: This function is used to send the control information
*
* Arguments  : None
*
* Returns    : OK		              if Success
*              ERR_INVALID_BOOTSIG    if Failed
*
**************************************************************************************************************
*/

USB_INT32S  Host_CtrlSend (          USB_INT08U   bm_request_type,
                                     USB_INT08U   b_request,
                                     USB_INT16U   w_value,
                                     USB_INT16U   w_index,
                                     USB_INT16U   w_length,
                           volatile  USB_INT08U  *buffer)
{
    USB_INT32S  rc;


    Host_FillSetup(bm_request_type, b_request, w_value, w_index, w_length);

    rc = Host_ProcessTD(EDCtrl, TD_SETUP, TDBuffer, 8);
    if (rc == OK) {
        if (w_length) {
            rc = Host_ProcessTD(EDCtrl, TD_OUT, buffer, w_length);
        }
        if (rc == OK) {
            rc = Host_ProcessTD(EDCtrl, TD_IN, NULL, 0);
        }
    }
    return (rc);
}

/*
**************************************************************************************************************
*                                          FILL SETUP PACKET
*
* Description: This function is used to fill the setup packet
*
* Arguments  : None
*
* Returns    : OK		              if Success
*              ERR_INVALID_BOOTSIG    if Failed
*
**************************************************************************************************************
*/

void  Host_FillSetup (USB_INT08U   bm_request_type,
                      USB_INT08U   b_request,
                      USB_INT16U   w_value,
                      USB_INT16U   w_index,
                      USB_INT16U   w_length)
{
    TDBuffer[0] = bm_request_type;
    TDBuffer[1] = b_request;
    WriteLE16U(&TDBuffer[2], w_value);
    WriteLE16U(&TDBuffer[4], w_index);
    WriteLE16U(&TDBuffer[6], w_length);
}



/*
**************************************************************************************************************
*                                         INITIALIZE THE TRANSFER DESCRIPTOR
*
* Description: This function initializes transfer descriptor
*
* Arguments  : Pointer to TD structure
*
* Returns    : None
*
**************************************************************************************************************
*/

void  Host_TDInit (volatile  HCTD *td)
{

    td->Control    = 0;
    td->CurrBufPtr = 0;
    td->Next       = 0;
    td->BufEnd     = 0;
}

/*
**************************************************************************************************************
*                                         INITIALIZE THE ENDPOINT DESCRIPTOR
*
* Description: This function initializes endpoint descriptor
*
* Arguments  : Pointer to ED strcuture
*
* Returns    : None
*
**************************************************************************************************************
*/

void  Host_EDInit (volatile  HCED *ed)
{

    ed->Control = 0;
    ed->TailTd  = 0;
    ed->HeadTd  = 0;
    ed->Next    = 0;
}

/*
**************************************************************************************************************
*                                 INITIALIZE HOST CONTROLLER COMMUNICATIONS AREA
*
* Description: This function initializes host controller communications area
*
* Arguments  : Pointer to HCCA
*
* Returns    :
*
**************************************************************************************************************
*/

void  Host_HCCAInit (volatile  HCCA  *hcca)
{
    USB_INT32U  i;


    for (i = 0; i < 32; i++) {

        hcca->IntTable[i] = 0;
        hcca->FrameNumber = 0;
        hcca->DoneHead    = 0;
    }

}

/*
**************************************************************************************************************
*                                         WAIT FOR WDH INTERRUPT
*
* Description: This function is infinite loop which breaks when ever a WDH interrupt rises
*
* Arguments  : None
*
* Returns    : None
*
**************************************************************************************************************
*/

void  Host_WDHWait (void)
{
  while (!HOST_WdhIntr) {
    ;
  }
  HOST_WdhIntr = 0;
}

/*
**************************************************************************************************************
*                                         READ LE 32U
*
* Description: This function is used to read an unsigned integer from a charecter buffer.
*              The number has been saved in little endian mode.
*
* Arguments  : pmem    Pointer to the charecter buffer
*
* Returns    : val     Unsigned integer
*
**************************************************************************************************************
*/

USB_INT32U  ReadLE32U (volatile  USB_INT08U  *pmem)
{
    return (((((pmem[3] << 8) | pmem[2]) << 8) | pmem[1]) << 8) | pmem[0];
}

/*
**************************************************************************************************************
*                                        WRITE LE 32U
*
* Description: This function is used to write an unsigned integer into a charecter buffer
*              in little endian mode.
*
* Arguments  : pmem    Pointer to the charecter buffer
*              val     Integer value to be placed in the charecter buffer
*
* Returns    : None
*
**************************************************************************************************************
*/

void  WriteLE32U (volatile  USB_INT08U  *pmem,
                            USB_INT32U   val)
{
    pmem[0] = val;
    pmem[1] = val >> 8;
    pmem[2] = val >> 16;
    pmem[3] = val >> 24;

}

/*
**************************************************************************************************************
*                                          READ LE 16U
*
* Description: This function is used to read an unsigned short integer from a charecter buffer.
*              The number has been saved in little endian mode.
*
* Arguments  : pmem    Pointer to the charecter buffer
*
* Returns    : val     Unsigned short integer
*
**************************************************************************************************************
*/

USB_INT16U  ReadLE16U (volatile  USB_INT08U  *pmem)
{
    return (pmem[1] << 8) | pmem[0];
}

/*
**************************************************************************************************************
*                                         WRITE LE 16U
*
* Description: This function is used to write an unsigned short integer into a charecter buffer
*              in little endian mode.
*
* Arguments  : pmem    Pointer to the charecter buffer
*              val     Value to be placed in the charecter buffer
*
* Returns    : None
*
**************************************************************************************************************
*/

void  WriteLE16U (volatile  USB_INT08U  *pmem,
                            USB_INT16U   val)
{
    pmem[0] = val;
    pmem[1] = val >> 8;

}

/*
**************************************************************************************************************
*                                         READ BE 32U
*
* Description: This function is used to read an unsigned integer from a charecter buffer.
*              The number has been saved in big endian mode.
*
* Arguments  : pmem    Pointer to the charecter buffer
*
* Returns    : val     Unsigned integer
*
**************************************************************************************************************
*/

USB_INT32U  ReadBE32U (volatile  USB_INT08U  *pmem)
{
    return (((((pmem[0] << 8) | pmem[1]) << 8) | pmem[2]) << 8) | pmem[3];
}

/*
**************************************************************************************************************
*                                         WRITE BE 32U
*
* Description: This function is used to write an unsigned integer into a charecter buffer
*              in big endian mode
*
* Arguments  : pmem    Pointer to the charecter buffer
*              val     Value to be placed in the charecter buffer
*
* Returns    : None
*
**************************************************************************************************************
*/

void  WriteBE32U (volatile  USB_INT08U  *pmem,
                            USB_INT32U   val)
{
    pmem[3] = val;
    pmem[2] = val >> 8;
    pmem[1] = val >> 16;
    pmem[0] = val >> 24;

}

/*
**************************************************************************************************************
*                                         READ BE 16U
*
* Description: This function is used to read an unsigned short integer from a charecter buffer.
*              The number has been saved in big endian mode.
*
* Arguments  : pmem    Pointer to the charecter buffer
*
* Returns    : val     Unsigned short integer
*
**************************************************************************************************************
*/

USB_INT16U  ReadBE16U (volatile  USB_INT08U  *pmem)
{
    return (pmem[0] << 8) | pmem[1];
}

/*
**************************************************************************************************************
*                                         WRITE BE 16U
*
* Description: This function is used to write an unsigned short integer into the charecter buffer
*              in big endian mode
*
* Arguments  : pmem    Pointer to the charecter buffer
*              val     Value to be placed in the charecter buffer
*
* Returns    : None
*
**************************************************************************************************************
*/

void  WriteBE16U (volatile  USB_INT08U  *pmem,
                            USB_INT16U   val)
{
    pmem[0] = val >> 8;
    pmem[1] = val;
}
