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

void ioinit()
{
        int ubrr_val = (F_CPU)/(8*BAUD) - 1;
        usart_init(ubrr_val);
        leds_init();
        DDRD |= (1 << PD6); // Buzzer
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
