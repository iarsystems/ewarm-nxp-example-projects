/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : drv_ipu.c
 *    Description : IPU module driver
 *
 *    History :
 *    1. Date        : 06.2.2009 
 *       Author      : Stoyan Choynev
 *       Description : 1.0 initial revision
 *
 *    $Revision: 5880 $
 **************************************************************************/

/** include files **/
/** include files **/
#include <NXP/iomcimx31.h>
#include <string.h>
#include "arm_comm.h"
#include "board.h"
#include "drv_lcd.h"

/** local definitions **/
#define SENSOR_HEIGHT 600
#define SENSOR_WIDTH  800

#define IC_PARAM_WORD(c0,c1,c2,a,scale,sat_mode) {(c0 & 0x1FF)|((c1 & 0x1FF)<<9)|((c2 & 0x1FF)<<18)|((a & 0x1F)<<27),\
                      ((a>>5) & 0xFF)|(scale<<8)|(sat_mode<<10)}

/** default settings **/


/** external functions **/
extern void Delay(Int32U time);

/** external data **/
extern Int16U menu[];

/** internal functions **/
void Write_IPU_memory(Int32U mem_addr, Int32U * pData, Int32U size);

/** public data **/

/** private data **/
/* YUYV to RGB converstion
R = ( 298 * (Y-16) +   0 * (U -128) + 409 * (V -128) + 128)/256
G = ( 298 * (Y-16) - 100 * (U -128) - 208 * (V -128) + 128)/256
B = ( 298 * (Y-16) + 516 * (U -128) +   0 * (V -128) + 128)/256
*/ 
const Int32U IC_Params[][2] ={IC_PARAM_WORD(0x0,0x1E7,0x4B,0x1F21,3,0),
                              IC_PARAM_WORD(0x4B,0x4B,0x0,0x88,0,0),
                              IC_PARAM_WORD(0x81,0x1CC,0x66,0x1EEC,0,0)
                             };
                             
const Int32U DMASDC0_Params[10] ={0, 0x00004000,0,0x7F1DF000,0x2,(Int32U)LcdBgBuff,0,0x1E081DFA,0x96416502,0} ;   /*BG DMA Init Data*/
const Int32U DMASDC1_Params[10] ={0, 0x00004000,0,0x2B18F000,0x1,(Int32U)LcdFgBuff,0,0x1E0818FA,0x96416502,0} ;   /*FG DMA Init Data*/
const Int32U DMAIC1_Params[10] ={0, 0x00004000,0,0x2B18F000,0x1,(Int32U)LcdFgBuff,0,0x1E2818FA,0x96416502,0} ;    /*IC DMA Init Data*/

/** public functions **/
/*************************************************************************
 * Function Name: IPU_Init
 * Parameters:None
 *
 * Return:None  
 *
 * Description:Gonfigure IPU
 *
 *************************************************************************/
void IPU_Init(void)
{
  CCM_CGR1 |= 3<<22;                    /*Enable IPU Clock*/

  IPU_CONF = 0;                         /*Disable All IPU Tasks*/
  IDMAC_CHA_EN = 0;                     /*Disable All Channels*/
  IC_CONF = 0;                          /*Disable All IPU Tasks*/
  SDC_COM_CONF = 0;
  /******************************************************************************************
  *System Configuration
  *******************************************************************************************/
  /*Configuring commom parameters*/
  IPU_CONF_bit.PXL_ENDIAN = 0;          /*Select pixel Endianness in the IPU_CONF Register.*/
  IDMAC_CONF_bit.SINGLE_AHB_M_EN = 0;   /*Select single or dual AHB mode in the IDMAC_CONF Register.*/
  
  /*Configuring sensor interface*/
  CSI_SENS_CONF_bit.DATA_WIDTH = 1;       /*8bit data width*/
  CSI_SENS_CONF_bit.SENS_DATA_FORMAT = 2; /*YUV data format*/
  CSI_SENS_CONF_bit.EXT_VSYNC = 0;        /*External Vertical sync*/
  CSI_SENS_CONF_bit.DIV_RATIO = 5;        /*HSP_CLK/6 = 22.16MHz*/
  CSI_SENS_CONF_bit.SENS_CLK_SRC = 1;     /*HSP_CLK*/
  CSI_SENS_CONF_bit.SENS_PRTCL = 0;       /*Gated pixel clock*/
  
  CSI_SENS_FRM_SIZE_bit.SENS_FRM_HEIGHT = SENSOR_HEIGHT-1;/*Define the sensor frame size*/
  CSI_SENS_FRM_SIZE_bit.SENS_FRM_WIDTH = SENSOR_WIDTH-1;
  
  /*Configuring display interface*/
  DI_HSP_CLK_PER = 0x00100010;              
  DI_DISP_SIG_POL_bit.D3_DATA_POL = 0;  /*Configure display interface polarities in the DI_DISP_SIG_POL Register.*/
  DI_DISP_SIG_POL_bit.D3_CLK_POL = 1;
  DI_DISP_SIG_POL_bit.D3_HSYNC_POL = 0;
  DI_DISP_SIG_POL_bit.D3_VSYNC_POL = 0;
  /*Configure display timings in the DI_DISP3_TIME_CONF Register.*/
  DI_DISP3_TIME_CONF_bit.DISP_IF_CLK_PER_WR = ((HSP_CLK*16)/LCD_PXL_CLK);
  DI_DISP3_TIME_CONF_bit.DISP_IF_CLK_DOWN_WR = ((HSP_CLK*2)/LCD_PXL_CLK-1);
  /*Configure display data bus mapping*/
  DI_DISP3_B0_MAP = 0x0005000F; /* Bit 0 & 1 masked. All the athers enabled in fitst Clock. Output position 5*/
  DI_DISP3_B1_MAP = 0x000B000F; /* Bit 0 & 1 masked. All the athers enabled in fitst Clock. Output position 11*/
  DI_DISP3_B2_MAP = 0x0011000F; /* Bit 0 & 1 masked. All the athers enabled in fitst Clock. Output position 17*/
  /*Define the count of display clock cycles required to output one pixel*/
  DI_DISP_ACC_CC_bit.DISP3_IF_CLK_CNT_D = 0;
  
  SDC_COM_CONF = 1;  /*Color TFT mode*/
  /*Define horizontal and vertical dimensions of the synchronous display*/
  SDC_HOR_CONF_bit.SCREEN_WIDTH = LCD_PPL - 1;
  SDC_HOR_CONF_bit.H_SYNC_WIDTH = LCD_H_SYNC_WIDTH - 1;
  //SDC_HOR_CONF |= 4;
  
  SDC_VER_CONF_bit.SCREEN_HEIGHT = LCD_LPF-1;
  SDC_VER_CONF_bit.V_SYNC_WIDTH = LCD_V_SYNC_WIDTH - 1;
  SDC_VER_CONF_bit.V_SYNC_WIDTH_L = 1;
  
 
  /******************************************************************************************
  *Task Configuration
  *******************************************************************************************/
  /*CSI Configuring*/
  CSI_ACT_FRM_SIZE_bit.ACT_FRM_HEIGHT = SENSOR_HEIGHT-1;/*Define actual frame size*/
  CSI_ACT_FRM_SIZE_bit.ACT_FRM_WIDTH = SENSOR_WIDTH-1;
  //CSI_OUT_FRM_CTRL_bit.HORZ_DWNS = 0;                   /**/
  //CSI_OUT_FRM_CTRL_bit.VERT_DWNS = 0;
  
  /*IC Configuring*/  
  IC_PRP_VF_RSC_bit.PRPVF_DS_R_V = SENSOR_HEIGHT/PICTURE_HEIGHT - 1; /*Down size*/
  IC_PRP_VF_RSC_bit.PRPVF_DS_R_H = SENSOR_WIDTH/PICTURE_WIDTH - 1;
  IC_PRP_VF_RSC_bit.PRPVF_RS_R_V = 8192;
  IC_PRP_VF_RSC_bit.PRPVF_RS_R_H = 8192;
  
  Write_IPU_memory(0x00002D28,(Int32U*)IC_Params,6);
  /*SDC Configuring*/
  SDC_GRAPH_WIND_CTRL = 0xFF000000;
  SDC_BG_POS_bit.BGXP = LCD_H_SYNC_WIDTH;
  SDC_BG_POS_bit.BGYP = LCD_V_SYNC_WIDTH;
  SDC_FG_POS_bit.FGXP = LCD_H_SYNC_WIDTH+40;
  SDC_FG_POS_bit.FGYP = LCD_V_SYNC_WIDTH+49;
  SDC_CUR_BLINK_PWM_CTRL_bit.SCR = 0;   //Source is HSYNC
  SDC_CUR_BLINK_PWM_CTRL_bit.PWM = 255; //
  SDC_CUR_BLINK_PWM_CTRL_bit.CC_EN = 1; //
  /*IDMA Configuring*/
  Write_IPU_memory(0x000100E0,(Int32U*)DMASDC0_Params,10);  /*BG DMA Channel*/
  Write_IPU_memory(0x000100F0,(Int32U*)DMASDC1_Params,10);  /*FG DMA Channel*/
  Write_IPU_memory(0x00010010,(Int32U*)DMAIC1_Params,10);   /*IC DMA Channel*/
  
  IPU_CHA_DB_MODE_SEL_bit.DMASDC_0_DBMS = 0;
  IPU_CHA_DB_MODE_SEL_bit.DMASDC_1_DBMS = 0;
  IPU_CHA_DB_MODE_SEL_bit.DMAIC_1_DBMS = 0;
  IPU_CHA_CUR_BUF = 0xFFFFFFFF;
  IPU_FS_DISP_FLOW_bit.SDC0_SRC_SEL = 0;
  IPU_FS_DISP_FLOW_bit.SDC1_SRC_SEL = 0;
  IPU_FS_PROC_FLOW_bit.PRPVF_DEST_SEL = 5;
  
  /*Enable SDC*/
  IPU_CONF_bit.SDC_EN = 1;
  /*Enable Dispaly Interface*/
  IPU_CONF_bit.DI_EN = 1;
  /*Enable the Camera Sensor Interface*/
  IPU_CONF_bit.CSI_EN = 1;
}

/*************************************************************************
 * Function Name: ShowMenu
 * Parameters: None
 *
 * Return: None
 *
 * Description: Start BG tastk
 *
 *************************************************************************/
void ShowMenu(void)
{
  /**/
  SDC_COM_CONF_bit.BG_EN = 0;
  /**/
  /*Enabling Task*/
  /*Enable IDMAC channel 0*/
  IDMAC_CHA_EN_bit.DMASDC_0_EN = 1;
  /*Enable IC tasks IC_CONF*/
  /*Copy menu image to LCD Buffer*/
  memcpy(LcdBgBuff,&menu[8],*((Int32U *)&menu[0]));
  /*Clear buffer*/
 // IPU_CHA_CUR_BUF = 0x1<<14;
  IPU_CHA_BUF0_RDY_bit.DMASDC_0_BUF_RDY = 1;
  /*Enable the background plane*/
  SDC_COM_CONF_bit.BG_EN = 1;
}
/*************************************************************************
 * Function Name: HideMenu
 * Parameters: None
 *
 * Return: None
 *
 * Description: Stop BG Task
 *
 *************************************************************************/
void HideMenu(void)
{
  /*Disable the background plane*/
  SDC_COM_CONF_bit.BG_EN = 0;
   /*Disable IDMAC channel 0*/
  IDMAC_CHA_EN_bit.DMASDC_0_EN = 0;
}

/*************************************************************************
 * Function Name: ShowMenu
 * Parameters: None
 *
 * Return: None
 *
 * Description: Start FG tastk
 *
 *************************************************************************/
void ShowPicture(void)
{
  /*Configure tasks*/
  /*Enabling Task*/
  /*Enable IDMAC channel 1*/
  IDMAC_CHA_EN_bit.DMASDC_1_EN = 1;
  /*Enable the background plane*/
  SDC_COM_CONF_bit.FG_EN = 1;
  /*Enable IC tasks IC_CONF*/
  IPU_CHA_BUF0_RDY_bit.DMASDC_1_BUF_RDY = 1;
}
/*************************************************************************
 * Function Name: HidePicture
 * Parameters: None
 *
 * Return: None
 *
 * Description: Stop FG Task
 *
 *************************************************************************/
void HidePicture(void)
{
  /*Disable the foreground plane*/
  SDC_COM_CONF_bit.FG_EN = 0;
  /*Disable IDMAC channel 1*/
  IDMAC_CHA_EN_bit.DMASDC_1_EN = 0;
}

/*************************************************************************
 * Function Name: StartImgCapturet
 * Parameters:None
 *
 * Return:None
 *
 * Description:enable DMAIC channel 1 and start VF tasks
 *
 *************************************************************************/
void StartImgCapture(void)
{
  /*Enable the Camera Sensor Interface*/
  IPU_CONF_bit.CSI_EN = 1;
  /*Enable Image Converter*/
  IPU_CONF_bit.IC_EN = 1;
 /*Enable IDMAC channel 0*/
  IDMAC_CHA_EN_bit.DMAIC_1_EN = 1;
  /*Enable View finder tasks*/
  IC_CONF_bit.PRPVF_EN = 1;
  IC_CONF_bit.PRPVF_CSC1 = 1;
}

/*************************************************************************
 * Function Name: StopImgCapture
 * Parameters:None
 *
 * Return:None
 *
 * Description:Init Stop VF task and disable DMAIC channel 1
 *
 *************************************************************************/
void StopImgCapture(void)
{
  IDMAC_CHA_EN_bit.DMAIC_1_EN = 0;
 /*Disable Image Converter*/
  IC_CONF_bit.PRPVF_EN = 0;
  IC_CONF_bit.PRPVF_CSC1 = 0;
  /**/
  IPU_CONF_bit.IC_EN = 0;
  /*Disable the Camera Sensor Interface*/
  IPU_CONF_bit.CSI_EN = 0;
}
/** private functions **/
/*************************************************************************
 * Function Name: Write_IPU_memory
 * Parameters:mem_addr - Internal IPU memory address
 *            pData    - Data pointer
 *            size     - Data size in words
 * Return:None
 *
 * Description: Write Data into internal IPU memory
 *
 *************************************************************************/
void Write_IPU_memory(Int32U mem_addr, Int32U * pData, Int32U size)
{
  IPU_IMA_ADDR = mem_addr;  /*Chanel 0 parameters*/
  for(Int32U cntr = 0;size > cntr; cntr++) 
  {
    IPU_IMA_DATA = *pData++;
    Delay(10);           /*Delay 10us*/
  }
}  
