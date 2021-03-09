/*
 * blink.c
 *
 * Created: 2021-01-03 5:10:25 PM
 * Author : gord
 */

#include <avr/io.h>
#include <util/delay.h>

int main(void){
   DDRB = 0b00100001;
   /* Replace with your application code */
   while (1)
   {
      PORTB = 0b00100001;
      _delay_ms(1000);
      PORTB = 0b00000000;
      _delay_ms(1000);
   }
}
