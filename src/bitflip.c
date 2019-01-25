#define __AVR_ATmega328P__
#include <stdio.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include "util/utils.h"

enum STATE { IDLE, CHECKING, DISPENSING };
volatile _Bool buttonDown;

// Debouncing strategy. see https://www.avrfreaks.net/sites/default/files/forum_attachments/debounce.pdf
void debounce(){

  static uint8_t counter = 0;
  static _Bool buttonState = false;

  _Bool currentState = (~PINB & (1<<4));

  if (currentState != buttonState) {
    counter++;
    if (counter >= 4) {
      buttonState = currentState;
      if (currentState != 0) {
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

   OutPin(direction) = 5th bit on DDRB register
   OutPin(write) = 5th bit on PORTB register ( 0=0v?, 1=5v? )
   OutPinValue(read) = 5th bit on PINB register

   ButtonPin(direction) = 4th bit on DDRB Register
   ButtonPin(write) = 4th bit on PORTB register
   ButtonPinValue(read) = 4th bit on PINB register

   0 | 1 = 1
   1 | 1 = 1
   This will ensure the bit is 1

   1 is ...00000001 in binary. Shifting it by 5 will make it 00100000.

   set 4th bit of DDRB to 0 for the corresponding pin to be used as input
   DDRB &= ~(1 << 4);
   ~(1<<x) is good to keep in mind. It will first create a bitmask of 0001 0000
   then invert it with the notter (~) to get a mask of 11101111
   So the comparison wont change a bit if its true, because 1 & 1 is 1
   and it wont change a bit if its 0, because 0 & 1 is 0. 
   so the only bit with potential to change is the false bit, since 1 & 0 is false.
   This will ensure that pin4 is on input. 

   I really shouldnt have to write this out.
   0 & 0 = 0
   1 & 0 = 0
   1 & 1 = 1
  */
  

  
  // set 5th bit of DDRB(Data direction register B) to 1 for the corresponding pin to be used as output.
  // this pin will connect(and provide power) to the dispenser mechanism
  DDRB |= (1 << PB5);

  // set 4th bit of PORTB to high so we can rely on internal pull up resistor 
  PORTB |= (1 << PB4);

  // init state
  enum STATE toyState = IDLE;
  // TODO: check if anything is set in eeprom, if not set eeprom to current time
  // NOTE: All the time shit https://www.avrfreaks.net/forum/tut-c-creating-rtc-using-internal-countertimer?page=all
  
  

  while(true) {

    debounce();

    // TODO: polly dat state
    switch (toyState) {
      case IDLE:
        // We're in idle state, check to see if button was pressed
        // clear the 5th bit on portB incase we're coming back from dispensing state
        PORTB &= ~(1 << PB5);
        toyState = buttonDown ? CHECKING : IDLE;
        buttonDown = false;
      break;

      case CHECKING:
      // read the last date from eeprom
      
      break;

      case DISPENSING:
        _delay_ms(DISPENSE_DELAY);
      break;
    }
    //This conditional checks to see if the 4th bit in PINB is 0 or 1.
    //Lets assume pinB is currently 0000 0000, meaning all low.
    //'anding' 0000 0000 over 1<<4 (0001 0000) would give us 0 straight up since 0 & anything is obviously false
    // but if a bit has value in it, I guess that means it evaluates to true?

    // tl;dr 0001 0000 is truthy
    // note: pinx register is read only. it gets the state of the corresponding pin
    //if (!(PINB & (1<<4))) {
      //Button is pressed, do stuff.
      
    //}
    /* toggle 5th bit high to low to turn led connected to the corresponding pin on and off */
    PORTB ^= 1 << 5;
    // 0 xor 1 = 1
    // 1 xor 1 = 0
    // Toggles
    _delay_ms(CLOCK_DELAY);
   }
}
