#include "led.h"
#include "OSAL_Timers.h"
#include "comdef.h"
#include "dbg.h"

static uint8 g_zigmo_led_task_id = 0;

#define ZIGMO_LED_TIMER_EVT      0x0080

struct ZigmoLed {
  uint8 num_blinks;
} g_zigmo_led;

static void zigmo_led_start_timer(void)
{
  osal_start_timerEx(
    g_zigmo_led_task_id,
    ZIGMO_LED_TIMER_EVT,
    ZIGMO_LED_BLINK_DURATION_MS);
}

static bool zigmo_led_is_on(void)
{
  return ZIGMO_LED_PIN == 1;
}

void zigmo_led_init(uint8 task_id)
{
  // Set P1_0 to GPIO
  P1SEL &= ~(1 << 0);
  // Set P1_0 direction to Output.
  P1DIR |= (1 << 0);

  g_zigmo_led_task_id = task_id;
}

uint16 zigmo_led_event_loop(uint8 task_id, uint16 events)
{
  if (events & ZIGMO_LED_TIMER_EVT)
  {
    // Timer fired.
    if (zigmo_led_is_on())
    {
      zigmo_led_off();
      g_zigmo_led.num_blinks--;
      if (g_zigmo_led.num_blinks > 0)
      {
        zigmo_led_start_timer();
      }
    }
    else
    {
      zigmo_led_on();
      zigmo_led_start_timer();
    }

    return (events & ~ZIGMO_LED_TIMER_EVT);
  }

  return 0;
}

void zigmo_led_on(void)
{
  ZIGMO_LED_PIN = 1;
}

void zigmo_led_off(void)
{
  ZIGMO_LED_PIN = 0;
}

void zigmo_led_blink(uint8 times)
{
  zigmo_led_on();
  g_zigmo_led.num_blinks = times;

  zigmo_led_start_timer();
}