#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdio.h>

//#include "usart.h"
#include "screenfont.h"
#include "smallfont.h"

#define F_CPU 12000000UL
#define BAUD 9600UL

#define RED_PIN PD4
#define GREEN_PIN PD5
#define BLUE_PIN PD6

#define HSYNC_PIN PB2
#define VSYNC_PIN PB0

#define horizontal_bytes 20
#define vertical_pixels 480

#define vertical_lines (vertical_pixels/16)
#define horizontal_pixels (horizontal_bytes*8)

#define nop asm volatile ("nop\n\t");

const uint8_t vertical_backporch_lines = 35;

volatile uint16_t vline = 0;
volatile uint16_t message_line = 0;
volatile uint8_t backporch_lines_togo = 0;

char message[vertical_lines][horizontal_bytes];

uint8_t i;

ISR(TIMER2_COMP_vect) /* vertical pulses */
{
        sleep_disable();
        PORTB |= (1 << PB0);
        _delay_us(64);
        PORTB &= ~(1 << PB0);
        
        vline = 0;
        message_line = 0;
        backporch_lines_togo = vertical_backporch_lines;
}

ISR(TIMER1_COMPA_vect) /* horizontal pulses */
{
        sleep_disable();
}

void timer_init(void)
{

        OCR2 = 194;

        TCCR2 |= (1 << WGM21) | (1 << COM21);
        TIMSK |= (1 << OCIE2) | (1 << OCIE1A);
        //TIFR |= (1 << OCF2) | (1 << OCF1A);
        TCCR2 |= (1 << CS20) | (1 << CS21) | (1 << CS22);

        TCCR1A |= (1 << WGM12) | (1 << WGM11) | (1 << COM1B1);
        TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS11);

        ICR1 = 47;
        OCR1B = 5;
}

void do_one_scan_line()
{
        if(backporch_lines_togo) {
                backporch_lines_togo--;
                return;
        }

        if(vline >= vertical_pixels)
                return;


        //const register uint8_t *line_ptr = &screen_font[(vline >> 1) & 0x07][0];
        register char *message_ptr = &(message[message_line][0]);
        
        register uint8_t i = horizontal_bytes;

        //UCSRB |= (1 << TXEN);

        while(i--) {
                PORTD = *message_ptr++;
                //UDR = pgm_read_byte(line_ptr + (*message_ptr++));
        }

        /*while(!(UCSRA & (1 << TXC)))
        {}

        UCSRB &= ~(1 << TXEN);*/


        vline++;

        if((vline & 0xF) == 0)
                message_line++;
}

void ioinit()
{
        timer_init();
        //usart_init(0);
        //leds_init();
        
        /* timer interrupt related pins */
        DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3); 
        /* rgb pins */
        DDRD |= (1 << RED_PIN) | (1 << GREEN_PIN) | (1 << BLUE_PIN);

        set_sleep_mode(SLEEP_MODE_IDLE);
}

int main() 
{
        /* initial message */
        int x,y;
        /*for(j = 0; j < vertical_lines; j++)
                sprintf(message[j], "%03i - hello!", i);*/
        for(y = 0; y < vertical_lines; y++) {
                for(x = 0; x < horizontal_bytes; x++) {
                        message[y][x] = (x+y) << 4;
                }
        }


        ioinit();
        sei();

        while(1) {
                //dummy_pulse();
                do_one_scan_line();
                sleep_enable();
                sleep_cpu();
        }

        return 1;
}
