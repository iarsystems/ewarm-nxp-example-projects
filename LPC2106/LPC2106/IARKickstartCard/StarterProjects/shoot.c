
/*
 * $Revision: 28 $
 */

/*
 Shoot
 Kickstart Card shooting game. The object is to hit the led that blinks by
 pressing the userbutton*) when the moving light reaches it.
 
 Uses timer interrupt and button interrupt (external interrupt).
 
 The game is played like this:
 
 1. The program starts in idle mode and displays running lights.
 2. Press the userbutton to get to the start position.
 3. All LED:s blinks two times and then the current game level is displayed 
    as a binary number.
 4. The start position is reached and the target starts to blink.
 5. Press the userbutton to release the ball.
 6. When the ball reaches the blinking target, press the userbutton and try
    to hit it.
 7. If the ball misses, the ball continues to run past the target slower and slower,
    and the game is over. The program starts over from 1.
 8. If the ball hits the target an "explosion" is displayed. The level is increased
    which means that the speed will be higher on the next run of the ball. The game
    continues at 3.

  *) The userbutton is the INT2 button when ROM-monitor is used, and the INT0
  button when the program is run with J-Link or standalone.

  Jumper settings:
  ----------------
  For ROM-monitor:
  Set JP4 (enables UART0, used for ROM-monitor communication).
  Set JP5 (enables INT2, the userbutton when ROM-monitor is used).
  Set jumpers p0.2-p0.14, p0.16, p0.17 and p0.31 in the LED jumper block.

  For J-Link or Standalone:
  Set JP6 to JTAG1.
  Set JP1 (enables INT0, the userbutton when the program runs with J-Link or Standalone).
  Set jumpers p0.0-p0.15 in the LED jumper block.
*/

#include <stdio.h>
#include <string.h>
#include <intrinsics.h>
#include "timer.h"
#include "lpc210x.h"

//The target is led DS13
#define TARGET 0x1000

typedef enum {sInit, sIdle, sDisplayLevel, sWaiting, sRunning, sFired} state_t;

static state_t state = sInit;
static int rundelay;
static unsigned short level;
static unsigned short pos;

void gameover()
{
  // Display gameover pattern
  LPC210xLedSet(0x2000,0xffff);
  Sleep(700);
  LPC210xLedSet(0x4000,0xffff);
  Sleep(1000);
  LPC210xLedSet(0x8000,0xffff);
  Sleep(1300);
  LPC210xLedSet(0x0000,0xffff);
}

void displaylevel()
{
  // Blink all leds two times then show level
  LPC210xLedSet(0x0000,0xffff);
  Sleep(300);
  LPC210xLedSet(0xffff,0xffff);
  Sleep(300);
  LPC210xLedSet(0x0000,0xffff);
  Sleep(300);
  LPC210xLedSet(0xffff,0xffff);
  Sleep(300);
  LPC210xLedSet(0x0000,0xffff);
  Sleep(300);
  LPC210xLedSet(level,0xffff);
  Sleep(2000);
  LPC210xLedSet(0x0000,0xffff);
  Sleep(500);
}

void nextlevel()
{
  level++;                   // Goto next level
  rundelay = rundelay*9/10;  // Increase speed of shot.
}

void target_hit()
{
  // The target has been hit
  unsigned short ledl,ledr;
  unsigned int i,j;

  //Display explosion...
  for(i=0; i<5; i++)
  {
    for(j=0; j<12; j++)
    {
      ledl |= TARGET;
      ledr |= TARGET;
      ledl <<= 1;
      ledr >>= 1;
      LPC210xLedSet(ledl|ledr,0xffff);
      Sleep(50);
    }
    ledl=0;
    ledr=0;
  }
  nextlevel();  //Go to next level.
}


// The handler for the INT2 button.
// Called from the IRQ handler exactly once for each button press.
void UserButtonHandler()
{
  // User button pressed, goto next state.
  switch(state)
  {
  case sInit:
    //No state change
    break;
  case sIdle:
    state = sDisplayLevel;
    break;
  case sDisplayLevel:
    break;
  case sWaiting:
    state = sRunning;
    break;
  case sRunning:
    state = sFired;
    break;
  case sFired:
    //No state change
    break;
  default:
    break;
  }
}

int main(void)
{
  unsigned int leds = 0x001f;
 
  // System initialization, this will map the exception vectors.
  LPC210xSystemInit();
 
  // Set up peripheral registers.
  LPC210xInitPIO();

  // First disable interrupts.
  __disable_interrupt();
  // Setup interrupt controller.
  LPC210xInitVIC();
  LPC210xInitTimerInterrupt(TimerBeat);
#ifdef ROM_MONITOR_DEBUG
  //Uses button INT2 when ROM-monitor is used.
  LPC210xInitExternalInterrupt2(UserButtonHandler);
#else
  //Uses button INT0 when running with J-Link debugger or standalone.
  LPC210xInitExternalInterrupt0(UserButtonHandler);
#endif

  // Periodic timer initialization.
  LPC210xInitTimer();

  // External interrupt initialization.
#ifdef ROM_MONITOR_DEBUG
  //Uses button INT2 when ROM-monitor is used.
  LPC210xInitEINT2();
#else
  //Uses button INT0 when running with J-Link debugger or standalone.
  LPC210xInitEINT0();
#endif
  
  // Enable interrupts.
  __enable_interrupt();

  // Start periodic timer.
  LPC210xStartTimer();

  // Loop forever.
  for (;;)
  {
    switch(state)
    {
    case sInit:
      level = 1;
      rundelay = 200;
      leds = 0x001f;
      state = sIdle;
      break;
    case sIdle:
      // Display running lights
      LPC210xLedSet(leds,0xffff);
      Sleep(65);        // Display for 65 ms.
      leds <<= 1;
      leds |= leds>>16;
      leds &= 0xffff;
      break;
    case sDisplayLevel:
      //Display level
      displaylevel();
      state = sWaiting;
      pos = 0x0001;
      leds = TARGET;
      break;
    case sWaiting:
      //Wait for start
      leds ^= TARGET;
      LPC210xLedSet(leds|pos,0xffff);
      Sleep(rundelay);
      break;
    case sRunning:
      //Run towards target
      pos <<= 1;
      if(pos > TARGET)
      {
        //Passed target, game over.
        gameover();
        state = sInit;
      }
      else
      {
        //Blink target LED.
        leds ^= TARGET;
        LPC210xLedSet(leds|pos,0xffff);
        Sleep(rundelay);
      }
      break;
    case sFired:
      if(pos == TARGET)
      {
        // A HIT!
        target_hit();
        state = sDisplayLevel;
      }
      else
      {
        // Miss...
        gameover();
        state = sInit;
      }
      break;
    }
  }
}
