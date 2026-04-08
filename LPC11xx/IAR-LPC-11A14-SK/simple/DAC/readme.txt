Purpose:
To Demonstrate the Digital To Anlog (DAC) peripheral on the LPC11A14

Running mode:
* Compile, Flash the program and reset.

Pins used:
P0_4 for AOUT 

How to use it?

>>To generate a sine wave: enable the sine_wave flag in dac.h. The DAC outputs a Sine wave of Max freq 68Mhz on the P0_4 pin. Figure scope_0.png (placed in the project folder)shows the output obtained.
>>To generate a saw tooth wave, disable sine_wave flag in dac.h.

>>In order to test timer trigger, ATRG_ENABLED needs to be set along with TIMER_TRIG_ENABLED.This outputs a Saw tooth waveform on the P0_4 pin.

Tip:
*To vary the frequency of the sine wave obtained, change the delay
*in Line 130 (of dactest.c), the dividend value in the eqn (sinewave[i]/80) << 6 ) controls the output voltage   