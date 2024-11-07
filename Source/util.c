#include "util.h"
#include "bdb_interface.h"

void zigmo_util_delay(uint32 num_cycles)
{
   volatile uint32 i;
   for (i = 0; i < num_cycles; i++) {
      asm("NOP");
   };
}


void zigmo_util_delay_microsec(uint16 microSecs) {
  while (microSecs--) {
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
  }
}

#if BDBREPORTING_MAX_ANALOG_ATTR_SIZE == 4
  uint8 reportableChange[] = {0x01, 0x00, 0x00, 0x00};
#endif