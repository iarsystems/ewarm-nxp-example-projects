#ifndef _RTC_PLATFORM_DS1307_H_
#define _RTC_PLATFORM_DS1307_H_

#define RTC_INIT_ERROR  1
uint8_t pcf8523t_toBCD(uint8_t bin_val);
uint8_t pcf8523t_fromBCD(uint8_t bcd_val);
int pcf8523t_wr_regs(uint8_t * buf);
int pcf8523t_rd_regs(uint8_t * buf);

#endif
