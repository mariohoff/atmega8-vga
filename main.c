#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "usart.h"
#include "leds.h"

#define F_CPU 12000000UL
#define BAUD 9600UL
//#define MYUBRR ((F_CPU+BAUD*8)/(BAUD*16)-1)

void beep()
{
        int i;
        for(i=0; i<300; ++i) {
                PORTD |= (1 << PD6);
                _delay_us(200);
                PORTD &= ~(1 << PD6);
                _delay_us(10);
        }
}

void timer_init(void)
{
        /* prescaler of 1024 */
        TCCR2 |= (1 << CS20) | (1 << CS21) | (1 << CS22);
        /* clear on compare match; clear timer on compare match */
        TCCR2 |= (1 << COM21) | (1 << WGM21);
        OCR2 = 194; /* count up 195 ticks */

        /* clear on compare */
        TCCR1B |= (1 << WGM12);

        /* prescaler of 1024 */
        TCCR1B |= (1 << CS12);

        /* TODO: calculate counter */
        OCR1A = 123;
        OCR1B = 321; 

        TIMSK |= (1 << TOIE1) | (1 << TOIE2);

        sei();
}

void ioinit()
{
        int ubrr_val = (F_CPU)/(8*BAUD) - 1;
        usart_init(ubrr_val);
        leds_init();
        DDRD |= (1 << PD6); // Buzzer
        timer_init();
}

int main() 
{
        ioinit();

        unsigned char val = 0;

        while(1) {
                //val = usart_receive();
                while(val <= 255) {
                        //beep();
                        leds_set(val);
                        usart_transmit(val);
                        val++; 
                        _delay_ms(1000);
                }
                val =  0;
        }

        return 1;
}
