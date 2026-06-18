#include "debug_frmwrk.h"

void init_serial (void)
{
  debug_frmwrk_init();
}

int sendchar(int ch)
{
  if (ch == '\n')
    _DBC(0x0D);
  _DBC(ch);
  return (ch);
}

int getkey(void)
{
  return (_DG);
}

unsigned char getchar(void)
{
  return getkey();
}
