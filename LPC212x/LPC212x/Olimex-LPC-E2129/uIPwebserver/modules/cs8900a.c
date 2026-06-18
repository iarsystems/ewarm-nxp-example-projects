/***************************************************************************
 **
 **    CS8900A device driver
 **
 **    Used with ARM IAR C/C++ Compiler and Assembler.
 **
 **    (c) Copyright IAR Systems 2005
 **
 **    $Revision: 28 $
 **
 ***************************************************************************/

#include <NXP/iolpc2129.h>
#include "cs8900a.h"
#include "uip.h"
#include "uip_arp.h"

typedef struct {                                    // stores CS8900A init sequence
  unsigned int Addr;
  unsigned int Data;
} TInitSeq;

static void skip_frame(void);

const TInitSeq InitSeq[] =
{
  REG_IA,       UIP_ETHADDR0 + (UIP_ETHADDR1 << 8), // set the MAC as Individual Address
  REG_IA + 2,   UIP_ETHADDR2 + (UIP_ETHADDR3 << 8),
  REG_IA + 4,   UIP_ETHADDR4 + (UIP_ETHADDR5 << 8),
  REG_LineCTL,  SERIAL_RX_ON | SERIAL_TX_ON,        // configure the Interface
  REG_RxCTL,    RX_OK_ACCEPT | RX_IA_ACCEPT | RX_BROADCAST_ACCEPT
};

// Writes a word in little-endian byte order to a specified port address
void cs8900a_write(unsigned addr, unsigned int data)
{
  IO0DIR |= 0xff << 16;                             // Data port to output

  IO0CLR = 0xf << 4;                                // Put address on bus
  IO0SET = addr << 4;

  IO0CLR = 0xff << 16;                              // Write the less significant byte to data bus
  IO0SET = data << 16;

  IO0CLR = IOW_PIN;                                 // Toggle IOW-signal
  IO0SET = IOW_PIN;

  IO0CLR = 0xf << 4;
  IO0SET = ((addr | 1) << 4);                       // And put next address on bus

  IO0CLR = 0xff << 16;                              // Write most significant byte to data bus
  IO0SET = data >> 8 << 16;

  IO0CLR = IOW_PIN;                                 // Toggle IOW-signal
  IO0SET = IOW_PIN;
}

// Reads a word in little-endian byte order from a specified port-address
unsigned cs8900a_read(unsigned addr)
{
  unsigned int value;

  IO0DIR &= ~(0xff << 16);                          // Data port to input

  IO0CLR = 0xf << 4;                                // Put address on bus
  IO0SET = addr << 4;

  IO0CLR = IOR_PIN;                                 // IOR-signal low
  value = (IO0PIN >> 16) & 0xff;                    // get less significant byte from data bus
  IO0SET = IOR_PIN;

  IO0SET = 1 << 4;                                  // IOR high and put next address on bus

  IO0CLR = IOR_PIN;                                 // IOR-signal low
  value |= ((IO0PIN >> 8) & 0xff00);                // get most significant byte from data bus
  IO0SET = IOR_PIN;                                 // IOR-signal low

  return value;
}

// Reads a word in big-endian byte order from a specified port-address
unsigned cs8900a_read_big_endian(unsigned addr)
{
  unsigned int value;

  IO0DIR &= ~(0xff << 16);                          // Data port to input

  IO0CLR = 0xf << 4;                                // Put address on bus
  IO0SET = (addr+1) << 4;

  IO0CLR = IOR_PIN;                                 // IOR-signal low
  value = ((IO0PIN >> 8) & 0xff00);                 // get most significant byte from data bus
  IO0SET = IOR_PIN;                                 // IOR-signal high

  IO0CLR = 1 << 4;                                  // Put low address on bus

  IO0CLR = IOR_PIN;                                 // IOR-signal low
  value |= (IO0PIN >> 16) & 0xff;                   // get less significant byte from data bus
  IO0SET = IOR_PIN;

  return value;
}

void cs8900a_init(void)
{
  int i;


  IO0SET = IOR_PIN | IOW_PIN;                       // Reset outputs, control lines high

  IO0SET = RED_LED | YELLOW_LED | GREEN_LED;        // All LEDs OFF

  IO0DIR = ~0U;                                     // all as outputs

  IO0CLR = 0xff << 16;                              // clear data outputs

  // Reset the CS8900A
  cs8900a_write(ADD_PORT, REG_SelfCTL);
  cs8900a_write(DATA_PORT, POWER_ON_RESET);

  // Wait until chip reset
  cs8900a_write(ADD_PORT, REG_SelfST);
  while ((cs8900a_read(DATA_PORT) & INIT_DONE) == 0)
    ;

  // Configure the CS8900A
  for (i = 0; i < sizeof InitSeq / sizeof (TInitSeq); ++i)
    {
      cs8900a_write(ADD_PORT, InitSeq[i].Addr);
      cs8900a_write(DATA_PORT, InitSeq[i].Data);
    }
}

void
cs8900a_send(void)
{
  unsigned u;

  IO0CLR = RED_LED;             // Turn the RED LED ON when frame starts

  // Transmit command
  cs8900a_write(TX_CMD_PORT, TX_START_ALL_BYTES);
  cs8900a_write(TX_LEN_PORT, uip_len);


  u = 8;                        // Set the number of retries
  for (;;)
    {
      // Check for available buffer space
      cs8900a_write(ADD_PORT, REG_BusST);
      if (cs8900a_read(DATA_PORT) & READY_FOR_TX_NOW)
        break;
      if (u -- == 0)
        {
          IO0SET = RED_LED;     // Turn the RED LED OFF at the end of a frame
          return;
        }


      skip_frame();             // No space available, skip the received frame and try again
    }

  IO0DIR |= 0xff << 16;                     // Data port as output

  // Send 40+14=54 bytes of header
  for (u = 0; u < 54; u += 2)
    {
      IO0CLR = 0xf << 4;                    // Put address on the bus
      IO0SET = TX_FRAME_PORT << 4;

      IO0CLR = 0xff << 16;                  // Write less significant byte to data bus
      IO0SET = uip_buf[u] << 16;            // write less significant byte to data bus

      IO0CLR = IOW_PIN;                     // Toggle IOW-signal
      IO0SET = IOW_PIN;

      IO0CLR = 0xf << 4;                    // Put address on the bus
      IO0SET = (TX_FRAME_PORT | 1) << 4;    // and put the next address

      IO0CLR = 0xff << 16;                  // Write less significant byte to data bus
      IO0SET = uip_buf[u+1] << 16;          // write less significant byte to data bus

      IO0CLR = IOW_PIN;                     // Toggle IOW-signal
      IO0SET = IOW_PIN;
    }

  if (uip_len <= 54)
    {
      IO0SET = RED_LED;         // Turn the RED LED OFF at the end of frame
      return;
    }

  // Send remainder of packet, the application data
  uip_len -= 54;
  for (u = 0; u < uip_len; u += 2)
    {
      IO0CLR = 0xf << 4;                    // address
      IO0SET = TX_FRAME_PORT << 4;

      IO0CLR = 0xff << 16;                  // Write less significant byte to data bus
      IO0SET = uip_appdata[u] << 16;        // write less significant byte to data bus

      IO0CLR = IOW_PIN;                     // Toggle IOW-signal
      IO0SET = IOW_PIN;

      IO0CLR = 0xf << 4;                    // Put address on bus
      IO0SET = (TX_FRAME_PORT | 1) << 4;    // and put next address on bus

      IO0CLR = 0xff << 16;                  // Write less significant byte to data bus
      IO0SET = uip_appdata[u+1] << 16;      // write less significant byte to data bus

      IO0CLR = IOW_PIN;                     // Toggle IOW-signal
      IO0SET = IOW_PIN;
    }
  IO0SET = RED_LED;  // Turn the RED LED OFF on the end of frame
}

static void skip_frame(void)
{
  // No space available, skip the received frame and try again
  cs8900a_write(ADD_PORT, REG_RxCFG);
  cs8900a_write(DATA_PORT, cs8900a_read(DATA_PORT) | SKIP_1);
}

u16_t cs8900a_poll(void)
{
  u16_t len, u;

  // Check the receiver event register for a valid frame available
  cs8900a_write(ADD_PORT, REG_RxEvent);
  if ((cs8900a_read(DATA_PORT) & 0xd00) == 0)
    return 0;

  IO0CLR = GREEN_LED;                           // Turn the GREED LED ON when frame is received


  cs8900a_read_big_endian(RX_FRAME_PORT);  // Read the receiver status and discard it


  len = cs8900a_read_big_endian(RX_FRAME_PORT); // Read frame length

  // If the frame is too big to handle, throw it away
  if (len > UIP_BUFSIZE)
    {
      skip_frame();
      return 0;
    }

  IO0DIR &= ~(0xff << 16);                  // Data port to input
  IO0CLR = 0xf << 4;                        // put address on bus
  IO0SET = RX_FRAME_PORT << 4;

  // Read bytes into uip_buf
  u = 0;
  while (u < len)
    {
      IO0CLR = 1 << 4;                      // put address on bus

      IO0CLR = IOR_PIN;                     // IOR-signal low
      uip_buf[u] = IO0PIN >> 16;            // get most significant byte from data bus
      IO0SET = IOR_PIN;                     // IOR-signal high

      IO0SET = 1 << 4;                      // put address on bus

      IO0CLR = IOR_PIN;                     // IOR-signal low
      uip_buf[u+1] = IO0PIN >> 16;          // get most significant byte from data bus
      IO0SET = IOR_PIN;                     // IOR-signal high
      u += 2;
    }

  IO0SET = GREEN_LED;                       // Turn the GREED LED OFF when frame finished.
  return len;
}
