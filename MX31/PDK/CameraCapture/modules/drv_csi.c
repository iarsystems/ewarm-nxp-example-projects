/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : drv_csi.c
 *    Description : CSI driver module
 *
 *    History :
 *    1. Date        : May, 3 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 5880 $
 **************************************************************************/
#include <NXP/iomcimx31.h>
#include "arm_comm.h"
#include "drv_csi.h"
#include "drv_lcd.h"
#include "drv_i2c1.h"
#include "drv_atlas.h"



extern void Delay(Int32U time);
/*
*OV2640 Init Data. Ask Omnivision for details.
*/
Int8U OV2640_Data[] =
{
/*Reset*/
0xff, 0x01,0x12, 0x80,
/**/
0xff, 0x00,0x2c, 0xff,
0x2e, 0xdf,0xff, 0x01,
0x3c, 0x32,0x11, 0x01,
0x09, 0x00,0x04, 0x28,
0x13, 0xe5,0x14, 0x48,
0x2c, 0x0c,0x33, 0x78,
0x3a, 0x33,0x3b, 0xfb,
0x3e, 0x00,0x43, 0x11,
0x16, 0x10,0x39, 0x92,
0x35, 0xda,0x22, 0x1a,
0x37, 0xc3,0x23, 0x00,
0x34, 0xc0,0x36, 0x1a,
0x06, 0x88,0x07, 0xc0,
0x0d, 0x87,0x0e, 0x41,
0x4c, 0x00,0x4a, 0x81,
0x21, 0x99,0x24, 0x40,
0x25, 0x38,0x26, 0x82,
0x5c, 0x00,0x63, 0x00,
0x46, 0x22,0x0c, 0x3c,
0x5d, 0x55,0x5e, 0x7d,
0x5f, 0x7d,0x60, 0x55,
0x61, 0x70,0x62, 0x80,
0x7c, 0x05,0x20, 0x80,
0x28, 0x30,0x6c, 0x00,
0x6d, 0x80,0x6e, 0x00,
0x70, 0x02,0x71, 0x94,
0x73, 0xc1,0x12, 0x40,
0x17, 0x11,0x18, 0x43,
0x19, 0x00,0x1a, 0x4b,
0x32, 0x09,0x37, 0xc0,
0x4f, 0xca,0x50, 0xa8,
0x6d, 0x00,0x3d, 0x38,
0xff, 0x00,0xe5, 0x7f,
0xf9, 0xc0,0x41, 0x24,
0x44, 0x06,0xe0, 0x14,
0x76, 0xff,0x33, 0xa0,
0x42, 0x20,0x43, 0x18,
0x4c, 0x00,0x87, 0xd0,
0x88, 0x3f,0xd7, 0x03,
0xd9, 0x10,0xd3, 0x82,
0xc8, 0x08,0xc9, 0x80,
0x7c, 0x00,0x7d, 0x00,
0x7c, 0x03,0x7d, 0x48,
0x7d, 0x48,0x7c, 0x08,
0x7d, 0x20,0x7d, 0x10,
0x7d, 0x0e,0x90, 0x00,
0x91, 0x0e,0x91, 0x1a,
0x91, 0x31,0x91, 0x5a,
0x91, 0x69,0x91, 0x75,
0x91, 0x7e,0x91, 0x88,
0x91, 0x8f,0x91, 0x96,
0x91, 0xa3,0x91, 0xaf,
0x91, 0xc4,0x91, 0xd7,
0x91, 0xe8,0x91, 0x20,
0x92, 0x00,0x93, 0x06,
0x93, 0xe3,0x93, 0x03,
0x93, 0x03,0x93, 0x00,
0x93, 0x02,0x93, 0x00,
0x93, 0x00,0x93, 0x00,
0x93, 0x00,0x93, 0x00,
0x93, 0x00,0x93, 0x00,
0x96, 0x00,0x97, 0x08,
0x97, 0x19,0x97, 0x02,
0x97, 0x0c,0x97, 0x24,
0x97, 0x30,0x97, 0x28,
0x97, 0x26,0x97, 0x02,
0x97, 0x98,0x97, 0x80,
0x97, 0x00,0x97, 0x00,
0xa4, 0x00,0xa8, 0x00,
0xc5, 0x11,0xc6, 0x51,
0xbf, 0x80,0xc7, 0x10,
0xb6, 0x66,0xb8, 0xa5,
0xb7, 0x64,0xb9, 0x7c,
0xb3, 0xaf,0xb4, 0x97,
0xb5, 0xff,0xb0, 0xc5,
0xb1, 0x94,0xb2, 0x0f,
0xc4, 0x5c,0xa6, 0x00,
0xa7, 0x20,0xa7, 0xd8,
0xa7, 0x1b,0xa7, 0x31,
0xa7, 0x00,0xa7, 0x18,
0xa7, 0x20,0xa7, 0xd8,
0xa7, 0x19,0xa7, 0x31,
0xa7, 0x00,0xa7, 0x18,
0xa7, 0x20,0xa7, 0xd8,
0xa7, 0x19,0xa7, 0x31,
0xa7, 0x00,0xa7, 0x18,
0xc0, 0x64,0xc1, 0x4b,
0x86, 0x1d,0x50, 0x00,
0x51, 0xc8,0x52, 0x96,
0x53, 0x00,0x54, 0x00,
0x55, 0x00,0x57, 0x00,
0x5a, 0xc8,0x5b, 0x96,
0x5c, 0x00,0xc3, 0xef,
0x7f, 0x00,0xda, 0x01,
0xe5, 0x1f,0xe1, 0x67,
0xe0, 0x00,0xdd, 0x7f,
0x05, 0x00,
};
/**/
/*************************************************************************
 * Function Name: Csi_init
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init CSI module
 *
 *************************************************************************/
void Csi_init (void)
{
    
  IOMUX_SW_MUX_CTL37_bit.SW_MUX_OUT_EN3 = 1;  /*Function Enable CSI_HSYNC*/
  IOMUX_SW_MUX_CTL37_bit.SW_MUX_IN_EN3 = 2;   /* */
  IOMUX_SW_MUX_CTL37_bit.SW_MUX_OUT_EN2 = 1;  /*Function Enable CSI_PXCLK*/
  IOMUX_SW_MUX_CTL37_bit.SW_MUX_IN_EN2 = 2;   /* */  
  IOMUX_SW_MUX_CTL38_bit.SW_MUX_OUT_EN3 = 1;  /*Function Enable CSI_D14*/
  IOMUX_SW_MUX_CTL38_bit.SW_MUX_IN_EN3 = 2;   /* */
  IOMUX_SW_MUX_CTL38_bit.SW_MUX_OUT_EN2 = 1;  /*Function Enable CSI_D15*/
  IOMUX_SW_MUX_CTL38_bit.SW_MUX_IN_EN2 = 2;   /* */
  IOMUX_SW_MUX_CTL38_bit.SW_MUX_OUT_EN1 = 1;  /*Function Enable CSI_MCLK*/
  IOMUX_SW_MUX_CTL38_bit.SW_MUX_IN_EN1 = 2;   /* */
  IOMUX_SW_MUX_CTL38_bit.SW_MUX_OUT_EN0 = 1;  /*Function Enable CSI_VSYNC*/
  IOMUX_SW_MUX_CTL38_bit.SW_MUX_IN_EN0 = 2;   /* */
  IOMUX_SW_MUX_CTL39_bit.SW_MUX_OUT_EN3 = 1;  /*Function Enable CSI_D10*/
  IOMUX_SW_MUX_CTL39_bit.SW_MUX_IN_EN3 = 2;   /* */
  IOMUX_SW_MUX_CTL39_bit.SW_MUX_OUT_EN2 = 1;  /*Function Enable CSI_D11*/
  IOMUX_SW_MUX_CTL39_bit.SW_MUX_IN_EN2 = 2;   /* */
  IOMUX_SW_MUX_CTL39_bit.SW_MUX_OUT_EN1 = 1;  /*Function Enable CSI_D12*/
  IOMUX_SW_MUX_CTL39_bit.SW_MUX_IN_EN1 = 2;   /* */
  IOMUX_SW_MUX_CTL39_bit.SW_MUX_OUT_EN0 = 1;  /*Function Enable CSI_D13*/
  IOMUX_SW_MUX_CTL39_bit.SW_MUX_IN_EN0 = 2;   /* */
  IOMUX_SW_MUX_CTL40_bit.SW_MUX_OUT_EN3 = 1;  /*Function Enable CSI_D6*/
  IOMUX_SW_MUX_CTL40_bit.SW_MUX_IN_EN3 = 2;   /* */
  IOMUX_SW_MUX_CTL40_bit.SW_MUX_OUT_EN2 = 1;  /*Function Enable CSI_D7*/
  IOMUX_SW_MUX_CTL40_bit.SW_MUX_IN_EN2 = 2;   /* */
  IOMUX_SW_MUX_CTL40_bit.SW_MUX_OUT_EN1 = 1;  /*Function Enable CSI_D8*/
  IOMUX_SW_MUX_CTL40_bit.SW_MUX_IN_EN1 = 2;   /* */
  IOMUX_SW_MUX_CTL40_bit.SW_MUX_OUT_EN0 = 1;  /*Function Enable CSI_D9*/
  IOMUX_SW_MUX_CTL40_bit.SW_MUX_IN_EN0 = 2;   /* */
  
}

/*************************************************************************
 * Function Name: OV2640_Reset
 * Parameters:None
 *
 * Return:None
 *
 * Description:Enable OV2640 power and Reset the camera
 *
 *************************************************************************/
void OV2640_Reset(void)
{

  IOMUX_SW_MUX_CTL78_bit.SW_MUX_OUT_EN1 = 0;  /*GPIO3_5 pin Enable*/
  IOMUX_SW_PAD_CTL106_bit.IPP_ODE0 = 0;       /*Standard CMOS Push Pull*/ 
  IOMUX_SW_PAD_CTL106_bit.IPP_DSE0 = 0;       /*Standard Drive Strength*/ 
  IOMUX_SW_PAD_CTL106_bit.IPP_PUE0 = 0;       /*Disable Pull Up and Keepre*/ 
  IOMUX_SW_PAD_CTL106_bit.IPP_PKE0 = 0;  
  GPIO3_DR_bit.no5 = 1;     /*CSI_PWRDWN High*/
  GPIO3_GDIR_bit.no5 = 1;   /*CSI_PWRDWN pin as output*/
  
  IOMUX_SW_MUX_CTL26_bit.SW_MUX_OUT_EN3 = 0; /*GPIO2_14 pin Func Enable*/
  IOMUX_SW_PAD_CTL37_bit.IPP_ODE1 = 0;       /*Standard CMOS Push Pull*/ 
  IOMUX_SW_PAD_CTL37_bit.IPP_DSE1 = 0;       /*Standard Drive Strength*/ 
  IOMUX_SW_PAD_CTL37_bit.IPP_PUE1 = 0;       /*Disable Pull Up and Keepre*/ 
  IOMUX_SW_PAD_CTL37_bit.IPP_PKE1 = 0;          
  GPIO2_DR_bit.no14 = 1;    /*CMOS_RST_B_IND High*/
  GPIO2_GDIR_bit.no14 = 1;  /*CMOS_RST_B_IND pin as output*/
  
   /*Enable Power*/
  /*Set SW2B 1.8V and enable it*/
  Atlas_Rd_Wr((36<<0)|(36<<6)|(36<<12),ATLAS_SWITCH3,1);
  Atlas_Rd_Wr((Atlas_Rd_Wr(0,ATLAS_SWITCH5,0) & ~((3<<10)|(3<<12)))|(1<<10)|(1<<12),ATLAS_SWITCH5,1);
  Atlas_Rd_Wr((Atlas_Rd_Wr(0,ATLAS_PWR_MISC,0)|(1<<10)) & ~((1<<15)|(1<<16)),ATLAS_PWR_MISC,1);
  /* Set VMMC1 2.8V and enable it*/
  Atlas_Rd_Wr((Atlas_Rd_Wr(0,ATLAS_REG_SET1,0) & ~(7<<6)) | (5<<6),ATLAS_REG_SET1,1);
  Atlas_Rd_Wr(Atlas_Rd_Wr(0,ATLAS_REG_MODE1,0) | (1<<18),ATLAS_REG_MODE1,1);
  /* Set VVIB 1.3V and enable it*/
  Atlas_Rd_Wr(Atlas_Rd_Wr(0,ATLAS_REG_SET1,0)  & ~(3<<0),ATLAS_REG_SET1,1);
  Atlas_Rd_Wr(Atlas_Rd_Wr(0,ATLAS_REG_MODE1,0) | (1<<11),ATLAS_REG_MODE1,1);

  Delay(100000);      /*Delay 100ms*/
  /*PWRDN 0*/
  GPIO3_DR_bit.no5 = 0;     /*CSI_PWRDWN Low*/
  Delay(100000);            /*Delay 100ms*/
  GPIO2_DR_bit.no14 = 0;    /*CMOS_RST_B_IND*/
  Delay(100000);            /*Delay 100ms*/
  GPIO2_DR_bit.no14 = 1;    /*CMOS_RST_B_IND High*/
  Delay(100000);            /*Delay 100ms*/
}

/*************************************************************************
 * Function Name: OV2640_Init
 * Parameters:None
 *
 * Return:None
 *
 * Description:Init OV2640 registers
 *
 *************************************************************************/
void OV2640_Init(void)
{
Int8U * Source = OV2640_Data;
  OV2640_Reset();
  /*Copy init data to sensor registers*/
  I2C1_Write(Source, 0x30, 2); 
  Source += 2;
  I2C1_Write(Source, 0x30, 2);  
  Delay(10000);      /*Delay 10ms*/
     
  for(Int32U i = 0; 193 > i ; i++)
  {
    Source += 2;
    I2C1_Write(Source, 0x30,  2);  
  }
}

