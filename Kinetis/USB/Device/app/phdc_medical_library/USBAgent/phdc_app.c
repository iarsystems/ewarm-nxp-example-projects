/******************************************************************************
 *
 * Freescale Semiconductor Inc.
 * (c) Copyright 2004-2010 Freescale Semiconductor, Inc.
 * ALL RIGHTS RESERVED.
 *
 **************************************************************************//*!
 *
 * @file phdc_app.c
 *
 * @author
 *
 * @version
 *
 * @date May-28-2009
 *
 * @brief When the device is connected using continua manager it comes into
 *        operating state and now we can send measurements by pressing 
 *        PTG1/PTG2/PTG3 keys on the board
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "phdc_app.h"       /* PAN Application Header File */

#if (defined _MCF51MM256_H) || (defined _MCF51JE256_H)
#include "exceptions.h"
#endif
/*****************************************************************************
 * Constant and Macro's 
 *****************************************************************************/
#define SHIM_COUNT		1
#define OBSERVATION_SCAN_SIZE   512
#define SHIMID  SHIM_USB
/*****************************************************************************
 * Global Functions Prototypes
 *****************************************************************************/
extern void display_led(uint_8 val);

/****************************************************************************
 * Global Variables - None
 ****************************************************************************/
/* Global DIM variable */
extern DIM g_DIM;
/* Data Proto List */
extern const intu8 g_DataProtoList[];
/*****************************************************************************
 * Local Types - None
 *****************************************************************************/
/*****************************************************************************
 * Local Functions Prototypes
 *****************************************************************************/
#ifdef TIMER_CALLBACK_ARG
static void SelectTimerCallback(void * arg);
#else
static void SelectTimerCallback(void);
#endif
/* Application Callback Function */
static void MedAppCallback(IEEE11073_EVENT event_id, void *pvoid);
/* Function to check any button pressed */
static void Button_Pressed(void);
/* Function to Send Peri Scanner Measurements */
static void SendPeriScannerMsr(HANDLE handle);
static void UpdateBPMeasurements(void); 
static void UpdateTime(void);
static void UpdateTemperature(void);
static ERR_CODE SendBPMeasurements(void);
static ERR_CODE SendTempMeasurements(void);
static uint_8 Val2HexBCD(uint_8 val);

ERR_CODE RTCSetTime(AbsoluteTime date_time, FLOAT_Type accuracy);
ERR_CODE RTCGetTime(AbsoluteTime *pdate_time);
void GetAbsoluteTime(AbsoluteTime *pAbsTime);
void GetRelativeTime(RelativeTime *pRelTime);
void GetHiResRelativeTime(HighResRelativeTime *pHiResRelTime);
ERR_CODE WritePMSegment(HANDLE Handle, InstNumber InstNum, void *pData, intu32 size);
/*****************************************************************************
 * Local Variables
 *****************************************************************************/

/* Shim Array */
const PSHIM ShimArray[SHIM_COUNT] = 
{
	(const PSHIM)&USBShim
};

const TIL g_Til = 
{
	SHIM_COUNT,
	(PSHIM (*)[])&ShimArray
};

/* variable to store timer index */
uint_8 g_app_timer = (uint_8)INVALID_TIME_COUNT;

/* Set to true when device reaches operating state 
   and is ready to send measurements */
uint_8 g_send_msr = FALSE;

/* Set to true when an event report send is complete */
volatile uint_8 event_rpt_sent;

/* Set to true when manager triggers Segment data transmission 
   and segment has data to send */
uint_8 g_send_PmSegmData = FALSE; 

/* Array  for temperature measurements */
static uint_8 temp[] = 
{
#ifdef LITTLE_ENDIAN	
	0x20, 0x75, 0x38, 0xFB
#else
	0xFB, 0x38, 0x75, 0x20
#endif
};

/* Array for Absolute Time Stamp - BCD encoded */
static uint_8 time[] = 
{
    0x20, 0x09, 0x08, 0x09,
    0x06, 0x30, 0x00, 0x00
};

/* Internally managed Absolute Time */
static uint_8 absoluteTimeMinutes = 30;
static uint_8 absoluteTimeHours = 6;

/* Array for Enumeration Class data */
static uint_8 Enum[] = 
{
    0x02, 0x14
};

/* Array for Blood Pressure measurements */
static uint_8 BloodPressure[] = 
{
    0x00, 0x03, 
    0x00, 0x06,
    0x00, 0x78,
    0x00, 0x50,
    0x00, 0x64
};

/* Array to store pointers to the Observation Scan List 
   and Scan Per Var List */
static uint_8 *gAppBuffer;

/* Pointers to the Observation Scan List */
const ObservationScanList* gObsPtr[2];

/* Pointers to the Scan Report Per Var List. 
   Used to Store Multi Person Measurements */
const ScanReportPerVarList* gPerObsPtr[2];

/* To Store PM Segment Information */
PMSEGDATAXFER g_PmSegmDataXfer;
/*****************************************************************************
 * Local Functions
 *****************************************************************************/
#if (defined(_MC9S08MM128_H) || defined(_MC9S08JE128_H))
#pragma CODE_SEG DEFAULT
#endif
/******************************************************************************
 *
 *   @name        TestApp_Init
 *
 *   @brief       This function is the entry for the PAN Application
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************
 * This function starts the PAN device application
 *****************************************************************************/
void TestApp_Init(void)
{
    DisableInterrupts;
	#if (defined _MCF51MM256_H) || (defined _MCF51JE256_H)
	usb_int_dis();
	#endif	
    event_rpt_sent = TRUE;
    gAppBuffer = (intu8*)mempool_malloc(OBSERVATION_SCAN_SIZE);
    gObsPtr[0] = (ObservationScanList*)&gAppBuffer[0];
    gObsPtr[1] = (ObservationScanList*)&gAppBuffer[100];
    gPerObsPtr[0] = (ScanReportPerVarList*)&gAppBuffer[200];
    gPerObsPtr[1] = (ScanReportPerVarList*)&gAppBuffer[300];

	/* Initialize TIL */
	TIL_Initialize((PTIL)&g_Til);
	/* Initialize IEEE11073 and start Transport */
	(void)Ieee11073Initialize((PTIL)&g_Til, SHIMID, MedAppCallback);

    (void)memset(gAppBuffer, 0x00, OBSERVATION_SCAN_SIZE);
    
    EnableInterrupts;
	#if (defined _MCF51MM256_H) || (defined _MCF51JE256_H)
	usb_int_en();
	#endif	
}

/******************************************************************************
 *
 *   @name        TestApp_Task
 *
 *   @brief       Application task function. It is called from the main loop
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************
 * Application task function. It is called from the main loop
 *****************************************************************************/
void TestApp_Task(void)
{

        if(g_send_msr == TRUE)
        {
            /* When Device is ready to send measurements 
               check for any Button Pressed */
            Button_Pressed();
        }
        
        /* Segment Data Transfer Triggered */
        if(g_send_PmSegmData == TRUE)
        {
            SegmentDataEvent *pSegm = (SegmentDataEvent *)g_SegmDataEvent;
            pSegm->segm_data_event_descr.segm_instance = ieee_ntohs(g_PmSegmDataXfer.SegInstNum);
            /* Send Segment Data */
            (void)SendSegmentData((SegmentDataEvent*)&g_SegmDataEvent, g_PmSegmDataXfer.Handle);

            /* Clear Segment Data Flag */      
            g_send_PmSegmData = FALSE;
        }
}

/******************************************************************************
 *
 *   @name        UpdateBPMeasurements
 *
 *   @brief       This function updates Blood Pressure Measurements to be sent 
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************
 * This function updates Blood Pressure Measurements
 *****************************************************************************/
void UpdateBPMeasurements(void)
{
    /* update Blood pressure measurements */
    BloodPressure[5]++;
    if(BloodPressure[5] == 0) 
    {
     BloodPressure[5] = 0x78;
    }
    
    BloodPressure[7]++;
    if(BloodPressure[7] == 0) 
    {
     BloodPressure[7] = 0x50;
    }
    
    BloodPressure[9] = (uint_8)((BloodPressure[5] + BloodPressure[7])/2);
}

/******************************************************************************
 *
 *   @name        Val2HexBCD
 *
 *   @brief       This function converts an 8 bit integer value into a Hex BCD representation
 *
 *   @param       value to be converted in BCD
 *
 *   @return      BCD correspondent
 *
 *****************************************************************************
 * This function converts an 8 bit integer value into Hex BCD
 *****************************************************************************/
static uint_8 Val2HexBCD(uint_8 val) 
{
 uint_8 ret = 0; /* intial value for the return BCD */
 uint_8 pow = 1; /* initial value for the power multiplier = 16^0) */
 
 while(val)
 { 
  uint_8 r, d;
  d = (uint_8)(val / 10);
  r = (uint_8)(val - (d * (uint_8)10));
  
  ret += r*pow;
  
  pow *= 16; /* 16^i */
  val = d;
 }
 
 return ret;
}
/******************************************************************************
 *
 *   @name        UpdateTime
 *
 *   @brief       This function Updates Absolute Time Stamp of the Measurements
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************
 * This function Updates Absolute Time Stamp of the Measurements
 *****************************************************************************/
void UpdateTime(void)
{
    /* update Absolute Time Stamp */
    absoluteTimeMinutes++;
    if(absoluteTimeMinutes >= 60)
    {
        absoluteTimeMinutes = 0;
        absoluteTimeHours++;
        if(absoluteTimeHours >= 24)
        {
            absoluteTimeHours = 0;
        }
    }
    /* Convert into BCD values */
    time[4] = Val2HexBCD(absoluteTimeHours);
    time[5] = Val2HexBCD(absoluteTimeMinutes);
}
/******************************************************************************
 *
 *   @name        UpdateTemperature
 *
 *   @brief       This function Updates Temperature Measurements to be sent
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************
 * This function Updates Temperature Measurements
 *****************************************************************************/
void UpdateTemperature(void)
{
    /* update Temperature Measurements */
#ifdef LITTLE_ENDIAN
    temp[2]++;
#else
    temp[1]++;
#endif
}
/******************************************************************************
 *
 *   @name        SendBPMeasurements
 *
 *   @brief       This function adds Blood pressure Measurements to the 
 *                Observation Scan List and Scan Report Per Var List
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************
 * This function adds Blood pressure Measurements to the Observation Scan List 
 * and Scan Report Per Var List
 *****************************************************************************/
ERR_CODE SendBPMeasurements(void)
{
    ERR_CODE err = ERROR_SUCCESS;
    
    do
    {
        UpdateBPMeasurements();
        /* Add Blood Pressure Measurements to Observation Scan List */
        if(ERROR_SUCCESS != (err = AddEntryToObsScanList(1, 
            MDC_ATTR_NU_CMPD_VAL_OBS_BASIC, 10, (void*)&BloodPressure[0], 
            (ObservationScanList*)gAppBuffer)))
        {
            break;
        }
        UpdateTime();
        /* Add Absolute Time Stamp to Observation Scan List*/
        if(ERROR_SUCCESS != (err = AddEntryToObsScanList(1, 
            MDC_ATTR_TIME_STAMP_ABS, 8, (void*)&time[0], 
            (ObservationScanList*)gAppBuffer)))
        {
            break;
        }
        UpdateBPMeasurements();
        /* Add Blood Pressure Measurements to Observation Scan List */
        if(ERROR_SUCCESS != (err = AddEntryToObsScanList(1, 
            MDC_ATTR_NU_CMPD_VAL_OBS_BASIC, 10, (void*)&BloodPressure[0], 
            (ObservationScanList*)&gAppBuffer[100])))
        {
            break;
        }
        UpdateTime();
        /* Add Absolute Time Stamp to Observation Scan List*/
        if(ERROR_SUCCESS != (err = AddEntryToObsScanList(1, 
            MDC_ATTR_TIME_STAMP_ABS, 8, (void*)&time[0], 
            (ObservationScanList*)&gAppBuffer[100])))
        {
            break;
        }
        UpdateBPMeasurements();
        /* Add Blood Pressure Measurements to Observation Scan List */
        if(ERROR_SUCCESS != (err = AddEntryToScanRptPerVarList(1, 1, 
            MDC_ATTR_NU_CMPD_VAL_OBS_BASIC, 10, (void*)&BloodPressure[0], 
            (ScanReportPerVarList*)&gAppBuffer[200])))
        {
            break;
        }
        UpdateTime();
        /* Add Absolute Time Stamp to Observation Scan List*/
        if(ERROR_SUCCESS != (err = AddEntryToScanRptPerVarList(1, 1, 
            MDC_ATTR_TIME_STAMP_ABS, 8, (void*)&time[0], 
            (ScanReportPerVarList*)&gAppBuffer[200])))
        {
            break;
        }
        UpdateBPMeasurements();
        /* Add Blood Pressure Measurements to Scan Report Per Var List */
        if(ERROR_SUCCESS != (err = AddEntryToScanRptPerVarList(1, 2, 
            MDC_ATTR_NU_CMPD_VAL_OBS_BASIC, 10, (void*)&BloodPressure[0], 
            (ScanReportPerVarList*)&gAppBuffer[200])))
        {
            break;
        }
        UpdateTime();
        /* Add Absolute Time Stamp to Scan Report Per Var List*/
        if(ERROR_SUCCESS != (err = AddEntryToScanRptPerVarList(1, 2, 
            MDC_ATTR_TIME_STAMP_ABS, 8, (void*)&time[0], 
            (ScanReportPerVarList*)&gAppBuffer[200])))
        {
            break;
        }

    }while(0);
    if(ERROR_SUCCESS != err)
    {
        /* If there is an error, clear memory */
        (void)memset(gAppBuffer, 0x00, OBSERVATION_SCAN_SIZE);
    }
    return err;
}


/******************************************************************************
 *
 *   @name        SendTempMeasurements
 *
 *   @brief       This function adds Temperature Measurements to the 
 *                Observation Scan List and Scan Report Per Var List
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************
 * This function adds Temperature Measurements to the Observation Scan List 
 * and Scan Report Per Var List
 *****************************************************************************/
ERR_CODE SendTempMeasurements(void)
{
    ERR_CODE err = ERROR_SUCCESS;
    
    do
    {
        UpdateTemperature();
        /* Add Temperature Measurements to Observation Scan List */
        if(ERROR_SUCCESS != (err = AddEntryToObsScanList(2, 
            MDC_ATTR_NU_VAL_OBS_SIMP, 4, (void*)&temp[0], 
            (ObservationScanList*)gAppBuffer)))
        {
            break;
        }
        UpdateTime();
        /* Add Absolute Time Stamp to Observation Scan List*/
        if(ERROR_SUCCESS != (err = AddEntryToObsScanList(2, 
            MDC_ATTR_TIME_STAMP_ABS, 8, (void*)&time[0], 
            (ObservationScanList*)gAppBuffer)))
        {
            break;
        }
        /* Add Enumeration Class Measurements to Observation Scan List*/
        if(ERROR_SUCCESS != (err = AddEntryToObsScanList(7, 
            MDC_ATTR_ENUM_OBS_VAL_SIMP_OID, 2, (void*)&Enum[0], 
            (ObservationScanList*)gAppBuffer)))
        {
            break;
        }
        UpdateTime();
        /* Add Absolute Time Stamp to Observation Scan List*/
        if(ERROR_SUCCESS != (err = AddEntryToObsScanList(7, 
            MDC_ATTR_TIME_STAMP_ABS, 8, (void*)&time[0], 
            (ObservationScanList*)gAppBuffer)))
        {
            break;
        }
        UpdateTemperature();
        /* Add Temperature Measurements to Observation Scan List */
        if(ERROR_SUCCESS != (err = AddEntryToObsScanList(2, 
            MDC_ATTR_NU_VAL_OBS_SIMP, 4, (void*)&temp[0], 
            (ObservationScanList*)&gAppBuffer[100])))
        {
            break;
        }
        UpdateTime();
        /* Add Absolute Time Stamp to Observation Scan List*/
        if(ERROR_SUCCESS != (err = AddEntryToObsScanList(2, 
            MDC_ATTR_TIME_STAMP_ABS, 8, (void*)&time[0], 
            (ObservationScanList*)&gAppBuffer[100])))
        {
            break;
        }
        /* Add Enumeration Class Measurements to Observation Scan List*/
        if(ERROR_SUCCESS != (err = AddEntryToObsScanList(7, 
            MDC_ATTR_ENUM_OBS_VAL_SIMP_OID, 2, (void*)&Enum[0], 
            (ObservationScanList*)&gAppBuffer[100])))
        {
            break;
        }
        UpdateTime();
        /* Add Absolute Time Stamp to Observation Scan List*/
        if(ERROR_SUCCESS != (err = AddEntryToObsScanList(7, 
            MDC_ATTR_TIME_STAMP_ABS, 8, (void*)&time[0], 
            (ObservationScanList*)&gAppBuffer[100])))
        {
            break;
        }
        UpdateTemperature();
        /* Add Temperature Measurements to Scan Report Per Var List */
        if(ERROR_SUCCESS != (err = AddEntryToScanRptPerVarList(2, 1, 
            MDC_ATTR_NU_VAL_OBS_SIMP, 4, (void*)&temp[0], 
            (ScanReportPerVarList*)&gAppBuffer[200])))
        {
            break;
        }
        UpdateTime();
        /* Add Absolute Time Stamp to Scan Report Per Var List*/
        if(ERROR_SUCCESS != (err = AddEntryToScanRptPerVarList(2, 1, 
            MDC_ATTR_TIME_STAMP_ABS, 8, (void*)&time[0], 
            (ScanReportPerVarList*)&gAppBuffer[200])))
        {
            break;
        }
        UpdateTemperature();
        /* Add Temperature Measurements to Scan Report Per Var List */
        if(ERROR_SUCCESS != (err = AddEntryToScanRptPerVarList(2, 2, 
            MDC_ATTR_NU_VAL_OBS_SIMP, 4, (void*)&temp[0], 
            (ScanReportPerVarList*)&gAppBuffer[200])))
        {
            break;
        }
        UpdateTime();
        /* Add Absolute Time Stamp to Scan Report Per Var List*/
        if(ERROR_SUCCESS != (err = AddEntryToScanRptPerVarList(2, 2, 
            MDC_ATTR_TIME_STAMP_ABS, 8, (void*)&time[0], 
            (ScanReportPerVarList*)&gAppBuffer[200])))
        {
            break;
        }
    }while(0);
    
    if(ERROR_SUCCESS != err)
    {
        /* If there is an error, clear memory */
        (void)memset(gAppBuffer, 0x00, OBSERVATION_SCAN_SIZE);
    }
    return err;
}

/******************************************************************************
 *
 *   @name        Button_Pressed
 *
 *   @brief       This Function Checks for any button pressed on the Keyboard 
 *                and sends the measurements 
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************
 * This Function Checks for any button pressed on the Keyboard and sends the 
 * measurements
 *****************************************************************************/
static void Button_Pressed(void)
{
    ERR_CODE err = ERROR_SUCCESS;
    if(kbi_stat > 0)
    {
        switch(kbi_stat & KBI_STAT_MASK)
        {
            case SEND_BPM_MSR : /* PTG1 is pressed */
            {
                if(event_rpt_sent)
                {
                    err = SendBPMeasurements();
                    if(ERROR_SUCCESS != err)
                    {
                        break;
                    }
                    event_rpt_sent = FALSE;
                    /* Send Multi Person Variable Format Event Report */
                    err = AgentSendPersonMeasurements(
                        (ScanReportPerVarList* (*)[])gPerObsPtr, 0, 
                        SCAN_REPORT_MP_VAR, 1, TRUE);
                    if(ERROR_SUCCESS != err)
                    {
                        /* If any error, clear memory */
                        event_rpt_sent = TRUE;
                        (void)memset(gAppBuffer, 0x00, OBSERVATION_SCAN_SIZE);
                    }
                }
            }
            break;

            case SEND_TEMPERATURE_MSR : /* PTG2 is pressed */            
            {
                if(event_rpt_sent)
                {
                    err = SendTempMeasurements(); 
                    if(ERROR_SUCCESS != err)
                    {
                        break;
                    }
                    event_rpt_sent = FALSE;
                    /* Send Fixed Format Event Report */
                    err = AgentSendMeasurements(
                        (ObservationScanList* (*)[])gObsPtr, 0, 
                        SCAN_REPORT_FIXED, 2, TRUE);
                    if(ERROR_SUCCESS != err)
                    {
                        /* If any error, clear memory */
                        event_rpt_sent = TRUE;
                        (void)memset(gAppBuffer, 0x00, OBSERVATION_SCAN_SIZE);
                    }
                } 
            }
            break;

            case SEND_EPISOIDIC_SCANNER_MSR: /* PTG3 is pressed*/
            {            
            #ifdef MDS_EPISOIDIC_SCANNER
                if(event_rpt_sent)
                {
                    err = SendTempMeasurements(); 
                    if(ERROR_SUCCESS != err)
                    {
                        break;
                    }
                    event_rpt_sent = FALSE;
                    /* Send Multi Person Grouped Format Event Report */
                    err = AgentSendPersonMeasurements(
                        (ScanReportPerVarList* (*)[])gPerObsPtr, 5, 
                        SCAN_REPORT_MP_GROUPED, 1, TRUE);
                    if(ERROR_SUCCESS != err)
                    {
                        /* If any error, clear memory */
                        event_rpt_sent = TRUE;
                        (void)memset(gAppBuffer, 0x00, OBSERVATION_SCAN_SIZE);
                    }
                }
            #endif 
            }
            break;
            
            /* otherwise */ 
            default:break;
        }
        /* reset status after servicing interrupt */
        kbi_stat = 0x00; 
    }
    return;
}
/******************************************************************************
 *
 *   @name        SelectTimerCallback
 *
 *   @brief       This function is called when the timer used for selecting the
 *                the device specialization fires
 *
 *   @param       arg   : Argument passed by Timer ISR (optional)
 *
 *   @return      None
 *
 *****************************************************************************
 * This function when called initiates the association procedure for the 
 * selected device specialization
 *****************************************************************************/
#ifdef TIMER_CALLBACK_ARG
static void SelectTimerCallback(void * arg)
#else
static void SelectTimerCallback(void)
#endif
{    
#ifdef TIMER_CALLBACK_ARG
    UNUSED (arg)
#endif
    g_app_timer = (uint_8)INVALID_TIME_COUNT;

    /* Send Association Request */
    (void)AgentSendAssociationRequest((DataProtoList*)g_DataProtoList);
}
/******************************************************************************
 *
 *   @name        SendPeriScannerMsr
 *
 *   @brief       This Function Sends Periodic Scanner measurements 
 *
 *   @param       handle    :   Handle of the Periodic Scanner Object
 *
 *   @return      None
 *
 *****************************************************************************
 * This function sends Periodic scanner measurements
 *****************************************************************************/
static void SendPeriScannerMsr(HANDLE handle)
{
    ERR_CODE err = ERROR_SUCCESS;
    if(event_rpt_sent)
    {
        err = SendTempMeasurements(); 
        if(ERROR_SUCCESS != err)
        {
            return;
        }
        event_rpt_sent = FALSE;
        /* Send Multi Person Grouped Format Event Report */
        err = AgentSendPersonMeasurements(
            (ScanReportPerVarList* (*)[])gPerObsPtr, handle, 
            SCAN_REPORT_MP_GROUPED, 1, TRUE);
        if(ERROR_SUCCESS != err)
        {
            /* If any error, clear memory */
            event_rpt_sent = TRUE;
            (void)memset(gAppBuffer, 0x00, OBSERVATION_SCAN_SIZE);
        }
    }
    return;
}

/******************************************************************************
 *
 *   @name        MedAppCallback
 *
 *   @brief       This function is a callback Function and is called whenever 
 *                an event occurs
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************
 * This function is a callback Function and is called whenever an event occurs
 *****************************************************************************/
static void MedAppCallback(IEEE11073_EVENT event_id, void *pvoid)
{
	switch(event_id)
	{
	    case IEEE11073_ASSOCIATION_RELEASING:
	        break;
	    case IEEE11073_ASSOCIATION_RELEASED:       
		case IEEE11073_TRANSPORT_CONNECT: 
			{
                TIMER_OBJECT TimerObject;
                TimerObject.msCount = SELECT_TIMEOUT;
                TimerObject.pfnTimerCallback = SelectTimerCallback;
                g_app_timer = AddTimerQ(&TimerObject);
			}
			break;
		case IEEE11073_TRANSPORT_DISCONNECT:
			/* Clean Up Application */
            memset(gAppBuffer, 0x00, OBSERVATION_SCAN_SIZE);
            event_rpt_sent = TRUE;
            (void)RemoveTimerQ(g_app_timer);
			break;
	    case IEEE11073_INITIALIZE_DIM:
	    {
	        /* return Global DIM variable */
	        *(PDIM*)pvoid = &g_DIM;
	        break;
	    }
	    case IEEE11073_GET_DATAPROTO:
	    {
	        /* Return Data Proto List */
	        *(DataProtoList**)pvoid = (DataProtoList*)&g_DataProtoList;
	        break;
	    }
	    case IEEE11073_OPERATING:
	    {	        
	        /* Ready to send Measurements */
	        g_send_msr = TRUE; 
	        break;
	    }
#ifdef MDS_PERIODIC_SCANNER
	    case IEEE11073_PERIODIC_SCANNER_EVENT:
	    {
	        /* Send Peri Scanner measurements */
	        /* Event comes periodically */
	        HANDLE handle = *(HANDLE *)pvoid;
	        SendPeriScannerMsr(handle);
	        break;
	    }
#endif
	    case IEEE11073_EVNTRPT_SENT:
	    {
	        event_rpt_sent = TRUE;
	        /* Clear the measurements sent */
            (void)memset(gAppBuffer, 0x00, OBSERVATION_SCAN_SIZE);	        
	        break;
	    }

	    case IEEE11073_TRIG_PMSEGMENT:
        {
            PTRIGSEGMDATAXFRRSP pRsp = (PTRIGSEGMDATAXFRRSP)pvoid;
            SegmentDataEvent *pSegm = (SegmentDataEvent *)g_SegmDataEvent;
            /* Set the segment status */
            pRsp->TrigSegmDataRsp.trig_segm_xfer_rsp = TSXR_SUCCESSFUL;	            

            /* Fetch PM Segment Data to be transferred */
            g_PmSegmDataXfer.Handle = pRsp->Handle;
            g_PmSegmDataXfer.SegInstNum = pRsp->TrigSegmDataRsp.seg_inst_no;
            /* If Segment has data to send and is not active */
            g_send_PmSegmData = TRUE;
            return;
        }
	    case IEEE11073_CLEAR_PMSEGMENT:
        {
            /* Erase PM Segment Data */
            PCLRPMSEGMINFO pClrSegmInfo = (PCLRPMSEGMINFO)pvoid;
	        break;
        }
        
		default:
			break;
	}
	return;
}
/*==========================EXTERNAL FUNCTIONs===================*/
/******************************************************************************
 *
 *   @name        RTCSetTime
 *
 *   @brief       This function should set the RTC with the input date 
 *                and time
 *
 *   @param       None
 *
 *   @return      ERROR_SUCCESS     :   always
 *
 *****************************************************************************
 * To be implemented  
 *****************************************************************************/
ERR_CODE RTCSetTime(
	AbsoluteTime date_time, 
	FLOAT_Type accuracy
)
{
    UNUSED(accuracy);
    UNUSED(date_time);
	return ERROR_SUCCESS;
}
/******************************************************************************
 *
 *   @name        RTCGetTime
 *
 *   @brief       This function should return the RTC date and time
 *
 *   @param       None
 *
 *   @return      ERROR_SUCCESS     :   always
 *
 *****************************************************************************
 * To be implemented  
 *****************************************************************************/
ERR_CODE RTCGetTime(
	AbsoluteTime *pdate_time
)
{
    UNUSED(pdate_time);
    return ERROR_SUCCESS;
}
/******************************************************************************
 *
 *   @name        GetAbsoluteTime
 *
 *   @brief       This function should return the Absolute Time 
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************
 * To be implemented  
 *****************************************************************************/
void GetAbsoluteTime(
	AbsoluteTime *pAbsTime
)
{
    UNUSED(pAbsTime);
}
/******************************************************************************
 *
 *   @name        GetRelativeTime
 *
 *   @brief       This function should return the Relative Time
 *                (125 Micro Seconds Resolution) 
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************
 * To be implemented  
 *****************************************************************************/
void GetRelativeTime(
	RelativeTime *pRelTime
)
{
    UNUSED(pRelTime);
}
/******************************************************************************
 *
 *   @name        GetHiResRelativeTime
 *
 *   @brief       This function should return the Hi Resolution Relative Time 
 *                (1 Micro Seconds Resolution) 
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************
 * To be implemented  
 *****************************************************************************/
void GetHiResRelativeTime(
	HighResRelativeTime *pHiResRelTime
)
{
    UNUSED(pHiResRelTime);
}
/******************************************************************************
 *
 *   @name        WritePMSegment
 *
 *   @brief       This function write the PM Segment Data into Flash
 *
 *   @param       None
 *
 *   @return      ERROR_SUCCESS     :   always
 *
 *****************************************************************************
 * To be implemented  
 *****************************************************************************/
ERR_CODE WritePMSegment(
	HANDLE Handle, 
	InstNumber InstNum, 
    void *pData, 
	intu32 size
)
{
    UNUSED(Handle)
    UNUSED(InstNum)
    UNUSED(pData)
    UNUSED(size)
    return ERROR_SUCCESS;   
}
