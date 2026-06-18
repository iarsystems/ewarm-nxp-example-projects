/*
 * File:		lcdc.h
 * Purpose:		LCD Controller header file
 *
 */

#ifndef __lcdc_H__
#define __lcdc_H__

#include "common.h"


#define SCREEN_XSIZE 480
#define SCREEN_YSIZE 272
#define GW_XSIZE (SCREEN_XSIZE/2)
#define GW_YSIZE (SCREEN_YSIZE/2)

#define FRAME_START_ADDRESS 0x80400000  //Screen in DDR
#define GW_START_ADDRESS 0x80200000

#define FRAME_BUFFER_SIZE (SCREEN_XSIZE * SCREEN_YSIZE * 4) //4 bytes per pixel for 24bpp
#define GW_BUFFER_SIZE (GW_XSIZE * GW_YSIZE * 4) //4 bytes per pixel for 24bpp

//Amount to add to image address so it points to alias at 0x80xx_xxxx
//  instead of where the linker puts all "code" at 0x08xx_xxxx
#define FRAME_DDR_OFFSET 0x78000000



/* Init Functions */
void lcdc_init_screen();
void lcdc_init_spi_lcd();
void lcdc_init_pins();

/* Color Demo */
void lcdc_color_demo();
void color_demo_data();
void color_demo_gw_setup();

/* FSL Demo */
void lcdc_fsl_demo();
void lcdc_fsl_isr();

/* Gallery Demo */
void lcdc_gallery_demo();


#endif /* __lcdc_H__ */
