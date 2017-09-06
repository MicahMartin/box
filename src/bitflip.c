#include <stdio.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <util/delay.h>
#include "util/utils.h"
// clean, but compiler errors when declared in code rather than
// gcc command
 
int main (void)
{
  const int foobar = 10000;
 /* set pin 5 of PORTB for output */
 DDRB |= 1 << 5;
 /* set pin 1 of PORTB for input */
 DDRB &= 0 << 1;
 
 while(1) {
  /* toggle pin 5 high to low to turn led on */
  PORTB ^= 1 << 5;
  _delay_ms(foobar);
 }
}
