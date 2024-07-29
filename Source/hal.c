
#include "util.h"
#include "hal.h"

#include "hal_key.h"
#include "hal_drivers.h"
#include "OSAL_timers.h"

#ifndef ISR_KEYINTERRUPT
#  error "We need interrupts!"
#endif

#define ZIGMO_BTN1_BIT (1 << 3)
#define HAL_KEY_DEBOUNCE_VALUE  25


static halKeyCBack_t pHalKeyProcessFunction;
// static uint8 HalKeyConfigured;
bool Hal_KeyIntEnable;            /* interrupt enable/disable flag */


void halProcessKeyInterrupt(void);
uint8 halGetJoyKeyInput(void);


void HalKeyInit() 
{
  P1SEL &= ~ZIGMO_BTN1_BIT; /* Set pin function to GPIO */
  P1DIR &= ~ZIGMO_BTN1_BIT; /* Set pin direction to Input */
   

  /* Initialize callback function */
  pHalKeyProcessFunction  = NULL;

  /* Start with key is not configured */
  /* HalKeyConfigured = FALSE; */
}

void HalKeyConfig (bool interruptEnable, halKeyCBack_t cback)
{  
  /* Enable/Disable Interrupt or */
  Hal_KeyIntEnable = interruptEnable;

  /* Register the callback fucntion */
  pHalKeyProcessFunction = cback;

  /* Rising/Falling edge configuration */
  PICTL |= 0x01; /* Port 1, inputs 3 to 0, falling edge */

  /* Enable interrupt on pin 1.3 */
  P1IEN |= ZIGMO_BTN1_BIT; 
  
  /* Set P1IE in IEN2 to enable interrupts on port 1 */
  IEN2 |= (1 << 4);  
    
  /* Clear any pending interrupt */
  P1IFG = ~ZIGMO_BTN1_BIT;
}


uint8 HalKeyRead ( void )
{
  return (uint8)!P1_3;
}


void HalKeyPoll (void) 
{
  uint8 btn1_pushed = HalKeyRead();

  /* Invoke Callback if new keys were depressed */
  if (btn1_pushed && (pHalKeyProcessFunction))
  {
    (pHalKeyProcessFunction) (btn1_pushed, HAL_KEY_STATE_NORMAL);
  }
}


void HalKeyEnterSleep ( void )
{
}


uint8 HalKeyExitSleep ( void )
{
  /* Wake up and read keys */
  return ( HalKeyRead () );
}


void halProcessKeyInterrupt (void)
{
  if (P1IFG & ZIGMO_BTN1_BIT)  /* Interrupt Flag has been set */
  {
    P1IFG = ~(ZIGMO_BTN1_BIT); /* Clear Interrupt Flag */
    
    osal_start_timerEx (Hal_TaskID, HAL_KEY_EVENT, HAL_KEY_DEBOUNCE_VALUE);
  }
}


HAL_ISR_FUNCTION( halKeyPort1Isr, P1INT_VECTOR )
{
  HAL_ENTER_ISR();

  if (P1IFG & ZIGMO_BTN1_BIT)
  {
    halProcessKeyInterrupt();
  }

  /*
    Clear the CPU interrupt flag for Port_1
    PxIFG has to be cleared before PxIF
  */
  P1IFG = 0;
  P1IF = 0;
  
  CLEAR_SLEEP_MODE();
  HAL_EXIT_ISR();
}