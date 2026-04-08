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
#define SHIMID  SHIM_USB

#define MEASUREMENTS_TYPE_BP            0
#define MEASUREMENTS_TYPE_PULSE         1

#define OBSERVATION_BUFFER_SIZE         1080     /* application buffer for observations in bytes */
#define OBSERVATION_SCAN_COUNT          30      /* number observations supported by the application */
#define OBSERVATION_SCAN_SIZE           36      /* reserved space in the application buffer for each observation */

#define SFLOAT_NAN                      0x07FF

#define TIMER_NOT_STARTED               0xFF

#define SEND_OBS_SCAN_COUNT_MAX         24      /* Maximum number of observations scans that can be sent in one message (24 is maximum allowed by the standard) */

#if (OBSERVATION_SCAN_COUNT == 0)
 #error "OBSERVATION_SCAN_COUNT cannot be 0"
#endif 

#if ((OBSERVATION_SCAN_SIZE * OBSERVATION_SCAN_COUNT) > OBSERVATION_BUFFER_SIZE)
 #error "Not enough space in the OBSERVATION_BUFFER to hold OBSERVATION_SCAN_COUNT observations"
#endif

/*****************************************************************************
 * Global Functions Prototypes
 *****************************************************************************/

/****************************************************************************
 * Global Variables - None
 ****************************************************************************/
/* Global DIM variable */
extern DIM g_DIM;
/* Data Proto List */
extern const intu8 g_DataProtoList[];
/*****************************************************************************
 * Local Types
 *****************************************************************************/
#pragma pack(1)
/* Structure to hold the sampled BP measurements */
typedef struct _BPSAMPLEMEASUREMENTS
{
  uint_8       type;         // 0 = BP (systolic,diastolic,mean) / 1 = Pulse
  union
  {    
   struct
   {    
    uint_16       systolic;
    uint_16       diastolic;
    uint_16       mean;
   }bp;
    
    uint_16       pulse;
  }u;
  AbsoluteTime timestamp;
}BPSAMPLEMEASUREMENTS;
#pragma options align=reset
/*****************************************************************************
 * Local Functions Prototypes
 *****************************************************************************/
#ifdef TIMER_CALLBACK_ARG
static void   SelectTimerCallback(void * arg);
#else
static void   SelectTimerCallback(void);
#endif

#ifdef TIMER_CALLBACK_ARG
static void   SecondsTimerCallback(void * arg);
#else
static void   SecondsTimerCallback(void);
#endif

/* Application Callback Function */
static void   MedAppCallback(IEEE11073_EVENT event_id, void *pvoid);
/* Function to check any button pressed */
static void   Button_Pressed(void);

static void   UpdateBPMeasurements(void); 
static void   UpdatePulseMeasurements(void);

static void   UpdateAbsoluteTime(AbsoluteTime *absTime);

static void   AppSendMeasurements(void);

static uint_8 ValDec2HexBCD(uint_8 val);
static uint_8 HexBCD2ValDec(uint_8 val);

static void   StartSendMeasurementsTimer(void);

ERR_CODE RTCSetTime(AbsoluteTime date_time, FLOAT_Type accuracy);
ERR_CODE RTCGetTime(AbsoluteTime *pdate_time);
void GetAbsoluteTime(AbsoluteTime *pAbsTime);
void GetHiResRelativeTime(HighResRelativeTime *pHiResRelTime);
void GetRelativeTime(RelativeTime *pRelTime);

#ifdef TIMER_CALLBACK_ARG
static void   MeasurementsTimerCallback(void * arg);
#else
static void   MeasurementsTimerCallback(void);
#endif
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
uint_8 g_app_timer = TIMER_NOT_STARTED;

/* Set to true when device reaches operating state 
   and is ready to send measurements */
uint_8 g_send_msr = FALSE;

/* Set to true when an event report send is complete */
volatile uint_8 event_rpt_sent;

/* Internally managed Absolute Time. Default start time is 2010-4-26 09:20:17 */
/* The absolute time will be set by the manager if MDS_SETTABLE_TIME and MDS_TIME_CAPAB_SET_CLOCK are set */
static uint_8 timeYear   = 10;   /* Year */                   
static uint_8 timeMonth  = 4;   /* Month */
static uint_8 timeDay    = 26;   /* Day */
static uint_8 timeHour   = 9;   /* Hour */
static uint_8 timeMinute = 20;   /* Minute */
static uint_8 timeSecond = 17;   /* Second */

/* Measurments source (simulated). 
 * A real Blood Pressure application will replace these values 
 * with ones taken from measuremnts sensors
 */
static uint_8 g_SimSystolic   = 0x78;
static uint_8 g_SimDiastolic  = 0x50;
static uint_8 g_SimMean       = 0x64;
static uint_8 g_SimPulse      = 0x3C;

/* Array to store pointers to the Observation Scan List 
   and Scan Per Var List */
static uint_8 *gAppBuffer;

/* Pointers to the Observation Scan List */
const ObservationScanList* gObsPtr[OBSERVATION_SCAN_COUNT];

/* BP Measurements arrays. Used also for storing data when the connection is not available */
BPSAMPLEMEASUREMENTS  g_BpMesArray[OBSERVATION_SCAN_COUNT];
  
AbsoluteTime g_MdcAttrTimeAbs = 
{
  
       0x20,    /* Century */
       0x10,    /* Year */                   
       0x04,    /* Month */
       0x26,    /* Day */
       0x19,    /* Hour */
       0x30,    /* Minute */
       0x10,    /* Second */
       0x00     /* Second Fraction */
};

#ifdef MDS_ADJUST_ABSOLUTE_TIME
AbsoluteTimeAdjust g_MdcAbsTimeadjust = 
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
#endif

#ifdef MDS_POWER_STATUS
PowerStatus g_MdcAttrPowerStatus = ON_MAINS;
#endif

#ifdef MDS_BATT_CHARGE	
/* Battery Charge: 100% charge */  
intu16 g_MdcAttrValBattCharge = 100;
#endif

/* Battery Measure */
#ifdef MDS_BATT_REMAIN
BatMeasure g_MdcAttrTimeBattRemain = 
	{
	    /* 1 Day of Battery Remaining */
	    0x1,        
	    MDC_DIM_DAY
	};
#endif /* MDS_BATT_REMAIN */

uint_8 g_app_secondstimer = TIMER_NOT_STARTED;  /* used to update the Absolute time */
volatile uint_8 g_app_send_timer = TIMER_NOT_STARTED;    /* used to count the time between measurents sending (minimum 1 second) */

volatile boolean g_app_schedule_send_obs_list;  /* schedule the observation list send when the timer expires */

boolean start_measurements_tmr;

uint_8 g_obs_queue_producer;
uint_8 g_obs_queue_consumer;
uint_8 g_obs_queue_cnt;

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
  TIMER_OBJECT TimerObject;
  intu16 i;
  
  DisableInterrupts;
	#if (defined _MCF51MM256_H) || (defined _MCF51JE256_H)
	usb_int_dis();	
	#endif	
  
  event_rpt_sent = TRUE;
  gAppBuffer = (intu8*)mempool_malloc(OBSERVATION_BUFFER_SIZE);
  
  /* initialize the observation pointers using the Application buffer */
  for(i = 0; i < OBSERVATION_SCAN_COUNT; i++)
  {    
    gObsPtr[i] = (ObservationScanList*)&gAppBuffer[i * OBSERVATION_SCAN_SIZE];
  }
  
	/* Initialize TIL */
	TIL_Initialize((PTIL)&g_Til);
	/* Initialize IEEE11073 and start Transport */
	(void)Ieee11073Initialize((PTIL)&g_Til, SHIMID, MedAppCallback);

  (void)memset(gAppBuffer, 0x00, OBSERVATION_BUFFER_SIZE);
    
  EnableInterrupts;	
	#if (defined _MCF51MM256_H) || (defined _MCF51JE256_H)
	usb_int_en();
	#endif	
  
  /* Start the seconds timer (1000ms). This is used to maintain the absolute time (1s resolution) */
  TimerObject.msCount = 1000; /* 1s count */
  TimerObject.pfnTimerCallback = SecondsTimerCallback;
  g_app_secondstimer = AddTimerQ(&TimerObject);
  
  
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
   /* Check for any Button Pressed */
   Button_Pressed();
       
   if(start_measurements_tmr == TRUE)
   {    
    start_measurements_tmr = FALSE;
    StartSendMeasurementsTimer();
   }
   else
   {    
    if(g_app_schedule_send_obs_list == TRUE)
    {
      if(g_send_msr == TRUE)
      {            
           g_app_schedule_send_obs_list = FALSE;
           AppSendMeasurements();
      }
    } 
   }
}  
/******************************************************************************
 *
 *   @name        UpdateBPMeasurements
 *
 *   @brief       This function updates the Blood Pressure Measurements to be sent 
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************
 * This function updates Blood Pressure Measurements
 *****************************************************************************/
static void UpdateBPMeasurements(void)
{   
    /* update the simulated blood pressure measurements */
    g_SimSystolic++;
    if(g_SimSystolic == 0) 
    {
     g_SimSystolic = 0x78;
    }
    
    g_SimDiastolic++;
    if(g_SimDiastolic == 0) 
    {
     g_SimDiastolic = 0x50;
    }
    
    g_SimMean = (uint_8)((g_SimSystolic + g_SimMean)/2);
    
    /* See if the measurements array has room to sample the measurements */
    if(g_obs_queue_cnt < OBSERVATION_SCAN_COUNT)
    {
      BPSAMPLEMEASUREMENTS *bpSample = &g_BpMesArray[g_obs_queue_producer];
      
      bpSample->type = MEASUREMENTS_TYPE_BP;
      
      bpSample->u.bp.systolic = g_SimSystolic;
      bpSample->u.bp.diastolic = g_SimDiastolic;
      bpSample->u.bp.mean = g_SimMean;
      
      UpdateAbsoluteTime(&(bpSample->timestamp));
      
      /* Update the number of queued measurments */
      g_obs_queue_cnt++;
      
      /* Update the leading index (producer) */
      g_obs_queue_producer++;
      if(g_obs_queue_producer >= OBSERVATION_SCAN_COUNT)
      {
       g_obs_queue_producer = 0;
      }
    }
}

/******************************************************************************
 *
 *   @name        UpdateBPMeasurements
 *
 *   @brief       This function updates the Pulse Measurements to be sent 
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************
 * This function updates Blood Pressure Measurements
 *****************************************************************************/
static void UpdatePulseMeasurements(void)
{
    /* update the simulated pulse measurements */
    g_SimPulse++;
    if(g_SimPulse == 0) 
    {
       g_SimPulse = 0x3C;
    }
    
     /* See if the measurements array has room to sample the measurements */
    if(g_obs_queue_cnt < OBSERVATION_SCAN_COUNT)
    {
      BPSAMPLEMEASUREMENTS *bpSample = &g_BpMesArray[g_obs_queue_producer];
      
      bpSample->type = MEASUREMENTS_TYPE_PULSE;
      
      bpSample->u.pulse = g_SimPulse;
      
      UpdateAbsoluteTime(&(bpSample->timestamp));
      
      /* Update the number of queued measurments */
      g_obs_queue_cnt++;
      
      /* Update the leading index (producer) */
      g_obs_queue_producer++;
      if(g_obs_queue_producer >= OBSERVATION_SCAN_COUNT)
      {
       g_obs_queue_producer = 0;
      }
    } 
}

/******************************************************************************
 *
 *   @name        ValDec2HexBCD
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
static uint_8 ValDec2HexBCD(uint_8 val) 
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
 *   @name        HexBCD2ValDec
 *
 *   @brief       This function converts an 8 bit Hex BCD representation into an integer value
 *
 *   @param       HexBCD value to be converted in Decimal
 *
 *   @return      Decimal correspondent
 *
 *****************************************************************************
 * This function converts an 8 bit Hex BCD into an integer decimal value
 *****************************************************************************/
static uint_8 HexBCD2ValDec(uint_8 val) 
{
 uint_8 ret = 0;
 uint_8 pow = 1; /* initial value for the power multiplier = 10^0) */ 
 
 while(val)
 { 
  uint_8 r, d;
  d = (uint_8)(val / 16);
  r = (uint_8)(val - (d * (uint_8)16));
  
  ret += r*pow;
  
  pow *= 10; /* 10^i */
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
void UpdateAbsoluteTime(AbsoluteTime *absTime)
{
  absTime->century = 0x20;
  
  /* Convert into BCD values */
  absTime->year   = ValDec2HexBCD(timeYear);
  absTime->month  = ValDec2HexBCD(timeMonth);
  absTime->day    = ValDec2HexBCD(timeDay);
  absTime->hour   = ValDec2HexBCD(timeHour);
  absTime->minute = ValDec2HexBCD(timeMinute);
  absTime->second = ValDec2HexBCD(timeSecond);
}

/******************************************************************************
 *
 *   @name        UpdateObservationScan
 *
 *   @brief       This function adds the sampled Measurements to the 
 *                Observation Scan List
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************
 * This function adds Blood pressure and Pulse Measurements to the Observation Scan List 
 *
 *****************************************************************************/
static ERR_CODE UpdateObservationScan(void)
{
  ERR_CODE err = ERROR_SUCCESS;
  uint_8 i, obs_idx, scan_idx;
  uint_8 objHandle;


    /* Clear the observation scan list*/
    (void)memset(gAppBuffer, 0x00, OBSERVATION_BUFFER_SIZE);
    
    if(g_obs_queue_cnt == 0)
    {
       /* No measurement samples available */ 
       /* Send the ObjHanlde 1 values with NaN */
       BPSAMPLEMEASUREMENTS *bpSample = &g_BpMesArray[0];
     
       bpSample->type = MEASUREMENTS_TYPE_BP;
      
       bpSample->u.bp.systolic = SFLOAT_NAN;   /*  SFLOAT NaN = 07FF */
       bpSample->u.bp.diastolic = SFLOAT_NAN;
       bpSample->u.bp.mean = SFLOAT_NAN;
      
       UpdateAbsoluteTime(&(bpSample->timestamp));
      
       /* Update the number of queued measurments */
       g_obs_queue_cnt++;
      
       /* Update the leading index (producer) */
       g_obs_queue_producer++;
       if(g_obs_queue_producer >= OBSERVATION_SCAN_COUNT)
       {
        g_obs_queue_producer = 0;
       }
        
    }
    
    /* Initialize indexes */
    obs_idx = g_obs_queue_consumer; /* Observation index is the queue consumer (trailing index) */
    scan_idx = 0; /* The scan list is constructed from the index 0 */
    
    if(g_obs_queue_cnt > SEND_OBS_SCAN_COUNT_MAX)
    {      
     i = SEND_OBS_SCAN_COUNT_MAX;
    }
    else
    {
     i = g_obs_queue_cnt;
    }
    
    while(i--)
    {
      BPSAMPLEMEASUREMENTS *bpSample = &g_BpMesArray[obs_idx];

      
      if(bpSample->type == MEASUREMENTS_TYPE_BP)
      {
        /* construct the BllodPressure attribute to be added to the observation scan list */
        uint_8 mdcAttrBloodPressure[10];
       
        (void)memset(&mdcAttrBloodPressure[0], 0x00, sizeof(mdcAttrBloodPressure) / sizeof(mdcAttrBloodPressure[0]));
       
        objHandle = 1;
       
        mdcAttrBloodPressure[1] = 3; /* Measurements count */
        mdcAttrBloodPressure[3] = 6; /* Measurements length */
        *(uint_16*)(&mdcAttrBloodPressure[4]) = bpSample->u.bp.systolic; /* systolic measurements */
        *(uint_16*)(&mdcAttrBloodPressure[6]) = bpSample->u.bp.diastolic; /* diastolic measurements */
        *(uint_16*)(&mdcAttrBloodPressure[8]) = bpSample->u.bp.mean; /* mean */
  
        /* Add Blood Pressure Measurements to Observation Scan List (objHandle = 1) */
        err = AddEntryToObsScanList(objHandle, MDC_ATTR_NU_CMPD_VAL_OBS_BASIC, 10, (void*)&mdcAttrBloodPressure[0], (ObservationScanList*)gObsPtr[scan_idx]);

        if(err != ERROR_SUCCESS)
        {
          break;
        }
      }
      else
      {
        /* Assume MEASUREMENTS_TYPE_PULSE type */
      
        /* construct the BllodPressure attribute to be added to the observation scan list */
        uint_8 mdcPulse[2];
        
        objHandle = 2;
      
        *(uint_16*)(&mdcPulse[0]) = bpSample->u.pulse;   /* pulse measurements */
      
        /* Add Blood Pressure Measurements to Observation Scan List (objHandle = 2) */
        err = AddEntryToObsScanList(objHandle, MDC_ATTR_NU_VAL_OBS_BASIC, 2, (void*)&mdcPulse[0], (ObservationScanList*)gObsPtr[scan_idx]);

        if(err != ERROR_SUCCESS)
        {
          break;
        }
      }           
      
      /* Add Absolute Time Stamp to Observation Scan List*/
      err = AddEntryToObsScanList(objHandle, MDC_ATTR_TIME_STAMP_ABS, 8, (void*)&(bpSample->timestamp), (ObservationScanList*)gObsPtr[scan_idx]);
      if(err != ERROR_SUCCESS)
      {
          break;
      }
      
      /* Update the trailing index (consumer) */
      obs_idx++;
      if(obs_idx >= OBSERVATION_SCAN_COUNT)
      {
        obs_idx = 0;
      }
      /* Update th scan index */
      scan_idx++;
    }
  return err;    
}

/******************************************************************************
 *
 *   @name        AppSendMeasurements
 *
 *   @brief       This Function sends the observation scan list to the manager 
 *
 *   @param       None
 *
 *   @return      None
 *
 *****************************************************************************
 * This Function sends the observation scan list to the manager
 *
 *****************************************************************************/
static void AppSendMeasurements(void)
{
  ERR_CODE err;
  
  if(g_app_send_timer == TIMER_NOT_STARTED)
  {                      
     /* Ok to send measurements. Update the observation scan based on the sample measurements */                      
     err = UpdateObservationScan();

     if(err == ERROR_SUCCESS)
     {
      uint_8 obs_cnt = g_obs_queue_cnt;
      if(obs_cnt > SEND_OBS_SCAN_COUNT_MAX)
      {
        obs_cnt = SEND_OBS_SCAN_COUNT_MAX;
      }
      
      /* Send Fixed Format Event Report */
      err = AgentSendMeasurements((ObservationScanList* (*)[])gObsPtr, 0, SCAN_REPORT_FIXED, obs_cnt, TRUE);
     }
                                            
     if(ERROR_SUCCESS == err)
     {
      /* start the 1 second guard timer for measurements sending */
      event_rpt_sent = FALSE;
      StartSendMeasurementsTimer();      
     }
  } 
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
            case KBI_PTG1_MASK:
            {                 
               /* PTG1 is pressed. Update and sample BP1 measurements (Systolic, Diastolic, MAP) */
               UpdateBPMeasurements();
               /* Schedule send measurements */       
               g_app_schedule_send_obs_list = TRUE; 
            }
            break;

            case KBI_PTG2_MASK:
            {
               /* PTG2 is pressed. Update and sample BP2 measurements (Pulse rate) */
               UpdatePulseMeasurements();
               /* Schedule send measurements */
               g_app_schedule_send_obs_list = TRUE;
            }
            break;
            
            case KBI_PTG3_MASK:
            {
             #ifndef BUTTON_PRESS_SIMULATION
               /* Force send measurements */
               g_app_schedule_send_obs_list = TRUE;
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
 *   @name        StartSendMeasurementsTimer
 *
 *   @brief       This function starts the measurements 1s guard timer.
 *
 *   @param       arg   : Argument passed by Timer ISR (optional)
 *
 *   @return      None
 *
 *****************************************************************************
 * This function is called each time measurements are sent to the manager.
 * The application makes sure that the next measurements are only sent after 
 * a minimum 1 second
 *****************************************************************************/
void StartSendMeasurementsTimer(void)
{
  TIMER_OBJECT TimerObject;

  TimerObject.msCount = 1000; /* 1s count */
  TimerObject.pfnTimerCallback = MeasurementsTimerCallback;
  DisableInterrupts;
  g_app_send_timer = AddTimerQ(&TimerObject); 
  EnableInterrupts;
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
    g_app_timer = TIMER_NOT_STARTED;

    /* Send Association Request */
    (void)AgentSendAssociationRequest((DataProtoList*)g_DataProtoList);
}

/******************************************************************************
 *
 *   @name        SecondsTimerCallback
 *
 *   @brief       This function is called when the one second timer expires.
 *                it is used to update the Abs time
 *
 *   @param       arg   : Argument passed by Timer ISR (optional)
 *
 *   @return      None
 *
 *****************************************************************************
 *****************************************************************************/
#ifdef TIMER_CALLBACK_ARG
static void SecondsTimerCallback(void * arg)
#else
static void SecondsTimerCallback(void)
#endif
{ 
  TIMER_OBJECT TimerObject;   
  #ifdef TIMER_CALLBACK_ARG
    UNUSED (arg)
  #endif
  
  /* Increment the number of passed seconds */
  timeSecond++;
  if(timeSecond >= 60)
  {
    timeSecond = 0;
  
    /* increment the number of passed minutes */
    timeMinute++;
    if(timeMinute >= 60)
    {
      timeMinute = 0;
    
      /* Increment the number of passed hours */
      timeHour++;
      if(timeHour >= 24)
      {
       timeHour = 0;
       /* Increment the number of passed days */     
       /* ... */
      }
        
    }
  }
  
  /* re-trigger the 1 second timer event */
  TimerObject.msCount = 1000; /* 1s count */
  TimerObject.pfnTimerCallback = SecondsTimerCallback;
  g_app_secondstimer = AddTimerQ(&TimerObject);
}

/******************************************************************************
 *
 *   @name        MeasurementsTimerCallback
 *
 *   @brief       This function is called when the one second timer expires.
 *                it is used to send the scheduled measurments
 *
 *   @param       arg   : Argument passed by Timer ISR (optional)
 *
 *   @return      None
 *
 *****************************************************************************
 *****************************************************************************/

#ifdef TIMER_CALLBACK_ARG
static void MeasurementsTimerCallback(void *arg)
#else
static void MeasurementsTimerCallback(void)
#endif
{
  #ifdef TIMER_CALLBACK_ARG
    UNUSED (arg)
  #endif
  
  g_app_send_timer = TIMER_NOT_STARTED;
   
  if(g_obs_queue_cnt)  
  {
    /* There are still queued measurments */ 
    g_app_schedule_send_obs_list = TRUE;    
  }  
    
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
          /* reinitialize the stack */
          TIMER_OBJECT TimerObject;
          TimerObject.msCount = SELECT_TIMEOUT;
          TimerObject.pfnTimerCallback = SelectTimerCallback;
          g_app_timer = AddTimerQ(&TimerObject);               
          g_send_msr = FALSE;
			}			
			break;
		case IEEE11073_TRANSPORT_DISCONNECT:
    		 /* Clean Up Application */
         event_rpt_sent = TRUE;
	       g_send_msr = FALSE;
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
          event_rpt_sent = TRUE;
	        g_send_msr = TRUE;          
          if(g_obs_queue_cnt)
          {
           /* The agent was connected to the manager and there are pending measurments to send */
           start_measurements_tmr = TRUE;
          }
	        break;
	    }
	    case IEEE11073_REJECT:
	     /* reset indexes - clean the obs queue */
	     g_obs_queue_cnt = 0;
	     g_obs_queue_consumer = g_obs_queue_producer;	     
	     event_rpt_sent = TRUE;
	    break;
	    
	    case IEEE11073_EVNTRPT_SENT:
	    {
	        uint_8 obs_sent;
	        event_rpt_sent = TRUE;	        
	        if(g_obs_queue_cnt > SEND_OBS_SCAN_COUNT_MAX)
	        {
	         obs_sent = SEND_OBS_SCAN_COUNT_MAX;
	         g_obs_queue_cnt -= SEND_OBS_SCAN_COUNT_MAX;
	        }
	        else
	        {
	         obs_sent = g_obs_queue_cnt;
	         g_obs_queue_cnt = 0;
	        }
	        
	        /* Update the trailing index */
	        g_obs_queue_consumer = (uint_8)((g_obs_queue_consumer + obs_sent) % OBSERVATION_SCAN_COUNT);
	        
	        if(g_obs_queue_cnt)
	        {
	         /* There are still observations to send. Schedule a new transmission */	         
	         start_measurements_tmr = TRUE;
	        }
	        break;
	    }

	    case IEEE11073_TRIG_PMSEGMENT:
	    case IEEE11073_CLEAR_PMSEGMENT:
	        break;
        
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
  
  timeYear   = HexBCD2ValDec(date_time.year);
  timeMonth  = HexBCD2ValDec(date_time.month);
  timeDay    = HexBCD2ValDec(date_time.day);
	timeHour   = HexBCD2ValDec(date_time.hour);
  timeMinute = HexBCD2ValDec(date_time.minute);
  timeSecond = HexBCD2ValDec(date_time.second);
	
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
  #ifdef MDS_ABSOLUTE_TIME   
    if(pAbsTime != NULL)
    {      
     (void)memcpy(pAbsTime, &g_MdcAttrTimeAbs, sizeof(AbsoluteTime));
    }
  #else  
    UNUSED(pAbsTime);
  #endif  
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
    if((pRelTime != NULL) && (g_DIM.pMds != NULL))
    {      
      *pRelTime = g_DIM.pMds->MdcAttrTimeRel;
    }
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
