/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : usercmd.c
 *    Description : Sine PWM user command tables and functions
 *
 *    History :
 *    1. Date        : 18.4.2008 
 *       Author      : Stoyan Choynev
 *       Description : Initial revision
 *
 *    $Revision: 4271 $
 **************************************************************************/

/** include files **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <NXP\iolpc2103.h>
#include "commands.h"

/** local definitions **/
#define LED_OFF()  {FIOSET = (1<<26);}
#define LED_ON()   {FIOCLR = (1<<26);}
/** default settings **/

/** external functions **/
extern int GetSineFreq();
extern int GetSineLevel();
extern void SetSineFreq(unsigned int Freq);
extern void SetSineLevel(unsigned int Level);
/** external data **/

/** internal functions **/
int Void(int * argc, char *** argv,const struct s_cmd * cmd);
int GetFreq(int * argc, char *** argv,const struct s_cmd * cmd);
int GetAmp(int * argc, char *** argv,const struct s_cmd * cmd);
int SetFreq(int * argc, char *** argv,const struct s_cmd * cmd);
int SetAmp(int * argc, char *** argv,const struct s_cmd * cmd);
int LedOn(int * argc, char *** argv,const struct s_cmd * cmd);
int LedOff(int * argc, char *** argv,const struct s_cmd * cmd);

/** public data **/
const unsigned char Prompt[]="SinePWM>>";
/** private data **/
const struct s_cmd TestCmd[]=
{
  {"Test1", TestCmd+1, NULL, Void, "Nested Menu Test1"},
  {"Test2", NULL, NULL, Void, "Nested Menu Test2"}
};

const struct s_cmd SetCmd[] = 
{
  {"Frequency", SetCmd+1, NULL, SetFreq,"Sets Sine Frequency(Hz)"},
  {"Amplitude", SetCmd+2, NULL, SetAmp,"Sets Sine Amplitude"},
  {"Test"     , NULL    , TestCmd,CmdExec, "Sub Command"}
};

const struct s_cmd GetCmd[] = 
{
  {"Frequency", GetCmd+1, NULL, GetFreq, "Shows the current Sine Frequency(Hz)"},
  {"Amplitude", NULL, NULL, GetAmp,"Shows the current Sine Amplitude"}
};
const struct s_cmd LedCmd[] = 
{
  {"On" , LedCmd+1, NULL, LedOn, "Turns The LED On"},
  {"Off", NULL    , NULL, LedOff, "Turns The LED Off"}
};

const struct s_cmd MainCmd[] = 
{
  {"Help" , MainCmd+1 , MainCmd+1, Help  ,""},
  {"Set"  , MainCmd+2 , SetCmd , CmdExec ,"Sub Command"},
  {"Get"  , MainCmd+3 , GetCmd , CmdExec ,"Sub Command"},
  {"Led"  , MainCmd+4 , LedCmd , CmdExec ,"Sub Command"},
  {"Quit" , NULL      , NULL   , NULL   ,"Quit"}  
};

/** public functions **/

/** private functions **/
int Void(int * argc, char *** argv,const struct s_cmd * cmd)
{
  return 0;
}
int GetFreq(int * argc, char *** argv,const struct s_cmd * cmd)
{
  printf("\tSine Frequency = %dHz",GetSineFreq());
  return 0;
}

int GetAmp(int * argc, char *** argv,const struct s_cmd * cmd)
{
  printf("\tSine Amplitude = %d", GetSineLevel());
  return 0;
}

int SetFreq(int * argc, char *** argv,const struct s_cmd * cmd)
{
  if(*argc)
  {
    int Freq = atoi(**argv);
    (*argv)++;
    (*argc)--;    
    if((9 < Freq) && ( 2000 > Freq))
    {
      SetSineFreq(Freq); 
    }
    else 
    {
      *argc = 0;
      return -1;
    }
  }
  
  printf("\tSine Frequency = %dHz",GetSineFreq());
  return 0;
}

int SetAmp(int * argc, char *** argv,const struct s_cmd * cmd)
{
  if(*argc)
  {
    int Level = atoi(**argv);
    (*argv)++;
    (*argc)--;    
    if((0 < Level) && ( 256 > Level))
    {
      SetSineLevel(Level); 
    }
    else 
    { 
      *argc = 0;
      return -1;
    }
  }
  printf("\tSine Amplitude = %d", GetSineLevel());
  return 0;
}

int LedOn(int * argc, char *** argv,const struct s_cmd * cmd)
{
  LED_ON();
  return 0;
}
int LedOff(int * argc, char *** argv,const struct s_cmd * cmd)
{
  LED_OFF();
  return 0;
}
