#include "lpc11Uxx.h"
#include "lpc_types.h"
#include "ps2kbd.h"
#include "scancodes.h"
#include "glcd_api.h"
#include "lpc11xx_syscon.h"
#include "gpio.h"


#define kbDAT (LPC_GPIO->PIN[1] & KB_DAT)
#define kbCLK (LPC_GPIO->PIN[1] & KB_CLK)
#define K_STARTBIT    1
#define K_PARITYBIT  10
#define K_STOPBIT    11


uint8_t keyBuf[KBUFFSIZE];  //PS2 Keyboard buffer, the register to store characters key in
uint8_t keyIn;              //Index into PS/2 key buf where next scan code will be inserted
uint8_t keyOut;             //Index into PS/2 key buf where next scan code will be removed
uint8_t keyRead;            //Number of keys read from the PS/2 keyboard
uint8_t bitcount=0;         // 0 = neg.  1 = pos.
uint8_t flag = 0;

/*****************************************************************************
** Function name:   FLEX_INT0_IRQHandler
**
** Descriptions:    external INT handler
**
** parameters:      None
**
** Returned value:  None
**
*****************************************************************************/
void FLEX_INT0_IRQHandler(void)
{
static uint8_t key_data;          // Holds the received scan code
uint32_t clkstat;
uint32_t datstat;
volatile uint32_t paritystat;

  if ( LPC_GPIO_PIN_INT->FALL & (0x1<<0) )
  {
    clkstat = kbCLK;             //check CLK pin state;
    datstat = kbDAT;             //check DAT pin state;
    LPC_GPIO_PIN_INT->FALL = 0x1<<0;
    bitcount++;
    if (bitcount==K_STARTBIT)
    {
      if (datstat || clkstat) bitcount=0;
      key_data=0;
      paritystat=0;
    }
    else if (bitcount==K_PARITYBIT)
    {
      paritystat = datstat;
    }
    else if (bitcount==K_STOPBIT)
    {
      decode (key_data);
      bitcount=0;
    }
    else
    {
      // For all bits from 2, 3...9 (total 8-bit)
      key_data= key_data >> 1;
      if (datstat)
      {
        key_data = key_data | 0x80;
      }
    }
  }
   LPC_GPIO_PIN_INT->IST = 0x1<<0;
}


/**
*****************************************************************************
  Function Name : init_keyboard()

  Description   :

  Input         :

  Output        :

  Note          :
******************************************************************************
*/
void init_keyboard(void)
{
  bitcount = 0;
  keyIn =0;
  keyOut=0;
  keyRead=0;

  GPIOInit();

  GPIOSetDir(PORT1, 15, 0);
  GPIOSetDir(PORT1, 16, 0);

  GPIOSetFlexInterrupt( CHANNEL0,PORT1, 16, 0, 0);
  GPIOFlexIntEnable( CHANNEL0, 0);
}

/**
*****************************************************************************
  Function Name : is_valid_ascii()

  Description   :

  Input         :

  Output        :

  Note          :
******************************************************************************
*/
uint8_t is_valid_ascii(int8_t key)
{
uint8_t retval = FALSE;

  if(key >= ' ' && key <= '~')
    retval = TRUE;

  return retval;
}

/**
*****************************************************************************
  Function Name : process_keyboard()

  Description   :

  Input         :

  Output        :

  Note          :
******************************************************************************
*/
void process_keyboard()
{
//S8 key;
int8_t key = 0;
static int position = 0;
static int char_count=0;
  key = xgetkey();
  if(is_valid_ascii(key))
  {
    init_xy_axis(PAGE0 + 6,COL0+position);
    lcd_display_char(key);
    if(char_count == MAX_CHARS_IN_ONE_LINE)
    {
      glcd_clear();
      position = 0;
      char_count=0;
    }
    position+=7;
    char_count++;
  }
}

/**
*****************************************************************************
  Function Name : decode()

  Description   :

  Input         :

  Output        :

  Note          :
******************************************************************************
*/
void decode(uint8_t sc)
{
static unsigned char is_up=0, shift = 0;    //, mode = 0;
unsigned char i;

  if (!is_up)                                 /* Last data received was the up-key identifier */
  {
    switch (sc)
    {
      case BREAKCHAR :                      /*  The up-key identifier */
        is_up = 1;
        break;
      case NONUMCODE :
        //is_up = 1;
        break;
      case LEFTSHIFT :
        shift = 1;
        break;
      case RIGHTSHIFT:
        shift = 1;
        break;
      //case 0x58: //caps lock
        //break;
      default:
      if (!shift)                          /* If shift not pressed */
      {
        // do a table look-up
        for (i = 0; unshifted[i][0]!=sc && unshifted[i][0]; i++);
        if (unshifted[i][0] == sc)
        {
          putkeybuf((unshifted[i][1]));
        }
      }
      else
      {
        /* look-up */
        for (i = 0; shifted[i][0]!=sc && shifted[i][0]; i++);
        if (shifted[i][0] == sc)
        {
          putkeybuf((shifted[i][1]));
        }
      }
      break;
    }
  }
  else
  {
    is_up = 0;
    switch (sc)
    {
      case LEFTSHIFT :
      case RIGHTSHIFT:
        shift = 0;
        break;
    }
  }
}

/**
*****************************************************************************
  Function Name : putkeybuf()

  Description   :

  Input         :

  Output        :

  Note          :
******************************************************************************
*/
void putkeybuf (uint8_t key)
{
  if (keyRead <KBUFFSIZE)        //make sure that we don't overflow the buffer
  {
    keyRead++;                    //Increment the number of keys read
    keyBuf[keyIn++] = key;        //Store the scan code into the buffer
    if (keyIn >= KBUFFSIZE)       //Adjust index to the next scan code to put in buffer
    {
      keyIn = 0;
    }
  }
}

/**
*****************************************************************************
  Function Name : keyhit()

  Description   :

  Input         :

  Output        :

  Note          :
******************************************************************************
*/
uint8_t keyhit(void)
{
unsigned char hit;

  hit = (unsigned char)(keyRead > 0)? TRUE : FALSE;
  return(hit);
}

/**
*****************************************************************************
  Function Name : xgetkey()

  Description   :

  Input         :

  Output        :

  Note          :
******************************************************************************
*/
uint8_t xgetkey(void)
{
  unsigned char key=0;

  if (keyRead > 0)
  {
    keyRead--;                    /* Decrement the number of keys in the buffer */
    key = keyBuf[keyOut];         /* Get scan code from the buffer */
    keyOut++;
    if (keyOut >= KBUFFSIZE)
    {
      keyOut = 0;
    }
  }
  else
  {
    return (0xFF);               /* No scan codes in buffer, return -1 */
  }
  return (key);                  /* return the scan code of the key pressed */
}

