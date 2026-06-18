#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_

/* Joystick defines */
#define KEY_CENTER 0x01
#define KEY_UP     0x02
#define KEY_RIGHT  0x04
#define KEY_LEFT   0x08
#define KEY_DOWN   0x10

void joyInit(void);
UNS_8 joyState(void);

#endif
