#include "dbg.h"

#include "hal_types.h"
#include "DebugTrace.h"

void dprintf(const char* fmt, ...)
{
  char buf[64];
  va_list argp;
  va_start(argp, fmt);
  vsprintf(buf, fmt, argp);
  debug_str((uint8*)buf);
  va_end(argp);
}