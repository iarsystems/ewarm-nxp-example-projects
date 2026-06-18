#ifndef _QUADSPI_BOOT_H_
#define _QUADSPI_BOOT_H_

#include <stddef.h>
#include <stdint.h>

/*************************************
 *  QuadSPI Configuration Information
 *************************************/
#define  COMAND_SEQ_LEN       128 //interms of instruction word size
#define  QSPI_TAG             0xC0FFEE01
/*Quad SPI configuration structures */
typedef struct __sflash_configuration_parameter
{
  uint32_t dqs_loopback;              //DQS LoopBack Mode to enable Dummy Pad, 0 - Disable, 1 - Enable
  uint32_t hold_delay;                //Hold Delay for QSPI[0,1] A/B
                                      //Value   QSPI1 B       QSPI1 A
                                      //00      Disable       Disable
                                      //01      Disable       Enable
                                      //10      Enable        Disable
                                      //11      Enable        Enable
  uint32_t rsvd0[2];
  uint32_t device_quad_mode_en;       //Send Quad enable command to SPI device.
  uint32_t device_cmd;                //Command to send to SPI device.
  uint32_t write_cmd_ipcr;            //IPCR register value for write command
  uint32_t write_enable_ipcr;         //IPCR register value for Enable
  uint32_t cs_hold_time;              //CS hold time in terms of serial clock.(for example 1 serial clock cyle)
  uint32_t cs_setup_time;             //CS setup time in terms of serial clock.(for example 1 serial clock cyle)
  uint32_t sflash_A1_size;            //interms of Bytes
  uint32_t sflash_A2_size;            //interms of Bytes
  uint32_t sflash_B1_size;            //interms of Bytes
  uint32_t sflash_B2_size;            //interms of Bytes
  uint32_t sclk_freq;                 //This is serial clock frequency select parameter.
                                      //Value Clock
                                      //00    18 MHz
                                      //01    49 MHz
                                      //02    55 MHz
                                      //03    60 MHz
                                      //04    66 MHz
                                      //05    76 MHz
                                      //06    99Mhz (only SDR mode)
  uint32_t busy_bit_offset;           //SPI Flash device busy bit offset in its status register, used for enabling Quad mode of SPI device
  uint32_t sflash_mode;               //0-Single,1--Dual 2--Quad
  uint32_t sflash_port;               //Port A is always available. This field informs the device ROM the availability of Port B.
  uint32_t ddr_mode_enable;           //This field enables the device ROM to enable DDR mode.
  uint32_t dqs_enable;                //This field enables Data Strobe signal in Serial Flash which supports it.
  uint32_t parallel_mode_enable;      //This field enables parallel mode. Data will be read from serial Flash in parallel
  uint32_t portA_cs1;                 //This field enables CS1 on port A
  uint32_t portB_cs1;                 //This field enables CS1 on port B
  uint32_t fsphs;                     //Select the edge of the sampling clock valid for full speed commands:
  uint32_t fsdly;                     //Select the delay w.r.t. the reference edge for the sample point valid for full speed commands
  uint32_t ddrsmp;                    //Select the sampling point for incoming data when serial flash is in DDR mode. Valid Values are (b000-b111)
  uint16_t command_seq[COMAND_SEQ_LEN]; //set of seq to perform optimum read on SFLASH as as per vendor SFLASH
  uint32_t read_status_ipcr;          //IPCR value of Read Status Reg
  uint32_t enable_dqs_phase;          //Enable DQS Phase
  uint32_t rsvd1[9];
  uint32_t dqs_pad_setting_override;  //DQS pin pad setting override
  uint32_t sclk_pad_setting_override; //SCLK pin pad setting override
  uint32_t data_pad_setting_override; //DATA pins pad setting override
  uint32_t cs_pad_setting_override;   //CS pins pad setting override
  uint32_t dqs_loopback_internal;     //dqs loopback
  uint32_t dqs_phase_sel;             //dqs phase selection
  uint32_t dqs_fa_delay_chain_sel;    //dqs fa delay chain selection
  uint32_t dqs_fb_delay_chain_sel;    //dqs fb delay chain selection
  uint32_t sclk_fa_delay_chain_sel;   //sclk fa delay chain selection
  uint32_t sclk_fb_delay_chain_sel;   //sclk fb delay chain selection
  uint32_t rsvd2[16];
  uint32_t tag;
}SFLASH_CONFIGURATION_PARAM,*SFLASH_CONFIGURATION_PARAM_PTR;


/*************************************
 *  DCD Data
 *************************************/
#define DCD_TAG_HEADER            (0xD2)
#define DCD_TAG_HEADER_SHIFT      (24)
#define DCD_VERSION               (0x41)


/*************************************
 *  IVT Data
 *************************************/
typedef struct _ivt_ {
    /** @ref hdr with tag #HAB_TAG_IVT, length and HAB version fields
     *  (see @ref data)
     */
    uint32_t hdr;
    /** Absolute address of the first instruction to execute from the
     *  image
     */
    uint32_t entry;
    /** Reserved in this version of HAB: should be NULL. */
    uint32_t reserved1;
    /** Absolute address of the image DCD: may be NULL. */
    uint32_t dcd;
    /** Absolute address of the Boot Data: may be NULL, but not interpreted
     *  any further by HAB
     */
    uint32_t boot_data;
    /** Absolute address of the IVT.*/
    uint32_t self;
    /** Absolute address of the image CSF.*/
    uint32_t csf;
    /** Reserved in this version of HAB: should be zero. */
    uint32_t reserved2;
 } ivt;

#define IVT_MAJOR_VERSION           0x4
#define IVT_MAJOR_VERSION_SHIFT     0x4
#define IVT_MAJOR_VERSION_MASK      0xF
#define IVT_MINOR_VERSION           0x1
#define IVT_MINOR_VERSION_SHIFT     0x0
#define IVT_MINOR_VERSION_MASK      0xF

#define IVT_VERSION(major, minor)   \
  ((((major) & IVT_MAJOR_VERSION_MASK) << IVT_MAJOR_VERSION_SHIFT) |  \
  (((minor) & IVT_MINOR_VERSION_MASK) << IVT_MINOR_VERSION_SHIFT))


#define IVT_TAG_HEADER        (0xD1)       /**< Image Vector Table */
#define IVT_SIZE              (uint16_t)(((sizeof(ivt)&0x00FF) << 8) |			\
                                        ((sizeof(ivt)&0xFF00) >> 8) )
#define IVT_PAR               IVT_VERSION(IVT_MAJOR_VERSION, IVT_MINOR_VERSION)

#define IVT_HEADER          (IVT_TAG_HEADER | (IVT_SIZE << 8) | (IVT_PAR << 24))
#define IVT_RSVD            (uint32_t)(0x00000000)


/*************************************
 *  Boot Data
 *************************************/
typedef struct _boot_data_ {
  void * start;           /* boot start location */
  int size;               /* size */
  uint32_t plugin;        /* plugin flag - 1 if downloaded application is plugin */
  uint32_t placeholder;   /* placehoder to make even 0x10 size */
}BOOT_DATA_T;

/* External Variables */
extern const uint32_t device_config_data[];
extern const BOOT_DATA_T boot_data;
extern void __iar_program_start(void);


#endif
