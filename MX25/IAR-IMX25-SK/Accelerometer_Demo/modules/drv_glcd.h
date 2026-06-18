/*************************************************************************
 *
*    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : drv_glcd.h
 *    Description : Graphical LCD driver include file
 *
 *    History :
 *    1. Date        : 6, March 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *
 *    $Revision: 16 $
 **************************************************************************/
#include "arm_comm.h"

#ifndef __GLCD_DRV_H
#define __GLCD_DRV_H

typedef struct _Bmp_t {
  Int32U  H_Size;
  Int32U  V_Size;
  Int32U  BitsPP;
  Int32U  BytesPP;
  pInt32U pPalette;
  pInt32U pPicStream;
  pInt8U  pPicDesc;
} Bmp_t, *pBmp_t;

typedef struct _FontType_t {
  Int32U H_Size;
  Int32U V_Size;
  Int32U CharacterOffset;
  Int32U CharactersNuber;
  pInt8U pFontStream;
  pInt8U pFontDesc;
} FontType_t, *pFontType_t;

typedef Int32U LdcPixel_t, *pLdcPixel_t;

// pixel clock frequency < 6.4MHz
// ~5.4Mhz = 408 clk per line * (240 line + 3 Vsynch pulse
//           + 5 front porch + 15 back porch)

#define C_GLCD_H_SIZE           320
#define C_GLCD_V_SIZE           240
#define CURSOR_H_SIZE           10
#define CURSOR_V_SIZE           10

#define TEXT_DEF_TAB_SIZE       5

#define TEXT_BEL1_FUNC()

#define MAX_GLCD_STR_SIZE       256

typedef enum _CursorContrl_t
{
  CURSOR_DIS = 0, CURSOR_COL_MAP, CURSOR_BACKGND_INV, CURSOR_COL_AND_BACKGND
} CursorContrl_t;

typedef enum _GLCD_Layer_t
{
  GLCD_BACKGROUND_LAYER, GLCD_GRAPHIC_WINDOW_LAYER
} GLCD_Layer_t;

void GLCD_Init (const Int32U *pPain, const Int32U * pPallete);
void GLCD_SetPallet (const Int32U * pPallete);
void GLCD_Ctrl (Boolean bEna);
void GLCD_Cursor_Cfg(Int32U BlinkSpeed, Int32U H_Size, Int32U V_Size, Int32U ColMap);
void GLCD_Cursor_Ctlr(CursorContrl_t CC, Boolean bOP);
void GLCD_Move_Cursor(int x, int y);
void GLCD_SetFont(pFontType_t pFont, LdcPixel_t Color, LdcPixel_t BackgndColor);
void GLCD_SetWindow(Int32U X_Left, Int32U Y_Up,
                    Int32U X_Right, Int32U Y_Down);
void GLCD_TextSetPos(Int32U X, Int32U Y);
void GLCD_TextSetTabSize(Int32U Size);
static void LCD_SET_WINDOW (Int32U X_Left, Int32U X_Right,
                            Int32U Y_Up, Int32U Y_Down);
static void LCD_WRITE_PIXEL (Int32U Pixel);
static Boolean GLCD_TextCalcWindow (pInt32U pXL, pInt32U pXR,
                                    pInt32U pYU, pInt32U pYD,
                                    pInt32U pH_Size, pInt32U pV_Size);
void GLCD_LoadPic (Int32U X_Left, Int32U Y_Up, Bmp_t * pBmp, Int32U Mask);
void GLCD_LoadPicTransparent (Int32U X_Left, Int32U Y_Up, Bmp_t * pBmp, Int32U Transp);
void GLCD_print (const char *fmt, ...);

static Int8U _GLCD_SPI_Transfer (Int8U Data);
static void _GLCD_Delay_ms(Int32U Dly);
static void _GLCD_WriteCom(Int16U Addr);
static void _GLCD_WriteData(Int16U Data);
void GLCD_InitGraphic (const Int32U *pPain, const Int32U * pPallete);
void GLCD_GraphicColorKeying (Boolean Enable, Int8U Blue, Int8U Green, Int8U Red);
void GLCD_GraphicTransparentCtrl (Boolean Enable, Int8U Transparent);
void GLCD_PrintfLayerSelect (GLCD_Layer_t Layer);
void GLCD_Point (Int32U X, Int32U Y, Int32U Color);
void GLCD_Line(Int32U X1,Int32U Y1, Int32U X2, Int32U Y2,
               Int32U Color, Int32U Thickness);

#endif // __GLCD_DRV_H
