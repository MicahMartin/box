#include <stdio.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <util/delay.h>
#include "util/utils.h"
 
int main (void){
 /* set pin 5 of PORTB for output */
 DDRB |= 1 << 5;
 // 0 | 1 = 1
 // 1 | 1 = 1
 // This will ensure the bit is 1
 // 1 is ...00000001 in binary. Shifting it by 5 will make it 00100000
 
 while(1) {
  /* toggle pin 5 high to low to turn led on */
  PORTB ^= 1 << 5;
  // 0 xor 1 = 1
  // 1 xor 1 = 0
  // Toggles
  _delay_ms(MY_DELAY);
 }
}
