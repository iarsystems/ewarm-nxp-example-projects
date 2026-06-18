/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * lcd_test.c  for TWR-KM34Z75M board
 ******************************************************************************/
#include "drivers.h"
#include "LCD.h"

/* LEDs definition                                                            */
#define LED_G  PIN3        /* PTJ3 */
#define LED_R  PIN4        /* PTJ4 */
#define LED_O  PIN0        /* PTD0 */

#define DELAY()                 { arch_delay(100000); }

static const char digits[]=
{
  '0','1','2','3','4','5','6','7','8','9','A','b','c','d','E','F','8'
};

static uint32 tmp32;
static uint16 tmp16;
static uint8  tmpNum;
static uint8  led_state = 0;

/******************************************************************************/
void main (void)
{
  /* initialize SIM module                                                    */
  SIM_Init  (SIM_MODULE_ALL_PERIPH_ON_CONFIG);

  /* clock mode 2:1:1, 48MHz                                                  */
  SIM_SetClkMode (SYSCLK_MODE1);
  SIM_SetClkDiv  (SYSCLK_DIV1);

  ADC_Init (ADC_MODULE_16B_SWTRG_XREF_CONFIG, HWAVG_32, ADC_CH_SE_POLL_CONFIG(AD_OFF),
            ADC_CH_DISABLE_CONFIG, ADC_CH_DISABLE_CONFIG, ADC_CH_DISABLE_CONFIG,
            0, NULL);

  LCD_Init (LCD_CHARGEPUMP_EN_SOURCE_ALT_BACKPLANES_8,                         \
            LCD_FRONT_PLANE_PINS_TWR256, LCD_BACK_PLANE_PINS_TWR256,           \
            LCD_ASSIGN_BACK_PLANES_TWR256, 0, NULL);

  PORT_Init (PORTJ, PORT_MODULE_LED_MODE, LED_G);
  PORT_Init (PORTJ, PORT_MODULE_LED_MODE, LED_R);
  PORT_Init (PORTD, PORT_MODULE_LED_MODE, LED_O);

  GPIO_Init (GPIOJ, GPIO_OUT_LOGIC1_MODE, LED_G);
  GPIO_Init (GPIOJ, GPIO_OUT_LOGIC1_MODE, LED_R);
  GPIO_Init (GPIOD, GPIO_OUT_LOGIC1_MODE, LED_O);

  GPIO_Set  (GPIOJ, LED_G);
  GPIO_Set  (GPIOJ, LED_R);
  GPIO_Set  (GPIOD, LED_O);

  /* test ON/OFF segments                                                     */
  DELAY();
  LCD_Write_Segments(LCD_ALL_SEGMENTS_ON_TWR256);
  DELAY(); DELAY(); DELAY(); DELAY(); DELAY(); DELAY(); DELAY(); DELAY();
  LCD_Write_Segments(LCD_ALL_SEGMENTS_OFF_TWR256);
  DELAY(); DELAY(); DELAY(); DELAY(); DELAY(); DELAY(); DELAY(); DELAY();

  /* turn ON special LCD segments                                             */
  _LCD_CHAR_V_S3_ON;
  _LCD_CHAR_V_S6_ON;
  _LCD_CHAR_V_S7_ON;
  _LCD_CHAR_V_S9_ON;
  _LCD_CHAR_V_S11_ON;
  _LCD_CHAR_V_S12_ON;
  _LCD_CHAR_V_S13_ON;

  _LCD_DOT_P3_ON;

  while (1)
  {
    ADC_Start(CHA, ADC_SE8);
    while (!ADC_Ready (CHA));
    tmp16 = ADC_Read(CHA);
    tmp32 = 50050 * tmp16;

    tmpNum = (uint8) (tmp32/1000000000);
    tmp32 = tmp32 % 1000000000;
    LCD_Delete_Char(3);
    LCD_Write_Char ( digits[tmpNum] );

    tmpNum = (uint8) (tmp32/100000000);
    tmp32 = tmp32 % 100000000;
    LCD_Delete_Char(2);
    LCD_Write_Char ( digits[tmpNum] );

    tmpNum = (uint8) (tmp32/10000000);
    tmp32 = tmp32 % 10000000;
    LCD_Delete_Char(1);
    LCD_Write_Char ( digits[tmpNum] );

    tmpNum = (uint8) (tmp32/1000000);

    LCD_Delete_Char(0);
    LCD_Write_Char ( digits[tmpNum] );

    switch(led_state)
    {
      case 0:
         GPIO_Clr  (GPIOJ, LED_G);
         GPIO_Set  (GPIOJ, LED_R);
         GPIO_Set  (GPIOD, LED_O);
         led_state = 1;
         break;

      case 1:
         GPIO_Set  (GPIOJ, LED_G);
         GPIO_Clr  (GPIOJ, LED_R);
         GPIO_Set  (GPIOD, LED_O);
         led_state = 2;
         break;

      case 2:
         GPIO_Set  (GPIOJ, LED_G);
         GPIO_Set  (GPIOJ, LED_R);
         GPIO_Clr  (GPIOD, LED_O);
         led_state = 0;
         break;
    }
    DELAY();
    DELAY();
    DELAY();
    DELAY();
  }
}
