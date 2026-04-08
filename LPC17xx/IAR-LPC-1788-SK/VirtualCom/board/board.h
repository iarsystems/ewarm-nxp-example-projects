/***************************************************************************
 **
 **    This file defines the board specific definition
 **
 **    Used with ARM IAR C/C++ Compiler and Assembler.
 **
 **    (c) Copyright IAR Systems 2011
 **
 **    $Revision: 28 $
 **
 ***************************************************************************/
#include <intrinsics.h>
#include "arm_comm.h"

#ifndef __BOARD_H
#define __BOARD_H

#if defined(IAR_LPC_1788_SK)

#define MOSC            (12MHZ)
#define RTCOSC          (32768UL)
/*UART DEF*/
#define UART0_TX_MASK       (1UL<<2)
#define UART0_TX_DIR        FIO0DIR
#define UART0_TX_IOCON      IOCON_P0_02
#define UART0_TX_IOCON_INIT 0x21

#define UART0_RX_MASK       (1UL<<3)
#define UART0_RX_DIR        FIO0DIR
#define UART0_RX_IOCON      IOCON_P0_03
#define UART0_RX_IOCON_INIT 0x21
/* USB OTG Link LED */
#define USB_OTG_LINK_LED_MASK   (1UL<<18)
#define USB_OTG_LINK_LED_FDIR   FIO1DIR
#define USB_OTG_LINK_LED_FSET   FIO1SET
#define USB_OTG_LINK_LED_FCLR   FIO1CLR
#define USB_OTG_LINK_LED_FIO    FIO1PIN
#define USB_OTG_LINK_LED_IOCON  IOCON_P1_18
/* USB Host Link LED */
#define USB_H_LINK_LED_MASK   (1UL<<13)
#define USB_H_LINK_LED_FDIR   FIO1DIR
#define USB_H_LINK_LED_FSET   FIO1SET
#define USB_H_LINK_LED_FCLR   FIO1CLR
#define USB_H_LINK_LED_FIO    FIO1PIN
#define USB_H_LINK_LED_IOCON  IOCON_P1_13
/*USB D+ D- and I2C*/
#define USB_DP_IOCON          IOCON_P0_29
#define USB_DM_IOCON          IOCON_P0_30
#define USB_SDA_IOCON         IOCON_P0_27
#define USB_SCL_IOCON         IOCON_P0_28
/*USB VBUS*/
#define USB_VBUS_MASK         (1UL<<14)
#define USB_VBUS_FDIR         FIO0DIR
#define USB_VBUS_FSET         FIO0SET
#define USB_VBUS_FCLR         FIO0CLR
#define USB_VBUS_FIO          FIO0PIN
#define USB_VBUS_IOCON        IOCON_P0_14
/*USB #PPWR2*/
#define USB_PPWR2_MASK         (1UL<<12)
#define USB_PPWR2_FDIR         FIO0DIR
#define USB_PPWR2_FSET         FIO0SET
#define USB_PPWR2_FCLR         FIO0CLR
#define USB_PPWR2_FIO          FIO0PIN
#define USB_PPWR2_IOCON        IOCON_P0_12
/*USB #OVRCR2*/
#define USB_OVRCR2_MASK         (1UL<<31)
#define USB_OVRCR2_FDIR         FIO1DIR
#define USB_OVRCR2_FSET         FIO1SET
#define USB_OVRCR2_FCLR         FIO1CLR
#define USB_OVRCR2_FIO          FIO1PIN
#define USB_OVRCR2_IOCON        IOCON_P1_31
/*USB #PWRD2*/
#define USB_PWRD2_MASK         (1UL<<30)
#define USB_PWRD2_FDIR         FIO1DIR
#define USB_PWRD2_FSET         FIO1SET
#define USB_PWRD2_FCLR         FIO1CLR
#define USB_PWRD2_FIO          FIO1PIN
#define USB_PWRD2_IOCON        IOCON_P1_30

/* Buttons */
#define BUT1_MASK           (1UL<<19)
#define BUT1_FDIR           FIO2DIR
#define BUT1_FIO            FIO2PIN

#define BUT2_MASK           (1UL<<21)
#define BUT2_FDIR           FIO2DIR
#define BUT2_FIO            FIO2PIN
/* MMC/SD card*/
#define SD_CMD_IOCON        IOCON_P1_03
#define SD_CLK_IOCON        IOCON_P1_02
#define SD_DAT_0_IOCON      IOCON_P1_06
#define SD_DAT_1_IOCON      IOCON_P1_07
#define SD_DAT_2_IOCON      IOCON_P1_11
#define SD_DAT_3_IOCON      IOCON_P1_12
/* Card power*/
#define SD_PWR_MASK         (1UL << 5)
#define SD_PWR_FDIR         FIO1DIR
#define SD_PWR_FSET         FIO1SET
#define SD_PWR_FCLR         FIO1CLR
#define SD_PWR_IOCON        IOCON_P1_05
/* Card present */
#define SD_CP_MASK          (1UL << 11)
#define SD_CP_FDIR          FIO2DIR
#define SD_CP_FIO           FIO2PIN
#define SD_CP_IOCON         IOCON_P2_11
/* Card write protect */
#define SD_WP_MASK         (1UL << 19)
#define SD_WP_FDIR         FIO4DIR
#define SD_WP_FIO          FIO4PIN
#define SD_WP_IOCON        IOCON_P4_19
/* Analog trim */
#define ANALOG_TRIM_CHANNEL   7
#define ANALOG_TRIM_IOCON     IOCON_P0_13
/*AUDIO*/
#define AU_CS_MASK   (1UL << 15)
#define AU_CS_FDIR   FIO4DIR
#define AU_CS_FSET   FIO4SET
#define AU_CS_FCLR   FIO4CLR
#define AU_CS_FIO    FIO4PIN
#define AU_CS_IOCON  IOCON_P4_15

#define AU_DREQ_MASK    (1UL << 16)
#define AU_DREQ_FDIR    FIO4DIR
#define AU_DREQ_FIO     FIO4PIN
#define AU_DREQ_IOCON   IOCON_P4_16

#define AU_SS_MASK      (1UL << 23)
#define AU_SS_FDIR      FIO2DIR
#define AU_SS_FIO       FIO2PIN
#define AU_SS_IOCON     IOCON_P2_23
#define AU_MISO_IOCON   IOCON_P2_26
#define AU_MOSI_IOCON   IOCON_P2_27
#define AU_SCLK_IOCON   IOCON_P2_22
/*CAN*/
#define CAN_RD_MASK     (1UL << 0)
#define CAN_RD_FDIR     FIO0DIR
#define CAN_RD_FIO      FIO0PIN
#define CAN_RD_IOCON    IOCON_P0_00

#define CAN_TD_MASK     (1UL << 1)
#define CAN_TD_FDIR     FIO0DIR
#define CAN_TD_FSET     FIO0SET
#define CAN_TD_FCLR     FIO0CLR
#define CAN_TD_FIO      FIO0PIN
#define CAN_TD_IOCON    IOCON_P0_01
/*ACC*/
#define ACC_INT_MASK    (1UL << 20)
#define ACC_INT_FDIR    FIO0DIR
#define ACC_INT_FIO     FIO0PIN
#define ACC_INT_IOCON   IOCON_P0_20

#define ACC_SDA_IOCON   IOCON_P2_14
#define ACC_SCL_IOCON   IOCON_P2_15
// VS1053
#define VS1002_CS_AU_MASK   (1UL << 15)
#define VS1002_CS_AU_FDIR   FIO4DIR
#define VS1002_CS_AU_FSET   FIO4SET
#define VS1002_CS_AU_FCLR   FIO4CLR
#define VS1002_CS_AU_FIO    FIO4PIN
#define VS1002_CS_AU_IOCON  IOCON_P4_15

#define VS1002_DREQ_MASK    (1UL << 16)
#define VS1002_DREQ_FDIR    FIO4DIR
#define VS1002_DREQ_FIO     FIO4PIN
#define VS1002_DREQ_IOCON  IOCON_P4_16

#define VS1002_SS_MASK      (1UL << 23)
#define VS1002_SS_FDIR      FIO2DIR
#define VS1002_SS_FIO       FIO2PIN
#define VS1002_SS_IOCON     IOCON_P2_23

#define VS1002_MISO_IOCON   IOCON_P2_26

#define VS1002_MOSI_IOCON   IOCON_P2_27

#define VS1002_SCLK_IOCON   IOCON_P2_22
/*Touch screen*/
#define TS_X1_MASK          (1UL << 24)
#define TS_X1_FIO           FIO0PIN
#define TS_X1_FDIR          FIO0DIR
#define TS_X1_FSET          FIO0SET
#define TS_X1_FCLR          FIO0CLR
#define TS_X1_INTR_R        IO0INTENR
#define TS_X1_INTR_CLR      IO0INTCLR
#define TS_X1_IOCON         IOCON_P0_24

#define TS_X2_MASK          (1UL << 19)
#define TS_X2_FIO           FIO0PIN
#define TS_X2_FDIR          FIO0DIR
#define TS_X2_FSET          FIO0SET
#define TS_X2_FCLR          FIO0CLR
#define TS_X2_INTR_R        IO0INTENR
#define TS_X2_INTR_CLR      IO0INTCLR
#define TS_X2_IOCON         IOCON_P0_19

#define TS_Y1_MASK          (1UL << 23)
#define TS_Y1_FIO           FIO0PIN
#define TS_Y1_FDIR          FIO0DIR
#define TS_Y1_FSET          FIO0SET
#define TS_Y1_FCLR          FIO0CLR
#define TS_Y1_INTR_R        IO0INTENR
#define TS_Y1_INTR_CLR      IO0INTCLR
#define TS_Y1_IOCON         IOCON_P0_23

#define TS_Y2_MASK          (1UL << 21)
#define TS_Y2_FIO           FIO0PIN
#define TS_Y2_FDIR          FIO0DIR
#define TS_Y2_FSET          FIO0SET
#define TS_Y2_FCLR          FIO0CLR
#define TS_Y2_INTR_R        IO0INTENR
#define TS_Y2_INTR_CLR      IO0INTCLR
#define TS_Y2_IOCON         IOCON_P0_21
/*IRDA*/
#define U4_TX_IOCON       IOCON_P5_04
#define U4_RX_IOCON       IOCON_P5_03

#else
#error define type of the board
#endif


#endif /* __BOARD_H */
