#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <util/delay.h>

//#include "leds.h"
#include "screenfont.h"

#define F_CPU 12000000UL
#define BAUD 9600UL
//#define MYUBRR ((F_CPU+BAUD*8)/(BAUD*16)-1)

#define VGA_WIDTH 640
#define VGA_HEIGHT 480
//#define HORIZONTAL_BYTES (640/8/4) /* 160 pixels wide */
#define HORIZONTAL_BYTES 20
#define H_BACKPORCH 48
#define H_SYNCPULSE 96
#define H_FRONTPORCH 16
#define V_BACKPORCH 33
#define V_SYNCPULSE 2
#define V_FRONTPORCH 10

#define R_WIDTH (VGA_WIDTH + H_BACKPORCH + H_SYNCPULSE + H_FRONTPORCH)
#define R_HEIGHT (VGA_HEIGHT + V_BACKPORCH + V_SYNCPULSE + V_FRONTPORCH)

#define FONT_WIDTH 8
#define FONT_HEIGHT 8

#define V_BACKPORCH_LINES 35

#define VERTICAL_LINES (VGA_HEIGHT/FONT_HEIGHT/2)
#define HORIZONTAL_PIXELS (HORIZONTAL_BYTES*FONT_WIDTH)

const uint16_t vertical_lines = (VGA_HEIGHT / FONT_HEIGHT / 2);
const uint16_t horizontal_pixels = HORIZONTAL_BYTES*FONT_WIDTH;

const uint16_t vertical_frontporchlines = R_HEIGHT - V_BACKPORCH_LINES;

volatile uint16_t vline = 0;
volatile uint16_t message_line = 0;
volatile uint8_t backporch_lines_togo = 0;

char message[VERTICAL_LINES][HORIZONTAL_BYTES];

uint8_t i;

ISR(TIMER2_COMP_vect) /* vertical pulses */
{
        sleep_disable();
        PORTB |= (1 << PB0);
        _delay_us(64);
        PORTB &= ~(1 << PB0);
        
        vline = 0;
        message_line = 0;
        backporch_lines_togo = V_BACKPORCH_LINES;
}

ISR(TIMER1_COMPA_vect) /* horizontal pulses */
{
        sleep_disable();
}

void beep()
{
        int i;
        for(i=0; i<300; ++i) {
                PORTD |= (1 << PD6);
                _delay_us(300);
                PORTD &= ~(1 << PD6);
                _delay_us(300);
        }
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

        sei();
}

void ioinit()
{
        timer_init();
        //leds_init();
        
        DDRD |= (1 << PD6); // Buzzer
        DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3);

        /* initialize uart */
        DDRD |= (1 << PD1);
        DDRD &= ~(1 << PD0);
        PORTD |= (1 << PD0);

        int ubrr_val = 255; 
        UBRRH = 0;
        UBRRL = 0;

        PORTD |= (1 << PD4);
        /* enable receiver and transmitter */
        UCSRB = 0;
        UCSRC |= (1 << UMSEL) | (1 << UCSZ0) | (1 << UCPOL);
        
        set_sleep_mode(SLEEP_MODE_IDLE);
}

void do_one_scan_line()
{
        if(backporch_lines_togo) {
                backporch_lines_togo--;
                return;
        }

        if(vline >= vertical_lines)
                return;

        const register uint8_t * line_ptr = &screen_font[(vline >> 1) & 0x07][0];
        register char * message_ptr = &(message[message_line][0]);

        uint8_t i = HORIZONTAL_BYTES;

        UCSRB |= (1 << TXEN);

        while(i--) {
                //usart_transmit(pgm_read_byte(line_ptr + (* message_ptr++)));
                UDR = pgm_read_byte(line_ptr + (* message_ptr++));
        }

        while(!(UCSRA & (1 << TXC)))
        {}

        UCSRB = 0;

        vline++;

        if((vline & 0x0F) == 0)
                message_line++;
}

void do_toggle_led()
{
        UCSRB |= (1 << TXEN);
        UDR = 0xAA;
        while(!(UCSRA & (1 << TXC)))
        {}
        UCSRB &= ~(1 << TXEN);

        /*uint8_t j;
        for(j = 0; j < 50; j++) {
                PORTD |= (1 << PD7);
                PORTD &= ~(1 << PD7);
        }*/
}


int main() 
{
        i = 0;
        ioinit();
        _delay_us(500); // let settings settle

        while(1) {
                //do_toggle_led();
                do_one_scan_line();
                sleep_cpu();
                sleep_enable();
        }

        return 1;
}
