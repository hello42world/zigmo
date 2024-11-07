#ifndef ZIGMO_UTIL_H
#define ZIGMO_UTIL_H

#include "hal_types.h"

extern uint8 reportableChange[];

#define STATIC_ASSERT(COND, MSG) typedef char static_assertion_##MSG[(COND)?1:-1]

extern void zigmo_util_delay(uint32 num_cycles);

extern void zigmo_util_delay_microsec(uint16 micro_secs);

#endif