#define __AVR_ATmega328P__
#include <stdio.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <util/delay.h>
#include "util/utils.h"
 
int main (void){
  /* set 5th bit of DDRB to 1 for the corresponding pin to be used as output */
  DDRB |= (1 << 5);
  // 0 | 1 = 1
  // 1 | 1 = 1
  // This will ensure the bit is 1
  // 1 is ...00000001 in binary. Shifting it by 5 will make it 00100000.

  /* set 4th bit of DDRB to 0 for the corresponding pin to be used as input*/
  //DDRB &= ~(1 << 4);
  // ~(1<<x) is good to keep in mind. It will first create a bitmask of 0001 0000
  // then invert it with the notter (~) to get a mask of 11101111
  // So the comparison wont change a bit if its true, because 1 & 1 is 1, and it wont change a bit if its 0, because 0 & 1 is 0. so the only bit with potential to change is the false bit, since 1 & 0 is false.
  // 0 & 0 = 0
  // 1 & 0 = 0
  // 1 & 1 = 1
  // This will ensure that pin4 is on input. 

  /*set 4th bit of PORTB to high so we can rely on internal pull up resistor*/ 
  //PORTB |= (1 << 4);

  while(1) {
    //This conditional checks to see if the 4th bit in PINB is 0 or 1.
    //Lets assume pinB is currently 0000 0000, meaning all low.
    //'anding' 0000 0000 over 1<<4 (0001 0000) would give us 0 straight up since 0 & anything is obviously false
    // but if a bit has any 1 value in it, I guess that means it evaluates to true?

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
    _delay_ms(MY_DELAY);
   }
}
