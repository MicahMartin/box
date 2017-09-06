#include "stdio.h"
#include "avr/io.h"
#include "avr/sfr_defs.h"
#include "util/delay.h"

// clean, but compiler errors when declared in code rather than
// gcc command
// #define BLINK_DELAY_MS 10000
 
int main (void)
{
 /* set pin 5 of PORTB for output */
 DDRB |= 1 << 5;
 /* set pin 1 of PORTB for input */
 DDRB &= 0 << 1;
 
 while(1) {
  /* toggle pin 5 high to low to turn led on */
  PORTB ^= 1 << 5;
  _delay_ms(BLINK_DELAY_MS);
 }
}
