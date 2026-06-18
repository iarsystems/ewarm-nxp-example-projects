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
 *    $Revision: 16 $
 **************************************************************************/
#include "drv_csi.h"
extern Int8U LcdBuff[LCD_WIDTH*LCD_HEIGHT*(LCD_BPP/8)];

static volatile Boolean Csi_Enable;

/*************************************************************************
 * Function Name: Csi_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: CSI interrupt handler
 *
 *************************************************************************/
void Csi_Handler (void)
{
  PRP_INTRSTATUS_bit.CH1B1CI = 1;
  PRP_CNTL_bit.CH1EN = Csi_Enable;
}

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
Int8U Csi_Data[2];

  // Camera reset
  CSI_RST_L();
  CSI_STB_L();
  CSI_RST_H();

  // Set PERDIV4 ~ 53MHz
  PCDR1_bit.PCLK_DIV4 = 5 - 1;
  PCCR0_bit.PERCLK4_EN = 1;
  PCCR0_bit.HCLK_CSI_EN = 1;

  //module init
  // The pins assign to the CSI module
  // CSI_D0-D7, CSI_HSYNC, CSI_VSYNC, CSI_PIXCLK, CSI_MCLK
  PTB_GIUS &= ~(0xFFF<<10);       // disable GPIO PB[21..10]
  CSICR1 = 0x0;                   // register clear
  CSICR1_bit.MCLKDIV = 1;         // MCLK = 13.5MHz HCLK/4
  //timing control
  CSICR1_bit.SOF_POL = 1;         // SOF rising edge
  CSICR1_bit.SOF_INTEN = 1;       // SOF INT enable
  CSICR1_bit.REDGE = 1;           // latch on rising edge
  CSICR1_bit.GCLK_MODE = 1;       // gated clock mode
  CSICR1_bit.HSYNC_POL = 1;       // hsync active high
  //FIFO control
  CSICR1_bit.FCC = 1;             // sync FIFO clear
  CSICR1_bit.RXFF_LEVEL = 2;      // RXFF level = 16
  //data manipulation
  CSICR1_bit.SWAP16_EN = 0;       // swap16 disable
  CSICR1_bit.PACK_DIR = 0;        // little endian
  //PRP i/f control
  CSICR1_bit.PRP_IF_EN = 1;       // PRP i/f enable
  CSICR1_bit.MCLKEN = 1;          // MCLK enable

  // Init PrP
  PCCR0_bit.EMMA_EN = 1;          // Clock enable for PRP
  PCCR0_bit.HCLK_EMMA_EN = 1;     // Enable EMMA AHB clock input
  PRP_CNTL_bit.SWRST = 1;         // PRP reset
  //YUV 4:2:2 => CamOutBuff565 (ch1)
  //common control
  PRP_CNTL_bit.CSIEN = 1;         // CSI connection enable
  PRP_CNTL_bit.DATA_IN_MODE = 1;  // 1 = YUV 4:2:2;2 = CSI 16-bit CamOutBuff data
  PRP_CNTL_bit.CH1_OUT_MODE = 1;  // Output format 16 bpp CamOutBuff
  PRP_CNTL_bit.CH1_LEN = 1;       // CH1 Loop mode
  PRP_CNTL_bit.CH1BYP = 1;        // Disable cascade of Channel-1 resize
  PRP_CNTL_bit.IN_TSKIP = 0;      // no skip input frames
  PRP_CNTL_bit.CH1_TSKIP = 0;     // no skip output frames
  PRP_CNTL_bit.INPUT_FIFO_LEVEL = 0; // Input FIFO Level — 128 words
  PRP_CNTL_bit.RZ_FIFO_LEVEL = 1; // Resize FIFO Level — 48 words
  //CSI i/f enable ch1
  PRP_INTR_CNTL = 0x00000000;     // disable interrupt
  //source control
  // YUV 4:2:2 YVYU input
  PRP_SRC_PIXEL_FORMAT_CNTL_bit.BLUE_WIDTH = 8;
  PRP_SRC_PIXEL_FORMAT_CNTL_bit.GREEN_WIDTH= 8;
  PRP_SRC_PIXEL_FORMAT_CNTL_bit.RED_WIDTH  = 8;
  PRP_SRC_PIXEL_FORMAT_CNTL_bit.BLUE_V_CR_OFFSET  = 16;
  PRP_SRC_PIXEL_FORMAT_CNTL_bit.GREEN_U_CB_OFFSET =  0;
  PRP_SRC_PIXEL_FORMAT_CNTL_bit.RED_Y_OFFSET      =  8;
  // source = 640 x 480
  PRP_SRC_FRAME_SIZE_bit.PICTURE_Y_SIZE = CAM_SIZE_Y;
  PRP_SRC_FRAME_SIZE_bit.PICTURE_X_SIZE = CAM_SIZE_X;

  // ch1 (CamOutBuff) dest control
  // The PrP place data directly to the LCD's buffer
  PRP_DEST_RGB1_PTR = (Int32U)&LcdBuff[(CAM_WINDOW_Y*IMAGE_WIDTH*(LCD_BPP/8))+(CAM_WINDOW_X*(LCD_BPP/8))];

  // CamOutBuff565 output
  PRP_CH1_PIXEL_FORMAT_CNTL_bit.BLUE_WIDTH  = 5;
  PRP_CH1_PIXEL_FORMAT_CNTL_bit.GREEN_WIDTH = 6;
  PRP_CH1_PIXEL_FORMAT_CNTL_bit.RED_WIDTH   = 5;
  PRP_CH1_PIXEL_FORMAT_CNTL_bit.BLUE_OFFSET = 0;
  PRP_CH1_PIXEL_FORMAT_CNTL_bit.GREEN_OFFSET= 5;
  PRP_CH1_PIXEL_FORMAT_CNTL_bit.RED_OFFSET  = 11;
  //dest size = ~200 x ~150
  PRP_CH1_OUT_IMAGE_SIZE_bit.CH1_OUT_IMAGE_HEIGHT = CAM_P_SIZE_Y;
  PRP_CH1_OUT_IMAGE_SIZE_bit.CH1_OUT_IMAGE_WIDTH  = CAM_P_SIZE_X;
  PRP_DEST_CH1_LINE_STRIDE_bit.CH1_OUT_LINE_STRIDE= IMAGE_WIDTH*(LCD_BPP/8);

  // Resize 3:1
  // Sum of the all coefficients must be 8
  PRP_CH1_RZ_VERT_COEF1_bit.VC0 = 2;            // Pic 0 weighting coef
  PRP_CH1_RZ_VERT_COEF1_bit.VC1 = 4;            // Pic 1 weighting coef
  PRP_CH1_RZ_VERT_COEF1_bit.VC2 = 2;            // Pic 2 weighting coef
  PRP_CH1_RZ_VERT_COEF2 = 0x00000000;
  PRP_CH1_RZ_VERT_VALID_bit.VERT_TBL_LEN = 3;   // Filter Len 3
  PRP_CH1_RZ_VERT_VALID_bit.VOV = 1UL<<2;       // Pix 2 valid

  PRP_CH1_RZ_HORI_COEF1_bit.HC0 = 2;            // Pic 0 weighting coef
  PRP_CH1_RZ_HORI_COEF1_bit.HC1 = 4;            // Pic 1 weighting coef
  PRP_CH1_RZ_HORI_COEF1_bit.HC2 = 2;            // Pic 2 weighting coef
  PRP_CH1_RZ_HORI_COEF2 = 0x00000000;
  PRP_CH1_RZ_HORI_VALID_bit.HORI_TBL_LEN = 3;   // Filter Len 3
  PRP_CH1_RZ_HORI_VALID_bit.HOV = 1UL<<2;       // Pix 2 valid

  I2C_Init();
  // Reset Sensor Core
  Csi_Data[1] = 0x04; // select Sensor Core registers
  Csi_Data[0] = 0x00;
  I2C_Transfer(CSI_SLAVE_ADDR,1,Csi_Data,2);
  Csi_Data[1] = 0x01; // reset
  Csi_Data[0] = 0x00;
  I2C_Transfer(CSI_SLAVE_ADDR,13,Csi_Data,2);
  Csi_Data[1] = 0x00; // release
  Csi_Data[0] = 0x00;
  I2C_Transfer(CSI_SLAVE_ADDR,13,Csi_Data,2);
  // Reset IFP
  Csi_Data[1] = 0x01; // select IFP registers
  Csi_Data[0] = 0x00;
  I2C_Transfer(CSI_SLAVE_ADDR,1,Csi_Data,2);
  Csi_Data[1] = 0x01; // reset
  Csi_Data[0] = 0x00;
  I2C_Transfer(CSI_SLAVE_ADDR,7,Csi_Data,2);
  Csi_Data[1] = 0x00; // release
  Csi_Data[0] = 0x00;
  I2C_Transfer(CSI_SLAVE_ADDR,7,Csi_Data,2);
  // Vertical flip
  Csi_Data[1] = 0x04; // select Sensor Core registers
  Csi_Data[0] = 0x00;
  I2C_Transfer(CSI_SLAVE_ADDR,1,Csi_Data,2);
  I2C_Transfer(CSI_SLAVE_ADDR|1,32,Csi_Data,2);
  Csi_Data[0] |= 0x80;
  I2C_Transfer(CSI_SLAVE_ADDR,32,Csi_Data,2);
  Csi_Data[1] = 0x01; // select IFP registers
  Csi_Data[0] = 0x00;
  I2C_Transfer(CSI_SLAVE_ADDR,1,Csi_Data,2);
  I2C_Transfer(CSI_SLAVE_ADDR|1,8,Csi_Data,2);
  Csi_Data[1] |= 0x02;
  I2C_Transfer(CSI_SLAVE_ADDR,8,Csi_Data,2);

  // Init PrP frame complete interrupt
  PRP_INTRSTATUS_bit.CH1B1CI = 1;
  AITC_SetupIntr(Csi_Handler,FALSE,INT_EMMAPRP,15);
  AITC_EnableIntSource(INT_EMMAPRP);
  PRP_INTR_CNTL_bit.CH1FCIE = 1;
  // Enable PrP
  Csi_Enable = FALSE;
  PRP_CNTL_bit.CH1EN = FALSE;
}

/*************************************************************************
 * Function Name: Csi_Ctrl
 * Parameters: Boolean Enable
 *
 * Return: none
 *
 * Description: Enable/Disable PrP controller
 *
 *************************************************************************/
void Csi_Ctrl(Boolean Enable)
{
  if(Enable)
  {
    Csi_Enable = TRUE;
    PRP_CNTL_bit.CH1EN = TRUE;
  }
  else
  {
    Csi_Enable = FALSE;
    // wait until the PrP is disable
    while(PRP_CNTL_bit.CH1EN);
  }
}
