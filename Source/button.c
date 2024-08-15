#include "button.h"

#include "OSAL_Timers.h"
#include "OSAL_Memory.h"

#define ZIGMO_BTN_NUM 4
#define ZIGMO_BTN_LONG_PRESS_DURATION 3000

#define ZIGMO_BTN_ST_PRESSED         0x01


struct ZigmoButton {
  uint8 state;
};

static uint8 g_button_task_id;
static ZigmoButton g_zigmo_btns[4] = {0};


void zigmo_buttons_init(uint8 task_id)
{
  g_button_task_id = task_id;
}

void zigmo_button_process_osal_evt(uint16 events)
{
  // 4 bits for the button event
  uint8 btn_evt = (events & ZIGMO_BTN_OSAL_EVT_MASK) >> 4;
  // Last 2 bits is button id.
  uint8 btn_id = btn_evt & 0x03;

  ZigmoButtonEvt* msgPtr = (ZigmoButtonEvt*)osal_msg_allocate(sizeof(ZigmoButtonEvt));
  if (msgPtr)
  {
    msgPtr->hdr.event = ZIGMO_BTN_CHANGE;
    msgPtr->btn_id = btn_id;
    osal_msg_send(g_button_task_id, (uint8*)msgPtr);
  }
}

void zigmo_button_notify_hw_state(uint8 btn_id, bool pressed)
{
  ZigmoButton* b = &g_zigmo_btns[btn_id];
  if (pressed)
  {
    if (!(b->state & ZIGMO_BTN_ST_PRESSED))
    {
      b->state |= ZIGMO_BTN_ST_PRESSED;
      osal_start_timerEx(g_button_task_id,
                         ZIGMO_BTN_OSAL_EVT | (btn_id << 4),
                         ZIGMO_BTN_LONG_PRESS_DURATION);
    }
  }
  else
  {
    if (b->state & ZIGMO_BTN_ST_PRESSED)
    {
      b->state &= ~ZIGMO_BTN_ST_PRESSED;
      osal_stop_timerEx(g_button_task_id,
                        ZIGMO_BTN_OSAL_EVT | (btn_id << 4));
    }
  }
}
