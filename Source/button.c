#include "button.h"

#include "AF.h"
#include "OSAL_Timers.h"
#include "OSAL_Memory.h"
#include "OnBoard.h"

#include "hal2.h"

#define ZIGMO_BTN_NUM 4
#define ZIGMO_BTN_LONG_PRESS_DURATION 3000

#define ZIGMO_BTN_TIMER_EVT      0x0080
#define ZIGMO_BTN_TIMER_EVT_MASK 0x00F0

#define ZIGMO_BTN_ST_PRESSED         0x01



struct ZigmoButton {
  uint8 state;
};

static uint8 g_button_task_id;
static uint8 g_target_task_id;
static ZigmoButton g_zigmo_btns[4] = {0};


void zigmo_buttons_init(uint8 task_id)
{
  g_button_task_id = task_id;
  RegisterForKeys( g_button_task_id );
}

void zigmo_buttons_set_target_task(uint8 task_id)
{
  g_target_task_id = task_id;
}

/**
 * Reroutes OSAL timer event to the target task.
 */
static void zigmo_button_process_timer_evt(uint16 events)
{
  // 4 bits for the button event
  uint8 btn_evt = (events & ZIGMO_BTN_TIMER_EVT_MASK) >> 4;
  // Last 2 bits is button id.
  uint8 btn_id = btn_evt & 0x03;

  ZigmoButtonEvt* msgPtr = (ZigmoButtonEvt*)osal_msg_allocate(sizeof(ZigmoButtonEvt));
  if (msgPtr)
  {
    msgPtr->hdr.event = ZIGMO_BTN_EVENT;
    msgPtr->btn_id = btn_id;
    osal_msg_send(g_target_task_id, (uint8*)msgPtr);
  }
}

static void zigmo_button_notify_hw_state(uint8 btn_id, bool pressed)
{
  ZigmoButton* b = &g_zigmo_btns[btn_id];
  if (pressed)
  {
    if (!(b->state & ZIGMO_BTN_ST_PRESSED))
    {
      b->state |= ZIGMO_BTN_ST_PRESSED;
      osal_start_timerEx(g_button_task_id,
                         ZIGMO_BTN_TIMER_EVT | (btn_id << 4),
                         ZIGMO_BTN_LONG_PRESS_DURATION);
    }
  }
  else
  {
    if (b->state & ZIGMO_BTN_ST_PRESSED)
    {
      b->state &= ~ZIGMO_BTN_ST_PRESSED;
      osal_stop_timerEx(g_button_task_id,
                        ZIGMO_BTN_TIMER_EVT | (btn_id << 4));
    }
  }
}


uint16 zigmo_buttons_event_loop(uint8 task_id, uint16 events)
{
  afIncomingMSGPacket_t *MSGpkt;

  if (events & ZIGMO_BTN_TIMER_EVT)
  {
    // Timer fired.
    zigmo_button_process_timer_evt(events);
    return (events & ~ZIGMO_BTN_TIMER_EVT_MASK);
  }

  if ( events & SYS_EVENT_MSG )
  {
    while ( (MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( g_button_task_id )) )
    {
      switch ( MSGpkt->hdr.event )
      {
        case KEY_CHANGE:
          // Hardware key pressed. (Only btn 1 is supported)
          zigmo_button_notify_hw_state(
            ZIGMO_BTN_1,
            (bool)(((keyChange_t *)MSGpkt)->keys & ZIGMO_HAL_KEY_BTN_1));
          break;
      }

      // Release the memory
      osal_msg_deallocate( (uint8 *)MSGpkt );
    }

    // return unprocessed events
    return (events ^ SYS_EVENT_MSG);
  }
  return 0;
}


