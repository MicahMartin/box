#define __AVR_ATmega328P__
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include "util/utils.h"

enum STATE { IDLE, CHECKING, DISPENSING };
volatile _Bool buttonDown;

int ticks = 0;
int seconds = 0;
int minutes = 0;
int hours = 0;
int days = 0;
// int dayDispensed = 0;
int lastDispensed = 0;
_Bool firstPress = true;

// Set up timer to generate an interrupt every 78 clock cycles with a prescaler of 1024
// https://www.avrfreaks.net/forum/tut-c-creating-rtc-using-internal-countertimer?page=all
void setupTimer(){
  // TCCR0A and TCCR0B registers (timer control register0 A & B) control the mode and prescalar for timer0
  
  // Set timer0 to CTC mode
  TCCR0A = (1 << WGM01);

  // Every 78 ticks generate an interrupt
  // output compare register for timer 0, comparison value A
  OCR0A = 78;
  // TODO: remember what these 2 lines are doing
  TIMSK0 = (1 << OCIE0A);
  sei();

  // Set the prescalar to 1024
  TCCR0B = (1 << CS02) | (1 << CS00);
}

// Interrupt when timer0 comparison is met 
ISR(TIMER0_COMPA_vect){
  ticks++;
  if(ticks == 100){
    ticks = 0;
    seconds++;
    if(seconds == 60){
      seconds = 0;
      minutes++;
      if(minutes == 60){
        minutes = 0;
        hours++;
        if(hours == 24){
          hours = 0;
          days++;
        }
      }
    }
  }

}

// Debouncing strategy. see https://www.avrfreaks.net/sites/default/files/forum_attachments/debounce.pdf
void debounce(){

  static uint8_t counter = 0;
  static _Bool buttonState = false; // false = not pressed

  // If PIND1 is low, then the button is pressed
  // PIND: 0000 0010 // 2nd bit is high because internal pullUp is set for PORTD1
  // 1<<PD1: 0000 0010


  // inverting the register since pullup is set for PORTD1
  _Bool currentState = (~PIND & (1<<PD1));

  if (currentState != buttonState) {
    counter++;
    if (counter >= 4) {
      buttonState = currentState;
      if (currentState) {
        buttonDown = true;
      }
      counter = 0;
    }
    
  } else {
    counter = 0;
  }

}

int main (void){

  /*
   from past kai: If you leave this project alone for a week(again), read this. love u

  **NOTE
    All the time shit:

    https://www.avrfreaks.net/forum/tut-c-creating-rtc-using-internal-countertimer?page=all
    https://electronics.stackexchange.com/questions/49959/how-to-display-current-time?rq=1
  **
  
  **NOTE
    IO Register shit:

    The DDxn bit in the DDRx Register selects the direction of this pin. 
    If DDxn is written logic one, Pxn is configured as an output pin. If DDxn is written logic zero, Pxn is configured as an input pin.
    If PORTxn is written logic one when the pin is configured as an input pin, the pull-up resistor is activated.
    To switch the pull-up resistor off, PORTxn has to be written logic zero or the pin has to be configured as an output pin.
    The port pins are tri-stated when reset condition becomes active, even if no clocks are running.
  **

  **NOTE
    Toggling a bit:

    PORTB ^= 1 << 5;

    // 0 xor 1 = 1
    // 1 xor 1 = 0
    // Toggles
  **
     

  **NOTE
   Clearing a bit:

   assume DDRD is currently 0101 1101. 

   DDRD &= ~(1 << PD4);
   ~(1 << 4) is good to keep in mind. 
   It will first create a bitmask of 0001 0000 (1 << 4)
   then invert it with the notter (~) to get a mask of 1110 1111.

     0101 1101
   & 1110 1111
     _________
     0 and 1 / 1 and 0 = 0
     1 and 1 = 1
     0 and 0 = 0

   So the comparison wont change the bits we aren't worried about because if theyre true, 1 & 1 is 1
   and it wont change those bits if theyre 0, because 0 & 1 is 0. 
   so the only bit with potential to change is the cleared bit in the mask, since 1 & 0 is 0
  **

  */
  

  
  // Set 1st bit of DDRD for the corresponding pin to be used as output.
  // This pin will connect(and provide power) to the dispenser mechanism
  DDRD |= (1 << PD0);

  // Clear the 2nd bit of DDRD for the corresponding pin to be used as input
  // This pin will connect to external pushButton
  DDRD &= ~(1 << PD1);

  // Set 2nd bit of PORTD to enable pullUp resistor
  PORTD |= (1 << PD1);

  // Init state
  enum STATE toyState = IDLE;

  // TODO: If we introduce an RTC... check if anything is set in eeprom, if not set eeprom to current time

  while(true) {

    debounce();

    // TODO: polly dat state
    switch (toyState) {
      case IDLE:
        // We're in idle state, check to see if button was pressed
        toyState = buttonDown ? CHECKING : IDLE;
        buttonDown = false;
      break;

      case CHECKING:
        // TODO: Better logic to handle the first 'check'
        // If first time button is pressed, go right to dispensing
        toyState = (lastDispensed < minutes || firstPress) ? DISPENSING : IDLE;
      break;

      case DISPENSING:
        // Last time we dispensed was like, now?
        lastDispensed = minutes;

        // Set the 1st bit on portD to power the dispense mechanism
        PORTD |= (1 << PD0);
        // Wait a bit, let mechanism do its thing
        _delay_ms(DISPENSE_DELAY);
        // Clear the 1st bit on portD to stop powering the dispense mechanism
        PORTD &= ~(1 << PD0);

        // TODO: I smell a better elegant solution here...
        firstPress = false;

        // Return to idle
        toyState = IDLE;

      break;
    }

    // Dont run too fast..
    _delay_ms(CLOCK_DELAY);
   }
}
