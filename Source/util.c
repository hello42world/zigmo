#include "util.h"

void zigmo_util_delay(uint16 num_cycles)
{
   volatile uint32 i;
   for (i = 0; i < num_cycles; i++) { };
}
