#include "quadspi_boot.h"

#pragma section="app_image"

/*QuadSPI controller definitions*/
#define QSPI_LUT_INST(code,pads,opr)       ((uint16_t)((code<<10) | (pads<<8) | (opr)))


#pragma location=".conf"
__root const SFLASH_CONFIGURATION_PARAM quadspi_conf = {
  .dqs_loopback = 0,                  //DQS LoopBack Mode to enable Dummy Pad, 0 - Disable, 1 - Enable
  .hold_delay = 0,                    //Hold Delay for QSPI[0,1] A/B
  .device_quad_mode_en = 0,           //Send Quad enable command to SPI device.
  .device_cmd = 0,                    //Command to send to SPI device.
  .write_cmd_ipcr = 0,                //IPCR register value for write command
  .write_enable_ipcr = 0,             //IPCR register value for Enable
  .cs_hold_time = 3,                  //CS hold time in terms of serial clock.(for example 1 serial clock cyle)
  .cs_setup_time = 3,                 //CS setup time in terms of serial clock.(for example 1 serial clock cyle)
  .sflash_A1_size = 0x2000000,        //interms of Bytes
  .sflash_A2_size = 0,                //interms of Bytes
  .sflash_B1_size = 0x2000000,        //interms of Bytes
  .sflash_B2_size = 0,                //interms of Bytes
  .sclk_freq = 4,                     //This is serial clock frequency select parameter.
                                      //Value Clock
                                      //00    18 MHz
                                      //01    49 MHz
                                      //02    55 MHz
                                      //03    60 MHz
                                      //04    66 MHz
                                      //05    76 MHz
                                      //06    99Mhz (only SDR mode)
  .busy_bit_offset = 7,               //SPI Flash device busy bit offset in its status register, used for enabling Quad mode of SPI device
  .sflash_mode = 2,                   //0-Single,1--Dual 2--Quad
  .sflash_port = 0,                   //Port A is always available. This field informs the device ROM the availability of Port B.
  .ddr_mode_enable = 0,               //This field enables the device ROM to enable DDR mode.
  .dqs_enable = 0,                    //This field enables Data Strobe signal in Serial Flash which supports it.
  .parallel_mode_enable = 0,          //This field enables parallel mode. Data will be read from serial Flash in parallel
  .portA_cs1 = 0,                     //This field enables CS1 on port A
  .portB_cs1 = 0,                     //This field enables CS1 on port B
  .fsphs = 0,                         //Select the edge of the sampling clock valid for full speed commands:
  .fsdly = 0,                         //Select the delay w.r.t. the reference edge for the sample point valid for full speed commands
  .ddrsmp = 0,                        //Select the sampling point for incoming data when serial flash is in DDR mode. Valid Values are (b000-b111)
  .command_seq[0] = QSPI_LUT_INST(1,0,0xEB),
  .command_seq[1] = QSPI_LUT_INST(2,2,24),
  .command_seq[2] = QSPI_LUT_INST(3,2,10),
  .command_seq[3] = QSPI_LUT_INST(7,2,8),
  .command_seq[4] = QSPI_LUT_INST(9,0,0),
  .read_status_ipcr = 0,              //IPCR value of Read Status Reg
  .enable_dqs_phase = 0,              //Enable DQS Phase
  .dqs_pad_setting_override = 0,      //DQS pin pad setting override
  .sclk_pad_setting_override = 0,     //SCLK pin pad setting override
  .data_pad_setting_override = 0,     //DATA pins pad setting override
  .cs_pad_setting_override = 0,       //CS pins pad setting override
  .dqs_loopback_internal = 0,         //dqs loopback
  .dqs_phase_sel = 0,                 //dqs phase selection
  .dqs_fa_delay_chain_sel = 0,        //dqs fa delay chain selection
  .dqs_fb_delay_chain_sel = 0,        //dqs fb delay chain selection
  .sclk_fa_delay_chain_sel = 0,       //sclk fa delay chain selection
  .sclk_fb_delay_chain_sel = 0,       //sclk fb delay chain selection
  .tag = QSPI_TAG,
};

#pragma location=".ivt"
__root const ivt image_vector_table = {
  IVT_HEADER,                         /* IVT Header */
  (uint32_t)__iar_program_start,      /* Image  Entry Function */
  IVT_RSVD,                           /* Reserved = 0 */
  NULL,                               /* Address where DCD information is stored */
  (uint32_t)&boot_data,               /* Address where BOOT Data Structure is stored */
  (uint32_t)&image_vector_table,      /* Pointer to IVT Self (absolute address */
  (uint32_t)0,                        /* Address where CSF file is stored */
  IVT_RSVD,
};

#pragma location=".ivt"
const BOOT_DATA_T boot_data = {
  __section_begin("app_image"),
  __section_size("app_image"),      /* size */
  0,                                /* Plugin flag*/
  0xFFFFFFFF                        /* empty - extra data word */
};
